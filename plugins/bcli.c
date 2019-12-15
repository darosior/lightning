#include <bitcoin/base58.h>
#include <bitcoin/block.h>
#include <bitcoin/feerate.h>
#include <bitcoin/script.h>
#include <bitcoin/shadouble.h>
#include <ccan/array_size/array_size.h>
#include <ccan/cast/cast.h>
#include <ccan/io/io.h>
#include <ccan/json_out/json_out.h>
#include <ccan/pipecmd/pipecmd.h>
#include <ccan/str/hex/hex.h>
#include <ccan/take/take.h>
#include <ccan/tal/grab_file/grab_file.h>
#include <ccan/tal/path/path.h>
#include <ccan/tal/str/str.h>
#include <common/json_helpers.h>
#include <common/memleak.h>
#include <common/timeout.h>
#include <common/utils.h>
#include <errno.h>
#include <inttypes.h>
//#include <lightningd/bitcoind.h>
#include <lightningd/chaintopology.h>
#include <lightningd/log.h>
//#include <lightningd/lightningd.h>
#include <plugins/libplugin.h>

/* Bitcoind's web server has a default of 4 threads, with queue depth 16.
 * It will *fail* rather than queue beyond that, so we must not stress it!
 *
 * This is how many request for each priority level we have.
 */
#define BITCOIND_MAX_PARALLEL 4

enum bitcoind_prio {
	BITCOIND_LOW_PRIO,
	BITCOIND_HIGH_PRIO
};
#define BITCOIND_NUM_PRIO (BITCOIND_HIGH_PRIO+1)

struct bitcoind {
	/* eg. "bitcoin-cli" */
	char *cli;

	/* -datadir arg for bitcoin-cli. */
	char *datadir;

	/* Is bitcoind synced?  If not, we retry. */
	bool synced;

	/* How many high/low prio requests are we running (it's ratelimited) */
	size_t num_requests[BITCOIND_NUM_PRIO];

	/* Pending requests (high and low prio). */
	struct list_head pending[BITCOIND_NUM_PRIO];

	/* If non-zero, time we first hit a bitcoind error. */
	unsigned int error_count;
	struct timemono first_error_time;

	/* Ignore results, we're shutting down. */
	bool shutdown;

	/* How long to keep trying to contact bitcoind
	 * before fatally exiting. */
	u64 retry_timeout;

	/* Passthrough parameters for bitcoin-cli */
	char *rpcuser, *rpcpass, *rpcconnect, *rpcport;

	struct list_head pending_getfilteredblock;
};

struct bitcoind *bitcoind;

struct bitcoin_cli {
	struct list_node list;
	int fd;
	int *exitstatus;
	pid_t pid;
	const char **args;
	struct timeabs start;
	enum bitcoind_prio prio;
	char *output;
	size_t output_bytes;
	size_t new_output;
	struct command_result *(*process)(struct bitcoin_cli *);
	struct bitcoin_cli **stopper;
	struct command *cmd;
};

/* Add the n'th arg to *args, incrementing n and keeping args of size n+1 */
static void add_arg(const char ***args, const char *arg)
{
	tal_arr_expand(args, arg);
}

static const char **gather_args(const tal_t *ctx, const char *cmd, const char **cmd_args)
{
	const char **args = tal_arr(ctx, const char *, 1);

	args[0] = bitcoind->cli ? bitcoind->cli : chainparams->cli;
	if (chainparams->cli_args)
		add_arg(&args, chainparams->cli_args);

	if (bitcoind->datadir)
		add_arg(&args, tal_fmt(args, "-datadir=%s", bitcoind->datadir));

	if (bitcoind->rpcconnect)
		add_arg(&args,
			tal_fmt(args, "-rpcconnect=%s", bitcoind->rpcconnect));

	if (bitcoind->rpcport)
		add_arg(&args,
			tal_fmt(args, "-rpcport=%s", bitcoind->rpcport));

	if (bitcoind->rpcuser)
		add_arg(&args, tal_fmt(args, "-rpcuser=%s", bitcoind->rpcuser));

	if (bitcoind->rpcpass)
		add_arg(&args,
			tal_fmt(args, "-rpcpassword=%s", bitcoind->rpcpass));

	add_arg(&args, cmd);

	for (size_t i = 0; i < tal_count(cmd_args); i++)
		add_arg(&args, cmd_args[i]);

	add_arg(&args, NULL);
	return args;
}

static struct io_plan *read_more(struct io_conn *conn, struct bitcoin_cli *bcli)
{
	bcli->output_bytes += bcli->new_output;
	if (bcli->output_bytes == tal_count(bcli->output))
		tal_resize(&bcli->output, bcli->output_bytes * 2);
	return io_read_partial(conn, bcli->output + bcli->output_bytes,
			       tal_count(bcli->output) - bcli->output_bytes,
			       &bcli->new_output, read_more, bcli);
}

static struct io_plan *output_init(struct io_conn *conn, struct bitcoin_cli *bcli)
{
	bcli->output_bytes = bcli->new_output = 0;
	bcli->output = tal_arr(bcli, char, 100);
	return read_more(conn, bcli);
}

static void next_bcli(enum bitcoind_prio prio);

/* For printing: simple string of args (no secrets!) */
static char *args_string(const tal_t *ctx, const char **args)
{
	size_t i;
	char *ret = tal_strdup(ctx, args[0]);

	for (i = 1; args[i]; i++) {
		ret = tal_strcat(ctx, take(ret), " ");
		if (strstarts(args[i], "-rpcpassword")) {
			ret = tal_strcat(ctx, take(ret), "-rpcpassword=...");
		} else if (strstarts(args[i], "-rpcuser")) {
			ret = tal_strcat(ctx, take(ret), "-rpcuser=...");
		} else {
			ret = tal_strcat(ctx, take(ret), args[i]);
		}
	}
	return ret;
}

static char *bcli_args(struct bitcoin_cli *bcli)
{
    return args_string(bcli, bcli->args);
}

static void retry_bcli(struct bitcoin_cli *bcli)
{
	list_add_tail(&bitcoind->pending[bcli->prio], &bcli->list);
	next_bcli(bcli->prio);
}

/* We allow 60 seconds of spurious errors, eg. reorg. */
static void bcli_failure(struct bitcoin_cli *bcli,
                         int exitstatus)
{
	struct timerel t;

	if (!bitcoind->error_count)
		bitcoind->first_error_time = time_mono();

	t = timemono_between(time_mono(), bitcoind->first_error_time);
	if (time_greater(t, time_from_sec(bitcoind->retry_timeout)))
		fatal("%s exited %u (after %u other errors) '%.*s'; "
		      "we have been retrying command for "
		      "--bitcoin-retry-timeout=%"PRIu64" seconds; "
		      "bitcoind setup or our --bitcoin-* configs broken?",
		      bcli_args(bcli),
		      exitstatus,
		      bitcoind->error_count,
		      (int)bcli->output_bytes,
		      bcli->output,
		      bitcoind->retry_timeout);

	plugin_log(LOG_UNUSUAL, "%s exited with status %u", bcli_args(bcli), exitstatus);

	bitcoind->error_count++;

	/* Retry in 1 second (not a leak!) */
	notleak(new_reltimer(&timers, notleak(bcli),
			     time_from_sec(1),
			     retry_bcli, bcli));
}

static void bcli_finished(struct io_conn *conn UNUSED, struct bitcoin_cli *bcli)
{
	int ret, status;
	struct command_result *res;
	enum bitcoind_prio prio = bcli->prio;
	u64 msec = time_to_msec(time_between(time_now(), bcli->start));

	/* If it took over 10 seconds, that's rather strange. */
	if (msec > 10000)
		plugin_log(LOG_UNUSUAL, "bitcoin-cli: finished %s (%"PRIu64" ms)",
		           bcli_args(bcli), msec);

	assert(bitcoind->num_requests[prio] > 0);

	/* FIXME: If we waited for SIGCHILD, this could never hang! */
	while ((ret = waitpid(bcli->pid, &status, 0)) < 0 && errno == EINTR);
	if (ret != bcli->pid)
		fatal("%s %s", bcli_args(bcli),
		      ret == 0 ? "not exited?" : strerror(errno));

	if (!WIFEXITED(status))
		fatal("%s died with signal %i",
		      bcli_args(bcli),
		      WTERMSIG(status));

	if (!bcli->exitstatus) {
		if (WEXITSTATUS(status) != 0) {
			bcli_failure(bcli, WEXITSTATUS(status));
			bitcoind->num_requests[prio]--;
			goto done;
		}
	} else
		*bcli->exitstatus = WEXITSTATUS(status);

	if (WEXITSTATUS(status) == 0)
		bitcoind->error_count = 0;

	bitcoind->num_requests[bcli->prio]--;

	/* Don't continue if were only here because we were freed for shutdown */
	if (bitcoind->shutdown)
		return;

	// FIXME: Legal ?
	res = bcli->process(bcli);
	if (!res)
		bcli_failure(bcli, WEXITSTATUS(status));
	else {
		//was_pending(res);
		tal_free(bcli);
	}

done:
	next_bcli(prio);
}

static void next_bcli(enum bitcoind_prio prio)
{
	struct bitcoin_cli *bcli;
	struct io_conn *conn;

	if (bitcoind->num_requests[prio] >= BITCOIND_MAX_PARALLEL)
		return;

	bcli = list_pop(&bitcoind->pending[prio], struct bitcoin_cli, list);
	if (!bcli)
		return;

	bcli->pid = pipecmdarr(NULL, &bcli->fd, &bcli->fd,
			       cast_const2(char **, bcli->args));
	if (bcli->pid < 0)
		fatal("%s exec failed: %s", bcli->args[0], strerror(errno));

	bcli->start = time_now();

	bitcoind->num_requests[prio]++;

	/* This lifetime is attached to bitcoind command fd */
	conn = notleak(io_new_conn(bitcoind, bcli->fd, output_init, bcli));
	io_set_finish(conn, bcli_finished, bcli);
}

/* If stopper gets freed first, set process() to a noop. */
static void stop_process_bcli(struct bitcoin_cli **stopper)
{
	(*stopper)->process = NULL;
	(*stopper)->stopper = NULL;
}

/* It command finishes first, free stopper. */
static void remove_stopper(struct bitcoin_cli *bcli)
{
	/* Calls stop_process_bcli, but we don't care. */
	tal_free(bcli->stopper);
}

/* If ctx is non-NULL, and is freed before we return, we don't call process().
 * process returns false() if it's a spurious error, and we should retry. */
static void
start_bitcoin_cli(const tal_t *ctx,
		  struct command *cmd,
		  struct command_result *(*process)(struct bitcoin_cli *),
		  bool nonzero_exit_ok,
		  enum bitcoind_prio prio,
		  char *method, const char **method_args)
{
	struct bitcoin_cli *bcli = tal(bitcoind, struct bitcoin_cli);

	bcli->process = process;
	bcli->cmd = cmd;
	bcli->prio = prio;
	if (ctx) {
		/* Create child whose destructor will stop us calling */
		bcli->stopper = tal(ctx, struct bitcoin_cli *);
		*bcli->stopper = bcli;
		tal_add_destructor(bcli->stopper, stop_process_bcli);
		tal_add_destructor(bcli, remove_stopper);
	} else
		bcli->stopper = NULL;

	if (nonzero_exit_ok)
		bcli->exitstatus = tal(bcli, int);
	else
		bcli->exitstatus = NULL;

	bcli->args = gather_args(bcli, method, method_args);

	list_add_tail(&bitcoind->pending[bcli->prio], &bcli->list);
	next_bcli(bcli->prio);
}

static struct command_result *process_gettxout(struct bitcoin_cli *bcli)
{
	const jsmntok_t *tokens, *valuetok, *scriptpubkeytok, *hextok;
	struct bitcoin_tx_output output;
	bool valid;
	struct json_out *out;

	/* As of at least v0.15.1.0, bitcoind returns "success" but an empty
	   string on a spent txout */
	if (*bcli->exitstatus != 0 || bcli->output_bytes == 0)
		return command_done_err(bcli->cmd, BCLI_ERROR, "Spent tx output", NULL);

	tokens = json_parse_input(bcli->output, bcli->output, bcli->output_bytes,
				  &valid);
	if (!tokens)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli, "%s: %s response",
		                                bcli_args(bcli),
		                                valid ? "partial" : "invalid"), NULL);

	if (tokens[0].type != JSMN_OBJECT)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli, "%s: gave non-object (%.*s)?",
		                                bcli_args(bcli), (int)bcli->output_bytes,
		                                bcli->output), NULL);

	valuetok = json_get_member(bcli->output, tokens, "value");
	if (!valuetok)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli,"%s: had no value member (%.*s)?",
		                                bcli_args(bcli), (int)bcli->output_bytes,
		                                bcli->output), NULL);

	if (!json_to_bitcoin_amount(bcli->output, valuetok, &output.amount.satoshis)) /* Raw: talking to bitcoind */
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli, "%s: had bad value (%.*s)?",
		                                bcli_args(bcli), (int)bcli->output_bytes,
		                                bcli->output), NULL);

	scriptpubkeytok = json_get_member(bcli->output, tokens, "scriptPubKey");
	if (!scriptpubkeytok)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli, "%s: had no scriptPubKey member (%.*s)?",
		                                bcli_args(bcli), (int)bcli->output_bytes,
		                                bcli->output), NULL);

	hextok = json_get_member(bcli->output, scriptpubkeytok, "hex");
	if (!hextok)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli, "%s: had no scriptPubKey->hex member (%.*s)?",
		                                bcli_args(bcli), (int)bcli->output_bytes,
		                                bcli->output), NULL);

	output.script = tal_hexdata(bcli, bcli->output + hextok->start,
	                            hextok->end - hextok->start);
	if (!output.script)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli, "%s: scriptPubKey->hex invalid hex (%.*s)?",
		                                bcli_args(bcli), (int)bcli->output_bytes,
		                                bcli->output), NULL);

	out = json_out_new(NULL);
	json_out_start(out, NULL, '{');
	json_out_add(out, "amount", false, "%lu", output.amount.satoshis);
	json_out_addstr(out, "script", tal_hexstr(out, output.script, sizeof(output.script)));
	json_out_end(out, '}');

	return command_success(bcli->cmd, out);
}

static struct command_result *process_getrawblock(struct bitcoin_cli *bcli)
{
	struct json_out *out;

	out = json_out_new(NULL);
	json_out_start(out, NULL, '{');
	json_out_addstr(out, "block", tal_fmt(out, "%.*s", (int)bcli->output_bytes,
	                                      bcli->output));
	json_out_end(out, '}');

	return command_success(bcli->cmd, out);
}

static struct command_result *process_getblockcount(struct bitcoin_cli *bcli)
{
	u32 blockcount;
	char *p, *end;
	struct json_out *out;

	p = tal_strndup(bcli, bcli->output, bcli->output_bytes);
	blockcount = strtol(p, &end, 10);
	if (end == p || *end != '\n')
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli, "%s: gave non-numeric blockcount %s",
		                                bcli_args(bcli), p), NULL);

	out = json_out_new(NULL);
	json_out_start(out, NULL, '{');
	json_out_add(out, "blockcount", false, "%u", blockcount);
	json_out_end(out, '}');

	return command_success(bcli->cmd, out);
}

static struct command_result *process_getblockhash(struct bitcoin_cli *bcli)
{
	struct bitcoin_blkid blkid;
	struct json_out *out;

	/* If it failed with error 8, return an empty blockhash. */
	if (*bcli->exitstatus != 0) {
		/* Other error means we have to retry. */
		if (*bcli->exitstatus != 8)
			return NULL;

		out = json_out_new(NULL);
		json_out_start(out, NULL, '{');
		json_out_addstr(out, "blockhash", "");
		json_out_end(out, '}');
		return command_success(bcli->cmd, out);
	}

	if (bcli->output_bytes == 0
	    || !bitcoin_blkid_from_hex(bcli->output, bcli->output_bytes-1,
				       &blkid)) {
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli, "%s: bad blockid '%.*s'",
		                                bcli_args(bcli),
		                                (int)bcli->output_bytes, bcli->output), NULL);
	}

	out = json_out_new(NULL);
	json_out_start(out, NULL, '{');
	json_out_addstr(out, "blockhash", tal_hexstr(out, &blkid, sizeof(blkid)));
	json_out_end(out, '}');
	return command_success(bcli->cmd, out);
}

static struct command_result *process_getclientversion(struct bitcoin_cli *bcli)
{
	const jsmntok_t *tokens, *versiontok;
	bool valid;
	struct json_out *out;
	u64 version;

	tokens = json_parse_input(bcli->output, bcli->output, bcli->output_bytes,
	                          &valid);
	if (!tokens)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: %s response (%.*s)",
		                                bcli_args(bcli),
		                                valid ? "partial" : "invalid",
		                                (int)bcli->output_bytes, bcli->output), NULL);

	if (tokens[0].type != JSMN_OBJECT)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: gave non-object (%.*s)",
		                                bcli_args(bcli),
		                                (int)bcli->output_bytes, bcli->output), NULL);

	versiontok = json_get_member(bcli->output, tokens, "version");
	if (!versiontok)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: no version member",
		                                bcli_args(bcli)), NULL);

	if (!json_to_u64(bcli->output, versiontok, &version))
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: gave bad version",
		                                bcli_args(bcli)), NULL);


	out = json_out_new(NULL);
	json_out_start(out, NULL, '{');
	json_out_add(out, "version", false, "%lu", version);
	json_out_end(out, '}');
	return command_success(bcli->cmd, out);
}

static struct command_result *process_getblockchaininfo(struct bitcoin_cli *bcli)
{
	const jsmntok_t *tokens, *tok;
	bool valid;
	struct json_out *out;
	u32 headers, blocks;
	bool ibd;

	tokens = json_parse_input(bcli, bcli->output, bcli->output_bytes,
	                          &valid);
	if (!tokens)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: %s response (%.*s)",
		                                bcli_args(bcli),
		                                valid ? "partial" : "invalid",
		                                (int)bcli->output_bytes, bcli->output), NULL);

	if (tokens[0].type != JSMN_OBJECT)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: gave non-object (%.*s)",
		                                bcli_args(bcli),
		                                (int)bcli->output_bytes, bcli->output), NULL);

	tok = json_get_member(bcli->output, tokens, "headers");
	if (!tok || !json_to_number(bcli->output, tok, &headers))
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: bad 'headers' field (%.*s)",
		                                bcli_args(bcli),
		                                (int)bcli->output_bytes, bcli->output), NULL);

	tok = json_get_member(bcli->output, tokens, "blocks");
	if (!tok || !json_to_number(bcli->output, tok, &blocks))
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: bad 'blocks' field (%.*s)",
		                                bcli_args(bcli),
		                                (int)bcli->output_bytes, bcli->output), NULL);

	tok = json_get_member(bcli->output, tokens, "initialblockdownload");
	if (!tok || !json_to_bool(bcli->output, tok, &ibd))
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: bad 'initialblockdownload' field (%.*s)",
		                                bcli_args(bcli),
		                                (int)bcli->output_bytes, bcli->output), NULL);

	out = json_out_new(NULL);
	json_out_start(out, NULL, '{');
	json_out_add(out, "headers", false, "%u", headers);
	json_out_add(out, "blocks", false, "%u", blocks);
	json_out_add(out, "initialblockdownload", false, "%s", ibd ? "true" : "false");
	json_out_end(out, '}');
	return command_success(bcli->cmd, out);
}

static struct command_result *process_estimatefee(struct bitcoin_cli *bcli)
{
	const jsmntok_t *tokens, *feeratetok;
	bool valid;
	struct json_out *out;
	u64 feerate;

	tokens = json_parse_input(bcli->output, bcli->output,
	                          (int)bcli->output_bytes, &valid);
	if (!tokens)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: %s response (%.*s)",
		                                bcli_args(bcli),
		                                valid ? "partial" : "invalid",
		                                (int)bcli->output_bytes, bcli->output), NULL);

	if (tokens[0].type != JSMN_OBJECT)
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: gave non-object (%.*s)",
		                                bcli_args(bcli),
		                                (int)bcli->output_bytes, bcli->output), NULL);

	feeratetok = json_get_member(bcli->output, tokens, "feerate");
	if (!feeratetok || !json_to_bitcoin_amount(bcli->output, feeratetok, &feerate))
		return command_done_err(bcli->cmd, BCLI_ERROR,
		                        tal_fmt(bcli->cmd, "%s: bad 'feerate' field (%.*s)",
		                                bcli_args(bcli),
		                                (int)bcli->output_bytes, bcli->output), NULL);

	out = json_out_new(NULL);
	json_out_start(out, NULL, '{');
	json_out_add(out, "feerate", false, "%lu", feerate);
	json_out_end(out, '}');
	return command_success(bcli->cmd, out);
}

static struct command_result *process_sendrawtransaction(struct bitcoin_cli *bcli)
{
	struct json_out *out;

	// FIXME: log the tx ?
	out = json_out_new(NULL);
	json_out_start(out, NULL, '{');
	json_out_add(out, "success", false, "%s",
	             *bcli->exitstatus ? "true" : "false");
	json_out_end(out, '}');
	return command_success(bcli->cmd, out);
}

static struct command_result *bitcoin_data_request(struct command *cmd,
                                                   const char *buf,
                                                   const jsmntok_t *toks)
{
	const jsmntok_t *result, *param;
	const char *method, **params;
	size_t i;

	result = json_get_member(buf, toks, "bitcoin_data");
	method = json_strdup(cmd, "command", result);
	params = tal_arr(cmd, const char *, 0);
	json_for_each_arr(i, param, json_get_member(buf, result, "params"))
		tal_arr_expand(&params, json_strdup(params, buf, param));

	if (streq(method, "getrawblock"))
		start_bitcoin_cli(NULL, cmd, process_getrawblock, false, BITCOIND_HIGH_PRIO,
		                  "getblock", tal_count(params) ? params : NULL);
	else if (streq(method, "getblockcount"))
		start_bitcoin_cli(NULL, cmd, process_getblockcount, false, BITCOIND_HIGH_PRIO,
		                  "getblockcount", tal_count(params) ? params : NULL);
	else if (streq(method, "getblockhash"))
		start_bitcoin_cli(NULL, cmd, process_getblockhash, false, BITCOIND_LOW_PRIO,
		                  "getblockhash", tal_count(params) ? params : NULL);
	else if (streq(method, "gettxout"))
		start_bitcoin_cli(NULL, cmd, process_gettxout, false, BITCOIND_LOW_PRIO,
		                  "gettxout", tal_count(params) ? params : NULL);
	else if (streq(method, "getclientversion"))
		start_bitcoin_cli(NULL, cmd, process_getclientversion, false, BITCOIND_HIGH_PRIO,
		                  "getnetworkinfo", tal_count(params) ? params : NULL);
	else if (streq(method, "getblockchaininfo"))
		start_bitcoin_cli(NULL, cmd, process_getblockchaininfo, false, BITCOIND_HIGH_PRIO,
		                  "getblockchaininfo", tal_count(params) ? params : NULL);
	else if (streq(method, "estimatefee"))
		start_bitcoin_cli(NULL, cmd, process_estimatefee, false, BITCOIND_LOW_PRIO,
		                  "estimatesmartfee", tal_count(params) ? params : NULL);
	else if (streq(method, "sendrawtransaction"))
		start_bitcoin_cli(NULL, cmd, process_sendrawtransaction, false, BITCOIND_HIGH_PRIO,
		                  "sendrawtransaction", tal_count(params) ? params : NULL);
	else
		return command_done_err(cmd, BCLI_BADREQUEST, "Method not found", NULL);

	// FIXME Can we actually do this ?
	return command_still_pending(cmd);
}

static void destroy_bitcoind(struct bitcoind *bitcoind)
{
	/* Suppresses the callbacks from bcli_finished as we free conns. */
	bitcoind->shutdown = true;
}

static void init(struct plugin_conn *conn, const char *buffer,
                 const jsmntok_t *config)
{
	bitcoind = tal(NULL, struct bitcoind);
	bitcoind->cli = NULL;
	bitcoind->datadir = NULL;
	for (size_t i = 0; i < BITCOIND_NUM_PRIO; i++) {
		bitcoind->num_requests[i] = 0;
		list_head_init(&bitcoind->pending[i]);
	}
	list_head_init(&bitcoind->pending_getfilteredblock);
	bitcoind->shutdown = false;
	bitcoind->error_count = 0;
	bitcoind->retry_timeout = 60;
	bitcoind->rpcuser = NULL;
	bitcoind->rpcpass = NULL;
	bitcoind->rpcconnect = NULL;
	bitcoind->rpcport = NULL;
	tal_add_destructor(bitcoind, destroy_bitcoind);
}

static struct plugin_hook hooks[] = {
	{"bitcoin_data", bitcoin_data_request},
};

int main(int argc, char *argv[])
{
	/* FIXME: handle bitcoind options at init */
	plugin_main(argv, init, PLUGIN_STATIC, NULL, 0, NULL, 0, hooks,
	            ARRAY_SIZE(hooks), NULL);
}

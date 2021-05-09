/* This file was generated by generate-wire.py */
/* Do not modify this file! Modify the .csv file it was generated from. */
/* Original template can be found at tools/gen/impl_template */

#include <openingd/dualopend_wiregen.h>
#include <assert.h>
#include <ccan/array_size/array_size.h>
#include <ccan/mem/mem.h>
#include <ccan/tal/str/str.h>
#include <common/utils.h>
#include <stdio.h>

#ifndef SUPERVERBOSE
#define SUPERVERBOSE(...)
#endif


const char *dualopend_wire_name(int e)
{
	static char invalidbuf[sizeof("INVALID ") + STR_MAX_CHARS(e)];

	switch ((enum dualopend_wire)e) {
	case WIRE_DUALOPEND_INIT: return "WIRE_DUALOPEND_INIT";
	case WIRE_DUALOPEND_REINIT: return "WIRE_DUALOPEND_REINIT";
	case WIRE_DUALOPEND_GOT_OFFER: return "WIRE_DUALOPEND_GOT_OFFER";
	case WIRE_DUALOPEND_GOT_OFFER_REPLY: return "WIRE_DUALOPEND_GOT_OFFER_REPLY";
	case WIRE_DUALOPEND_GOT_RBF_OFFER: return "WIRE_DUALOPEND_GOT_RBF_OFFER";
	case WIRE_DUALOPEND_GOT_RBF_OFFER_REPLY: return "WIRE_DUALOPEND_GOT_RBF_OFFER_REPLY";
	case WIRE_DUALOPEND_RBF_VALIDATE: return "WIRE_DUALOPEND_RBF_VALIDATE";
	case WIRE_DUALOPEND_RBF_VALID: return "WIRE_DUALOPEND_RBF_VALID";
	case WIRE_DUALOPEND_RBF_INIT: return "WIRE_DUALOPEND_RBF_INIT";
	case WIRE_DUALOPEND_COMMIT_RCVD: return "WIRE_DUALOPEND_COMMIT_RCVD";
	case WIRE_DUALOPEND_PSBT_CHANGED: return "WIRE_DUALOPEND_PSBT_CHANGED";
	case WIRE_DUALOPEND_PSBT_UPDATED: return "WIRE_DUALOPEND_PSBT_UPDATED";
	case WIRE_DUALOPEND_FAIL: return "WIRE_DUALOPEND_FAIL";
	case WIRE_DUALOPEND_OPENER_INIT: return "WIRE_DUALOPEND_OPENER_INIT";
	case WIRE_DUALOPEND_FUNDING_SIGS: return "WIRE_DUALOPEND_FUNDING_SIGS";
	case WIRE_DUALOPEND_SEND_TX_SIGS: return "WIRE_DUALOPEND_SEND_TX_SIGS";
	case WIRE_DUALOPEND_TX_SIGS_SENT: return "WIRE_DUALOPEND_TX_SIGS_SENT";
	case WIRE_DUALOPEND_PEER_LOCKED: return "WIRE_DUALOPEND_PEER_LOCKED";
	case WIRE_DUALOPEND_CHANNEL_LOCKED: return "WIRE_DUALOPEND_CHANNEL_LOCKED";
	case WIRE_DUALOPEND_DEPTH_REACHED: return "WIRE_DUALOPEND_DEPTH_REACHED";
	case WIRE_DUALOPEND_SEND_SHUTDOWN: return "WIRE_DUALOPEND_SEND_SHUTDOWN";
	case WIRE_DUALOPEND_GOT_SHUTDOWN: return "WIRE_DUALOPEND_GOT_SHUTDOWN";
	case WIRE_DUALOPEND_FAIL_FALLEN_BEHIND: return "WIRE_DUALOPEND_FAIL_FALLEN_BEHIND";
	case WIRE_DUALOPEND_SHUTDOWN_COMPLETE: return "WIRE_DUALOPEND_SHUTDOWN_COMPLETE";
	case WIRE_DUALOPEND_DEV_MEMLEAK: return "WIRE_DUALOPEND_DEV_MEMLEAK";
	case WIRE_DUALOPEND_DEV_MEMLEAK_REPLY: return "WIRE_DUALOPEND_DEV_MEMLEAK_REPLY";
	}

	snprintf(invalidbuf, sizeof(invalidbuf), "INVALID %i", e);
	return invalidbuf;
}

bool dualopend_wire_is_defined(u16 type)
{
	switch ((enum dualopend_wire)type) {
	case WIRE_DUALOPEND_INIT:;
	case WIRE_DUALOPEND_REINIT:;
	case WIRE_DUALOPEND_GOT_OFFER:;
	case WIRE_DUALOPEND_GOT_OFFER_REPLY:;
	case WIRE_DUALOPEND_GOT_RBF_OFFER:;
	case WIRE_DUALOPEND_GOT_RBF_OFFER_REPLY:;
	case WIRE_DUALOPEND_RBF_VALIDATE:;
	case WIRE_DUALOPEND_RBF_VALID:;
	case WIRE_DUALOPEND_RBF_INIT:;
	case WIRE_DUALOPEND_COMMIT_RCVD:;
	case WIRE_DUALOPEND_PSBT_CHANGED:;
	case WIRE_DUALOPEND_PSBT_UPDATED:;
	case WIRE_DUALOPEND_FAIL:;
	case WIRE_DUALOPEND_OPENER_INIT:;
	case WIRE_DUALOPEND_FUNDING_SIGS:;
	case WIRE_DUALOPEND_SEND_TX_SIGS:;
	case WIRE_DUALOPEND_TX_SIGS_SENT:;
	case WIRE_DUALOPEND_PEER_LOCKED:;
	case WIRE_DUALOPEND_CHANNEL_LOCKED:;
	case WIRE_DUALOPEND_DEPTH_REACHED:;
	case WIRE_DUALOPEND_SEND_SHUTDOWN:;
	case WIRE_DUALOPEND_GOT_SHUTDOWN:;
	case WIRE_DUALOPEND_FAIL_FALLEN_BEHIND:;
	case WIRE_DUALOPEND_SHUTDOWN_COMPLETE:;
	case WIRE_DUALOPEND_DEV_MEMLEAK:;
	case WIRE_DUALOPEND_DEV_MEMLEAK_REPLY:;
	      return true;
	}
	return false;
}





/* WIRE: DUALOPEND_INIT */
u8 *towire_dualopend_init(const tal_t *ctx, const struct chainparams *chainparams, const struct feature_set *our_feature_set, const u8 *their_init_features, const struct channel_config *our_config, u32 max_to_self_delay, struct amount_msat min_effective_htlc_capacity_msat, const struct per_peer_state *pps, const struct basepoints *our_basepoints, const struct pubkey *our_funding_pubkey, u32 minimum_depth, const u8 *msg)
{
	u16 their_init_features_len = tal_count(their_init_features);
	u16 len = tal_count(msg);
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_INIT);
	/* Which network are we configured for? */
	towire_chainparams(&p, chainparams);
	towire_feature_set(&p, our_feature_set);
	towire_u16(&p, their_init_features_len);
	towire_u8_array(&p, their_init_features, their_init_features_len);
	/* Base configuration we'll offer */
	towire_channel_config(&p, our_config);
	/* Minimum/maximum configuration values we'll accept */
	towire_u32(&p, max_to_self_delay);
	towire_amount_msat(&p, min_effective_htlc_capacity_msat);
	towire_per_peer_state(&p, pps);
	towire_basepoints(&p, our_basepoints);
	towire_pubkey(&p, our_funding_pubkey);
	/* Constraints in case the other end tries to open a channel. */
	towire_u32(&p, minimum_depth);
	/* Optional msg to send. */
	towire_u16(&p, len);
	towire_u8_array(&p, msg, len);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_init(const tal_t *ctx, const void *p, const struct chainparams **chainparams, struct feature_set **our_feature_set, u8 **their_init_features, struct channel_config *our_config, u32 *max_to_self_delay, struct amount_msat *min_effective_htlc_capacity_msat, struct per_peer_state **pps, struct basepoints *our_basepoints, struct pubkey *our_funding_pubkey, u32 *minimum_depth, u8 **msg)
{
	u16 their_init_features_len;
	u16 len;

	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_INIT)
		return false;
 	/* Which network are we configured for? */
	fromwire_chainparams(&cursor, &plen, chainparams);
 	*our_feature_set = fromwire_feature_set(ctx, &cursor, &plen);
	if (!*our_feature_set)
		return NULL;
 	their_init_features_len = fromwire_u16(&cursor, &plen);
 	// 2nd case their_init_features
	*their_init_features = their_init_features_len ? tal_arr(ctx, u8, their_init_features_len) : NULL;
	fromwire_u8_array(&cursor, &plen, *their_init_features, their_init_features_len);
 	/* Base configuration we'll offer */
	fromwire_channel_config(&cursor, &plen, our_config);
 	/* Minimum/maximum configuration values we'll accept */
	*max_to_self_delay = fromwire_u32(&cursor, &plen);
 	*min_effective_htlc_capacity_msat = fromwire_amount_msat(&cursor, &plen);
 	*pps = fromwire_per_peer_state(ctx, &cursor, &plen);
	if (!*pps)
		return NULL;
 	fromwire_basepoints(&cursor, &plen, our_basepoints);
 	fromwire_pubkey(&cursor, &plen, our_funding_pubkey);
 	/* Constraints in case the other end tries to open a channel. */
	*minimum_depth = fromwire_u32(&cursor, &plen);
 	/* Optional msg to send. */
	len = fromwire_u16(&cursor, &plen);
 	// 2nd case msg
	*msg = len ? tal_arr(ctx, u8, len) : NULL;
	fromwire_u8_array(&cursor, &plen, *msg, len);
	return cursor != NULL;
}

/* WIRE: DUALOPEND_REINIT */
/* master-dualopend: peer has reconnected */
u8 *towire_dualopend_reinit(const tal_t *ctx, const struct chainparams *chainparams, const struct feature_set *our_feature_set, const u8 *their_init_features, const struct channel_config *our_config, const struct channel_config *their_config, const struct channel_id *channel_id, u32 max_to_self_delay, struct amount_msat min_effective_htlc_capacity_msat, const struct per_peer_state *pps, const struct basepoints *our_basepoints, const struct pubkey *our_funding_pubkey, const struct pubkey *their_funding_pubkey, u32 minimum_depth, const struct bitcoin_txid *funding_txid, u16 funding_txout, u32 orignal_feerate_per_kw_funding, u32 most_recent_feerate_per_kw_funding, struct amount_sat funding_satoshi, struct amount_msat our_funding, const struct basepoints *their_basepoints, const struct pubkey *remote_per_commit, const struct wally_psbt *funding_psbt, enum side opener, bool local_funding_locked, bool remote_funding_locked, bool send_shutdown, bool remote_shutdown_received, const u8 *local_shutdown_scriptpubkey, const u8 *remote_shutdown_scriptpubkey, bool remote_funding_sigs_received, const struct fee_states *fee_states, u8 channel_flags, const u8 *msg)
{
	u16 their_init_features_len = tal_count(their_init_features);
	u16 local_shutdown_len = tal_count(local_shutdown_scriptpubkey);
	u16 remote_shutdown_len = tal_count(remote_shutdown_scriptpubkey);
	u16 len = tal_count(msg);
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_REINIT);
	towire_chainparams(&p, chainparams);
	towire_feature_set(&p, our_feature_set);
	towire_u16(&p, their_init_features_len);
	towire_u8_array(&p, their_init_features, their_init_features_len);
	towire_channel_config(&p, our_config);
	towire_channel_config(&p, their_config);
	towire_channel_id(&p, channel_id);
	towire_u32(&p, max_to_self_delay);
	towire_amount_msat(&p, min_effective_htlc_capacity_msat);
	towire_per_peer_state(&p, pps);
	towire_basepoints(&p, our_basepoints);
	towire_pubkey(&p, our_funding_pubkey);
	towire_pubkey(&p, their_funding_pubkey);
	towire_u32(&p, minimum_depth);
	towire_bitcoin_txid(&p, funding_txid);
	towire_u16(&p, funding_txout);
	towire_u32(&p, orignal_feerate_per_kw_funding);
	towire_u32(&p, most_recent_feerate_per_kw_funding);
	towire_amount_sat(&p, funding_satoshi);
	towire_amount_msat(&p, our_funding);
	towire_basepoints(&p, their_basepoints);
	towire_pubkey(&p, remote_per_commit);
	towire_wally_psbt(&p, funding_psbt);
	towire_side(&p, opener);
	towire_bool(&p, local_funding_locked);
	towire_bool(&p, remote_funding_locked);
	towire_bool(&p, send_shutdown);
	towire_bool(&p, remote_shutdown_received);
	towire_u16(&p, local_shutdown_len);
	towire_u8_array(&p, local_shutdown_scriptpubkey, local_shutdown_len);
	towire_u16(&p, remote_shutdown_len);
	towire_u8_array(&p, remote_shutdown_scriptpubkey, remote_shutdown_len);
	towire_bool(&p, remote_funding_sigs_received);
	towire_fee_states(&p, fee_states);
	towire_u8(&p, channel_flags);
	/* Optional msg to send. */
	towire_u16(&p, len);
	towire_u8_array(&p, msg, len);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_reinit(const tal_t *ctx, const void *p, const struct chainparams **chainparams, struct feature_set **our_feature_set, u8 **their_init_features, struct channel_config *our_config, struct channel_config *their_config, struct channel_id *channel_id, u32 *max_to_self_delay, struct amount_msat *min_effective_htlc_capacity_msat, struct per_peer_state **pps, struct basepoints *our_basepoints, struct pubkey *our_funding_pubkey, struct pubkey *their_funding_pubkey, u32 *minimum_depth, struct bitcoin_txid *funding_txid, u16 *funding_txout, u32 *orignal_feerate_per_kw_funding, u32 *most_recent_feerate_per_kw_funding, struct amount_sat *funding_satoshi, struct amount_msat *our_funding, struct basepoints *their_basepoints, struct pubkey *remote_per_commit, struct wally_psbt **funding_psbt, enum side *opener, bool *local_funding_locked, bool *remote_funding_locked, bool *send_shutdown, bool *remote_shutdown_received, u8 **local_shutdown_scriptpubkey, u8 **remote_shutdown_scriptpubkey, bool *remote_funding_sigs_received, struct fee_states **fee_states, u8 *channel_flags, u8 **msg)
{
	u16 their_init_features_len;
	u16 local_shutdown_len;
	u16 remote_shutdown_len;
	u16 len;

	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_REINIT)
		return false;
 	fromwire_chainparams(&cursor, &plen, chainparams);
 	*our_feature_set = fromwire_feature_set(ctx, &cursor, &plen);
	if (!*our_feature_set)
		return NULL;
 	their_init_features_len = fromwire_u16(&cursor, &plen);
 	// 2nd case their_init_features
	*their_init_features = their_init_features_len ? tal_arr(ctx, u8, their_init_features_len) : NULL;
	fromwire_u8_array(&cursor, &plen, *their_init_features, their_init_features_len);
 	fromwire_channel_config(&cursor, &plen, our_config);
 	fromwire_channel_config(&cursor, &plen, their_config);
 	fromwire_channel_id(&cursor, &plen, channel_id);
 	*max_to_self_delay = fromwire_u32(&cursor, &plen);
 	*min_effective_htlc_capacity_msat = fromwire_amount_msat(&cursor, &plen);
 	*pps = fromwire_per_peer_state(ctx, &cursor, &plen);
	if (!*pps)
		return NULL;
 	fromwire_basepoints(&cursor, &plen, our_basepoints);
 	fromwire_pubkey(&cursor, &plen, our_funding_pubkey);
 	fromwire_pubkey(&cursor, &plen, their_funding_pubkey);
 	*minimum_depth = fromwire_u32(&cursor, &plen);
 	fromwire_bitcoin_txid(&cursor, &plen, funding_txid);
 	*funding_txout = fromwire_u16(&cursor, &plen);
 	*orignal_feerate_per_kw_funding = fromwire_u32(&cursor, &plen);
 	*most_recent_feerate_per_kw_funding = fromwire_u32(&cursor, &plen);
 	*funding_satoshi = fromwire_amount_sat(&cursor, &plen);
 	*our_funding = fromwire_amount_msat(&cursor, &plen);
 	fromwire_basepoints(&cursor, &plen, their_basepoints);
 	fromwire_pubkey(&cursor, &plen, remote_per_commit);
 	*funding_psbt = fromwire_wally_psbt(ctx, &cursor, &plen);
	if (!*funding_psbt)
		return NULL;
 	*opener = fromwire_side(&cursor, &plen);
 	*local_funding_locked = fromwire_bool(&cursor, &plen);
 	*remote_funding_locked = fromwire_bool(&cursor, &plen);
 	*send_shutdown = fromwire_bool(&cursor, &plen);
 	*remote_shutdown_received = fromwire_bool(&cursor, &plen);
 	local_shutdown_len = fromwire_u16(&cursor, &plen);
 	// 2nd case local_shutdown_scriptpubkey
	*local_shutdown_scriptpubkey = local_shutdown_len ? tal_arr(ctx, u8, local_shutdown_len) : NULL;
	fromwire_u8_array(&cursor, &plen, *local_shutdown_scriptpubkey, local_shutdown_len);
 	remote_shutdown_len = fromwire_u16(&cursor, &plen);
 	// 2nd case remote_shutdown_scriptpubkey
	*remote_shutdown_scriptpubkey = remote_shutdown_len ? tal_arr(ctx, u8, remote_shutdown_len) : NULL;
	fromwire_u8_array(&cursor, &plen, *remote_shutdown_scriptpubkey, remote_shutdown_len);
 	*remote_funding_sigs_received = fromwire_bool(&cursor, &plen);
 	*fee_states = fromwire_fee_states(ctx, &cursor, &plen);
	if (!*fee_states)
		return NULL;
 	*channel_flags = fromwire_u8(&cursor, &plen);
 	/* Optional msg to send. */
	len = fromwire_u16(&cursor, &plen);
 	// 2nd case msg
	*msg = len ? tal_arr(ctx, u8, len) : NULL;
	fromwire_u8_array(&cursor, &plen, *msg, len);
	return cursor != NULL;
}

/* WIRE: DUALOPEND_GOT_OFFER */
/* dualopend->master: they offered channel */
u8 *towire_dualopend_got_offer(const tal_t *ctx, const struct channel_id *channel_id, struct amount_sat opener_funding, struct amount_sat dust_limit_satoshis, struct amount_msat max_htlc_value_in_flight_msat, struct amount_msat htlc_minimum_msat, u32 feerate_per_kw_funding, u32 feerate_per_kw_commitment, u16 to_self_delay, u16 max_accepted_htlcs, u8 channel_flags, u32 locktime, const u8 *shutdown_scriptpubkey)
{
	u16 shutdown_len = tal_count(shutdown_scriptpubkey);
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_GOT_OFFER);
	towire_channel_id(&p, channel_id);
	towire_amount_sat(&p, opener_funding);
	towire_amount_sat(&p, dust_limit_satoshis);
	towire_amount_msat(&p, max_htlc_value_in_flight_msat);
	towire_amount_msat(&p, htlc_minimum_msat);
	towire_u32(&p, feerate_per_kw_funding);
	towire_u32(&p, feerate_per_kw_commitment);
	towire_u16(&p, to_self_delay);
	towire_u16(&p, max_accepted_htlcs);
	towire_u8(&p, channel_flags);
	towire_u32(&p, locktime);
	towire_u16(&p, shutdown_len);
	towire_u8_array(&p, shutdown_scriptpubkey, shutdown_len);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_got_offer(const tal_t *ctx, const void *p, struct channel_id *channel_id, struct amount_sat *opener_funding, struct amount_sat *dust_limit_satoshis, struct amount_msat *max_htlc_value_in_flight_msat, struct amount_msat *htlc_minimum_msat, u32 *feerate_per_kw_funding, u32 *feerate_per_kw_commitment, u16 *to_self_delay, u16 *max_accepted_htlcs, u8 *channel_flags, u32 *locktime, u8 **shutdown_scriptpubkey)
{
	u16 shutdown_len;

	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_GOT_OFFER)
		return false;
 	fromwire_channel_id(&cursor, &plen, channel_id);
 	*opener_funding = fromwire_amount_sat(&cursor, &plen);
 	*dust_limit_satoshis = fromwire_amount_sat(&cursor, &plen);
 	*max_htlc_value_in_flight_msat = fromwire_amount_msat(&cursor, &plen);
 	*htlc_minimum_msat = fromwire_amount_msat(&cursor, &plen);
 	*feerate_per_kw_funding = fromwire_u32(&cursor, &plen);
 	*feerate_per_kw_commitment = fromwire_u32(&cursor, &plen);
 	*to_self_delay = fromwire_u16(&cursor, &plen);
 	*max_accepted_htlcs = fromwire_u16(&cursor, &plen);
 	*channel_flags = fromwire_u8(&cursor, &plen);
 	*locktime = fromwire_u32(&cursor, &plen);
 	shutdown_len = fromwire_u16(&cursor, &plen);
 	// 2nd case shutdown_scriptpubkey
	*shutdown_scriptpubkey = shutdown_len ? tal_arr(ctx, u8, shutdown_len) : NULL;
	fromwire_u8_array(&cursor, &plen, *shutdown_scriptpubkey, shutdown_len);
	return cursor != NULL;
}

/* WIRE: DUALOPEND_GOT_OFFER_REPLY */
/* master->dualopend: reply back with our first funding info/contribs */
u8 *towire_dualopend_got_offer_reply(const tal_t *ctx, struct amount_sat accepter_funding, const struct wally_psbt *psbt, const u8 *our_shutdown_scriptpubkey)
{
	u16 shutdown_len = tal_count(our_shutdown_scriptpubkey);
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_GOT_OFFER_REPLY);
	towire_amount_sat(&p, accepter_funding);
	towire_wally_psbt(&p, psbt);
	towire_u16(&p, shutdown_len);
	towire_u8_array(&p, our_shutdown_scriptpubkey, shutdown_len);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_got_offer_reply(const tal_t *ctx, const void *p, struct amount_sat *accepter_funding, struct wally_psbt **psbt, u8 **our_shutdown_scriptpubkey)
{
	u16 shutdown_len;

	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_GOT_OFFER_REPLY)
		return false;
 	*accepter_funding = fromwire_amount_sat(&cursor, &plen);
 	*psbt = fromwire_wally_psbt(ctx, &cursor, &plen);
	if (!*psbt)
		return NULL;
 	shutdown_len = fromwire_u16(&cursor, &plen);
 	// 2nd case our_shutdown_scriptpubkey
	*our_shutdown_scriptpubkey = shutdown_len ? tal_arr(ctx, u8, shutdown_len) : NULL;
	fromwire_u8_array(&cursor, &plen, *our_shutdown_scriptpubkey, shutdown_len);
	return cursor != NULL;
}

/* WIRE: DUALOPEND_GOT_RBF_OFFER */
/* dualopend->master: they offered a RBF */
u8 *towire_dualopend_got_rbf_offer(const tal_t *ctx, const struct channel_id *channel_id, struct amount_sat their_funding, u32 funding_feerate_per_kw, u32 locktime)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_GOT_RBF_OFFER);
	towire_channel_id(&p, channel_id);
	towire_amount_sat(&p, their_funding);
	towire_u32(&p, funding_feerate_per_kw);
	towire_u32(&p, locktime);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_got_rbf_offer(const void *p, struct channel_id *channel_id, struct amount_sat *their_funding, u32 *funding_feerate_per_kw, u32 *locktime)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_GOT_RBF_OFFER)
		return false;
 	fromwire_channel_id(&cursor, &plen, channel_id);
 	*their_funding = fromwire_amount_sat(&cursor, &plen);
 	*funding_feerate_per_kw = fromwire_u32(&cursor, &plen);
 	*locktime = fromwire_u32(&cursor, &plen);
	return cursor != NULL;
}

/* WIRE: DUALOPEND_GOT_RBF_OFFER_REPLY */
/* master->dualopend: reply back with our funding info/contribs */
u8 *towire_dualopend_got_rbf_offer_reply(const tal_t *ctx, struct amount_sat our_funding, const struct wally_psbt *psbt)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_GOT_RBF_OFFER_REPLY);
	towire_amount_sat(&p, our_funding);
	towire_wally_psbt(&p, psbt);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_got_rbf_offer_reply(const tal_t *ctx, const void *p, struct amount_sat *our_funding, struct wally_psbt **psbt)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_GOT_RBF_OFFER_REPLY)
		return false;
 	*our_funding = fromwire_amount_sat(&cursor, &plen);
 	*psbt = fromwire_wally_psbt(ctx, &cursor, &plen);
	if (!*psbt)
		return NULL;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_RBF_VALIDATE */
/* dualopend->master: is this a valid RBF candidate transaction? */
u8 *towire_dualopend_rbf_validate(const tal_t *ctx, const struct wally_psbt *proposed_funding_psbt)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_RBF_VALIDATE);
	towire_wally_psbt(&p, proposed_funding_psbt);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_rbf_validate(const tal_t *ctx, const void *p, struct wally_psbt **proposed_funding_psbt)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_RBF_VALIDATE)
		return false;
 	*proposed_funding_psbt = fromwire_wally_psbt(ctx, &cursor, &plen);
	if (!*proposed_funding_psbt)
		return NULL;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_RBF_VALID */
/* master->dualopend: this is a valid RBF candidate transaction */
u8 *towire_dualopend_rbf_valid(const tal_t *ctx)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_RBF_VALID);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_rbf_valid(const void *p)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_RBF_VALID)
		return false;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_RBF_INIT */
/* master->dualopend: attempt an RBF */
u8 *towire_dualopend_rbf_init(const tal_t *ctx, struct amount_sat our_funding, const struct wally_psbt *psbt)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_RBF_INIT);
	towire_amount_sat(&p, our_funding);
	towire_wally_psbt(&p, psbt);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_rbf_init(const tal_t *ctx, const void *p, struct amount_sat *our_funding, struct wally_psbt **psbt)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_RBF_INIT)
		return false;
 	*our_funding = fromwire_amount_sat(&cursor, &plen);
 	*psbt = fromwire_wally_psbt(ctx, &cursor, &plen);
	if (!*psbt)
		return NULL;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_COMMIT_RCVD */
/* dualopend->master: ready to commit channel open to database and */
/*                    get some signatures for the funding_tx. */
u8 *towire_dualopend_commit_rcvd(const tal_t *ctx, const struct channel_config *their_config, const struct bitcoin_tx *remote_first_commit, const struct penalty_base *pbase, const struct bitcoin_signature *first_commit_sig, const struct wally_psbt *psbt, const struct pubkey *revocation_basepoint, const struct pubkey *payment_basepoint, const struct pubkey *htlc_basepoint, const struct pubkey *delayed_payment_basepoint, const struct pubkey *their_per_commit_point, const struct pubkey *remote_fundingkey, const struct bitcoin_txid *funding_txid, u16 funding_txout, struct amount_sat funding_satoshis, struct amount_sat our_funding_sats, u8 channel_flags, u32 feerate_per_kw_funding, u32 feerate_per_kw_commitment, const u8 *local_shutdown_scriptpubkey, const u8 *remote_shutdown_scriptpubkey)
{
	u16 local_shutdown_len = tal_count(local_shutdown_scriptpubkey);
	u16 remote_shutdown_len = tal_count(remote_shutdown_scriptpubkey);
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_COMMIT_RCVD);
	towire_channel_config(&p, their_config);
	towire_bitcoin_tx(&p, remote_first_commit);
	if (!pbase)
		towire_bool(&p, false);
	else {
		towire_bool(&p, true);
		towire_penalty_base(&p, pbase);
	}
	towire_bitcoin_signature(&p, first_commit_sig);
	towire_wally_psbt(&p, psbt);
	towire_pubkey(&p, revocation_basepoint);
	towire_pubkey(&p, payment_basepoint);
	towire_pubkey(&p, htlc_basepoint);
	towire_pubkey(&p, delayed_payment_basepoint);
	towire_pubkey(&p, their_per_commit_point);
	towire_pubkey(&p, remote_fundingkey);
	towire_bitcoin_txid(&p, funding_txid);
	towire_u16(&p, funding_txout);
	towire_amount_sat(&p, funding_satoshis);
	towire_amount_sat(&p, our_funding_sats);
	towire_u8(&p, channel_flags);
	towire_u32(&p, feerate_per_kw_funding);
	towire_u32(&p, feerate_per_kw_commitment);
	towire_u16(&p, local_shutdown_len);
	towire_u8_array(&p, local_shutdown_scriptpubkey, local_shutdown_len);
	towire_u16(&p, remote_shutdown_len);
	towire_u8_array(&p, remote_shutdown_scriptpubkey, remote_shutdown_len);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_commit_rcvd(const tal_t *ctx, const void *p, struct channel_config *their_config, struct bitcoin_tx **remote_first_commit, struct penalty_base **pbase, struct bitcoin_signature *first_commit_sig, struct wally_psbt **psbt, struct pubkey *revocation_basepoint, struct pubkey *payment_basepoint, struct pubkey *htlc_basepoint, struct pubkey *delayed_payment_basepoint, struct pubkey *their_per_commit_point, struct pubkey *remote_fundingkey, struct bitcoin_txid *funding_txid, u16 *funding_txout, struct amount_sat *funding_satoshis, struct amount_sat *our_funding_sats, u8 *channel_flags, u32 *feerate_per_kw_funding, u32 *feerate_per_kw_commitment, u8 **local_shutdown_scriptpubkey, u8 **remote_shutdown_scriptpubkey)
{
	u16 local_shutdown_len;
	u16 remote_shutdown_len;

	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_COMMIT_RCVD)
		return false;
 	fromwire_channel_config(&cursor, &plen, their_config);
 	*remote_first_commit = fromwire_bitcoin_tx(ctx, &cursor, &plen);
	if (!*remote_first_commit)
		return NULL;
 	if (!fromwire_bool(&cursor, &plen))
		*pbase = NULL;
	else {
		*pbase = tal(ctx, struct penalty_base);
		fromwire_penalty_base(&cursor, &plen, *pbase);
	}
 	fromwire_bitcoin_signature(&cursor, &plen, first_commit_sig);
 	*psbt = fromwire_wally_psbt(ctx, &cursor, &plen);
	if (!*psbt)
		return NULL;
 	fromwire_pubkey(&cursor, &plen, revocation_basepoint);
 	fromwire_pubkey(&cursor, &plen, payment_basepoint);
 	fromwire_pubkey(&cursor, &plen, htlc_basepoint);
 	fromwire_pubkey(&cursor, &plen, delayed_payment_basepoint);
 	fromwire_pubkey(&cursor, &plen, their_per_commit_point);
 	fromwire_pubkey(&cursor, &plen, remote_fundingkey);
 	fromwire_bitcoin_txid(&cursor, &plen, funding_txid);
 	*funding_txout = fromwire_u16(&cursor, &plen);
 	*funding_satoshis = fromwire_amount_sat(&cursor, &plen);
 	*our_funding_sats = fromwire_amount_sat(&cursor, &plen);
 	*channel_flags = fromwire_u8(&cursor, &plen);
 	*feerate_per_kw_funding = fromwire_u32(&cursor, &plen);
 	*feerate_per_kw_commitment = fromwire_u32(&cursor, &plen);
 	local_shutdown_len = fromwire_u16(&cursor, &plen);
 	// 2nd case local_shutdown_scriptpubkey
	*local_shutdown_scriptpubkey = local_shutdown_len ? tal_arr(ctx, u8, local_shutdown_len) : NULL;
	fromwire_u8_array(&cursor, &plen, *local_shutdown_scriptpubkey, local_shutdown_len);
 	remote_shutdown_len = fromwire_u16(&cursor, &plen);
 	// 2nd case remote_shutdown_scriptpubkey
	*remote_shutdown_scriptpubkey = remote_shutdown_len ? tal_arr(ctx, u8, remote_shutdown_len) : NULL;
	fromwire_u8_array(&cursor, &plen, *remote_shutdown_scriptpubkey, remote_shutdown_len);
	return cursor != NULL;
}

/* WIRE: DUALOPEND_PSBT_CHANGED */
/* dualopend->master: peer updated the psbt */
u8 *towire_dualopend_psbt_changed(const tal_t *ctx, const struct channel_id *channel_id, u64 funding_serial, const struct wally_psbt *psbt)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_PSBT_CHANGED);
	towire_channel_id(&p, channel_id);
	towire_u64(&p, funding_serial);
	towire_wally_psbt(&p, psbt);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_psbt_changed(const tal_t *ctx, const void *p, struct channel_id *channel_id, u64 *funding_serial, struct wally_psbt **psbt)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_PSBT_CHANGED)
		return false;
 	fromwire_channel_id(&cursor, &plen, channel_id);
 	*funding_serial = fromwire_u64(&cursor, &plen);
 	*psbt = fromwire_wally_psbt(ctx, &cursor, &plen);
	if (!*psbt)
		return NULL;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_PSBT_UPDATED */
/* master->dualopend: we updated the psbt */
u8 *towire_dualopend_psbt_updated(const tal_t *ctx, const struct wally_psbt *psbt)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_PSBT_UPDATED);
	towire_wally_psbt(&p, psbt);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_psbt_updated(const tal_t *ctx, const void *p, struct wally_psbt **psbt)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_PSBT_UPDATED)
		return false;
 	*psbt = fromwire_wally_psbt(ctx, &cursor, &plen);
	if (!*psbt)
		return NULL;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_FAIL */
/* master->dualopend: fail this channel open */
u8 *towire_dualopend_fail(const tal_t *ctx, const wirestring *reason)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_FAIL);
	towire_wirestring(&p, reason);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_fail(const tal_t *ctx, const void *p, wirestring **reason)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_FAIL)
		return false;
 	*reason = fromwire_wirestring(ctx, &cursor, &plen);
	if (!*reason)
		return NULL;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_OPENER_INIT */
/* master->dualopend: hello */
u8 *towire_dualopend_opener_init(const tal_t *ctx, const struct wally_psbt *psbt, struct amount_sat funding_amount, const u8 *local_shutdown_scriptpubkey, u32 feerate_per_kw, u32 feerate_per_kw_funding, u8 channel_flags)
{
	u16 local_shutdown_len = tal_count(local_shutdown_scriptpubkey);
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_OPENER_INIT);
	towire_wally_psbt(&p, psbt);
	towire_amount_sat(&p, funding_amount);
	towire_u16(&p, local_shutdown_len);
	towire_u8_array(&p, local_shutdown_scriptpubkey, local_shutdown_len);
	towire_u32(&p, feerate_per_kw);
	towire_u32(&p, feerate_per_kw_funding);
	towire_u8(&p, channel_flags);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_opener_init(const tal_t *ctx, const void *p, struct wally_psbt **psbt, struct amount_sat *funding_amount, u8 **local_shutdown_scriptpubkey, u32 *feerate_per_kw, u32 *feerate_per_kw_funding, u8 *channel_flags)
{
	u16 local_shutdown_len;

	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_OPENER_INIT)
		return false;
 	*psbt = fromwire_wally_psbt(ctx, &cursor, &plen);
	if (!*psbt)
		return NULL;
 	*funding_amount = fromwire_amount_sat(&cursor, &plen);
 	local_shutdown_len = fromwire_u16(&cursor, &plen);
 	// 2nd case local_shutdown_scriptpubkey
	*local_shutdown_scriptpubkey = local_shutdown_len ? tal_arr(ctx, u8, local_shutdown_len) : NULL;
	fromwire_u8_array(&cursor, &plen, *local_shutdown_scriptpubkey, local_shutdown_len);
 	*feerate_per_kw = fromwire_u32(&cursor, &plen);
 	*feerate_per_kw_funding = fromwire_u32(&cursor, &plen);
 	*channel_flags = fromwire_u8(&cursor, &plen);
	return cursor != NULL;
}

/* WIRE: DUALOPEND_FUNDING_SIGS */
/* dualopend->master received tx_sigs from peer */
u8 *towire_dualopend_funding_sigs(const tal_t *ctx, const struct wally_psbt *signed_psbt)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_FUNDING_SIGS);
	towire_wally_psbt(&p, signed_psbt);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_funding_sigs(const tal_t *ctx, const void *p, struct wally_psbt **signed_psbt)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_FUNDING_SIGS)
		return false;
 	*signed_psbt = fromwire_wally_psbt(ctx, &cursor, &plen);
	if (!*signed_psbt)
		return NULL;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_SEND_TX_SIGS */
/* master->dualopend send our tx_sigs to peer */
u8 *towire_dualopend_send_tx_sigs(const tal_t *ctx, const struct wally_psbt *signed_psbt)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_SEND_TX_SIGS);
	towire_wally_psbt(&p, signed_psbt);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_send_tx_sigs(const tal_t *ctx, const void *p, struct wally_psbt **signed_psbt)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_SEND_TX_SIGS)
		return false;
 	*signed_psbt = fromwire_wally_psbt(ctx, &cursor, &plen);
	if (!*signed_psbt)
		return NULL;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_TX_SIGS_SENT */
/* dualopend->master tx sigs transmitted to peer */
u8 *towire_dualopend_tx_sigs_sent(const tal_t *ctx)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_TX_SIGS_SENT);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_tx_sigs_sent(const void *p)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_TX_SIGS_SENT)
		return false;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_PEER_LOCKED */
/* dualopend->peer peer locked channel */
u8 *towire_dualopend_peer_locked(const tal_t *ctx, const struct pubkey *remote_per_commit)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_PEER_LOCKED);
	towire_pubkey(&p, remote_per_commit);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_peer_locked(const void *p, struct pubkey *remote_per_commit)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_PEER_LOCKED)
		return false;
 	fromwire_pubkey(&cursor, &plen, remote_per_commit);
	return cursor != NULL;
}

/* WIRE: DUALOPEND_CHANNEL_LOCKED */
/* dualopend->master this channel has been locked */
u8 *towire_dualopend_channel_locked(const tal_t *ctx, const struct per_peer_state *pps)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_CHANNEL_LOCKED);
	towire_per_peer_state(&p, pps);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_channel_locked(const tal_t *ctx, const void *p, struct per_peer_state **pps)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_CHANNEL_LOCKED)
		return false;
 	*pps = fromwire_per_peer_state(ctx, &cursor, &plen);
	if (!*pps)
		return NULL;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_DEPTH_REACHED */
/* master->dualopend funding reached depth; tell peer */
u8 *towire_dualopend_depth_reached(const tal_t *ctx, u32 depth)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_DEPTH_REACHED);
	towire_u32(&p, depth);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_depth_reached(const void *p, u32 *depth)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_DEPTH_REACHED)
		return false;
 	*depth = fromwire_u32(&cursor, &plen);
	return cursor != NULL;
}

/* WIRE: DUALOPEND_SEND_SHUTDOWN */
/* Tell peer to shut down channel. */
u8 *towire_dualopend_send_shutdown(const tal_t *ctx, const u8 *shutdown_scriptpubkey)
{
	u16 shutdown_scriptpubkey_len = tal_count(shutdown_scriptpubkey);
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_SEND_SHUTDOWN);
	towire_u16(&p, shutdown_scriptpubkey_len);
	towire_u8_array(&p, shutdown_scriptpubkey, shutdown_scriptpubkey_len);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_send_shutdown(const tal_t *ctx, const void *p, u8 **shutdown_scriptpubkey)
{
	u16 shutdown_scriptpubkey_len;

	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_SEND_SHUTDOWN)
		return false;
 	shutdown_scriptpubkey_len = fromwire_u16(&cursor, &plen);
 	// 2nd case shutdown_scriptpubkey
	*shutdown_scriptpubkey = shutdown_scriptpubkey_len ? tal_arr(ctx, u8, shutdown_scriptpubkey_len) : NULL;
	fromwire_u8_array(&cursor, &plen, *shutdown_scriptpubkey, shutdown_scriptpubkey_len);
	return cursor != NULL;
}

/* WIRE: DUALOPEND_GOT_SHUTDOWN */
/* Peer told us that channel is shutting down */
u8 *towire_dualopend_got_shutdown(const tal_t *ctx, const u8 *scriptpubkey)
{
	u16 scriptpubkey_len = tal_count(scriptpubkey);
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_GOT_SHUTDOWN);
	towire_u16(&p, scriptpubkey_len);
	towire_u8_array(&p, scriptpubkey, scriptpubkey_len);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_got_shutdown(const tal_t *ctx, const void *p, u8 **scriptpubkey)
{
	u16 scriptpubkey_len;

	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_GOT_SHUTDOWN)
		return false;
 	scriptpubkey_len = fromwire_u16(&cursor, &plen);
 	// 2nd case scriptpubkey
	*scriptpubkey = scriptpubkey_len ? tal_arr(ctx, u8, scriptpubkey_len) : NULL;
	fromwire_u8_array(&cursor, &plen, *scriptpubkey, scriptpubkey_len);
	return cursor != NULL;
}

/* WIRE: DUALOPEND_FAIL_FALLEN_BEHIND */
/* Peer presented proof it was from the future. */
u8 *towire_dualopend_fail_fallen_behind(const tal_t *ctx)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_FAIL_FALLEN_BEHIND);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_fail_fallen_behind(const void *p)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_FAIL_FALLEN_BEHIND)
		return false;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_SHUTDOWN_COMPLETE */
/* Shutdown is complete */
u8 *towire_dualopend_shutdown_complete(const tal_t *ctx, const struct per_peer_state *per_peer_state)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_SHUTDOWN_COMPLETE);
	towire_per_peer_state(&p, per_peer_state);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_shutdown_complete(const tal_t *ctx, const void *p, struct per_peer_state **per_peer_state)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_SHUTDOWN_COMPLETE)
		return false;
 	*per_peer_state = fromwire_per_peer_state(ctx, &cursor, &plen);
	if (!*per_peer_state)
		return NULL;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_DEV_MEMLEAK */
/* master -> dualopend: do you have a memleak? */
u8 *towire_dualopend_dev_memleak(const tal_t *ctx)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_DEV_MEMLEAK);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_dev_memleak(const void *p)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_DEV_MEMLEAK)
		return false;
	return cursor != NULL;
}

/* WIRE: DUALOPEND_DEV_MEMLEAK_REPLY */
u8 *towire_dualopend_dev_memleak_reply(const tal_t *ctx, bool leak)
{
	u8 *p = tal_arr(ctx, u8, 0);

	towire_u16(&p, WIRE_DUALOPEND_DEV_MEMLEAK_REPLY);
	towire_bool(&p, leak);

	return memcheck(p, tal_count(p));
}
bool fromwire_dualopend_dev_memleak_reply(const void *p, bool *leak)
{
	const u8 *cursor = p;
	size_t plen = tal_count(p);

	if (fromwire_u16(&cursor, &plen) != WIRE_DUALOPEND_DEV_MEMLEAK_REPLY)
		return false;
 	*leak = fromwire_bool(&cursor, &plen);
	return cursor != NULL;
}
// SHA256STAMP:ac87526130110ceb0c1a9cc168d1574a5e3be96678d6420c998be7ab53ea1087

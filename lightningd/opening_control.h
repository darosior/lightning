#ifndef LIGHTNING_LIGHTNINGD_OPENING_CONTROL_H
#define LIGHTNING_LIGHTNINGD_OPENING_CONTROL_H
#include "config.h"
#include <ccan/short_types/short_types.h>

struct channel_id;
struct crypto_state;
struct json_stream;
struct lightningd;
struct per_peer_state;
struct uncommitted_channel;

void json_add_uncommitted_channel(struct json_stream *response,
				  const struct uncommitted_channel *uc);

void peer_start_openingd(struct peer *peer,
			 struct per_peer_state *pps,
			 const u8 *msg);

void kill_uncommitted_channel(struct uncommitted_channel *uc,
			      const char *why);

#if DEVELOPER
struct command;
/* Calls report_leak_info() async. */
void opening_dev_memleak(struct command *cmd);
#endif

#endif /* LIGHTNING_LIGHTNINGD_OPENING_CONTROL_H */

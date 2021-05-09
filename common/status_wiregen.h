/* This file was generated by generate-wire.py */
/* Do not modify this file! Modify the _csv file it was generated from. */
/* Original template can be found at tools/gen/header_template */

#ifndef LIGHTNING_COMMON_STATUS_WIREGEN_H
#define LIGHTNING_COMMON_STATUS_WIREGEN_H
#include <ccan/tal/tal.h>
#include <wire/tlvstream.h>
#include <wire/wire.h>
#include <common/per_peer_state.h>
#include <common/node_id.h>
#include <common/status_wire.h>

enum status_wire {
        WIRE_STATUS_LOG = 0xFFF0,
        WIRE_STATUS_IO = 0xFFF1,
        WIRE_STATUS_FAIL = 0xFFF2,
        WIRE_STATUS_PEER_CONNECTION_LOST = 0xFFF3,
        WIRE_STATUS_PEER_BILLBOARD = 0xFFF5,
        WIRE_STATUS_VERSION = 0xFFF6,
};

const char *status_wire_name(int e);

/**
 * Determine whether a given message type is defined as a message.
 *
 * Returns true if the message type is part of the message definitions we have
 * generated parsers for, false if it is a custom message that cannot be
 * handled internally.
 */
bool status_wire_is_defined(u16 type);


/* WIRE: STATUS_LOG */
u8 *towire_status_log(const tal_t *ctx, enum log_level level, const struct node_id *peer, const wirestring *entry);
bool fromwire_status_log(const tal_t *ctx, const void *p, enum log_level *level, struct node_id **peer, wirestring **entry);

/* WIRE: STATUS_IO */
u8 *towire_status_io(const tal_t *ctx, enum log_level iodir, const struct node_id *peer, const wirestring *who, const u8 *data);
bool fromwire_status_io(const tal_t *ctx, const void *p, enum log_level *iodir, struct node_id **peer, wirestring **who, u8 **data);

/* WIRE: STATUS_FAIL */
u8 *towire_status_fail(const tal_t *ctx, enum status_failreason failreason, const wirestring *desc);
bool fromwire_status_fail(const tal_t *ctx, const void *p, enum status_failreason *failreason, wirestring **desc);

/* WIRE: STATUS_PEER_CONNECTION_LOST */
u8 *towire_status_peer_connection_lost(const tal_t *ctx);
bool fromwire_status_peer_connection_lost(const void *p);

/* WIRE: STATUS_PEER_BILLBOARD */
u8 *towire_status_peer_billboard(const tal_t *ctx, bool perm, const wirestring *happenings);
bool fromwire_status_peer_billboard(const tal_t *ctx, const void *p, bool *perm, wirestring **happenings);

/* WIRE: STATUS_VERSION */
u8 *towire_status_version(const tal_t *ctx, const wirestring *version);
bool fromwire_status_version(const tal_t *ctx, const void *p, wirestring **version);


#endif /* LIGHTNING_COMMON_STATUS_WIREGEN_H */
// SHA256STAMP:6bc1d8b7640b2da697e1bf8b30b9fe33d2eb28a44536c9713372b53b6eb19f74

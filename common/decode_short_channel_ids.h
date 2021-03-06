#ifndef LIGHTNING_COMMON_DECODE_SHORT_CHANNEL_IDS_H
#define LIGHTNING_COMMON_DECODE_SHORT_CHANNEL_IDS_H
#include "config.h"
#include <ccan/short_types/short_types.h>
#include <ccan/tal/tal.h>
#include <common/bigsize.h>

struct tlv_query_short_channel_ids_tlvs_query_flags;

/* BOLT #7:
 *
 * Encoding types:
 * * `0`: uncompressed array of `short_channel_id` types, in ascending order.
 * * `1`: array of `short_channel_id` types, in ascending order, compressed with zlib deflate<sup>[1](#reference-1)</sup>
 */
enum scid_encode_types {
	SHORTIDS_UNCOMPRESSED = 0,
	SHORTIDS_ZLIB = 1
};

struct short_channel_id *decode_short_ids(const tal_t *ctx, const u8 *encoded);

/* BOLT-61a1365a45cc8b463ddbbe3429d350f8eac787dd #7:
 *
 * `encoded_query_flags` is an array of bitfields, one varint per bitfield,
 * one bitfield for each `short_channel_id`. Bits have the following meaning:
 *
 * | Bit Position  | Meaning                                  |
 * | ------------- | ---------------------------------------- |
 * | 0             | Sender wants `channel_announcement`      |
 * | 1             | Sender wants `channel_update` for node 1 |
 * | 2             | Sender wants `channel_update` for node 2 |
 * | 3             | Sender wants `node_announcement` for node 1 |
 * | 4             | Sender wants `node_announcement` for node 2 |
 */
enum scid_query_flag {
	SCID_QF_ANNOUNCE = 0x1,
	SCID_QF_UPDATE1 = 0x2,
	SCID_QF_UPDATE2 = 0x4,
	SCID_QF_NODE1 = 0x8,
	SCID_QF_NODE2 = 0x10,
};

#if EXPERIMENTAL_FEATURES
bigsize_t *decode_scid_query_flags(const tal_t *ctx,
				   const struct tlv_query_short_channel_ids_tlvs_query_flags *qf);
#endif /* EXPERIMENTAL_FEATURES */

#endif /* LIGHTNING_COMMON_DECODE_SHORT_CHANNEL_IDS_H */

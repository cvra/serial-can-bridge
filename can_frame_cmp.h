#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <serializer/cmp/cmp.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Writes a MessagePack encoded CAN frame.
 * @param [out] MessagePack context output.
 * @param [in] ext Extended frame flag.
 * @param [in] id CAN ID.
 * @param [in] data CAN frame data to be written.
 * @param [in] len CAN frame ata length.
 * @returns true if no error.
 */
bool can_frame_cmp_write(cmp_ctx_t *out, bool ext, uint32_t id, const void *data, uint8_t len);

/** Reads a MessagePack encoded CAN frame.
 * @param [in] MessagePack context input.
 * @param [out] ext Extended frame flag.
 * @param [out] id CAN ID.
 * @param [out] data CAN frame data output buffer, should be 8 bytes long.
 * @param [out] len CAN frame ata length.
 * @returns true if no error.
 */
bool can_frame_cmp_read(cmp_ctx_t *in, bool *ext, uint32_t *id, void *data, uint8_t *len);

#ifdef __cplusplus
}
#endif

#endif // CAN_FRAME_H

#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <serializer/cmp/cmp.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CAN_FRAME_CMP_EXT_ID_MAX    ((1<<29) - 1)
#define CAN_FRAME_CMP_STD_ID_MAX    ((1<<11) - 1)

struct can_bridge_frame {
    uint8_t ext : 1;        // extended frame flag
    uint8_t rtr : 1;        // remote transmission request
    uint8_t dlc : 4;        // data length code
    union {
        uint32_t std : 11;
        uint32_t ext : 29;
    } id;                   // 11 bit or 29 bit extended ID
    union {
        uint8_t u8[8];
        uint16_t u16[4];
        uint32_t u32[2];
    } data;
};

/** Writes a MessagePack encoded CAN frame.
 * @param [out] MessagePack context output.
 * @param [in] frame CAN frame to encode with MessagePack.
 * @returns true if no error.
 */
bool can_frame_cmp_write(cmp_ctx_t *out, struct can_bridge_frame *frame);

/** Reads a MessagePack encoded CAN frame.
 * @param [in] MessagePack context input.
 * @param [out] frame CAN frame output.
 * @returns true if no error.
 */
bool can_frame_cmp_read(cmp_ctx_t *in, struct can_bridge_frame *frame);

#ifdef __cplusplus
}
#endif

#endif // CAN_FRAME_H

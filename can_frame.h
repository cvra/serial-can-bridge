#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <cmp/cmp.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CAN_FRAME_STD_ID_MASK    ((1<<11) - 1)
#define CAN_FRAME_EXT_ID_MASK    ((1<<29) - 1)
#define CAN_FRAME_EXT_FLAG       (1<<29)
#define CAN_FRAME_RTR_FLAG       (1<<30)

/** id bit layout:
 * [0-28]:  29 bit extended id
 * ([0-10]:  11 bit basic id)
 * [29]:    extended frame flag
 * [30]:    remote transmission request flag
 * [31]:    unused, set to 0
 */

struct can_frame {
    uint32_t id;
    uint8_t dlc;
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
bool can_frame_cmp_write(cmp_ctx_t *out, struct can_frame *frame);

/** Reads a MessagePack encoded CAN frame.
 * @param [in] MessagePack context input.
 * @param [out] frame CAN frame output.
 * @returns true if no error.
 */
bool can_frame_cmp_read(cmp_ctx_t *in, struct can_frame *frame);

#ifdef __cplusplus
}
#endif

#endif // CAN_FRAME_H

#ifndef SERIAL_CAN_BRIDGE_H
#define SERIAL_CAN_BRIDGE_H

#include <stdint.h>
#include <stddef.h>
#include "can_frame_cmp.h"

#define CMD_CAN_FRAME       0x00
#define CMD_CFG_ID_FILTER   0x01

#ifdef __cplusplus
extern "C" {
#endif

/** Callback function for received datagrams
 * @param [in] input input buffer.
 * @param [in] len lenght of input buffer.
 */
void can_bridge_datagram_rcv_cb(const void *data, size_t len, void *arg);


/** Writes an encoded CAN Frame to a buffer
 * @param [in] frame CAN frame to be encoded.
 * @param [out] outbuf buffer to write to.
 * @param [in] len length of outbuf.
 * @param [out] len length of written bytes.
 * @returns true if no error.
 */
bool can_bridge_frame_write(struct can_bridge_frame *frame, uint8_t *outbuf,
    size_t *len);

#ifdef __cplusplus
}
#endif

#endif // SERIAL_CAN_BRIDGE_H

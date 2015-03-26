#ifndef SERIAL_CAN_BRIDGE_H
#define SERIAL_CAN_BRIDGE_H

#include <stdint.h>
#include <stddef.h>
#include "can_frame.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CMD_CAN_FRAME       0x00
#define CMD_CFG_ID_FILTER   0x01

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
bool can_bridge_frame_write(struct can_frame *frame, uint8_t *outbuf, size_t *len);

/* Returns true if the id passes the filter. */
bool can_bridge_id_passes_filter(uint32_t id);

#ifdef __cplusplus
}
#endif

#endif // SERIAL_CAN_BRIDGE_H

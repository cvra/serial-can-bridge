#include <serializer/serialization.h>
#include <cmp/cmp.h>
#include "can_frame_cmp.h"
#include "can_interface.h"
#include "serial_can_bridge.h"

static bool can_send_cmp_frame(cmp_ctx_t *ctx)
{
    struct can_bridge_frame frame;

    if (!can_frame_cmp_read(ctx, &frame)) {
        return false;
    }

    if (frame.ext == 1) {
        can_interface_send(true, frame.rtr == 1, frame.id.ext, frame.data.u8, frame.dlc);
    } else {
        can_interface_send(false, frame.rtr == 1, frame.id.std, frame.data.u8, frame.dlc);
    }
    return true;
}

void can_bridge_datagram_rcv_cb(const void *data, size_t len)
{
    uint32_t cmd;
    serializer_t ser;
    cmp_ctx_t ctx;
    serializer_init(&ser, (char *)data, len);
    serializer_cmp_ctx_factory(&ctx, &ser);

    // read command
    if (!cmp_read_uint(&ctx, &cmd)) {
        return;
    }

    switch (cmd) {
    case CMD_CAN_FRAME:
        can_send_cmp_frame(&ctx);
        break;
    case CMD_CFG_ID_FILTER:
        // todo
        break;
    }
}

bool can_bridge_frame_write(struct can_bridge_frame *frame, uint8_t *outbuf,
    size_t *len)
{
    serializer_t ser;
    cmp_ctx_t ctx;

    serializer_init(&ser, (char *) outbuf, *len);
    serializer_cmp_ctx_factory(&ctx, &ser);

    // encode with MessagePack
    if (!can_frame_cmp_write(&ctx, frame)) {
        return false;
    }

    *len = serializer_written_bytes_count(&ser);
    return true;
}

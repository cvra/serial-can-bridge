#include <serializer/serialization.h>
#include <cmp.h>
#include <serial-datagram/serial_datagram.h>
#include "can_frame_cmp.h"
#include "serial_interface.h"
#include "can_interface.h"
#include "serial_can_bridge.h"

#define CMD_CAN_FRAME       0x00
#define CMD_CFG_ID_FILTER   0x01


bool can_send_cmp_frame(cmp_ctx_t *ctx)
{
    bool ext;
    uint32_t id;
    uint8_t data[8];
    uint8_t len;

    if (!can_frame_cmp_read(ctx, &ext, &id, data, &len)) {
        return false;
    }
    can_interface_send(ext, id, data, len);
    return true;
}

void serial_datagram_cb(const void *data, size_t len)
{
    uint8_t cmd;
    serializer_t ser;
    cmp_ctx_t ctx;
    serializer_init(&ser, (char *)data, len);
    serializer_cmp_ctx_factory(&ctx, &ser);

    /* read command */
    cmp_read_u8(&ctx, &cmd);
    switch (cmd) {
    case CMD_CAN_FRAME:
        if (!can_send_cmp_frame(&ctx)) {
            // todo: report error
        }
        break;
    case CMD_CFG_ID_FILTER:
        // todo
        break;
    }
}

void serial_rx_main(void* arg)
{
    static char datagram_buf[1024];
    static char serial_buf[32];
    serial_datagram_rcv_handler_t rcv;

    serial_datagram_rcv_handler_init(
        &rcv,
        datagram_buf,
        sizeof(datagram_buf),
        serial_datagram_cb);

    while (1) {
        int err;
        size_t len;

        /* listen to serial connection, blocking */
        len = serial_interface_read(arg, serial_buf, sizeof(serial_buf));

        /* parse for datagram, calls serial_datagram_cb for every datagram */
        err = serial_datagram_receive(&rcv, serial_buf, len);
        if (err != SERIAL_DATAGRAM_RCV_NO_ERROR) {
            // todo: report error
        }
    }
}

void serial_tx_main(void *arg)
{
    static char outbuf[32];
    bool ext;
    uint32_t id;
    uint8_t data[8];
    uint8_t len;

    serializer_t ser;
    cmp_ctx_t ctx;

    while (1) {
        /* wait for can frame */
        can_interface_receive(&ext, &id, &data, &len);

        /* encode with MessagePack */
        serializer_init(&ser, outbuf, sizeof(outbuf));
        serializer_cmp_ctx_factory(&ctx, &ser);
        can_frame_cmp_write(&ctx, ext, id, data, len);

        /* send within a serial datagram */
        serial_datagram_send(outbuf, len, serial_interface_write, arg);
    }

}

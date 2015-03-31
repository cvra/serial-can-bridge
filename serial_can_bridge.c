#include <cmp_mem_access/cmp_mem_access.h>
#include <cmp/cmp.h>
#include "can_frame.h"
#include "can_interface.h"
#include "serial_can_bridge.h"

uint32_t can_bridge_filter_id = 0;
uint32_t can_bridge_filter_mask = 0;

static void can_bridge_set_filter(cmp_ctx_t *ctx);

static bool can_send_cmp_frame(cmp_ctx_t *ctx)
{
    struct can_frame frame;

    if (!can_frame_cmp_read(ctx, &frame)) {
        return false;
    }

    can_interface_send(&frame);

    return true;
}

void can_bridge_datagram_rcv_cb(const void *data, size_t len, void *arg)
{
    uint32_t cmd;
    cmp_mem_access_t cma;
    cmp_ctx_t ctx;
    (void) arg;

    cmp_mem_access_ro_init(&ctx, &cma, data, len);

    // read command
    if (!cmp_read_uint(&ctx, &cmd)) {
        return;
    }

    switch (cmd) {
    case CMD_CAN_FRAME:
        can_send_cmp_frame(&ctx);
        break;
    case CMD_CFG_ID_FILTER:
        can_bridge_set_filter(&ctx);
        break;
    }
}

bool can_bridge_frame_write(struct can_frame *frame, uint8_t *outbuf, size_t *len)
{
    cmp_mem_access_t cma;
    cmp_ctx_t ctx;

    cmp_mem_access_init(&ctx, &cma, outbuf, *len);

    // encode with MessagePack
    if (!can_frame_cmp_write(&ctx, frame)) {
        return false;
    }

    *len = cmp_mem_access_get_pos(&cma);
    return true;
}

static void can_bridge_set_filter(cmp_ctx_t *ctx)
{
    uint32_t id, mask, size;
    bool rtr, ide;
    if (!cmp_read_array(ctx, &size) || size != 6) {
        return;
    }
    if (!cmp_read_bool(ctx, &ide)) {
        return;
    }
    if (!cmp_read_bool(ctx, &rtr)) {
        return;
    }
    if (!cmp_read_uint(ctx, &id)) {
        return;
    }
    if (ide) {
        id = (id & CAN_FRAME_EXT_ID_MASK) | CAN_FRAME_EXT_FLAG;
    } else {
        id = id & CAN_FRAME_STD_ID_MASK;
    }
    if (rtr) {
        id |= CAN_FRAME_RTR_FLAG;
    }
    if (!cmp_read_bool(ctx, &ide)) {
        return;
    }
    if (!cmp_read_bool(ctx, &rtr)) {
        return;
    }
    if (!cmp_read_uint(ctx, &mask)) {
        return;
    }
    if (ide) {
        mask = (mask & CAN_FRAME_EXT_ID_MASK) | CAN_FRAME_EXT_FLAG;
    } else {
        mask = mask & CAN_FRAME_STD_ID_MASK;
    }
    if (rtr) {
        mask |= CAN_FRAME_RTR_FLAG;
    }
    can_bridge_filter_id = id;
    can_bridge_filter_mask = mask;
}

bool can_bridge_id_passes_filter(uint32_t id)
{
    if (((id ^ can_bridge_filter_id) & can_bridge_filter_mask) == 0) {
        return true;
    }
    return false;
}

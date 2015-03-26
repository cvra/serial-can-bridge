
#include "can_frame.h"

bool can_frame_cmp_write(cmp_ctx_t *out, struct can_frame *frame)
{
    bool ext = false;
    bool rtr = false;
    uint32_t id = frame->id;

    if (!cmp_write_array(out, 5)) {
        return false;
    }

    if ((id & CAN_FRAME_EXT_FLAG) != 0) {
        ext = true;
    }
    if ((id & CAN_FRAME_RTR_FLAG) != 0) {
        rtr = true;
    }

    // extended frame id flag
    if (!cmp_write_bool(out, ext)) {
        return false;
    }

    // remote transmission request flag
    if (!cmp_write_bool(out, rtr)) {
        return false;
    }

    // CAN id
    if (!ext) {
        if (!cmp_write_uint(out, (id & CAN_FRAME_STD_ID_MASK))) {
            return false;
        }
    } else {
        if (!cmp_write_uint(out, (id & CAN_FRAME_EXT_ID_MASK))) {
            return false;
        }
    }

    // data length code
    if (!cmp_write_uint(out, frame->dlc)) {
        return false;
    }

    // data
    if (!cmp_write_bin(out, frame->data.u8, frame->dlc)) {
        return false;
    }

    return true;
}

bool can_frame_cmp_read(cmp_ctx_t *in, struct can_frame *frame)
{
    bool ext, rtr;
    uint32_t id, length, size;

    if (!cmp_read_array(in, &size) || size != 5) {
        return false;
    }

    // extended frame flag
    if (!cmp_read_bool(in, &ext)) {
        return false;
    }

    // remote transmission request flag
    if (!cmp_read_bool(in, &rtr)) {
        return false;
    }

    // CAN id
    if (!cmp_read_uint(in, &id)) {
        return false;
    }

    if (ext) {
        id = (id & CAN_FRAME_EXT_ID_MASK) | CAN_FRAME_EXT_FLAG;
    } else {
        id = id & CAN_FRAME_STD_ID_MASK;
    }

    if (rtr) {
        id |= CAN_FRAME_RTR_FLAG;
    }

    frame->id = id;

    // data length code
    if (!cmp_read_uint(in, &length)) {
        return false;
    }
    frame->dlc = length;

    // data
    length = 8;    // read max 8 bytes
    if (!cmp_read_bin(in, &frame->data.u8, &length)) {
        return false;
    }

    // sanity check
    if (!rtr && length != frame->dlc) {
        return false;
    }

    return true;
}


#include "can_frame_cmp.h"

bool can_frame_cmp_write(cmp_ctx_t *out, struct can_bridge_frame *frame)
{
    // extended frame flag
    if (!cmp_write_bool(out, frame->ext == 1)) {
        return false;
    }

    // remote transmission request flag
    if (!cmp_write_bool(out, frame->rtr == 1)) {
        return false;
    }

    // CAN id
    if (frame->ext == 0) {
        if (!cmp_write_uint(out, frame->id.std)) {
            return false;
        }
    } else {
        if (!cmp_write_uint(out, frame->id.ext)) {
            return false;
        }
    }

    // data
    if (!cmp_write_bin(out, frame->data.u8, frame->dlc)) {
        return false;
    }

    return true;
}

bool can_frame_cmp_read(cmp_ctx_t *in, struct can_bridge_frame *frame)
{
    bool ext, rtr;
    uint32_t id;

    // extended frame flag
    if (!cmp_read_bool(in, &ext)) {
        return false;
    }

    if (ext) {
        frame->ext = 1;
    } else {
        frame->ext = 0;
    }

    // remote transmission request flag
    if (!cmp_read_bool(in, &rtr)) {
        return false;
    }

    if (rtr) {
        frame->rtr = 1;
    } else {
        frame->rtr = 0;
    }

    // CAN id
    if (!cmp_read_uint(in, &id)) {
        return false;
    }

    if (ext && id <= CAN_FRAME_CMP_EXT_ID_MAX) {
        frame->id.ext = id;
    } else if (!ext && id <= CAN_FRAME_CMP_STD_ID_MAX) {
        frame->id.std = id;
    } else {    // invalid id
        return false;
    }

    // data
    uint32_t length = 8;    // read max 8 bytes
    if (!cmp_read_bin(in, &frame->data.u8, &length)) {
        return false;
    }

    if (length > 8) {
        return false;
    } else {
        frame->dlc = length;
    }

    return true;
}

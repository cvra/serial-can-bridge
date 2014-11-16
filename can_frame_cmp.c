
#include "can_frame_cmp.h"

bool can_frame_cmp_write(cmp_ctx_t *out, bool ext, uint32_t id, const void *data, uint8_t len)
{
    bool success;

    success = cmp_write_bool(out, ext);
    if (!success) {
        return false;
    }

    success = cmp_write_uint(out, id);
    if (!success) {
        return false;
    }

    success = cmp_write_bin(out, data, len);

    return success;
}

bool can_frame_cmp_read(cmp_ctx_t *in, bool *ext, uint32_t *id, void *data, uint8_t *len)
{
    bool success;

    success = cmp_read_bool(in, ext);
    if (!success) {
        return false;
    }

    success = cmp_read_uint(in, id);
    if (!success) {
        return false;
    }

    uint32_t length = 8;    // read max 8 bytes
    success = cmp_read_bin(in, data, &length);
    *len = length;

    return success;
}

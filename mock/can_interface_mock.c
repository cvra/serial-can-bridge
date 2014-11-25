#include "../can_interface.h"
#include <string.h>
#include <stdio.h>

struct {
    bool valid;
    bool ext;
    bool rtr;
    uint32_t id;
    uint8_t data[8];
    uint8_t len;
} can_mock_log;

void can_interface_send(bool ext, bool rtr, uint32_t id, void *data, uint8_t len)
{
    can_mock_log.ext = ext;
    can_mock_log.rtr = rtr;
    can_mock_log.id = id;
    can_mock_log.len = len;
    memcpy(can_mock_log.data, data, 8);
    can_mock_log.valid = true;
}

bool can_interface_mock_status(bool *ext, bool *rtr, uint32_t *id, uint8_t *data, uint8_t *len)
{
    if (!can_mock_log.valid) {
        return false;
    }
    *ext = can_mock_log.ext;
    *rtr = can_mock_log.rtr;
    *id = can_mock_log.id;
    *len = can_mock_log.len;
    memcpy(data, can_mock_log.data, 8);
    can_mock_log.valid = false;
    return true;
}

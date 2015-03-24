#include "../can_interface.h"
#include <string.h>
#include <stdio.h>

struct {
    bool called;
    struct can_frame frame;
} can_mock_log;

void can_interface_send(struct can_frame *frame)
{
    can_mock_log.frame.id = frame->id;
    can_mock_log.frame.dlc = frame->dlc;
    memcpy(can_mock_log.frame.data.u8, frame->data.u8, 8);
    can_mock_log.called = true;
}

bool can_interface_mock_status(uint32_t *id, uint8_t *data, uint8_t *len)
{
    if (!can_mock_log.called) {
        return false;
    }
    *id = can_mock_log.frame.id;
    *len = can_mock_log.frame.dlc;
    memcpy(data, can_mock_log.frame.data.u8, 8);
    can_mock_log.called = false;
    return true;
}

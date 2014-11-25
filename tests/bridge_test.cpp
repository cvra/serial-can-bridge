#include <cstring>
#include "CppUTest/TestHarness.h"
#include "serializer/serialization.h"
#include "serial-datagram/serial_datagram.h"
#include "../can_frame_cmp.h"
#include "../serial_can_bridge.h"
#include "../mock/can_interface_mock.h"

TEST_GROUP(CANBridgeTestGroup)
{
    cmp_ctx_t context;
    serializer_t serializer;
    char buffer[42];
    uint8_t rcv_buf[42];

    void setup()
    {
        memset(buffer, 0, sizeof buffer);
        serializer_init(&serializer, buffer, sizeof buffer);
        serializer_cmp_ctx_factory(&context, &serializer);
    }
};

TEST(CANBridgeTestGroup, CanForwardCANFrame)
{
    struct can_bridge_frame write = {
        .ext = 0,
        .rtr = 0,
        .dlc = 8,
        .id.std = 123,
        .data.u8 = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}
    };

    cmp_write_uint(&context, CMD_CAN_FRAME);
    can_frame_cmp_write(&context, &write);

    size_t buf_len = serializer_written_bytes_count(&serializer);

    can_bridge_datagram_rcv_cb(buffer, buf_len);

    bool frame_received, ext, rtr;
    uint32_t id;
    uint8_t len;
    uint8_t data[8];
    frame_received = can_interface_mock_status(&ext, &rtr, &id, data, &len);

    CHECK_TRUE(frame_received);
    CHECK_EQUAL(write.ext, ext);
    CHECK_EQUAL(write.rtr, rtr);
    CHECK_EQUAL(write.id.std, id);
    CHECK_EQUAL(write.dlc, len);
    CHECK_EQUAL(0, memcmp(write.data.u8, data, write.dlc));
}

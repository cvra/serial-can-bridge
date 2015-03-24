#include <cstring>
#include "CppUTest/TestHarness.h"
#include "cmp_mem_access/cmp_mem_access.h"
#include "serial-datagram/serial_datagram.h"
#include "../can_frame.h"
#include "../serial_can_bridge.h"
#include "../mock/can_interface_mock.h"

TEST_GROUP(CANBridgeTestGroup)
{
    cmp_ctx_t context;
    cmp_mem_access_t cma;
    char buffer[42];
    uint8_t rcv_buf[42];

    void setup()
    {
        memset(buffer, 0, sizeof buffer);
        cmp_mem_access_init(&context, &cma, buffer, sizeof buffer);
    }

    void write_filter(cmp_ctx_t *ctx, uint32_t id, uint32_t mask)
    {
        bool rtr = false, ide = false;
        if ((id & CAN_FRAME_RTR_FLAG) != 0) {
            rtr = true;
        }
        if ((id & CAN_FRAME_EXT_FLAG) != 0) {
            ide = true;
            id &= CAN_FRAME_EXT_ID_MASK;
        } else {
            id &= CAN_FRAME_STD_ID_MASK;
        }
        cmp_write_array(ctx, 6);
        cmp_write_bool(ctx, ide);
        cmp_write_bool(ctx, rtr);
        cmp_write_uint(ctx, id);

        rtr = false;
        ide = false;
        if ((mask & CAN_FRAME_RTR_FLAG) != 0) {
            rtr = true;
        }
        if ((mask & CAN_FRAME_EXT_FLAG) != 0) {
            ide = true;
            mask &= CAN_FRAME_EXT_ID_MASK;
        } else {
            mask &= CAN_FRAME_STD_ID_MASK;
        }

        cmp_write_bool(ctx, ide);
        cmp_write_bool(ctx, rtr);
        cmp_write_uint(ctx, mask);
    }
};

TEST(CANBridgeTestGroup, CanForwardCANFrame)
{
    struct can_frame frame = {
        .id = 123,
        .dlc = 8,
        .data.u8 = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}
    };

    cmp_write_uint(&context, CMD_CAN_FRAME);
    can_frame_cmp_write(&context, &frame);

    size_t buf_len = cmp_mem_access_get_pos(&cma);

    can_bridge_datagram_rcv_cb(buffer, buf_len, NULL);

    bool frame_received;
    uint32_t id;
    uint8_t len, data[8];
    frame_received = can_interface_mock_status(&id, &data[0], &len);

    CHECK_TRUE(frame_received);
    CHECK_EQUAL(frame.id, id);
    CHECK_EQUAL(frame.dlc, len);
    CHECK_EQUAL(0, memcmp(frame.data.u8, data, frame.dlc));
}

TEST(CANBridgeTestGroup, CanSetFilter)
{
    bool pass;
    uint32_t id;
    uint32_t filter_id = 0xabcd | CAN_FRAME_EXT_FLAG;
    uint32_t filter_mask = 0x00ff | CAN_FRAME_EXT_FLAG;
    cmp_write_uint(&context, CMD_CFG_ID_FILTER);
    write_filter(&context, filter_id, filter_mask);

    size_t buf_len = cmp_mem_access_get_pos(&cma);
    can_bridge_datagram_rcv_cb(buffer, buf_len, NULL);

    pass = can_bridge_id_passes_filter(filter_id);
    CHECK_TRUE(pass);

    id = 0x42cd | CAN_FRAME_EXT_FLAG;
    pass = can_bridge_id_passes_filter(id);
    CHECK_TRUE(pass);

    id = 0xabcd;
    pass = can_bridge_id_passes_filter(id);
    CHECK_FALSE(pass);
}

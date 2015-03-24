#include <cstring>
#include "CppUTest/TestHarness.h"
#include "cmp_mem_access/cmp_mem_access.h"
#include "../can_frame.h"

TEST_GROUP(CANFrameTestGroup)
{
    cmp_ctx_t context;
    cmp_mem_access_t cma;
    char buffer[256];

    void setup()
    {
        memset(buffer, 0, sizeof buffer);
        cmp_mem_access_init(&context, &cma, buffer, sizeof buffer);
    }
};

TEST(CANFrameTestGroup, CanWriteStdFrame)
{
    bool success;
    struct can_frame write = {
        .dlc = 8,
        .id = (1 << 11) - 1,
        .data.u8 = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}
    };
    struct can_frame read;

    can_frame_cmp_write(&context, &write);
    cmp_mem_access_set_pos(&cma, 0);
    success = can_frame_cmp_read(&context, &read);

    CHECK_TRUE(success);
    CHECK_EQUAL(write.id, read.id);
    CHECK_EQUAL(write.dlc, read.dlc);
    CHECK_EQUAL(0, memcmp(write.data.u8, read.data.u8, write.dlc));
}

TEST(CANFrameTestGroup, CanWriteExtFrame)
{
    bool success;
    struct can_frame write = {
        .dlc = 4,
        .id = ((1<<29) - 1) | CAN_FRAME_EXT_FLAG,
        .data.u8 = "xkcd"
    };
    struct can_frame read;

    can_frame_cmp_write(&context, &write);
    cmp_mem_access_set_pos(&cma, 0);
    success = can_frame_cmp_read(&context, &read);

    CHECK_TRUE(success);
    CHECK_EQUAL(write.id, read.id);
    CHECK_EQUAL(write.dlc, read.dlc);
    CHECK_EQUAL(0, memcmp(write.data.u8, read.data.u8, write.dlc));
}

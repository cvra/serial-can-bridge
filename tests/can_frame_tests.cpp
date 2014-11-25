#include <cstring>
#include "CppUTest/TestHarness.h"
#include "serializer/serialization.h"
#include "../can_frame_cmp.h"

TEST_GROUP(CANFrameTestGroup)
{
    cmp_ctx_t context;
    serializer_t serializer;
    char buffer[256];

    void setup()
    {
        memset(buffer, 0, sizeof buffer);
        serializer_init(&serializer, buffer, sizeof buffer);
        serializer_cmp_ctx_factory(&context, &serializer);
    }
};

TEST(CANFrameTestGroup, CanWriteStdFrame)
{
    bool success;
    struct can_bridge_frame write = {
        .ext = 0,
        .rtr = 0,
        .dlc = 8,
        .id.std = (1 << 11) - 1,
        .data.u8 = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef}
    };
    struct can_bridge_frame read;

    can_frame_cmp_write(&context, &write);

    success = can_frame_cmp_read(&context, &read);

    CHECK_TRUE(success);
    CHECK_EQUAL(write.ext, read.ext);
    CHECK_EQUAL(write.rtr, read.rtr);
    CHECK_EQUAL(write.id.std, read.id.std);
    CHECK_EQUAL(write.dlc, read.dlc);
    CHECK_EQUAL(0, memcmp(write.data.u8, read.data.u8, write.dlc));
}

TEST(CANFrameTestGroup, CanWriteExtFrame)
{
    bool success;
    struct can_bridge_frame write = {
        .ext = 1,
        .rtr = 0,
        .dlc = 4,
        .id.ext = CAN_FRAME_CMP_EXT_ID_MAX,
        .data.u8 = "xkcd"
    };
    struct can_bridge_frame read;

    can_frame_cmp_write(&context, &write);

    success = can_frame_cmp_read(&context, &read);

    CHECK_TRUE(success);
    CHECK_EQUAL(write.ext, read.ext);
    CHECK_EQUAL(write.rtr, read.rtr);
    CHECK_EQUAL(write.id.ext, read.id.ext);
    CHECK_EQUAL(write.dlc, read.dlc);
    CHECK_EQUAL(0, memcmp(write.data.u8, read.data.u8, write.dlc));
}

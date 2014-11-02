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

TEST(CANFrameTestGroup, CanWriteFrame)
{
    bool ext;
    uint32_t id;
    uint8_t buf[8];
    uint8_t len;
    uint8_t out[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    bool res;

    can_frame_cmp_write(&context, true, 42, out, sizeof(out));

    res = can_frame_cmp_read(&context, &ext, &id, &buf, &len);

    CHECK_TRUE(res);
    CHECK_EQUAL(true, ext);
    CHECK_EQUAL(42, id);
    CHECK_EQUAL(0, memcmp(buf, out, sizeof(out)));
}

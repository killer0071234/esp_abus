#include <unity.h>
#include <cstdlib>
#include "abus_helper.h"

#define MAX_DATA_LEN 255

// builds a packet the same way as abus_socket::sendSocket()
static size_t buildPacket(char *buf, size_t bufsize, ab_socket &s)
{
    ab_header h;
    h.dir = 1;
    h.typ = s.config.socket_id;
    h.from = s.sender;
    h.len = s.bitdata.size() + s.intdata.size() * 2 + s.longdata.size() * 4 + s.realdata.size() * 4 + 4;
    ab_setHeader(buf, bufsize, h);
    ab_setSocket(buf, bufsize, s);
    ab_setUIntVal(buf, bufsize, h.len + 12, ab_calcCRC(buf, h.len + 12));
    return h.len + 14;
}

static ab_socket exampleSocket()
{
    ab_socket s;
    s.config.socket_id = 3;
    s.sender = 0x12345678;
    ab_addBitTag(&s, true);
    ab_addIntTag(&s, -123);
    ab_addLongTag(&s, -123456);
    ab_addRealTag(&s, 12.5f);
    return s;
}

void setUp() {}
void tearDown() {}

void test_roundtrip()
{
    char buf[MAX_DATA_LEN];
    ab_socket s = exampleSocket();
    size_t n = buildPacket(buf, sizeof(buf), s);
    TEST_ASSERT_EQUAL(29, n);
    TEST_ASSERT_TRUE(ab_checkValidPacket(buf, n));

    ab_header h = ab_getHeader(buf, n);
    ab_socket r = ab_getSocket(buf, n, h, s.config);
    TEST_ASSERT_TRUE(r.socket_valid);
    TEST_ASSERT_EQUAL(3, r.config.socket_id);
    TEST_ASSERT_EQUAL_HEX32(0x12345678, r.sender);
    TEST_ASSERT_EQUAL(1, r.bitdata[0]);
    TEST_ASSERT_EQUAL(-123, r.intdata[0]);
    TEST_ASSERT_EQUAL(-123456, r.longdata[0]);
    TEST_ASSERT_EQUAL_FLOAT(12.5f, r.realdata[0]);
}

void test_wildcard_keeps_socket_id()
{
    char buf[MAX_DATA_LEN];
    ab_socket s = exampleSocket();
    size_t n = buildPacket(buf, sizeof(buf), s);
    ab_header h = ab_getHeader(buf, n);
    ab_socket r = ab_getSocket(buf, n, h, 0, 1, 1, 1, 1);
    TEST_ASSERT_TRUE(r.socket_valid);
    TEST_ASSERT_EQUAL(3, r.config.socket_id);
}

void test_wrong_tag_count_is_invalid()
{
    char buf[MAX_DATA_LEN];
    ab_socket s = exampleSocket();
    size_t n = buildPacket(buf, sizeof(buf), s);
    ab_header h = ab_getHeader(buf, n);
    TEST_ASSERT_FALSE(ab_getSocket(buf, n, h, 3, 2, 1, 1, 1).socket_valid);
}

void test_corrupted_packet_is_invalid()
{
    char buf[MAX_DATA_LEN];
    ab_socket s = exampleSocket();
    size_t n = buildPacket(buf, sizeof(buf), s);
    buf[15]++;
    TEST_ASSERT_FALSE(ab_checkValidPacket(buf, n));
    buf[15]--;
    TEST_ASSERT_FALSE(ab_checkValidPacket(buf, n - 1));
}

void test_max_size_packet()
{
    // heap buffer of exact size so the sanitizer / allocator catches overflows
    char *buf = (char *)malloc(MAX_DATA_LEN);
    ab_socket s;
    s.config.socket_id = 7;
    s.sender = 1;
    for (int i = 0; i < MAX_DATA_LEN - 14 - 4; i++)
        ab_addBitTag(&s, i & 1);
    size_t n = buildPacket(buf, MAX_DATA_LEN, s);
    TEST_ASSERT_EQUAL(MAX_DATA_LEN, n);
    TEST_ASSERT_TRUE(ab_checkValidPacket(buf, n));

    ab_header h = ab_getHeader(buf, n);
    ab_socket r = ab_getSocket(buf, n, h, 7, MAX_DATA_LEN - 14 - 4, 0, 0, 0);
    TEST_ASSERT_TRUE(r.socket_valid);
    TEST_ASSERT_EQUAL(1, r.bitdata[235]);
    TEST_ASSERT_EQUAL(0, r.bitdata[236]);
    free(buf);
}

void test_accessors_respect_buffer_end()
{
    char buf[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    TEST_ASSERT_EQUAL(0, ab_getLongVal(buf, sizeof(buf), 5));
    TEST_ASSERT_EQUAL(0, ab_getUIntVal(buf, sizeof(buf), 7));
    TEST_ASSERT_FALSE(ab_getBoolVal(buf, sizeof(buf), 8));
    TEST_ASSERT_EQUAL_HEX32(0x08070605, ab_getULongVal(buf, sizeof(buf), 4));

    ab_setLongVal(buf, 6, 3, 0);
    ab_setUIntVal(buf, 6, 5, 0);
    ab_setBoolVal(buf, 6, 6, false);
    TEST_ASSERT_EQUAL(4, buf[3]);
    TEST_ASSERT_EQUAL(6, buf[5]);
    TEST_ASSERT_EQUAL(7, buf[6]);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_roundtrip);
    RUN_TEST(test_wildcard_keeps_socket_id);
    RUN_TEST(test_wrong_tag_count_is_invalid);
    RUN_TEST(test_corrupted_packet_is_invalid);
    RUN_TEST(test_max_size_packet);
    RUN_TEST(test_accessors_respect_buffer_end);
    return UNITY_END();
}

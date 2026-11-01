#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifdef __cplusplus
extern "C" {
#endif

const char *ares_inet_ntop(int af, const void *src, char *dst, size_t size);

#ifndef AF_INET6
#define AF_INET6 10
#endif

#ifdef __cplusplus
}
#endif

static void construct_ipv4_mapped(unsigned char *buf) {
    memset(buf, 0, 12);
    buf[12] = 0xC0;
    buf[13] = 0x00;
    buf[14] = 0x02;
    buf[15] = 0x01;
}

static void construct_trailing_zeros(unsigned char *buf) {
    buf[0] = 0x20; buf[1] = 0x01;
    buf[2] = 0x0d; buf[3] = 0xb8;
    memset(buf + 4, 0, 12);
}

static void construct_single_zero(unsigned char *buf) {
    memset(buf, 0xFF, 16);
    buf[2] = 0x00; buf[3] = 0x00;
}

static void construct_internal_zeros(unsigned char *buf) {
    memset(buf, 0xFF, 16);
    buf[4] = 0x00; buf[5] = 0x00;
    buf[6] = 0x00; buf[7] = 0x00;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    unsigned char src[16];
    char dst[64];
    size_t test_size;
    const char *result;

    memcpy(src, Data, 16);

    /* Strategy 1: Test with a small buffer to trigger line 213 (Overflow) */
    test_size = (Data[16] % 20);
    if (test_size == 0) test_size = 1;
    memset(dst, 0, sizeof(dst));
    result = ares_inet_ntop(AF_INET6, src, dst, test_size);
    (void)result;

    /* Strategy 2: Test with normal buffer size to hit logic paths */
    test_size = sizeof(dst);
    memset(dst, 0, sizeof(dst));
    
    /* Scenario A: IPv4-mapped address (Lines 192-195) */
    construct_ipv4_mapped(src);
    result = ares_inet_ntop(AF_INET6, src, dst, test_size);
    (void)result;

    /* Scenario B: Trailing zeros (Lines 204-205) */
    construct_trailing_zeros(src);
    result = ares_inet_ntop(AF_INET6, src, dst, test_size);
    (void)result;

    /* Scenario C: Single zero run (Line 170) */
    construct_single_zero(src);
    result = ares_inet_ntop(AF_INET6, src, dst, test_size);
    (void)result;

    /* Scenario D: Internal zeros (Lines 148-150, 157-158, 165-166, 180-182) */
    construct_internal_zeros(src);
    result = ares_inet_ntop(AF_INET6, src, dst, test_size);
    (void)result;

    /* Scenario E: All zeros (Full compression) */
    memset(src, 0, 16);
    result = ares_inet_ntop(AF_INET6, src, dst, test_size);
    (void)result;

    /* Scenario F: All non-zeros (No compression) */
    memset(src, 0xFF, 16);
    result = ares_inet_ntop(AF_INET6, src, dst, test_size);
    (void)result;

    /* Scenario G: Use the raw fuzz input for general coverage */
    memcpy(src, Data, 16);
    result = ares_inet_ntop(AF_INET6, src, dst, test_size);
    (void)result;

    return 0;
}
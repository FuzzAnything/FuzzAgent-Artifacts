#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Include c-ares headers */
#include "ares.h"

/* 
 * Helper to ensure we have a valid 16-byte IPv6 address from fuzzer input.
 * We pad with zeros if input is short, or truncate if too long.
 */
static void prepare_ipv6_input(const uint8_t *data, size_t len, uint8_t *out) {
    memset(out, 0, 16);
    size_t copy_len = (len < 16) ? len : 16;
    if (copy_len > 0) {
        memcpy(out, data, copy_len);
    }
}

/* 
 * Specialized test cases to force specific code paths in inet_ntop6
 */
static void test_special_cases(const uint8_t *data, size_t len) {
    uint8_t addr[16];
    char dst[INET6_ADDRSTRLEN + 1];
    const char *result;
    
    /* Case 1: IPv4-Mapped Address (::ffff:x.x.x.x) */
    /* Uncovered lines: 192-195 */
    memset(addr, 0, 16);
    /* Set the 6th word (bytes 10-11) to 0xffff */
    addr[10] = 0xff;
    addr[11] = 0xff;
    /* Copy remaining bytes from fuzzer input to simulate IPv4 part */
    if (len >= 4) {
        addr[12] = data[0];
        addr[13] = data[1];
        addr[14] = data[2];
        addr[15] = data[3];
    } else {
        /* Default to 192.0.2.1 if not enough data */
        addr[12] = 0xc0; addr[13] = 0x00; addr[14] = 0x02; addr[15] = 0x01;
    }
    
    result = ares_inet_ntop(AF_INET6, addr, dst, sizeof(dst));
    (void)result; /* Suppress unused warning */

    /* Case 2: Trailing zeros (e.g., 2001:db8::) */
    /* Uncovered lines: 204-205 */
    memset(addr, 0, 16);
    /* Set first few bytes to non-zero */
    if (len >= 4) {
        addr[0] = data[0]; addr[1] = data[1];
        addr[2] = data[2]; addr[3] = data[3];
    } else {
        addr[0] = 0x20; addr[1] = 0x01; addr[2] = 0x0d; addr[3] = 0xb8;
    }
    /* Rest are zeros */
    result = ares_inet_ntop(AF_INET6, addr, dst, sizeof(dst));
    (void)result;

    /* Case 3: Single zero run (should NOT compress, hits line 170 logic) */
    /* Uncovered lines: 170 */
    memset(addr, 0xff, 16);
    addr[2] = 0x00; addr[3] = 0x00; /* Only the second word is zero */
    result = ares_inet_ntop(AF_INET6, addr, dst, sizeof(dst));
    (void)result;
}

int LLVMFuzzerTestOneInput_111(const uint8_t *data, size_t size) {
    uint8_t addr[16];
    char dst[INET6_ADDRSTRLEN + 1];
    const char *result;
    
    if (size == 0) {
        return 0;
    }

    /* Prepare standard input */
    prepare_ipv6_input(data, size, addr);

    /* Test 1: Normal conversion with large buffer (hits lines 141, 148-150, 157-158, 165-166, 178, 188, 201) */
    result = ares_inet_ntop(AF_INET6, addr, dst, sizeof(dst));
    (void)result;

    /* Test 2: Buffer overflow check (hits line 213) */
    /* Pass a very small size to trigger the overflow check */
    result = ares_inet_ntop(AF_INET6, addr, dst, 1);
    (void)result;

    /* Test 3: Specific edge cases for uncovered lines */
    test_special_cases(data, size);

    return 0;
}
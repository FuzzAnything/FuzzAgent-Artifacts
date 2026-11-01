#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ares.h>

/* 
 * Fuzz driver for inet_ntop6 via ares_inet_ntop.
 * Target: /mnt/raid/home/lyuyunlong/work/FuzzWork/ProteusFuzz_Code/experiments/results/fuzzagent20/sources/c-ares/src/lib/inet_ntop.c:inet_ntop6
 * Uncovered lines: 141, 148-150, 157-158, 165-166, 170, 178, 180-182, 188, 192-195, 204-205, 213
 */

static void test_ipv6_conversion(const unsigned char *src, size_t size, int case_id) {
    char dst[INET6_ADDRSTRLEN];
    const char *result;

    /* 
     * Case 0: Standard conversion (covers 141, 148-150, 157-158, 165-166, 178, 188)
     * Case 1: Single zero run (covers 170 - best.len < 2)
     * Case 2: Trailing zeros (covers 204-205)
     * Case 3: IPv4-mapped (covers 192-195)
     * Case 4: Small buffer (covers 213)
     */

    if (case_id == 4) {
        /* Test buffer overflow path (line 213) */
        char small_dst[1];
        result = ares_inet_ntop(AF_INET6, src, small_dst, 1);
        if (result != NULL) {
            /* Should not happen if logic is correct */
        }
        return;
    }

    /* Normal conversion */
    result = ares_inet_ntop(AF_INET6, src, dst, sizeof(dst));
    if (result != NULL) {
        /* Success path */
    }
}

static void generate_test_cases(const uint8_t *data, size_t size) {
    if (size < 16) {
        /* Pad or skip if input is too small for IPv6 */
        return;
    }

    unsigned char addr[16];
    int case_id = 0;

    /* Use first byte to select test case strategy */
    if (size > 0) {
        case_id = data[0] % 5;
    }

    /* Copy input data to address buffer */
    memcpy(addr, data + 1, 15);
    /* Ensure we have 16 bytes */
    if (size > 16) {
        addr[15] = data[16];
    } else {
        addr[15] = 0;
    }

    /* Modify address to hit specific uncovered lines based on case_id */
    switch (case_id) {
        case 0:
            /* Standard: Ensure some zeros exist to hit 148-150, 157-158 */
            /* Set words 2 and 3 to 0 (bytes 4-7) */
            addr[4] = addr[5] = addr[6] = addr[7] = 0;
            /* Ensure non-zero elsewhere to trigger 157-158 */
            addr[0] = 0x10; 
            break;

        case 1:
            /* Single zero run: Hit line 170 (best.len < 2) */
            /* Set only word 2 to 0 */
            addr[4] = addr[5] = 0;
            addr[6] = 0x01; /* Break the run immediately */
            break;

        case 2:
            /* Trailing zeros: Hit lines 204-205 */
            /* Set last two words to 0 */
            addr[12] = addr[13] = addr[14] = addr[15] = 0;
            addr[0] = 0x20; /* Ensure non-zero start */
            break;

        case 3:
            /* IPv4-mapped: Hit lines 192-195 */
            /* Format: ::ffff:192.0.2.1 */
            /* words 0-4 = 0, word 5 = 0xffff */
            memset(addr, 0, 12); /* First 6 words (12 bytes) = 0 */
            addr[12] = 0xff; addr[13] = 0xff; /* word 5 = 0xffff */
            addr[14] = 0xC0; addr[15] = 0x00; /* 192.0... */
            break;

        case 4:
            /* Small buffer test: Just use random data */
            /* No modification needed, test function handles size */
            break;
    }

    test_ipv6_conversion(addr, 16, case_id);
}

int LLVMFuzzerTestOneInput_66(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }

    /* 
     * We need at least 16 bytes for an IPv6 address.
     * If the input is shorter, we pad it with zeros or skip.
     * To maximize coverage, we try to construct valid inputs even from short seeds.
     */
    if (size < 17) {
        /* If input is too short, we might not have enough data for our specific case logic.
         * However, we can still try to run a minimal test if we pad.
         * But the logic above expects at least 16 bytes of address data + 1 byte for case_id.
         * Let's just return if too short to avoid complex padding logic that might miss cases.
         * Actually, let's pad to 17 bytes to ensure we can run the test.
         */
        static unsigned char padded[17];
        memset(padded, 0, 17);
        memcpy(padded, data, size);
        generate_test_cases(padded, 17);
    } else {
        generate_test_cases(data, size);
    }

    return 0;
}
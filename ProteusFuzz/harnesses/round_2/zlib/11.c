// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static uLong fuzz_adler32(const uint8_t *data, size_t size) {
    uLong adler = adler32(0L, Z_NULL, 0);
    return adler32(adler, data, size);
}

static uLong fuzz_crc32(const uint8_t *data, size_t size) {
    uLong crc = crc32(0L, Z_NULL, 0);
    return crc32(crc, data, size);
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Split input into two parts for combination testing
    size_t split = Size / 2;
    if (split == 0) split = 1;
    if (split >= Size) split = Size - 1;

    const uint8_t *part1 = Data;
    size_t len1 = split;
    const uint8_t *part2 = Data + split;
    size_t len2 = Size - split;

    // Fuzz adler32_combine64
    uLong adler1 = fuzz_adler32(part1, len1);
    uLong adler2 = fuzz_adler32(part2, len2);
    z_off64_t len2_64 = (z_off64_t)len2;
    uLong combined_adler = adler32_combine64(adler1, adler2, len2_64);
    (void)combined_adler;

    // Fuzz crc32_combine_gen
    z_off_t len2_off = (z_off_t)len2;
    uLong crc_table = crc32_combine_gen(len2_off);
    (void)crc_table;

    // Fuzz crc32_combine_op
    uLong crc1 = fuzz_crc32(part1, len1);
    uLong crc2 = fuzz_crc32(part2, len2);
    uLong op = (uLong)len2; // Use length as operation factor
    if (op == 0) op = 1; // Avoid zero op which returns 0
    uLong combined_op = crc32_combine_op(crc1, crc2, op);
    (void)combined_op;

    // Fuzz crc32_combine
    uLong combined_crc = crc32_combine(crc1, crc2, len2_off);
    (void)combined_crc;

    // Fuzz crc32_combine_gen64
    z_off64_t len2_64_gen = (z_off64_t)len2;
    uLong crc_table_64 = crc32_combine_gen64(len2_64_gen);
    (void)crc_table_64;

    // Fuzz crc32_combine64
    uLong combined_crc_64 = crc32_combine64(crc1, crc2, len2_64);
    (void)combined_crc_64;

    // Test with edge cases using data from input
    if (Size >= 8) {
        z_off64_t neg_len = -1;
        uLong zero_crc = crc32_combine_gen64(neg_len);
        (void)zero_crc;

        uLong test_op = 0;
        uLong zero_op_result = crc32_combine_op(crc1, crc2, test_op);
        (void)zero_op_result;
    }

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

static uLong fuzz_uLong_from_input(const uint8_t *data, size_t size) {
    if (size >= sizeof(uLong)) {
        uLong val;
        memcpy(&val, data, sizeof(uLong));
        return val;
    }
    return 0;
}

static z_off_t fuzz_z_off_t_from_input(const uint8_t *data, size_t size) {
    if (size >= sizeof(z_off_t)) {
        z_off_t val;
        memcpy(&val, data, sizeof(z_off_t));
        return val;
    }
    return 0;
}

static z_off64_t fuzz_z_off64_t_from_input(const uint8_t *data, size_t size) {
    if (size >= sizeof(z_off64_t)) {
        z_off64_t val;
        memcpy(&val, data, sizeof(z_off64_t));
        return val;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    uLong crc1 = fuzz_uLong_from_input(Data, Size);
    uLong crc2 = fuzz_uLong_from_input(Data + 4, Size - 4);
    uLong op = fuzz_uLong_from_input(Data + 8, Size - 8);
    
    z_off_t len2_32 = fuzz_z_off_t_from_input(Data + 4, Size - 4);
    z_off64_t len2_64 = fuzz_z_off64_t_from_input(Data + 8, Size - 8);

    // Test adler32_combine64
    uLong adler_combined = adler32_combine64(crc1, crc2, len2_64);
    (void)adler_combined;

    // Test crc32_combine_gen64
    uLong gen64 = crc32_combine_gen64(len2_64);
    (void)gen64;

    // Test crc32_combine_op
    uLong crc_op_result = crc32_combine_op(crc1, crc2, op);
    (void)crc_op_result;

    // Test crc32_combine
    uLong crc_combined = crc32_combine(crc1, crc2, len2_32);
    (void)crc_combined;

    // Test crc32_combine64
    uLong crc_combined64 = crc32_combine64(crc1, crc2, len2_64);
    (void)crc_combined64;

    // Test crc32_combine_gen
    uLong gen32 = crc32_combine_gen(len2_32);
    (void)gen32;

    // Additional diverse invocations with modified inputs
    uLong crc1_mod = crc1 ^ 0x12345678;
    uLong crc2_mod = crc2 ^ 0x87654321;
    z_off64_t len_neg = -1;
    
    uLong adler_neg = adler32_combine64(crc1_mod, crc2_mod, len_neg);
    (void)adler_neg;

    uLong gen_neg = crc32_combine_gen64(len_neg);
    (void)gen_neg;

    uLong crc_op_zero = crc32_combine_op(crc1_mod, crc2_mod, 0);
    (void)crc_op_zero;

    return 0;
}
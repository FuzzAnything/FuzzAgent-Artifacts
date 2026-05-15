// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t extract_uLong(const uint8_t **data, size_t *size) {
    if (*size >= sizeof(uint32_t)) {
        uint32_t value;
        memcpy(&value, *data, sizeof(uint32_t));
        *data += sizeof(uint32_t);
        *size -= sizeof(uint32_t);
        return value;
    }
    return 0;
}

static int64_t extract_z_off64_t(const uint8_t **data, size_t *size) {
    if (*size >= sizeof(int64_t)) {
        int64_t value;
        memcpy(&value, *data, sizeof(int64_t));
        *data += sizeof(int64_t);
        *size -= sizeof(int64_t);
        return value;
    }
    return 0;
}

static int32_t extract_z_off_t(const uint8_t **data, size_t *size) {
    if (*size >= sizeof(int32_t)) {
        int32_t value;
        memcpy(&value, *data, sizeof(int32_t));
        *data += sizeof(int32_t);
        *size -= sizeof(int32_t);
        return value;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    const uint8_t *data_ptr = Data;
    size_t data_size = Size;

    // Extract parameters from fuzzer input
    uLong crc1 = extract_uLong(&data_ptr, &data_size);
    uLong crc2 = extract_uLong(&data_ptr, &data_size);
    uLong op = extract_uLong(&data_ptr, &data_size);
    z_off64_t len2_64 = extract_z_off64_t(&data_ptr, &data_size);
    z_off_t len2 = extract_z_off_t(&data_ptr, &data_size);

    // For crc32_z, use remaining data as buffer
    const Bytef *buf = data_ptr;
    z_size_t len = (z_size_t)(data_size > 0 ? data_size : 0);

    // Call target functions with extracted parameters
    uLong result1 = crc32_combine_gen64(len2_64);
    uLong result2 = crc32_combine_gen(len2);
    uLong result3 = crc32_combine64(crc1, crc2, len2_64);
    uLong result4 = crc32_z(crc1, buf, len);
    uLong result5 = crc32_combine_op(crc1, crc2, op);
    uLong result6 = crc32_combine(crc1, crc2, len2);

    // Use results to avoid dead code elimination
    (void)result1;
    (void)result2;
    (void)result3;
    (void)result4;
    (void)result5;
    (void)result6;

    return 0;
}
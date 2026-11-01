// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static void fuzz_crc32(const uint8_t *data, size_t size) {
    uLong crc = 0;
    const Bytef *buf = (const Bytef *)data;
    uInt len = (uInt)size;
    if (len == 0 && size > 0) len = 1;
    crc = crc32(crc, buf, len);
    (void)crc;
}

static void fuzz_crc32_combine_gen64(const uint8_t *data, size_t size) {
    z_off64_t len2 = 0;
    if (size >= sizeof(z_off64_t)) {
        memcpy(&len2, data, sizeof(z_off64_t));
    } else if (size > 0) {
        len2 = (z_off64_t)data[0];
    }
    uLong factor = crc32_combine_gen64(len2);
    (void)factor;
}

static void fuzz_crc32_combine_op(const uint8_t *data, size_t size) {
    uLong crc1 = 0, crc2 = 0, op = 0;
    size_t required = 3 * sizeof(uLong);
    
    if (size >= required) {
        memcpy(&crc1, data, sizeof(uLong));
        memcpy(&crc2, data + sizeof(uLong), sizeof(uLong));
        memcpy(&op, data + 2 * sizeof(uLong), sizeof(uLong));
    } else {
        // Fallback for small inputs: extract bytes individually or use defaults
        if (size > 0) crc1 = (uLong)data[0];
        if (size > 1) crc2 = (uLong)data[1];
        if (size > 2) op = (uLong)data[2];
    }
    uLong result = crc32_combine_op(crc1, crc2, op);
    (void)result;
}

static void fuzz_get_crc_table(const uint8_t *data, size_t size) {
    (void)data;
    (void)size;
    const z_crc_t *table = get_crc_table();
    if (table != NULL) {
        volatile z_crc_t val = table[0];
        (void)val;
    }
}

static void fuzz_crc32_combine(const uint8_t *data, size_t size) {
    uLong crc1 = 0, crc2 = 0;
    z_off_t len2 = 0;
    
    // Check if we have enough data for all three parameters
    size_t required = 2 * sizeof(uLong) + sizeof(z_off_t);
    
    if (size >= required) {
        memcpy(&crc1, data, sizeof(uLong));
        memcpy(&crc2, data + sizeof(uLong), sizeof(uLong));
        memcpy(&len2, data + 2 * sizeof(uLong), sizeof(z_off_t));
    } else {
        // Fallback for small inputs
        if (size > 0) crc1 = (uLong)data[0];
        if (size > 1) crc2 = (uLong)data[1];
        if (size > 2) len2 = (z_off_t)data[2];
    }
    uLong result = crc32_combine(crc1, crc2, len2);
    (void)result;
}

static void fuzz_crc32_combine_gen(const uint8_t *data, size_t size) {
    z_off_t len2 = 0;
    if (size >= sizeof(z_off_t)) {
        memcpy(&len2, data, sizeof(z_off_t));
    } else if (size > 0) {
        len2 = (z_off_t)data[0];
    }
    uLong table = crc32_combine_gen(len2);
    (void)table;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_crc32(Data, Size);
    fuzz_crc32_combine_gen64(Data, Size);
    fuzz_crc32_combine_op(Data, Size);
    fuzz_get_crc_table(Data, Size);
    fuzz_crc32_combine(Data, Size);
    fuzz_crc32_combine_gen(Data, Size);

    return 0;
}
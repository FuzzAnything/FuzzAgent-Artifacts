// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32 at crc32.c:946:15 in zlib.h
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
    uInt len = 0;

    if (size > 0) {
        len = (size_t)(size > 65535 ? 65535 : size);
        crc = crc32(crc, buf, len);
    }

    // Process remaining data in chunks if larger than uInt max
    size_t offset = len;
    while (offset < size) {
        uInt chunk_len = (size_t)(size - offset > 65535 ? 65535 : size - offset);
        crc = crc32(crc, buf + offset, chunk_len);
        offset += chunk_len;
    }
}

static void fuzz_crc32_combine_gen64(const uint8_t *data, size_t size) {
    if (size >= sizeof(z_off64_t)) {
        z_off64_t len2;
        memcpy(&len2, data, sizeof(z_off64_t));
        uLong gen = crc32_combine_gen64(len2);
        (void)gen;
    }
}

static void fuzz_crc32_combine_op(const uint8_t *data, size_t size) {
    // Need 3 * sizeof(uLong) bytes
    size_t required = 3 * sizeof(uLong);
    if (size >= required) {
        uLong crc1, crc2, op;
        memcpy(&crc1, data, sizeof(uLong));
        memcpy(&crc2, data + sizeof(uLong), sizeof(uLong));
        memcpy(&op, data + 2 * sizeof(uLong), sizeof(uLong));
        uLong result = crc32_combine_op(crc1, crc2, op);
        (void)result;
    }
}

static void fuzz_get_crc_table(const uint8_t *data, size_t size) {
    (void)data;
    (void)size;
    const z_crc_t FAR *table = get_crc_table();
    if (table != NULL) {
        // Just access a few entries to ensure the table is valid
        (void)table[0];
        (void)table[1];
        (void)table[255];
    }
}

static void fuzz_crc32_combine(const uint8_t *data, size_t size) {
    // Need 2 * sizeof(uLong) + sizeof(z_off_t) bytes
    size_t required = 2 * sizeof(uLong) + sizeof(z_off_t);
    if (size >= required) {
        uLong crc1, crc2;
        z_off_t len2;
        memcpy(&crc1, data, sizeof(uLong));
        memcpy(&crc2, data + sizeof(uLong), sizeof(uLong));
        memcpy(&len2, data + 2 * sizeof(uLong), sizeof(z_off_t));
        uLong result = crc32_combine(crc1, crc2, len2);
        (void)result;
    }
}

static void fuzz_crc32_combine_gen(const uint8_t *data, size_t size) {
    if (size >= sizeof(z_off_t)) {
        z_off_t len2;
        memcpy(&len2, data, sizeof(z_off_t));
        uLong gen = crc32_combine_gen(len2);
        (void)gen;
    }
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Fuzz crc32
    fuzz_crc32(Data, Size);

    // Fuzz crc32_combine_gen64
    fuzz_crc32_combine_gen64(Data, Size);

    // Fuzz crc32_combine_op
    fuzz_crc32_combine_op(Data, Size);

    // Fuzz get_crc_table
    fuzz_get_crc_table(Data, Size);

    // Fuzz crc32_combine
    fuzz_crc32_combine(Data, Size);

    // Fuzz crc32_combine_gen
    fuzz_crc32_combine_gen(Data, Size);

    return 0;
}
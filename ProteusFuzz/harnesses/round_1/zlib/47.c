// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void fuzz_crc32(const uint8_t *data, size_t size) {
    uLong crc = 0;
    /* Process entire buffer in one go */
    crc = crc32(crc, (const Bytef *)data, (uInt)size);
    (void)crc;

    /* Process buffer in chunks to test incremental updates */
    crc = 0;
    size_t chunk_size = 1;
    if (size > 0) {
        chunk_size = size / 2;
        if (chunk_size == 0) chunk_size = 1;
    }
    size_t offset = 0;
    while (offset < size) {
        size_t current_len = (offset + chunk_size <= size) ? chunk_size : (size - offset);
        crc = crc32(crc, (const Bytef *)(data + offset), (uInt)current_len);
        offset += current_len;
    }
    (void)crc;
}

static void fuzz_crc32_combine_gen64(const uint8_t *data, size_t size) {
    /* Interpret first 8 bytes as length if available, else use size */
    z_off64_t len2 = 0;
    if (size >= 8) {
        memcpy(&len2, data, 8);
    } else if (size > 0) {
        len2 = (z_off64_t)size;
    }
    
    /* Test with negative length */
    uLong op_neg = crc32_combine_gen64(-1);
    (void)op_neg;

    /* Test with positive length */
    uLong op_pos = crc32_combine_gen64(len2);
    (void)op_pos;

    /* Test with zero length */
    uLong op_zero = crc32_combine_gen64(0);
    (void)op_zero;
}

static void fuzz_crc32_combine_op(const uint8_t *data, size_t size) {
    uLong crc1 = 0;
    uLong crc2 = 0;
    uLong op = 0;

    if (size >= 12) {
        memcpy(&crc1, data, 4);
        memcpy(&crc2, data + 4, 4);
        memcpy(&op, data + 8, 4);
    } else if (size >= 8) {
        memcpy(&crc1, data, 4);
        memcpy(&crc2, data + 4, 4);
    } else if (size >= 4) {
        memcpy(&crc1, data, 4);
    }

    /* Test with op = 0 */
    uLong res_zero = crc32_combine_op(crc1, crc2, 0);
    (void)res_zero;

    /* Test with op != 0 */
    uLong res_nonzero = crc32_combine_op(crc1, crc2, op);
    (void)res_nonzero;

    /* Test with random op if not set */
    if (size < 12) {
        uLong random_op = (size > 0) ? data[0] : 1;
        if (random_op == 0) random_op = 1;
        res_nonzero = crc32_combine_op(crc1, crc2, random_op);
        (void)res_nonzero;
    }
}

static void fuzz_get_crc_table(const uint8_t *data, size_t size) {
    const z_crc_t FAR *table = get_crc_table();
    if (table == NULL) return;

    /* Read some values from the table based on input */
    if (size > 0) {
        size_t index = data[0] % 256;
        (void)table[index];
    }
}

static void fuzz_crc32_combine(const uint8_t *data, size_t size) {
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off_t len2 = 0;

    if (size >= 12) {
        memcpy(&crc1, data, 4);
        memcpy(&crc2, data + 4, 4);
        memcpy(&len2, data + 8, 4);
    } else if (size >= 8) {
        memcpy(&crc1, data, 4);
        memcpy(&crc2, data + 4, 4);
    } else if (size >= 4) {
        memcpy(&crc1, data, 4);
    }

    /* Test with negative length */
    uLong res_neg = crc32_combine(crc1, crc2, -1);
    (void)res_neg;

    /* Test with positive length */
    uLong res_pos = crc32_combine(crc1, crc2, len2);
    (void)res_pos;

    /* Test with zero length */
    uLong res_zero = crc32_combine(crc1, crc2, 0);
    (void)res_zero;
}

static void fuzz_crc32_combine_gen(const uint8_t *data, size_t size) {
    z_off_t len2 = 0;
    if (size >= 4) {
        memcpy(&len2, data, 4);
    } else if (size > 0) {
        len2 = (z_off_t)size;
    }

    /* Test with negative length */
    uLong op_neg = crc32_combine_gen(-1);
    (void)op_neg;

    /* Test with positive length */
    uLong op_pos = crc32_combine_gen(len2);
    (void)op_pos;

    /* Test with zero length */
    uLong op_zero = crc32_combine_gen(0);
    (void)op_zero;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_crc32(Data, Size);
    fuzz_crc32_combine_gen64(Data, Size);
    fuzz_crc32_combine_op(Data, Size);
    fuzz_get_crc_table(Data, Size);
    fuzz_crc32_combine(Data, Size);
    fuzz_crc32_combine_gen(Data, Size);

    return 0;
}
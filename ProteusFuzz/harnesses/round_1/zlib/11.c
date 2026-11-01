// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

/* Helper function to extract a uLong from input data safely */
static uLong extract_uLong(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(uLong) > size) {
        return 0;
    }
    uLong val = 0;
    memcpy(&val, data + *offset, sizeof(uLong));
    *offset += sizeof(uLong);
    return val;
}

/* Helper function to extract a z_off_t from input data safely */
static z_off_t extract_z_off_t(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(z_off_t) > size) {
        return 0;
    }
    z_off_t val = 0;
    memcpy(&val, data + *offset, sizeof(z_off_t));
    *offset += sizeof(z_off_t);
    return val;
}

/* Helper function to extract a z_off64_t from input data safely */
static z_off64_t extract_z_off64_t(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(z_off64_t) > size) {
        return 0;
    }
    z_off64_t val = 0;
    memcpy(&val, data + *offset, sizeof(z_off64_t));
    *offset += sizeof(z_off64_t);
    return val;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    size_t offset = 0;
    uLong crc1, crc2, op;
    z_off_t len2_32;
    z_off64_t len2_64;

    /* Fuzz crc32_combine_op */
    if (Size >= 3 * sizeof(uLong)) {
        crc1 = extract_uLong(Data, Size, &offset);
        crc2 = extract_uLong(Data, Size, &offset);
        op = extract_uLong(Data, Size, &offset);
        uLong res1 = crc32_combine_op(crc1, crc2, op);
        (void)res1;
    }

    /* Fuzz crc32_combine_gen64 */
    if (Size >= sizeof(z_off64_t)) {
        offset = 0;
        len2_64 = extract_z_off64_t(Data, Size, &offset);
        uLong res2 = crc32_combine_gen64(len2_64);
        (void)res2;
    }

    /* Fuzz crc32_combine */
    if (Size >= 2 * sizeof(uLong) + sizeof(z_off_t)) {
        offset = 0;
        crc1 = extract_uLong(Data, Size, &offset);
        crc2 = extract_uLong(Data, Size, &offset);
        len2_32 = extract_z_off_t(Data, Size, &offset);
        uLong res3 = crc32_combine(crc1, crc2, len2_32);
        (void)res3;
    }

    /* Fuzz adler32_combine64 */
    if (Size >= 2 * sizeof(uLong) + sizeof(z_off64_t)) {
        offset = 0;
        crc1 = extract_uLong(Data, Size, &offset);
        crc2 = extract_uLong(Data, Size, &offset);
        len2_64 = extract_z_off64_t(Data, Size, &offset);
        uLong res4 = adler32_combine64(crc1, crc2, len2_64);
        (void)res4;
    }

    /* Fuzz crc32_combine_gen */
    if (Size >= sizeof(z_off_t)) {
        offset = 0;
        len2_32 = extract_z_off_t(Data, Size, &offset);
        uLong res5 = crc32_combine_gen(len2_32);
        (void)res5;
    }

    /* Fuzz crc32_combine64 */
    if (Size >= 2 * sizeof(uLong) + sizeof(z_off64_t)) {
        offset = 0;
        crc1 = extract_uLong(Data, Size, &offset);
        crc2 = extract_uLong(Data, Size, &offset);
        len2_64 = extract_z_off64_t(Data, Size, &offset);
        uLong res6 = crc32_combine64(crc1, crc2, len2_64);
        (void)res6;
    }

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <zlib.h>

static uLong extract_uLong(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 > size) {
        return 0;
    }
    uLong val = (uLong)data[*offset] |
                ((uLong)data[*offset + 1] << 8) |
                ((uLong)data[*offset + 2] << 16) |
                ((uLong)data[*offset + 3] << 24);
    *offset += 4;
    return val;
}

static z_off_t extract_z_off_t(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 > size) {
        return 0;
    }
    z_off_t val = (z_off_t)data[*offset] |
                  ((z_off_t)data[*offset + 1] << 8) |
                  ((z_off_t)data[*offset + 2] << 16) |
                  ((z_off_t)data[*offset + 3] << 24);
    *offset += 4;
    return val;
}

static z_off64_t extract_z_off64_t(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 8 > size) {
        return 0;
    }
    z_off64_t val = (z_off64_t)data[*offset] |
                    ((z_off64_t)data[*offset + 1] << 8) |
                    ((z_off64_t)data[*offset + 2] << 16) |
                    ((z_off64_t)data[*offset + 3] << 24) |
                    ((z_off64_t)data[*offset + 4] << 32) |
                    ((z_off64_t)data[*offset + 5] << 40) |
                    ((z_off64_t)data[*offset + 6] << 48) |
                    ((z_off64_t)data[*offset + 7] << 56);
    *offset += 8;
    return val;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    size_t offset = 0;
    uLong crc1 = extract_uLong(Data, &offset, Size);
    uLong crc2 = extract_uLong(Data, &offset, Size);
    uLong op = extract_uLong(Data, &offset, Size);
    z_off_t len2_32 = extract_z_off_t(Data, &offset, Size);
    z_off64_t len2_64 = extract_z_off64_t(Data, &offset, Size);

    // Fuzz crc32_combine_op
    if (op != 0) {
        crc32_combine_op(crc1, crc2, op);
    }

    // Fuzz crc32_combine_gen
    crc32_combine_gen(len2_32);

    // Fuzz crc32_combine
    crc32_combine(crc1, crc2, len2_32);

    // Fuzz crc32_combine_gen64
    crc32_combine_gen64(len2_64);

    // Fuzz crc32_combine64
    crc32_combine64(crc1, crc2, len2_64);

    // Fuzz adler32_combine64
    adler32_combine64(crc1, crc2, len2_64);

    return 0;
}
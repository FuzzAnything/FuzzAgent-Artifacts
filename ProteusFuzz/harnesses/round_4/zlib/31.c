// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static uLong extract_uLong(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + sizeof(uLong) > size) {
        return 0;
    }
    uLong val = 0;
    memcpy(&val, data + *offset, sizeof(uLong));
    *offset += sizeof(uLong);
    return val;
}

static z_off64_t extract_z_off64_t(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + sizeof(z_off64_t) > size) {
        return 0;
    }
    z_off64_t val = 0;
    memcpy(&val, data + *offset, sizeof(z_off64_t));
    *offset += sizeof(z_off64_t);
    return val;
}

static z_off_t extract_z_off_t(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + sizeof(z_off_t) > size) {
        return 0;
    }
    z_off_t val = 0;
    memcpy(&val, data + *offset, sizeof(z_off_t));
    *offset += sizeof(z_off_t);
    return val;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    size_t offset = 0;
    int choice = Data[0] % 6;
    offset++;

    if (offset >= Size) {
        return 0;
    }

    switch (choice) {
        case 0: {
            // crc32_combine_op
            if (offset + 3 * sizeof(uLong) > Size) break;
            uLong crc1 = extract_uLong(Data, &offset, Size);
            uLong crc2 = extract_uLong(Data, &offset, Size);
            uLong op = extract_uLong(Data, &offset, Size);
            (void)crc32_combine_op(crc1, crc2, op);
            break;
        }
        case 1: {
            // crc32_combine64
            if (offset + 2 * sizeof(uLong) + sizeof(z_off64_t) > Size) break;
            uLong crc1 = extract_uLong(Data, &offset, Size);
            uLong crc2 = extract_uLong(Data, &offset, Size);
            z_off64_t len2 = extract_z_off64_t(Data, &offset, Size);
            (void)crc32_combine64(crc1, crc2, len2);
            break;
        }
        case 2: {
            // crc32_combine_gen64
            if (offset + sizeof(z_off64_t) > Size) break;
            z_off64_t len2 = extract_z_off64_t(Data, &offset, Size);
            (void)crc32_combine_gen64(len2);
            break;
        }
        case 3: {
            // crc32_combine_gen
            if (offset + sizeof(z_off_t) > Size) break;
            z_off_t len2 = extract_z_off_t(Data, &offset, Size);
            (void)crc32_combine_gen(len2);
            break;
        }
        case 4: {
            // crc32_combine
            if (offset + 2 * sizeof(uLong) + sizeof(z_off_t) > Size) break;
            uLong crc1 = extract_uLong(Data, &offset, Size);
            uLong crc2 = extract_uLong(Data, &offset, Size);
            z_off_t len2 = extract_z_off_t(Data, &offset, Size);
            (void)crc32_combine(crc1, crc2, len2);
            break;
        }
        case 5: {
            // adler32_combine64
            if (offset + 2 * sizeof(uLong) + sizeof(z_off64_t) > Size) break;
            uLong adler1 = extract_uLong(Data, &offset, Size);
            uLong adler2 = extract_uLong(Data, &offset, Size);
            z_off64_t len2 = extract_z_off64_t(Data, &offset, Size);
            (void)adler32_combine64(adler1, adler2, len2);
            break;
        }
        default:
            break;
    }

    return 0;
}
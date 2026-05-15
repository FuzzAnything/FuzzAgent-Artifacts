// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// get_crc_table at crc32.c:482:29 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uLong read_uLong(const uint8_t **data, size_t *size) {
    if (*size >= sizeof(uLong)) {
        uLong value = 0;
        memcpy(&value, *data, sizeof(uLong));
        *data += sizeof(uLong);
        *size -= sizeof(uLong);
        return value;
    }
    return 0;
}

static z_off_t read_z_off_t(const uint8_t **data, size_t *size) {
    if (*size >= sizeof(z_off_t)) {
        z_off_t value = 0;
        memcpy(&value, *data, sizeof(z_off_t));
        *data += sizeof(z_off_t);
        *size -= sizeof(z_off_t);
        return value;
    }
    return 0;
}

static z_off64_t read_z_off64_t(const uint8_t **data, size_t *size) {
    if (*size >= sizeof(z_off64_t)) {
        z_off64_t value = 0;
        memcpy(&value, *data, sizeof(z_off64_t));
        *data += sizeof(z_off64_t);
        *size -= sizeof(z_off64_t);
        return value;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const uint8_t *cursor = Data;
    size_t remaining = Size;
    
    uint8_t selector = cursor[0] % 6;
    cursor++;
    remaining--;
    
    switch (selector) {
        case 0: {
            // Test crc32_combine64
            uLong crc1 = read_uLong(&cursor, &remaining);
            uLong crc2 = read_uLong(&cursor, &remaining);
            z_off64_t len2 = read_z_off64_t(&cursor, &remaining);
            // Ensure len2 is non-negative
            if (len2 < 0) len2 = -len2;
            crc32_combine64(crc1, crc2, len2);
            break;
        }
        case 1: {
            // Test crc32_combine
            uLong crc1 = read_uLong(&cursor, &remaining);
            uLong crc2 = read_uLong(&cursor, &remaining);
            z_off_t len2 = read_z_off_t(&cursor, &remaining);
            // Ensure len2 is non-negative
            if (len2 < 0) len2 = -len2;
            crc32_combine(crc1, crc2, len2);
            break;
        }
        case 2: {
            // Test crc32_combine_gen
            z_off_t len2 = read_z_off_t(&cursor, &remaining);
            // Ensure len2 is non-negative
            if (len2 < 0) len2 = -len2;
            uLong op = crc32_combine_gen(len2);
            (void)op; // Use result to avoid unused variable warning
            break;
        }
        case 3: {
            // Test crc32_combine_op
            uLong crc1 = read_uLong(&cursor, &remaining);
            uLong crc2 = read_uLong(&cursor, &remaining);
            uLong op = read_uLong(&cursor, &remaining);
            crc32_combine_op(crc1, crc2, op);
            break;
        }
        case 4: {
            // Test crc32
            uLong crc = read_uLong(&cursor, &remaining);
            uInt len = (uInt)remaining;
            if (len > remaining) len = (uInt)remaining;
            crc32(crc, cursor, len);
            break;
        }
        case 5: {
            // Test get_crc_table
            const z_crc_t *table = get_crc_table();
            (void)table; // Use result to avoid unused variable warning
            
            // Optionally use the table to compute a CRC
            if (remaining > 0) {
                uLong crc = 0;
                for (size_t i = 0; i < remaining && i < 256; i++) {
                    crc = table[(crc ^ cursor[i]) & 0xFF] ^ (crc >> 8);
                }
                (void)crc;
            }
            break;
        }
    }
    
    return 0;
}
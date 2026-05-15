// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // Initialize variables with safe defaults
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off_t len2 = 0;
    z_off64_t len2_64 = 0;
    
    // Safely extract parameters from fuzzer input
    size_t offset = 0;
    
    // Read crc1 if enough data available
    if (offset + sizeof(uLong) <= Size) {
        memcpy(&crc1, Data + offset, sizeof(uLong));
        offset += sizeof(uLong);
    }
    
    // Read crc2 if enough data available
    if (offset + sizeof(uLong) <= Size) {
        memcpy(&crc2, Data + offset, sizeof(uLong));
        offset += sizeof(uLong);
    }
    
    // Read len2 if enough data available
    if (offset + sizeof(z_off_t) <= Size) {
        memcpy(&len2, Data + offset, sizeof(z_off_t));
        offset += sizeof(z_off_t);
        if (len2 < 0) {
            len2 = -len2;
        }
    }
    
    // Read len2_64 if enough data available
    if (offset + sizeof(z_off64_t) <= Size) {
        memcpy(&len2_64, Data + offset, sizeof(z_off64_t));
        offset += sizeof(z_off64_t);
        if (len2_64 < 0) {
            len2_64 = -len2_64;
        }
    }

    // Test crc32_combine64
    uLong result1 = crc32_combine64(crc1, crc2, len2_64);

    // Test crc32_combine
    uLong result2 = crc32_combine(crc1, crc2, len2);

    // Test crc32_combine_gen and crc32_combine_op
    uLong op = crc32_combine_gen(len2);
    uLong result3 = crc32_combine_op(crc1, crc2, op);

    // Test crc32 with remaining data
    uLong crc = crc32(0L, Z_NULL, 0);
    if (Size > offset) {
        // Process in chunks to handle any size
        size_t remaining = Size - offset;
        const uint8_t *buf = Data + offset;
        
        while (remaining > 0) {
            uInt chunk = remaining > 65535 ? 65535 : (uInt)remaining;
            crc = crc32(crc, buf, chunk);
            buf += chunk;
            remaining -= chunk;
        }
    }

    // Test get_crc_table
    const z_crc_t *table = get_crc_table();
    (void)table; // Use to avoid unused variable warning

    // Use results to avoid compiler optimizations
    (void)result1;
    (void)result2;
    (void)result3;
    (void)crc;

    return 0;
}
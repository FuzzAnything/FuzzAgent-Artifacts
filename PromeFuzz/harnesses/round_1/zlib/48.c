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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // Early exit if not enough data for basic operations
    if (Size < 8) return 0;

    // Extract values for CRC parameters from fuzz data
    uLong crc1 = *(uLong*)Data;
    uLong crc2 = *(uLong*)(Data + sizeof(uLong));
    
    // Extract length parameters ensuring non-negative values
    z_off_t len2_t = (z_off_t)(Size & 0x7FFFFFFF); // Ensure non-negative
    z_off64_t len2_64 = (z_off64_t)(Size & 0x7FFFFFFF); // Ensure non-negative
    
    // Test crc32_combine64
    (void)crc32_combine64(crc1, crc2, len2_64);
    
    // Test crc32_combine
    (void)crc32_combine(crc1, crc2, len2_t);
    
    // Test crc32_combine_gen
    uLong op = crc32_combine_gen(len2_t);
    
    // Test crc32_combine_op
    (void)crc32_combine_op(crc1, crc2, op);
    
    // Test crc32 with the input data as buffer
    uLong crc = 0;
    if (Size > 0) {
        // Process in chunks to test incremental updates
        size_t chunk_size = Size / 4;
        if (chunk_size > 0) {
            for (size_t i = 0; i < 4; i++) {
                size_t offset = i * chunk_size;
                size_t remaining = Size - offset;
                size_t current_chunk = (remaining < chunk_size) ? remaining : chunk_size;
                crc = crc32(crc, Data + offset, (uInt)current_chunk);
            }
        } else {
            crc = crc32(crc, Data, (uInt)Size);
        }
    }
    
    // Test get_crc_table
    const z_crc_t *table = get_crc_table();
    (void)table; // Use table to avoid unused variable warning
    
    return 0;
}
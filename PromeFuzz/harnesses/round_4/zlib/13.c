// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Early return if no data
    if (Size == 0) {
        return 0;
    }

    // Initialize variables
    uLong crc1 = 0, crc2 = 0;
    uLong op = 0;
    z_off_t len = 0;
    z_off64_t len64 = 0;
    
    // Extract values from fuzzer input for parameters
    // Use first 4 bytes for crc1, next 4 for crc2, etc.
    // Ensure we don't read beyond Size
    if (Size >= sizeof(uLong)) {
        memcpy(&crc1, Data, sizeof(uLong));
    }
    if (Size >= 2 * sizeof(uLong)) {
        memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
    }
    if (Size >= 3 * sizeof(uLong)) {
        memcpy(&op, Data + 2 * sizeof(uLong), sizeof(uLong));
    }
    
    // Extract length values (signed types)
    if (Size >= 3 * sizeof(uLong) + sizeof(z_off_t)) {
        memcpy(&len, Data + 3 * sizeof(uLong), sizeof(z_off_t));
    }
    if (Size >= 3 * sizeof(uLong) + sizeof(z_off_t) + sizeof(z_off64_t)) {
        memcpy(&len64, Data + 3 * sizeof(uLong) + sizeof(z_off_t), sizeof(z_off64_t));
    }
    
    // Ensure non-negative lengths for functions that require it
    if (len < 0) len = -len;
    if (len64 < 0) len64 = -len64;
    
    // Buffer for crc32 function
    const Bytef *buf = (const Bytef *)Data;
    uInt buf_len = (Size > UINT_MAX) ? UINT_MAX : (uInt)Size;
    
    // Call target functions
    // 1. crc32_combine_op
    uLong result1 = crc32_combine_op(crc1, crc2, op);
    
    // 2. crc32_combine
    uLong result2 = crc32_combine(crc1, crc2, len);
    
    // 3. crc32 - compute CRC incrementally in chunks
    uLong crc = crc32(0L, Z_NULL, 0);
    size_t offset = 0;
    while (offset < Size) {
        size_t chunk_size = (Size - offset > 1024) ? 1024 : Size - offset;
        crc = crc32(crc, buf + offset, (uInt)chunk_size);
        offset += chunk_size;
    }
    uLong result3 = crc;
    
    // 4. crc32_combine_gen
    uLong result4 = crc32_combine_gen(len);
    
    // 5. crc32_combine64
    uLong result5 = crc32_combine64(crc1, crc2, len64);
    
    // 6. get_crc_table
    const z_crc_t *table = get_crc_table();
    if (table != NULL) {
        // Use the table to compute a CRC manually for a small input
        uLong manual_crc = 0L ^ 0xffffffffL;
        if (Size > 0) {
            manual_crc = table[(manual_crc ^ Data[0]) & 0xff] ^ (manual_crc >> 8);
        }
    }
    
    // Additional test: combine using generated operator
    if (result4 != 0) {
        uLong result6 = crc32_combine_op(crc1, crc2, result4);
    }
    
    // Test with different initial CRC values
    uLong alt_crc = crc32(0xffffffffL, buf, buf_len);
    
    // Test combination with different lengths
    if (Size >= sizeof(uLong)) {
        uLong temp_len = *(const uLong*)Data;
        if (temp_len < 0) temp_len = -temp_len;
        uLong result7 = crc32_combine_gen((z_off_t)temp_len);
        uLong result8 = crc32_combine(crc1, crc2, (z_off_t)temp_len);
    }
    
    return 0;
}
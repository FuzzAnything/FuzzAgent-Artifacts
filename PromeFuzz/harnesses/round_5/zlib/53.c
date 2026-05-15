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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "zlib.h"

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    // Ensure minimum input size for extracting parameters
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from fuzzer input
    uLong crc1 = 0, crc2 = 0;
    z_off_t len2 = 0;
    z_off64_t len2_64 = 0;
    uLong op = 0;
    
    // Use first 4 bytes for crc1
    memcpy(&crc1, Data, sizeof(uLong) > 4 ? 8 : 4);
    
    // Use next 4 bytes for crc2 if available
    if (Size >= 8) {
        memcpy(&crc2, Data + 4, sizeof(uLong) > 4 ? 8 : 4);
    }
    
    // Use next bytes for len2 (z_off_t) if available
    if (Size >= 12) {
        memcpy(&len2, Data + 8, sizeof(z_off_t) > 4 ? 8 : 4);
    }
    
    // Use next bytes for len2_64 (z_off64_t) if available  
    if (Size >= 20) {
        memcpy(&len2_64, Data + 12, sizeof(z_off64_t) > 4 ? 8 : 4);
    }
    
    // Use next bytes for op if available
    if (Size >= 24) {
        memcpy(&op, Data + 20, sizeof(uLong) > 4 ? 8 : 4);
    }

    // Call target functions
    // 1. crc32_combine64
    if (len2_64 >= 0) {
        uLong result1 = crc32_combine64(crc1, crc2, len2_64);
        (void)result1; // Use result to avoid unused variable warning
    }
    
    // 2. crc32_combine
    uLong result2 = crc32_combine(crc1, crc2, len2);
    (void)result2;
    
    // 3. crc32_combine_gen
    if (len2 >= 0) {
        uLong gen_op = crc32_combine_gen(len2);
        (void)gen_op;
    }
    
    // 4. crc32_combine_op
    if (op != 0) {
        uLong result4 = crc32_combine_op(crc1, crc2, op);
        (void)result4;
    }
    
    // 5. crc32
    // Use remaining data as buffer for crc32
    size_t buf_start = 24; // Starting position for buffer data
    if (buf_start < Size) {
        const Bytef* buf = (const Bytef*)(Data + buf_start);
        uInt buf_len = (uInt)(Size - buf_start);
        if (buf_len > 0) {
            // Test incremental CRC calculation
            uLong crc = crc32(0L, Z_NULL, 0); // Initialize
            
            // Process in chunks to test streaming
            size_t chunk_size = 1024;
            for (size_t i = 0; i < buf_len; i += chunk_size) {
                uInt current_chunk = (uInt)((i + chunk_size) < buf_len ? chunk_size : (buf_len - i));
                crc = crc32(crc, buf + i, current_chunk);
            }
            (void)crc;
        }
    }
    
    // 6. get_crc_table
    const z_crc_t* crc_table = get_crc_table();
    (void)crc_table;
    
    return 0;
}
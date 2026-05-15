// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32 at crc32.c:946:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
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

static void test_crc32_combine_functions(uLong crc1, uLong crc2, z_off64_t len2_64, z_off_t len2) {
    // Test crc32_combine64 - requires non-negative len2
    if (len2_64 >= 0) {
        crc32_combine64(crc1, crc2, len2_64);
    }
    
    // Test crc32_combine - returns zero if len2 is negative
    crc32_combine(crc1, crc2, len2);
    
    // Test crc32_combine_gen and crc32_combine_op
    if (len2 >= 0) {
        uLong op = crc32_combine_gen(len2);
        if (op != 0) {
            crc32_combine_op(crc1, crc2, op);
        }
    }
    
    // Test crc32_combine_op with zero operator
    crc32_combine_op(crc1, crc2, 0);
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        // Need minimum data for meaningful tests
        return 0;
    }
    
    // Extract parameters from input data safely
    uLong crc1 = 0;
    uLong crc2 = 0;
    
    // Read crc1 and crc2 safely, ensuring we don't read beyond bounds
    if (Size >= sizeof(uLong)) {
        memcpy(&crc1, Data, sizeof(uLong));
    }
    
    if (Size >= 2 * sizeof(uLong)) {
        memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
    }
    
    // Ensure we don't read beyond bounds for len2_64
    size_t offset = 2 * sizeof(uLong);
    z_off64_t len2_64 = 0;
    z_off_t len2 = 0;
    
    if (Size >= offset + sizeof(uint64_t)) {
        uint64_t len_val = 0;
        memcpy(&len_val, Data + offset, sizeof(uint64_t));
        len2_64 = (z_off64_t)(len_val % 1000000);
        len2 = (z_off_t)(len2_64 % 1000000);
        offset += sizeof(uint64_t);
    }
    
    // Test combine functions
    test_crc32_combine_functions(crc1, crc2, len2_64, len2);
    
    // Test crc32 function with different buffer sizes
    if (offset >= Size) {
        return 0;
    }
    
    size_t remaining = Size - offset;
    const Bytef* buf = (const Bytef*)(Data + offset);
    
    // Test crc32 with initial crc = 0
    uInt safe_len = (uInt)((remaining > 65535) ? 65535 : remaining);
    if (safe_len > 0) {
        uLong result1 = crc32(0L, buf, safe_len);
        
        // Test crc32 with previous result as initial crc
        if (remaining > 100) {
            uInt chunk1 = (uInt)((remaining / 2) % 65536);
            if (chunk1 > remaining - offset) {
                chunk1 = (uInt)(remaining - offset);
            }
            
            uInt chunk2 = 0;
            if (chunk1 < remaining) {
                chunk2 = (uInt)((remaining - chunk1) % 65536);
                if (chunk2 > remaining - chunk1) {
                    chunk2 = (uInt)(remaining - chunk1);
                }
            }
            
            if (chunk1 > 0) {
                uLong intermediate = crc32(0L, buf, chunk1);
                if (chunk2 > 0) {
                    crc32(intermediate, buf + chunk1, chunk2);
                }
            }
        }
        
        // Test crc32 with NULL buffer and zero length
        crc32(result1, Z_NULL, 0);
    }
    
    // Test get_crc_table
    const z_crc_t FAR* crc_table = get_crc_table();
    if (crc_table != NULL) {
        // Access a few table entries to ensure they're valid
        volatile z_crc_t dummy = crc_table[0];
        if (dummy) {} // Use dummy to avoid unused variable warning
    }
    
    return 0;
}
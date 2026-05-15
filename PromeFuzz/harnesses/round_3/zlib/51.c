// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
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
#include <stddef.h>
#include <string.h>

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        // Not enough data for meaningful fuzzing
        return 0;
    }

    // Extract parameters from fuzzer input
    uLong crc1 = *(const uLong*)Data;
    uLong crc2 = *(const uLong*)(Data + sizeof(uLong));
    
    // For z_off_t and z_off64_t parameters, use absolute values to avoid negative lengths
    z_off_t len2 = (z_off_t)(*(const int32_t*)(Data + 2*sizeof(uLong)));
    if (len2 < 0) len2 = -len2;
    
    z_off64_t len2_64 = (z_off64_t)(*(const int64_t*)(Data + 2*sizeof(uLong)));
    if (len2_64 < 0) len2_64 = -len2_64;
    
    // For crc32_combine_op, we need an operator
    uLong op = *(const uLong*)(Data + 3*sizeof(uLong));
    
    // Buffer for crc32 function
    const Bytef* buf = (const Bytef*)(Data + 4*sizeof(uLong));
    uInt buf_len = (uInt)(Size - 4*sizeof(uLong));
    if (buf_len > Size - 4*sizeof(uLong)) {
        buf_len = (uInt)(Size - 4*sizeof(uLong));
    }

    // Test crc32_combine64
    uLong result1 = crc32_combine64(crc1, crc2, len2_64);
    (void)result1; // Avoid unused variable warning

    // Test crc32_combine
    uLong result2 = crc32_combine(crc1, crc2, len2);
    (void)result2;

    // Test crc32_combine_op
    uLong result3 = crc32_combine_op(crc1, crc2, op);
    (void)result3;

    // Test crc32
    uLong result4 = crc32(0L, Z_NULL, 0); // Initialize
    result4 = crc32(result4, buf, buf_len); // Update with data
    (void)result4;

    // Test crc32_combine_gen64
    uLong op64 = crc32_combine_gen64(len2_64);
    (void)op64;

    // Test crc32_combine_gen
    uLong op_gen = crc32_combine_gen(len2);
    (void)op_gen;

    // Test crc32_combine_op with generated operators
    if (op64 != 0) {
        uLong result5 = crc32_combine_op(crc1, crc2, op64);
        (void)result5;
    }
    
    if (op_gen != 0) {
        uLong result6 = crc32_combine_op(crc1, crc2, op_gen);
        (void)result6;
    }

    // Test incremental crc32 calculation
    if (buf_len > 0) {
        // Calculate CRC in chunks
        uLong incremental_crc = crc32(0L, Z_NULL, 0);
        size_t chunk_size = buf_len / 3;
        if (chunk_size > 0) {
            incremental_crc = crc32(incremental_crc, buf, (uInt)chunk_size);
            incremental_crc = crc32(incremental_crc, buf + chunk_size, (uInt)chunk_size);
            if (buf_len > 2 * chunk_size) {
                incremental_crc = crc32(incremental_crc, buf + 2 * chunk_size, 
                                      (uInt)(buf_len - 2 * chunk_size));
            }
        } else {
            incremental_crc = crc32(incremental_crc, buf, buf_len);
        }
        (void)incremental_crc;
    }

    return 0;
}
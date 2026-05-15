// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Extract parameters safely without pointer arithmetic
    uLong crc1, crc2;
    memcpy(&crc1, Data, sizeof(uLong));
    memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
    
    // Use remaining data for buffer if needed
    const Bytef *buf = NULL;
    z_size_t buf_len = 0;
    
    if (Size > 2 * sizeof(uLong)) {
        buf = Data + 2 * sizeof(uLong);
        buf_len = Size - 2 * sizeof(uLong);
    }
    
    // Extract length parameters (ensure non-negative)
    uint32_t len32;
    uint64_t len64;
    
    memcpy(&len32, Data, sizeof(uint32_t));
    memcpy(&len64, Data, sizeof(uint64_t));
    
    z_off_t len2 = (z_off_t)len32;
    z_off64_t len2_64 = (z_off64_t)len64;
    
    // Ensure non-negative lengths
    if (len2 < 0) len2 = 0;
    if (len2_64 < 0) len2_64 = 0;
    
    // Test crc32_combine64
    crc32_combine64(crc1, crc2, len2_64);
    
    // Test crc32_combine
    crc32_combine(crc1, crc2, len2);
    
    // Test crc32_combine_gen
    uLong op = crc32_combine_gen(len2);
    
    // Test crc32_combine_op with generated operator
    if (op != 0) {
        crc32_combine_op(crc1, crc2, op);
    }
    
    // Test crc32_z
    if (buf_len > 0) {
        crc32_z(crc1, buf, buf_len);
    }
    
    // Test crc32_combine_gen64
    uLong op64 = crc32_combine_gen64(len2_64);
    
    // Test crc32_combine_op with 64-bit generated operator
    if (op64 != 0) {
        crc32_combine_op(crc1, crc2, op64);
    }

    return 0;
}
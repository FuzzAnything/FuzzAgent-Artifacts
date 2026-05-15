// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <string.h>

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 4) {
        return 0;
    }

    // Extract values from fuzzer input
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off_t len2 = 0;
    z_off64_t len2_64 = 0;
    
    // Use first 4 bytes for crc1
    memcpy(&crc1, Data, sizeof(crc1));
    crc1 = crc1 & 0xFFFFFFFF; // Ensure 32-bit
    
    // Use next 4 bytes for crc2 if available
    if (Size >= 8) {
        memcpy(&crc2, Data + 4, sizeof(crc2));
        crc2 = crc2 & 0xFFFFFFFF; // Ensure 32-bit
    }
    
    // Use next bytes for len2 (z_off_t) if available
    if (Size >= 12) {
        memcpy(&len2, Data + 8, sizeof(len2));
        // Ensure non-negative for functions that require it
        if (len2 < 0) len2 = -len2;
    }
    
    // Use next bytes for len2_64 (z_off64_t) if available
    if (Size >= 20) {
        memcpy(&len2_64, Data + 12, sizeof(len2_64));
        // Ensure non-negative for functions that require it
        if (len2_64 < 0) len2_64 = -len2_64;
    }

    // Test crc32 function with remaining data
    uLong crc_result = 0;
    if (Size > 20) {
        size_t remaining = Size - 20;
        // Ensure we don't overflow uInt (max 32-bit)
        uInt buf_len = (remaining > 0xFFFFFFFF) ? 0xFFFFFFFF : (uInt)remaining;
        crc_result = crc32(0, Data + 20, buf_len);
    }

    // Test crc32_combine64
    uLong combine64_result = crc32_combine64(crc1, crc2, len2_64);

    // Test crc32_combine
    uLong combine_result = crc32_combine(crc1, crc2, len2);

    // Test crc32_combine_gen64
    uLong gen64_op = crc32_combine_gen64(len2_64);
    
    // Test crc32_combine_gen
    uLong gen_op = crc32_combine_gen(len2);

    // Test crc32_combine_op with generated operators
    uLong combine_op_result1 = crc32_combine_op(crc1, crc2, gen64_op);
    uLong combine_op_result2 = crc32_combine_op(crc1, crc2, gen_op);

    // Test crc32 with different initial CRC values
    if (Size > 20) {
        size_t remaining = Size - 20;
        uInt buf_len = (remaining > 0xFFFFFFFF) ? 0xFFFFFFFF : (uInt)remaining;
        
        // Test with initial CRC from previous calculation
        crc32(crc_result, Data + 20, buf_len);
        
        // Test with CRC values from combine operations
        crc32(combine64_result, Data + 20, buf_len);
        crc32(combine_result, Data + 20, buf_len);
    }

    // Test edge cases
    // Zero length for combine functions
    crc32_combine64(crc1, crc2, 0);
    crc32_combine(crc1, crc2, 0);
    
    // Test with zero CRC values
    crc32_combine64(0, 0, len2_64);
    crc32_combine(0, 0, len2);
    
    // Test crc32_combine_op with zero operator
    crc32_combine_op(crc1, crc2, 0);

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <unistd.h>
#include <fcntl.h>

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 8) {
        return 0;
    }

    // Extract parameters from input data
    uLong crc1 = *(const uLong*)Data;
    uLong crc2 = *((const uLong*)Data + 1);
    z_off64_t len64 = (z_off64_t)(Size);
    z_off_t len = (z_off_t)(Size);
    
    // For buffer operations, use the remaining data
    const Bytef* buf = (const Bytef*)(Data + 16);
    z_size_t buf_len = (z_size_t)(Size > 16 ? Size - 16 : 0);
    
    // Test crc32_z with different initial CRC values
    uLong crc_result = crc32_z(crc1, buf, buf_len);
    crc_result = crc32_z(0L, buf, buf_len);
    crc_result = crc32_z(crc2, buf, buf_len);
    
    // Test crc32_combine_gen64
    uLong op64 = crc32_combine_gen64(len64);
    uLong op64_neg = crc32_combine_gen64(-len64);
    
    // Test crc32_combine64
    uLong combined64 = crc32_combine64(crc1, crc2, len64);
    uLong combined64_neg = crc32_combine64(crc1, crc2, -len64);
    
    // Test crc32_combine
    uLong combined = crc32_combine(crc1, crc2, len);
    uLong combined_neg = crc32_combine(crc1, crc2, -len);
    
    // Test crc32_combine_gen
    uLong op = crc32_combine_gen(len);
    uLong op_neg = crc32_combine_gen(-len);
    
    // Test crc32_combine_op with different operators
    uLong combined_op1 = crc32_combine_op(crc1, crc2, op64);
    uLong combined_op2 = crc32_combine_op(crc1, crc2, op);
    uLong combined_op_zero = crc32_combine_op(crc1, crc2, 0L);
    
    // Test chaining operations
    uLong chain_crc = crc32_z(crc1, buf, buf_len);
    uLong chain_op = crc32_combine_gen((z_off_t)buf_len);
    uLong chain_combined = crc32_combine_op(crc2, chain_crc, chain_op);
    
    // Test with extreme values
    uLong max_crc = ~0UL;
    uLong zero_crc = 0L;
    uLong combined_max = crc32_combine64(max_crc, zero_crc, len64);
    uLong combined_zero = crc32_combine64(zero_crc, zero_crc, 0);
    
    // Test with various buffer sizes
    if (buf_len > 0) {
        // Test with single byte
        crc_result = crc32_z(crc1, buf, 1);
        
        // Test with half buffer
        crc_result = crc32_z(crc2, buf, buf_len / 2);
        
        // Test with full buffer
        crc_result = crc32_z(0L, buf, buf_len);
    }
    
    // Test operator reuse
    static uLong saved_op = 0;
    if (saved_op == 0) {
        saved_op = crc32_combine_gen(len);
    }
    uLong reused_combined = crc32_combine_op(crc1, crc2, saved_op);
    
    return 0;
}
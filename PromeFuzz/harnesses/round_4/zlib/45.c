// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <string.h>

static void generate_crc_values(const uint8_t *data, size_t size, 
                               uLong *crc1, uLong *crc2, 
                               const Bytef **buf, z_size_t *buf_len) {
    // Use first half of data for crc1, second half for crc2
    size_t half = size / 2;
    
    // Generate crc1 from first half (or all data if size < 2)
    *crc1 = crc32_z(0, data, half > 0 ? half : size);
    
    // Generate crc2 from second half (or 0 if no second half)
    if (size > half) {
        *crc2 = crc32_z(0, data + half, size - half);
    } else {
        *crc2 = 0;
    }
    
    // Set buffer parameters for crc32_z
    *buf = data;
    *buf_len = size;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    uLong crc1, crc2;
    const Bytef *buf;
    z_size_t buf_len;
    
    // Generate CRC values from input data
    generate_crc_values(Data, Size, &crc1, &crc2, &buf, &buf_len);
    
    // Test crc32_z with the input buffer
    uLong crc_result = crc32_z(crc1, buf, buf_len);
    
    // Generate lengths for combine functions
    // Use first 8 bytes to derive length values
    z_off_t len32 = 0;
    z_off64_t len64 = 0;
    
    if (Size >= sizeof(z_off_t)) {
        memcpy(&len32, Data, sizeof(z_off_t) > Size ? Size : sizeof(z_off_t));
        // Ensure non-negative for functions that require it
        if (len32 < 0) len32 = -len32;
    }
    
    if (Size >= sizeof(z_off64_t)) {
        memcpy(&len64, Data, sizeof(z_off64_t) > Size ? Size : sizeof(z_off64_t));
        // Ensure non-negative for functions that require it
        if (len64 < 0) len64 = -len64;
    }
    
    // Test crc32_combine_gen and crc32_combine_op
    uLong op = crc32_combine_gen(len32);
    uLong combined_op = crc32_combine_op(crc1, crc2, op);
    
    // Test crc32_combine
    uLong combined = crc32_combine(crc1, crc2, len32);
    
    // Test crc32_combine_gen64 and crc32_combine_op with 64-bit operator
    uLong op64 = crc32_combine_gen64(len64);
    uLong combined_op64 = crc32_combine_op(crc1, crc2, op64);
    
    // Test crc32_combine64
    uLong combined64 = crc32_combine64(crc1, crc2, len64);
    
    // Test edge cases
    // Test with zero operator
    uLong zero_op_result = crc32_combine_op(crc1, crc2, 0);
    
    // Test with zero length
    uLong zero_len_combine = crc32_combine(crc1, crc2, 0);
    uLong zero_len_combine64 = crc32_combine64(crc1, crc2, 0);
    
    // Test crc32_z with zero length buffer
    uLong zero_buf_crc = crc32_z(crc1, NULL, 0);
    
    // Test with same CRC values
    uLong same_crc_combine = crc32_combine(crc1, crc1, len32);
    uLong same_crc_combine64 = crc32_combine64(crc1, crc1, len64);
    
    // Test with zero CRC values
    uLong zero_crc_combine = crc32_combine(0, 0, len32);
    uLong zero_crc_combine64 = crc32_combine64(0, 0, len64);
    
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32 at crc32.c:946:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // Early return for insufficient data
    if (Size < sizeof(uLong)) {
        return 0;
    }

    // Extract values from fuzzer input safely
    uLong crc1 = 0;
    uLong crc2 = 0;
    
    // Copy first uLong safely
    memcpy(&crc1, Data, sizeof(uLong));
    
    // Check if we have enough data for second uLong
    if (Size >= 2 * sizeof(uLong)) {
        memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
    }
    
    // Prepare buffer for crc32 function
    size_t data_offset = 2 * sizeof(uLong);
    const Bytef* buf = NULL;
    uInt buf_len = 0;
    
    if (Size > data_offset) {
        buf = (const Bytef*)(Data + data_offset);
        buf_len = (uInt)(Size - data_offset);
        if (buf_len > Size - data_offset) {
            buf_len = (uInt)(Size - data_offset);
        }
    }
    
    // Test crc32 function
    uLong crc_result = crc32(crc1, buf, buf_len);
    
    // Test get_crc_table function
    const z_crc_t FAR* crc_table = get_crc_table();
    if (crc_table) {
        // Use the table to compute a CRC manually for verification
        uLong manual_crc = crc1;
        for (uInt i = 0; i < buf_len && i < 16; ++i) {
            manual_crc = crc_table[(manual_crc ^ buf[i]) & 0xFF] ^ (manual_crc >> 8);
        }
    }
    
    // Generate non-negative lengths for combine functions
    z_off_t len2 = (z_off_t)(buf_len % 1024);
    z_off64_t len2_64 = (z_off64_t)(buf_len % 1024);
    
    // Test crc32_combine
    uLong combined = crc32_combine(crc1, crc2, len2);
    
    // Test crc32_combine64
    uLong combined64 = crc32_combine64(crc1, crc2, len2_64);
    
    // Test crc32_combine_gen and crc32_combine_op
    if (len2 >= 0) {
        uLong op = crc32_combine_gen(len2);
        if (op != 0) {
            uLong combined_op = crc32_combine_op(crc1, crc2, op);
        }
    }
    
    // Additional test with different combinations
    if (Size > 2 * sizeof(uLong)) {
        uLong crc3 = 0;
        size_t offset = 2 * sizeof(uLong);
        if (Size >= offset + sizeof(uLong)) {
            memcpy(&crc3, Data + offset, sizeof(uLong));
            uLong combined2 = crc32_combine(crc_result, crc3, (z_off_t)(buf_len % 512));
        }
    }
    
    return 0;
}
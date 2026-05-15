// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
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
// crc32 at crc32.c:946:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < 4) {
        return 0;
    }

    // Safely extract values from fuzzer input
    uLong crc1 = 0, crc2 = 0;
    
    // Extract crc1 (first 4 bytes)
    memcpy(&crc1, Data, sizeof(uLong) > Size ? Size : sizeof(uLong));
    
    // Extract crc2 (next 4 bytes if available)
    if (Size >= 2 * sizeof(uLong)) {
        memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
    } else if (Size > sizeof(uLong)) {
        memcpy(&crc2, Data + sizeof(uLong), Size - sizeof(uLong));
    }
    
    // For length parameters, use available data
    z_off_t len2_t = 0;
    z_off64_t len2_t64 = 0;
    
    // Extract len2_t (next 4 bytes if available)
    size_t offset = 2 * sizeof(uLong);
    if (offset < Size) {
        size_t bytes_to_copy = sizeof(z_off_t) < (Size - offset) ? sizeof(z_off_t) : (Size - offset);
        memcpy(&len2_t, Data + offset, bytes_to_copy);
    }
    
    // Extract len2_t64 (next 8 bytes if available)
    offset += sizeof(z_off_t);
    if (offset < Size) {
        size_t bytes_to_copy = sizeof(z_off64_t) < (Size - offset) ? sizeof(z_off64_t) : (Size - offset);
        memcpy(&len2_t64, Data + offset, bytes_to_copy);
    }
    
    // Ensure non-negative lengths
    if (len2_t < 0) len2_t = -len2_t;
    if (len2_t64 < 0) len2_t64 = -len2_t64;
    
    // Test crc32_combine64
    uLong result1 = crc32_combine64(crc1, crc2, len2_t64);
    
    // Test crc32_combine
    uLong result2 = crc32_combine(crc1, crc2, len2_t);
    
    // Test crc32_combine_gen64
    uLong op64 = crc32_combine_gen64(len2_t64);
    
    // Test crc32_combine_gen
    uLong op = crc32_combine_gen(len2_t);
    
    // Test crc32_combine_op with generated operators
    uLong result3 = crc32_combine_op(crc1, crc2, op64);
    uLong result4 = crc32_combine_op(crc1, crc2, op);
    
    // Test crc32 function with the remaining fuzzer data
    offset += sizeof(z_off64_t);
    if (offset < Size) {
        const Bytef* buf = (const Bytef*)(Data + offset);
        size_t buf_size = Size - offset;
        
        // Test incremental CRC calculation
        uLong crc = crc32(0L, Z_NULL, 0);
        
        // Calculate CRC in chunks to test streaming behavior
        if (buf_size > 0) {
            // Process in at most 3 chunks
            size_t num_chunks = 3;
            size_t chunk_size = buf_size / num_chunks;
            
            if (chunk_size > 0) {
                for (size_t i = 0; i < num_chunks - 1; i++) {
                    if (i * chunk_size < buf_size) {
                        size_t current_chunk_size = chunk_size;
                        if (i * chunk_size + current_chunk_size > buf_size) {
                            current_chunk_size = buf_size - i * chunk_size;
                        }
                        crc = crc32(crc, buf + i * chunk_size, (uInt)current_chunk_size);
                    }
                }
                
                // Last chunk with remaining data
                size_t remaining_start = (num_chunks - 1) * chunk_size;
                if (remaining_start < buf_size) {
                    size_t remaining = buf_size - remaining_start;
                    crc = crc32(crc, buf + remaining_start, (uInt)remaining);
                }
            } else {
                // Small buffer - process all at once
                crc = crc32(crc, buf, (uInt)buf_size);
            }
            
            // Also test single call for comparison
            uLong single_crc = crc32(0L, buf, (uInt)buf_size);
        }
    }
    
    // Test with zero-length buffer
    uLong zero_crc = crc32(0L, Z_NULL, 0);
    
    // Test combine functions with edge cases
    // Test with zero length
    uLong zero_len_result = crc32_combine(crc1, crc2, (z_off_t)0);
    uLong zero_len_result64 = crc32_combine64(crc1, crc2, (z_off64_t)0);
    
    // Test with same CRC values
    uLong same_crc_result = crc32_combine(crc1, crc1, len2_t);
    
    // Test crc32 with null buffer and zero length (should return initial CRC)
    uLong init_crc = crc32(crc1, Z_NULL, 0);
    
    return 0;
}
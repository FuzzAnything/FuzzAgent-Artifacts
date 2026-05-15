// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// compress at compress.c:82:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress at compress.c:82:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

#define MAX_SIZE (10 * 1024 * 1024) // 10 MB limit for safety

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    // Limit input size to prevent excessive memory usage
    if (Size > MAX_SIZE) {
        return 0;
    }

    // Allocate buffers for compression/decompression
    size_t max_compressed_size = compressBound(Size);
    Bytef *compressed_buf = (Bytef*)malloc(max_compressed_size);
    Bytef *decompressed_buf = (Bytef*)malloc(Size * 2); // Extra space for decompression
    
    if (!compressed_buf || !decompressed_buf) {
        free(compressed_buf);
        free(decompressed_buf);
        return 0;
    }

    // Test compress_z
    {
        z_size_t destLen = max_compressed_size;
        int ret = compress_z(compressed_buf, &destLen, Data, (z_size_t)Size);
        // We don't care about the return value for fuzzing, just want to exercise the code
    }

    // Test compress2_z with various compression levels
    {
        z_size_t destLen = max_compressed_size;
        for (int level = -1; level <= 9; level += 2) { // Test a subset of levels
            int ret = compress2_z(compressed_buf, &destLen, Data, (z_size_t)Size, level);
        }
    }

    // Test uncompress_z (needs valid compressed data first)
    {
        z_size_t compressed_size = max_compressed_size;
        z_size_t decompressed_size = Size * 2;
        
        // First compress some data
        int compress_ret = compress_z(compressed_buf, &compressed_size, Data, 
                                     (z_size_t)(Size > 100 ? 100 : Size));
        
        if (compress_ret == Z_OK) {
            // Now try to decompress it
            int ret = uncompress_z(decompressed_buf, &decompressed_size, 
                                  compressed_buf, compressed_size);
        }
    }

    // Test uncompress2_z
    {
        z_size_t compressed_size = max_compressed_size;
        z_size_t decompressed_size = Size * 2;
        z_size_t source_consumed = compressed_size;
        
        // First compress some data
        int compress_ret = compress_z(compressed_buf, &compressed_size, Data, 
                                     (z_size_t)(Size > 100 ? 100 : Size));
        
        if (compress_ret == Z_OK) {
            // Now try to decompress with uncompress2_z
            int ret = uncompress2_z(decompressed_buf, &decompressed_size,
                                   compressed_buf, &source_consumed);
        }
    }

    // Test uncompress (traditional API)
    {
        uLongf destLen = Size * 2;
        uLong sourceLen = Size > 1000 ? 1000 : Size;
        
        // First compress some data
        uLongf compressed_size = max_compressed_size;
        int compress_ret = compress(compressed_buf, &compressed_size, Data, sourceLen);
        
        if (compress_ret == Z_OK) {
            // Now try to decompress
            int ret = uncompress(decompressed_buf, &destLen, compressed_buf, compressed_size);
        }
    }

    // Test uncompress2 (traditional API)
    {
        uLongf destLen = Size * 2;
        uLong sourceLen = Size > 1000 ? 1000 : Size;
        uLong source_consumed = sourceLen;
        
        // First compress some data
        uLongf compressed_size = max_compressed_size;
        int compress_ret = compress(compressed_buf, &compressed_size, Data, sourceLen);
        
        if (compress_ret == Z_OK) {
            // Now try to decompress with uncompress2
            int ret = uncompress2(decompressed_buf, &destLen, compressed_buf, &source_consumed);
        }
    }

    // Also test with random/malformed data directly (without compression first)
    // This helps find bugs in decompression error handling
    if (Size > 0) {
        z_size_t destLen = Size * 2;
        int ret = uncompress_z(decompressed_buf, &destLen, Data, (z_size_t)Size);
        
        uLongf uDestLen = Size * 2;
        ret = uncompress(decompressed_buf, &uDestLen, Data, (uLong)Size);
        
        uLong source_consumed = Size;
        ret = uncompress2(decompressed_buf, &uDestLen, Data, &source_consumed);
    }

    free(compressed_buf);
    free(decompressed_buf);
    
    return 0;
}
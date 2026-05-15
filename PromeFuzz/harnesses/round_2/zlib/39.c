// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "code/zlib.h"

static void fuzz_compress(const uint8_t *data, size_t size) {
    // For compression functions
    if (size == 0) return;
    
    // Calculate maximum compressed size
    z_size_t max_compressed_size = compressBound(size);
    if (max_compressed_size == 0) return;
    
    // Allocate destination buffer
    Bytef *dest = (Bytef *)malloc(max_compressed_size);
    if (!dest) return;
    
    // Test compress_z
    z_size_t destLen = max_compressed_size;
    compress_z(dest, &destLen, data, size);
    
    // Test compress2_z with different compression levels
    for (int level = 0; level <= 9; level += 3) {
        destLen = max_compressed_size;
        compress2_z(dest, &destLen, data, size, level);
    }
    
    free(dest);
}

static void fuzz_uncompress(const uint8_t *data, size_t size) {
    // For decompression functions
    if (size == 0) return;
    
    // Allocate destination buffer (use size * 2 to handle expansion)
    size_t dest_size = size * 2;
    if (dest_size == 0) dest_size = 1;
    
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return;
    
    // Test uncompress with uLong types
    uLongf destLen = dest_size;
    uLong sourceLen = size;
    uncompress(dest, &destLen, data, sourceLen);
    
    // Test uncompress2 with uLong types
    destLen = dest_size;
    uLong sourceLen2 = size;
    uncompress2(dest, &destLen, data, &sourceLen2);
    
    // Test uncompress_z with z_size_t types
    z_size_t destLen_z = dest_size;
    z_size_t sourceLen_z = size;
    uncompress_z(dest, &destLen_z, data, sourceLen_z);
    
    // Test uncompress2_z with z_size_t types
    destLen_z = dest_size;
    z_size_t sourceLen2_z = size;
    uncompress2_z(dest, &destLen_z, data, &sourceLen2_z);
    
    free(dest);
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // First, try compression functions on the raw input
    fuzz_compress(Data, Size);
    
    // Then try decompression functions on the raw input
    fuzz_uncompress(Data, Size);
    
    // Also try decompression on compressed data
    if (Size > 0) {
        // Create compressed version of input
        z_size_t max_compressed = compressBound(Size);
        if (max_compressed > 0 && max_compressed < 10*1024*1024) { // Limit size
            Bytef *compressed = (Bytef *)malloc(max_compressed);
            if (compressed) {
                z_size_t compressed_len = max_compressed;
                // Try compression with default level
                if (compress_z(compressed, &compressed_len, Data, Size) == Z_OK) {
                    // Try decompressing the compressed data
                    fuzz_uncompress(compressed, compressed_len);
                }
                free(compressed);
            }
        }
    }
    
    return 0;
}
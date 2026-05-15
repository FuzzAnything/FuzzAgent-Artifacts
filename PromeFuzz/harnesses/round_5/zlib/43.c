// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress at compress.c:82:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

#define MAX_COMPRESSED_SIZE (1024 * 1024 * 100)  // 100MB limit
#define MAX_UNCOMPRESSED_SIZE (1024 * 1024 * 200) // 200MB limit

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    // Ensure we have at least some data to work with
    if (Size == 0) {
        return 0;
    }

    // Test compressBound
    uLong sourceLen = (uLong)(Size > UINT32_MAX ? UINT32_MAX : Size);
    uLong bound = compressBound(sourceLen);
    
    // If compressBound indicates overflow, skip further compression tests
    if (bound == (uLong)-1) {
        // Still test uncompress with the original data
        Bytef* dest = (Bytef*)malloc(MAX_UNCOMPRESSED_SIZE);
        if (dest) {
            uLongf destLen = MAX_UNCOMPRESSED_SIZE;
            uncompress(dest, &destLen, Data, (uLong)Size);
            free(dest);
        }
        return 0;
    }

    // Limit bound to a reasonable size for allocation
    if (bound > MAX_COMPRESSED_SIZE) {
        bound = MAX_COMPRESSED_SIZE;
    }

    // Allocate buffers for compression tests
    Bytef* compressed = (Bytef*)malloc(bound);
    Bytef* uncompressed = (Bytef*)malloc(MAX_UNCOMPRESSED_SIZE);
    
    if (!compressed || !uncompressed) {
        free(compressed);
        free(uncompressed);
        return 0;
    }

    // Test compress (default compression)
    uLongf destLen = bound;
    int ret = compress(compressed, &destLen, Data, sourceLen);
    if (ret == Z_OK) {
        // Try to uncompress the compressed data
        uLongf uncompLen = MAX_UNCOMPRESSED_SIZE;
        uncompress(uncompressed, &uncompLen, compressed, destLen);
    }

    // Test compress2 with various compression levels
    // Use first byte modulo 10 as compression level (0-9)
    int level = Data[0] % 10;
    destLen = bound;
    ret = compress2(compressed, &destLen, Data, sourceLen, level);
    if (ret == Z_OK) {
        uLongf uncompLen = MAX_UNCOMPRESSED_SIZE;
        uncompress(uncompressed, &uncompLen, compressed, destLen);
    }

    // Test compress_z (size_t version)
    z_size_t sourceLen_z = (z_size_t)sourceLen;
    z_size_t destLen_z = (z_size_t)bound;
    ret = compress_z(compressed, &destLen_z, Data, sourceLen_z);
    if (ret == Z_OK) {
        uLongf uncompLen = MAX_UNCOMPRESSED_SIZE;
        uncompress(uncompressed, &uncompLen, compressed, (uLong)destLen_z);
    }

    // Test compress2_z (size_t version with compression level)
    destLen_z = (z_size_t)bound;
    ret = compress2_z(compressed, &destLen_z, Data, sourceLen_z, level);
    if (ret == Z_OK) {
        uLongf uncompLen = MAX_UNCOMPRESSED_SIZE;
        uncompress(uncompressed, &uncompLen, compressed, (uLong)destLen_z);
    }

    // Test uncompress with original data (may fail if not compressed)
    uLongf uncompLen = MAX_UNCOMPRESSED_SIZE;
    uncompress(uncompressed, &uncompLen, Data, (uLong)Size);

    // Clean up
    free(compressed);
    free(uncompressed);

    return 0;
}
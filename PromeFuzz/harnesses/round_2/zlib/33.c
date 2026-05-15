// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE (1024 * 1024)  // 1 MB limit to avoid excessive memory usage

static void test_uncompress2_z(const uint8_t *data, size_t size) {
    // Allocate destination buffer with reasonable size
    z_size_t destLen = MAX_SIZE;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    // Prepare source parameters
    z_size_t sourceLen = size > MAX_SIZE ? MAX_SIZE : (z_size_t)size;
    const Bytef *source = (const Bytef *)data;
    
    // Call the function
    int ret = uncompress2_z(dest, &destLen, source, &sourceLen);
    
    // Cleanup
    free(dest);
}

static void test_uncompress(const uint8_t *data, size_t size) {
    // Allocate destination buffer
    uLongf destLen = MAX_SIZE;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    // Prepare source parameters
    uLong sourceLen = size > MAX_SIZE ? MAX_SIZE : (uLong)size;
    const Bytef *source = (const Bytef *)data;
    
    // Call the function
    int ret = uncompress(dest, &destLen, source, sourceLen);
    
    // Cleanup
    free(dest);
}

static void test_compress2_z(const uint8_t *data, size_t size) {
    // Calculate maximum compressed size
    z_size_t sourceLen = size > MAX_SIZE ? MAX_SIZE : (z_size_t)size;
    z_size_t destLen = compressBound(sourceLen);
    
    // Allocate destination buffer
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    const Bytef *source = (const Bytef *)data;
    
    // Test with different compression levels
    for (int level = 0; level <= 9; level++) {
        z_size_t currentDestLen = destLen;
        int ret = compress2_z(dest, &currentDestLen, source, sourceLen, level);
    }
    
    // Cleanup
    free(dest);
}

static void test_compress(const uint8_t *data, size_t size) {
    // Calculate maximum compressed size
    uLong sourceLen = size > MAX_SIZE ? MAX_SIZE : (uLong)size;
    uLongf destLen = compressBound(sourceLen);
    
    // Allocate destination buffer
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    const Bytef *source = (const Bytef *)data;
    
    // Call the function
    int ret = compress(dest, &destLen, source, sourceLen);
    
    // Cleanup
    free(dest);
}

static void test_compress_z(const uint8_t *data, size_t size) {
    // Calculate maximum compressed size
    z_size_t sourceLen = size > MAX_SIZE ? MAX_SIZE : (z_size_t)size;
    z_size_t destLen = compressBound(sourceLen);
    
    // Allocate destination buffer
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    const Bytef *source = (const Bytef *)data;
    
    // Call the function
    int ret = compress_z(dest, &destLen, source, sourceLen);
    
    // Cleanup
    free(dest);
}

static void test_uncompress_z(const uint8_t *data, size_t size) {
    // Allocate destination buffer
    z_size_t destLen = MAX_SIZE;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    // Prepare source parameters
    z_size_t sourceLen = size > MAX_SIZE ? MAX_SIZE : (z_size_t)size;
    const Bytef *source = (const Bytef *)data;
    
    // Call the function
    int ret = uncompress_z(dest, &destLen, source, sourceLen);
    
    // Cleanup
    free(dest);
}

int LLVMFuzzerTestOneInput_33(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Test all target functions
    test_uncompress2_z(data, size);
    test_uncompress(data, size);
    test_compress2_z(data, size);
    test_compress(data, size);
    test_compress_z(data, size);
    test_uncompress_z(data, size);
    
    return 0;
}
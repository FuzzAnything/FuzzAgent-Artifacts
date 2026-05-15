// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
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

#define MAX_COMPRESSED_SIZE (1024 * 1024)
#define MAX_UNCOMPRESSED_SIZE (10 * 1024 * 1024)

static void test_compress2_z(const uint8_t *data, size_t size) {
    z_size_t destLen = compressBound(size);
    if (destLen > MAX_COMPRESSED_SIZE) {
        destLen = MAX_COMPRESSED_SIZE;
    }
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t sourceLen = size;
    int level = data[0] % 10;  // Use first byte to determine compression level (0-9)
    
    int ret = compress2_z(dest, &destLen, data, sourceLen, level);
    
    free(dest);
}

static void test_uncompress2_z(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    z_size_t destLen = MAX_UNCOMPRESSED_SIZE;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t sourceLen = size;
    
    int ret = uncompress2_z(dest, &destLen, data, &sourceLen);
    
    free(dest);
}

static void test_uncompress_z(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    z_size_t destLen = MAX_UNCOMPRESSED_SIZE;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t sourceLen = size;
    
    int ret = uncompress_z(dest, &destLen, data, sourceLen);
    
    free(dest);
}

static void test_uncompress2(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    uLongf destLen = MAX_UNCOMPRESSED_SIZE;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLong sourceLen = size;
    
    int ret = uncompress2(dest, &destLen, data, &sourceLen);
    
    free(dest);
}

static void test_uncompress(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    uLongf destLen = MAX_UNCOMPRESSED_SIZE;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLong sourceLen = size;
    
    int ret = uncompress(dest, &destLen, data, sourceLen);
    
    free(dest);
}

static void test_compress_z(const uint8_t *data, size_t size) {
    z_size_t destLen = compressBound(size);
    if (destLen > MAX_COMPRESSED_SIZE) {
        destLen = MAX_COMPRESSED_SIZE;
    }
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t sourceLen = size;
    
    int ret = compress_z(dest, &destLen, data, sourceLen);
    
    free(dest);
}

static void test_compress_decompress_cycle(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // Compress with compress2_z
    z_size_t compressed_len = compressBound(size);
    if (compressed_len > MAX_COMPRESSED_SIZE) {
        compressed_len = MAX_COMPRESSED_SIZE;
    }
    
    Bytef *compressed = (Bytef *)malloc(compressed_len);
    if (!compressed) return;
    
    z_size_t source_len = size;
    int level = data[0] % 10;
    
    int compress_ret = compress2_z(compressed, &compressed_len, data, source_len, level);
    
    if (compress_ret == Z_OK) {
        // Try to decompress with various functions
        z_size_t decompressed_len = MAX_UNCOMPRESSED_SIZE;
        Bytef *decompressed = (Bytef *)malloc(decompressed_len);
        if (decompressed) {
            // Test uncompress2_z
            z_size_t consumed_len = compressed_len;
            uncompress2_z(decompressed, &decompressed_len, compressed, &consumed_len);
            
            // Test uncompress_z
            decompressed_len = MAX_UNCOMPRESSED_SIZE;
            uncompress_z(decompressed, &decompressed_len, compressed, compressed_len);
            
            // Test uncompress2
            uLongf dest_len = MAX_UNCOMPRESSED_SIZE;
            uLong src_len = compressed_len;
            uncompress2(decompressed, &dest_len, compressed, &src_len);
            
            // Test uncompress
            dest_len = MAX_UNCOMPRESSED_SIZE;
            uncompress(decompressed, &dest_len, compressed, compressed_len);
            
            free(decompressed);
        }
    }
    
    free(compressed);
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test each function individually
    test_compress2_z(Data, Size);
    test_uncompress2_z(Data, Size);
    test_uncompress_z(Data, Size);
    test_uncompress2(Data, Size);
    test_uncompress(Data, Size);
    test_compress_z(Data, Size);
    
    // Test compress-decompress cycle
    test_compress_decompress_cycle(Data, Size);
    
    return 0;
}
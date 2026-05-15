// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEST_SIZE (1024 * 1024)  // 1MB maximum for destination buffers

static void test_uncompress2_z(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    z_size_t sourceLen = (z_size_t)size;
    z_size_t destLen = MAX_DEST_SIZE;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int ret = uncompress2_z(dest, &destLen, data, &sourceLen);
    (void)ret;  // Use return value to avoid unused variable warning
    
    free(dest);
}

static void test_compress2(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    uLongf destLen = compressBound((uLong)size);
    if (destLen > MAX_DEST_SIZE) destLen = MAX_DEST_SIZE;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    // Use first byte as compression level (0-9)
    int level = data[0] % 10;
    const Bytef *source = data + 1;
    uLong sourceLen = (uLong)(size - 1);
    
    if (sourceLen > 0) {
        int ret = compress2(dest, &destLen, source, sourceLen, level);
        (void)ret;
    }
    
    free(dest);
}

static void test_uncompress(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    uLongf destLen = MAX_DEST_SIZE;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int ret = uncompress(dest, &destLen, data, (uLong)size);
    (void)ret;
    
    free(dest);
}

static void test_compress2_z(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    z_size_t sourceLen = (z_size_t)size;
    z_size_t destLen = (z_size_t)compressBound((uLong)size);
    if (destLen > MAX_DEST_SIZE) destLen = MAX_DEST_SIZE;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    // Use first byte as compression level (0-9)
    int level = data[0] % 10;
    const Bytef *source = data + 1;
    sourceLen = (z_size_t)(size - 1);
    
    if (sourceLen > 0) {
        int ret = compress2_z(dest, &destLen, source, sourceLen, level);
        (void)ret;
    }
    
    free(dest);
}

static void test_compress(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    uLongf destLen = compressBound((uLong)size);
    if (destLen > MAX_DEST_SIZE) destLen = MAX_DEST_SIZE;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int ret = compress(dest, &destLen, data, (uLong)size);
    (void)ret;
    
    free(dest);
}

static void test_compress_z(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    z_size_t sourceLen = (z_size_t)size;
    z_size_t destLen = (z_size_t)compressBound((uLong)size);
    if (destLen > MAX_DEST_SIZE) destLen = MAX_DEST_SIZE;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int ret = compress_z(dest, &destLen, data, sourceLen);
    (void)ret;
    
    free(dest);
}

int LLVMFuzzerTestOneInput_35(const uint8_t *data, size_t size) {
    // Test all target functions with the input data
    test_uncompress2_z(data, size);
    test_compress2(data, size);
    test_uncompress(data, size);
    test_compress2_z(data, size);
    test_compress(data, size);
    test_compress_z(data, size);
    
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compress2_z at compress.c:24:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void test_uncompress2(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLongf destLen = 65536;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLong sourceLen = (uLong)size;
    uLong consumed = sourceLen;
    
    int ret = uncompress2(dest, &destLen, data, &consumed);
    (void)ret;
    
    free(dest);
}

static void test_uncompress(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLongf destLen = 65536;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLong sourceLen = (uLong)size;
    
    int ret = uncompress(dest, &destLen, data, sourceLen);
    (void)ret;
    
    free(dest);
}

static void test_compress2(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLongf destLen = compressBound((uLong)size);
    if (destLen == 0) return;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLong sourceLen = (uLong)size;
    int level = data[0] % 10;
    
    int ret = compress2(dest, &destLen, data, sourceLen, level);
    (void)ret;
    
    free(dest);
}

static void test_uncompress2_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    z_size_t destLen = 65536;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t sourceLen = (z_size_t)size;
    z_size_t consumed = sourceLen;
    
    int ret = uncompress2_z(dest, &destLen, data, &consumed);
    (void)ret;
    
    free(dest);
}

static void test_compress(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLongf destLen = compressBound((uLong)size);
    if (destLen == 0) return;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLong sourceLen = (uLong)size;
    
    int ret = compress(dest, &destLen, data, sourceLen);
    (void)ret;
    
    free(dest);
}

static void test_compress2_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    z_size_t destLen = (z_size_t)compressBound((uLong)size);
    if (destLen == 0) return;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t sourceLen = (z_size_t)size;
    int level = data[0] % 10;
    
    int ret = compress2_z(dest, &destLen, data, sourceLen, level);
    (void)ret;
    
    free(dest);
}

int LLVMFuzzerTestOneInput_50(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    test_uncompress2(data, size);
    test_uncompress(data, size);
    test_compress2(data, size);
    test_uncompress2_z(data, size);
    test_compress(data, size);
    test_compress2_z(data, size);
    
    return 0;
}
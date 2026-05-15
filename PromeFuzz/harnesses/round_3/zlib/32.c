// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_compress_bound(const uint8_t *data, size_t size) {
    if (size > 0) {
        uLong sourceLen = (uLong)(size & 0xFFFFFFFF);
        compressBound(sourceLen);
    }
}

static void test_compress(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong sourceLen = (uLong)(size & 0xFFFFFFFF);
    uLongf destLen = compressBound(sourceLen);
    if (destLen == (uLong)-1) return;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int ret = compress(dest, &destLen, data, sourceLen);
    (void)ret;
    
    free(dest);
}

static void test_compress_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    z_size_t sourceLen = (z_size_t)size;
    uLong bound = compressBound((uLong)(size & 0xFFFFFFFF));
    if (bound == (uLong)-1) return;
    
    z_size_t destLen = (z_size_t)bound;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int ret = compress_z(dest, &destLen, data, sourceLen);
    (void)ret;
    
    free(dest);
}

static void test_compress2(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong sourceLen = (uLong)(size & 0xFFFFFFFF);
    uLongf destLen = compressBound(sourceLen);
    if (destLen == (uLong)-1) return;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int level = data[0] % 10;
    int ret = compress2(dest, &destLen, data, sourceLen, level);
    (void)ret;
    
    free(dest);
}

static void test_compress2_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    z_size_t sourceLen = (z_size_t)size;
    uLong bound = compressBound((uLong)(size & 0xFFFFFFFF));
    if (bound == (uLong)-1) return;
    
    z_size_t destLen = (z_size_t)bound;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int level = data[0] % 10;
    int ret = compress2_z(dest, &destLen, data, sourceLen, level);
    (void)ret;
    
    free(dest);
}

static void test_uncompress(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong sourceLen = (uLong)(size & 0xFFFFFFFF);
    uLongf destLen = sourceLen * 2 + 100;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int ret = uncompress(dest, &destLen, data, sourceLen);
    (void)ret;
    
    free(dest);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    test_compress_bound(Data, Size);
    test_compress(Data, Size);
    test_compress_z(Data, Size);
    test_compress2(Data, Size);
    test_compress2_z(Data, Size);
    test_uncompress(Data, Size);
    return 0;
}
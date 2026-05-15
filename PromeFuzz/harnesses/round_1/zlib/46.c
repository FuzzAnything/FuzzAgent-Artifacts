// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// uncompress2 at uncompr.c:83:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void test_uncompress2(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    uLongf destLen = size * 4;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLong sourceLen = size;
    int ret = uncompress2(dest, &destLen, data, &sourceLen);
    
    free(dest);
}

static void test_compress_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    z_size_t sourceLen = size;
    z_size_t destLen = compressBound(sourceLen);
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int ret = compress_z(dest, &destLen, data, sourceLen);
    
    free(dest);
}

static void test_compress2(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLongf destLen = compressBound(size);
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLong sourceLen = size;
    
    for (int level = 0; level <= 9; level++) {
        uLongf currentDestLen = destLen;
        int ret = compress2(dest, &currentDestLen, data, sourceLen, level);
    }
    
    free(dest);
}

static void test_compress2_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    z_size_t sourceLen = size;
    z_size_t destLen = compressBound(sourceLen);
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    for (int level = 0; level <= 9; level++) {
        z_size_t currentDestLen = destLen;
        int ret = compress2_z(dest, &currentDestLen, data, sourceLen, level);
    }
    
    free(dest);
}

static void test_uncompress2_z(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    z_size_t destLen = size * 4;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t sourceLen = size;
    int ret = uncompress2_z(dest, &destLen, data, &sourceLen);
    
    free(dest);
}

static void test_uncompress_z(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    z_size_t destLen = size * 4;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t sourceLen = size;
    int ret = uncompress_z(dest, &destLen, data, sourceLen);
    
    free(dest);
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_uncompress2(Data, Size);
    test_compress_z(Data, Size);
    test_compress2(Data, Size);
    test_compress2_z(Data, Size);
    test_uncompress2_z(Data, Size);
    test_uncompress_z(Data, Size);
    
    return 0;
}
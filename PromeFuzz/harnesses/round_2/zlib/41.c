// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
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

static void test_uncompress2_z(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    z_size_t destLen = size * 2 + 100;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t sourceLen = size;
    int ret = uncompress2_z(dest, &destLen, data, &sourceLen);
    
    free(dest);
}

static void test_uncompress(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    uLongf destLen = size * 2 + 100;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLong sourceLen = (uLong)size;
    int ret = uncompress(dest, &destLen, data, sourceLen);
    
    free(dest);
}

static void test_uncompress2(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    uLongf destLen = size * 2 + 100;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLong sourceLen = (uLong)size;
    int ret = uncompress2(dest, &destLen, data, &sourceLen);
    
    free(dest);
}

static void test_compress2_z(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    z_size_t sourceLen = size;
    z_size_t destLen = compressBound(sourceLen);
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int level = data[0] % 10;
    int ret = compress2_z(dest, &destLen, data, sourceLen, level);
    
    free(dest);
}

static void test_compress_z(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    z_size_t sourceLen = size;
    z_size_t destLen = compressBound(sourceLen);
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int ret = compress_z(dest, &destLen, data, sourceLen);
    
    free(dest);
}

static void test_uncompress_z(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    z_size_t destLen = size * 2 + 100;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t sourceLen = size;
    int ret = uncompress_z(dest, &destLen, data, sourceLen);
    
    free(dest);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_uncompress2_z(Data, Size);
    test_uncompress(Data, Size);
    test_uncompress2(Data, Size);
    test_compress2_z(Data, Size);
    test_compress_z(Data, Size);
    test_uncompress_z(Data, Size);
    
    return 0;
}
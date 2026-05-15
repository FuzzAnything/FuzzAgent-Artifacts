// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateBound at deflate.c:929:15 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateBound_z at deflate.c:856:18 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_compress(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong sourceLen = (uLong)size;
    uLongf destLen = compressBound(sourceLen);
    
    if (destLen == (uLong)-1) {
        return;
    }
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int ret = compress(dest, &destLen, data, sourceLen);
    (void)ret;
    
    free(dest);
}

static void test_compressBound_z(const uint8_t *data, size_t size) {
    z_size_t sourceLen = (z_size_t)size;
    z_size_t bound = compressBound_z(sourceLen);
    (void)bound;
}

static void test_compressBound(const uint8_t *data, size_t size) {
    uLong sourceLen = (uLong)size;
    uLong bound = compressBound(sourceLen);
    (void)bound;
}

static void test_compress2_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    z_size_t sourceLen = (z_size_t)size;
    z_size_t destLen = (z_size_t)compressBound((uLong)sourceLen);
    
    if (destLen == (z_size_t)-1) {
        return;
    }
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    int level = data[0] % 10;
    int ret = compress2_z(dest, &destLen, data, sourceLen, level);
    (void)ret;
    
    free(dest);
}

static void test_deflateBound(const uint8_t *data, size_t size) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return;
    }
    
    uLong sourceLen = (uLong)size;
    uLong bound = deflateBound(&strm, sourceLen);
    (void)bound;
    
    deflateEnd(&strm);
}

static void test_deflateBound_z(const uint8_t *data, size_t size) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return;
    }
    
    z_size_t sourceLen = (z_size_t)size;
    z_size_t bound = deflateBound_z(&strm, sourceLen);
    (void)bound;
    
    deflateEnd(&strm);
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    test_compress(Data, Size);
    test_compressBound_z(Data, Size);
    test_compressBound(Data, Size);
    test_compress2_z(Data, Size);
    test_deflateBound(Data, Size);
    test_deflateBound_z(Data, Size);
    
    return 0;
}
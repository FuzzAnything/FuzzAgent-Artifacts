// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound_z at compress.c:91:18 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

static void test_compress(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong sourceLen = (uLong)size;
    uLong destLen = compressBound(sourceLen);
    
    if (destLen == (uLong)-1) return;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLongf destLenPtr = destLen;
    int ret = compress(dest, &destLenPtr, (const Bytef *)data, sourceLen);
    
    free(dest);
    (void)ret;
}

static void test_compress2(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong sourceLen = (uLong)size;
    uLong destLen = compressBound(sourceLen);
    
    if (destLen == (uLong)-1) return;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    uLongf destLenPtr = destLen;
    int level = 0;
    if (size > 0) level = (int)data[0] % 10;
    
    int ret = compress2(dest, &destLenPtr, (const Bytef *)data, sourceLen, level);
    
    free(dest);
    (void)ret;
}

static void test_compress_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    z_size_t sourceLen = (z_size_t)size;
    z_size_t destLen = compressBound_z(sourceLen);
    
    if (destLen == (z_size_t)-1) return;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t destLenPtr = destLen;
    int ret = compress_z(dest, &destLenPtr, (const Bytef *)data, sourceLen);
    
    free(dest);
    (void)ret;
}

static void test_compress2_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    z_size_t sourceLen = (z_size_t)size;
    z_size_t destLen = compressBound_z(sourceLen);
    
    if (destLen == (z_size_t)-1) return;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return;
    
    z_size_t destLenPtr = destLen;
    int level = 0;
    if (size > 0) level = (int)data[0] % 10;
    
    int ret = compress2_z(dest, &destLenPtr, (const Bytef *)data, sourceLen, level);
    
    free(dest);
    (void)ret;
}

static void test_compressBound(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong sourceLen = (uLong)size;
    uLong bound = compressBound(sourceLen);
    (void)bound;
    
    if (size > 1) {
        uLong sourceLen2 = (uLong)data[0] | ((uLong)data[1] << 8);
        bound = compressBound(sourceLen2);
        (void)bound;
    }
}

static void test_compressBound_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    z_size_t sourceLen = (z_size_t)size;
    z_size_t bound = compressBound_z(sourceLen);
    (void)bound;
    
    if (size > 1) {
        z_size_t sourceLen2 = (z_size_t)data[0] | ((z_size_t)data[1] << 8);
        bound = compressBound_z(sourceLen2);
        (void)bound;
    }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_compressBound(Data, Size);
    test_compressBound_z(Data, Size);
    test_compress(Data, Size);
    test_compress2(Data, Size);
    test_compress_z(Data, Size);
    test_compress2_z(Data, Size);
    
    return 0;
}
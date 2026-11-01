// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound_z at compress.c:91:18 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static int test_compress_z(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    z_size_t destLen = compressBound_z(size);
    if (destLen == (z_size_t)-1) return 0;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return 0;
    
    z_size_t actualDestLen = destLen;
    int ret = compress_z(dest, &actualDestLen, (const Bytef *)data, size);
    
    free(dest);
    return ret;
}

static int test_compress2_z(const uint8_t *data, size_t size, int level) {
    if (size == 0) return 0;
    
    z_size_t destLen = compressBound_z(size);
    if (destLen == (z_size_t)-1) return 0;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return 0;
    
    z_size_t actualDestLen = destLen;
    int ret = compress2_z(dest, &actualDestLen, (const Bytef *)data, size, level);
    
    free(dest);
    return ret;
}

static int test_compress(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    uLong destLen = compressBound(size);
    if (destLen == (uLong)-1) return 0;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return 0;
    
    uLong actualDestLen = destLen;
    int ret = compress(dest, &actualDestLen, (const Bytef *)data, size);
    
    free(dest);
    return ret;
}

static int test_compress2(const uint8_t *data, size_t size, int level) {
    if (size == 0) return 0;
    
    uLong destLen = compressBound(size);
    if (destLen == (uLong)-1) return 0;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return 0;
    
    uLong actualDestLen = destLen;
    int ret = compress2(dest, &actualDestLen, (const Bytef *)data, size, level);
    
    free(dest);
    return ret;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test compressBound_z
    z_size_t bound_z = compressBound_z(Size);
    (void)bound_z;

    // Test compressBound
    uLong bound = compressBound(Size);
    (void)bound;

    // Test compress_z
    test_compress_z(Data, Size);

    // Test compress2_z with various levels
    for (int level = -1; level <= 10; level++) {
        test_compress2_z(Data, Size, level);
    }

    // Test compress
    test_compress(Data, Size);

    // Test compress2 with various levels
    for (int level = -1; level <= 10; level++) {
        test_compress2(Data, Size, level);
    }

    return 0;
}
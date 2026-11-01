// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compress_z at compress.c:77:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
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

static void test_uncompress(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    // Allocate destination buffer (assume max expansion)
    size_t dest_size = Size * 2 + 1024;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return;
    
    uLongf destLen = dest_size;
    uLong sourceLen = Size;
    
    uncompress(dest, &destLen, (const Bytef *)Data, sourceLen);
    
    free(dest);
}

static void test_compress2_z(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Allocate destination buffer
    z_size_t dest_size = compressBound(Size);
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return;
    
    z_size_t destLen = dest_size;
    int level = (Data[0] % 10) - 1; // Level -1 to 8
    
    compress2_z(dest, &destLen, (const Bytef *)Data, Size, level);
    
    free(dest);
}

static void test_uncompress2_z(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    // Allocate destination buffer
    z_size_t dest_size = Size * 2 + 1024;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return;
    
    z_size_t destLen = dest_size;
    z_size_t sourceLen = Size;
    
    uncompress2_z(dest, &destLen, (const Bytef *)Data, &sourceLen);
    
    free(dest);
}

static void test_uncompress_z(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    // Allocate destination buffer
    z_size_t dest_size = Size * 2 + 1024;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return;
    
    z_size_t destLen = dest_size;
    z_size_t sourceLen = Size;
    
    uncompress_z(dest, &destLen, (const Bytef *)Data, sourceLen);
    
    free(dest);
}

static void test_uncompress2(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    // Allocate destination buffer
    size_t dest_size = Size * 2 + 1024;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return;
    
    uLongf destLen = dest_size;
    uLong sourceLen = Size;
    
    uncompress2(dest, &destLen, (const Bytef *)Data, &sourceLen);
    
    free(dest);
}

static void test_compress_z(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Allocate destination buffer
    z_size_t dest_size = compressBound(Size);
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return;
    
    z_size_t destLen = dest_size;
    
    compress_z(dest, &destLen, (const Bytef *)Data, Size);
    
    free(dest);
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test uncompress
    test_uncompress(Data, Size);
    
    // Test compress2_z
    test_compress2_z(Data, Size);
    
    // Test uncompress2_z
    test_uncompress2_z(Data, Size);
    
    // Test uncompress_z
    test_uncompress_z(Data, Size);
    
    // Test uncompress2
    test_uncompress2(Data, Size);
    
    // Test compress_z
    test_compress_z(Data, Size);
    
    return 0;
}
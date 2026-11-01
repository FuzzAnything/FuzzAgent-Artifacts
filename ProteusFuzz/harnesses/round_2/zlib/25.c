// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void test_compress_z(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Allocate destination buffer with generous size
    size_t dest_size = compressBound(Size);
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return;
    
    z_size_t destLen = dest_size;
    int ret = compress_z(dest, &destLen, (const Bytef *)Data, Size);
    
    // Try to decompress the result if compression succeeded
    if (ret == Z_OK) {
        Bytef *uncomp = (Bytef *)malloc(Size);
        if (uncomp) {
            uLongf uncompLen = Size;
            uncompress(uncomp, &uncompLen, dest, destLen);
            free(uncomp);
        }
    }
    
    free(dest);
}

static void test_compress2_z(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Try different compression levels
    int levels[] = {Z_NO_COMPRESSION, Z_BEST_SPEED, Z_DEFAULT_COMPRESSION, Z_BEST_COMPRESSION};
    
    for (int i = 0; i < 4; i++) {
        size_t dest_size = compressBound(Size);
        Bytef *dest = (Bytef *)malloc(dest_size);
        if (!dest) continue;
        
        z_size_t destLen = dest_size;
        int ret = compress2_z(dest, &destLen, (const Bytef *)Data, Size, levels[i]);
        
        if (ret == Z_OK) {
            Bytef *uncomp = (Bytef *)malloc(Size);
            if (uncomp) {
                uLongf uncompLen = Size;
                uncompress(uncomp, &uncompLen, dest, destLen);
                free(uncomp);
            }
        }
        
        free(dest);
    }
}

static void test_uncompress(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Try to decompress with various destination sizes
    size_t dest_sizes[] = {Size, Size * 2, Size * 10, 1024};
    
    for (int i = 0; i < 4; i++) {
        size_t dest_size = dest_sizes[i];
        Bytef *dest = (Bytef *)malloc(dest_size);
        if (!dest) continue;
        
        uLongf destLen = dest_size;
        uLong sourceLen = Size;
        int ret = uncompress(dest, &destLen, (const Bytef *)Data, sourceLen);
        
        free(dest);
    }
}

static void test_uncompress_z(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    size_t dest_size = Size * 10;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return;
    
    z_size_t destLen = dest_size;
    z_size_t sourceLen = Size;
    int ret = uncompress_z(dest, &destLen, (const Bytef *)Data, sourceLen);
    
    free(dest);
}

static void test_uncompress2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    size_t dest_size = Size * 10;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return;
    
    uLongf destLen = dest_size;
    uLong sourceLen = Size;
    int ret = uncompress2(dest, &destLen, (const Bytef *)Data, &sourceLen);
    
    free(dest);
}

static void test_uncompress2_z(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    // Test with NULL dest to calculate required size
    z_size_t destLen = 0;
    z_size_t sourceLen = Size;
    int ret = uncompress2_z(NULL, &destLen, (const Bytef *)Data, &sourceLen);
    
    // If we got a size, allocate and try again
    if (ret == Z_OK && destLen > 0) {
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            z_size_t destLen2 = destLen;
            z_size_t sourceLen2 = Size;
            ret = uncompress2_z(dest, &destLen2, (const Bytef *)Data, &sourceLen2);
            free(dest);
        }
    }
    
    // Also test with a pre-allocated buffer
    size_t dest_size = Size * 10;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return;
    
    z_size_t destLen2 = dest_size;
    z_size_t sourceLen2 = Size;
    ret = uncompress2_z(dest, &destLen2, (const Bytef *)Data, &sourceLen2);
    
    free(dest);
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test all compression functions
    test_compress_z(Data, Size);
    test_compress2_z(Data, Size);
    
    // Test all decompression functions
    test_uncompress(Data, Size);
    test_uncompress_z(Data, Size);
    test_uncompress2(Data, Size);
    test_uncompress2_z(Data, Size);
    
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize buffers with reasonable sizes
    size_t max_dest_size = Size * 4 + 1024;
    if (max_dest_size > 1024 * 1024) max_dest_size = 1024 * 1024;
    
    Bytef *dest = (Bytef *)malloc(max_dest_size);
    Bytef *source_copy = (Bytef *)malloc(Size);
    if (!dest || !source_copy) {
        free(dest);
        free(source_copy);
        return 0;
    }
    memcpy(source_copy, Data, Size);
    
    // Test uncompress2
    {
        uLongf destLen = (uLongf)max_dest_size;
        uLong sourceLen = (uLong)Size;
        int ret = uncompress2(dest, &destLen, source_copy, &sourceLen);
        (void)ret; // Use ret to avoid unused variable warning
    }
    
    // Test uncompress_z
    {
        z_size_t destLen = (z_size_t)max_dest_size;
        z_size_t sourceLen = (z_size_t)Size;
        int ret = uncompress_z(dest, &destLen, source_copy, sourceLen);
        (void)ret;
    }
    
    // Test uncompress
    {
        uLongf destLen = (uLongf)max_dest_size;
        uLong sourceLen = (uLong)Size;
        int ret = uncompress(dest, &destLen, source_copy, sourceLen);
        (void)ret;
    }
    
    // Test uncompress2_z
    {
        z_size_t destLen = (z_size_t)max_dest_size;
        z_size_t sourceLen = (z_size_t)Size;
        int ret = uncompress2_z(dest, &destLen, source_copy, &sourceLen);
        (void)ret;
    }
    
    // Test compress_z
    {
        z_size_t destLen = (z_size_t)max_dest_size;
        z_size_t sourceLen = (z_size_t)Size;
        int ret = compress_z(dest, &destLen, source_copy, sourceLen);
        (void)ret;
    }
    
    // Test compress2_z
    {
        z_size_t destLen = (z_size_t)max_dest_size;
        z_size_t sourceLen = (z_size_t)Size;
        int level = Data[0] % 10; // Use first byte for compression level (0-9)
        int ret = compress2_z(dest, &destLen, source_copy, sourceLen, level);
        (void)ret;
    }
    
    free(dest);
    free(source_copy);
    return 0;
}

#ifdef __cplusplus
}
#endif
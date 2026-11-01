// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int fuzz_compress_z(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    size_t dest_size = compressBound(size);
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return 0;
    
    z_size_t destLen = dest_size;
    int ret = compress_z(dest, &destLen, (const Bytef *)data, size);
    
    free(dest);
    return ret;
}

static int fuzz_compress2_z(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    size_t dest_size = compressBound(size);
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return 0;
    
    z_size_t destLen = dest_size;
    int level = Z_DEFAULT_COMPRESSION;
    int ret = compress2_z(dest, &destLen, (const Bytef *)data, size, level);
    
    free(dest);
    return ret;
}

static int fuzz_uncompress(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    uLong dest_size = (uLong)(size * 10 + 1024);
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return 0;
    
    uLongf destLen = dest_size;
    int ret = uncompress(dest, &destLen, (const Bytef *)data, (uLong)size);
    
    free(dest);
    return ret;
}

static int fuzz_uncompress_z(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    z_size_t dest_size = size * 10 + 1024;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return 0;
    
    z_size_t destLen = dest_size;
    int ret = uncompress_z(dest, &destLen, (const Bytef *)data, size);
    
    free(dest);
    return ret;
}

static int fuzz_uncompress2(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    uLong dest_size = (uLong)(size * 10 + 1024);
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return 0;
    
    uLongf destLen = dest_size;
    uLong sourceLen = (uLong)size;
    int ret = uncompress2(dest, &destLen, (const Bytef *)data, &sourceLen);
    
    free(dest);
    return ret;
}

static int fuzz_uncompress2_z(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    z_size_t dest_size = size * 10 + 1024;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) return 0;
    
    z_size_t destLen = dest_size;
    z_size_t sourceLen = size;
    int ret = uncompress2_z(dest, &destLen, (const Bytef *)data, &sourceLen);
    
    free(dest);
    return ret;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz compress_z
    fuzz_compress_z(Data, Size);
    
    // Fuzz compress2_z with different compression levels
    for (int level = Z_NO_COMPRESSION; level <= Z_BEST_COMPRESSION; level++) {
        size_t dest_size = compressBound(Size);
        Bytef *dest = (Bytef *)malloc(dest_size);
        if (!dest) continue;
        
        z_size_t destLen = dest_size;
        compress2_z(dest, &destLen, (const Bytef *)Data, Size, level);
        free(dest);
    }
    
    // Fuzz uncompress
    fuzz_uncompress(Data, Size);
    
    // Fuzz uncompress_z
    fuzz_uncompress_z(Data, Size);
    
    // Fuzz uncompress2
    fuzz_uncompress2(Data, Size);
    
    // Fuzz uncompress2_z
    fuzz_uncompress2_z(Data, Size);
    
    return 0;
}
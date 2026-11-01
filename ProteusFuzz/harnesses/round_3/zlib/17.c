// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
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

static int fuzz_uncompress2_z(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t split = Size / 2;
    if (split == 0) split = 1;
    
    const Bytef *source = (const Bytef *)(Data + split);
    z_size_t sourceLen = Size - split;
    
    Bytef *dest = NULL;
    z_size_t destLen = 0;
    
    // First call to get required size
    int ret = uncompress2_z(NULL, &destLen, source, &sourceLen);
    if (ret != Z_OK && ret != Z_BUF_ERROR && ret != Z_DATA_ERROR) {
        return 0;
    }
    
    if (destLen > 0) {
        dest = (Bytef *)malloc(destLen);
        if (!dest) return 0;
        
        ret = uncompress2_z(dest, &destLen, source, &sourceLen);
        free(dest);
    }
    return 0;
}

static int fuzz_uncompress_z(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t split = Size / 2;
    if (split == 0) split = 1;
    
    const Bytef *source = (const Bytef *)(Data + split);
    z_size_t sourceLen = Size - split;
    
    Bytef *dest = NULL;
    z_size_t destLen = 0;
    
    // First call to get required size
    int ret = uncompress_z(NULL, &destLen, source, sourceLen);
    if (ret != Z_OK && ret != Z_BUF_ERROR && ret != Z_DATA_ERROR) {
        return 0;
    }
    
    if (destLen > 0) {
        dest = (Bytef *)malloc(destLen);
        if (!dest) return 0;
        
        ret = uncompress_z(dest, &destLen, source, sourceLen);
        free(dest);
    }
    return 0;
}

static int fuzz_uncompress2(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t split = Size / 2;
    if (split == 0) split = 1;
    
    const Bytef *source = (const Bytef *)(Data + split);
    uLong sourceLen = (uLong)(Size - split);
    
    Bytef *dest = NULL;
    uLongf destLen = 0;
    
    // First call to get required size
    int ret = uncompress2(NULL, &destLen, source, &sourceLen);
    if (ret != Z_OK && ret != Z_BUF_ERROR && ret != Z_DATA_ERROR) {
        return 0;
    }
    
    if (destLen > 0) {
        dest = (Bytef *)malloc(destLen);
        if (!dest) return 0;
        
        ret = uncompress2(dest, &destLen, source, &sourceLen);
        free(dest);
    }
    return 0;
}

static int fuzz_compress2_z(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t split = Size / 2;
    if (split == 0) split = 1;
    
    const Bytef *source = (const Bytef *)Data;
    z_size_t sourceLen = split;
    
    // Calculate maximum possible compressed size
    z_size_t destLen = compressBound(sourceLen);
    if (destLen == 0) return 0;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return 0;
    
    int level = (int)(Data[split] % 10);
    int ret = compress2_z(dest, &destLen, source, sourceLen, level);
    
    free(dest);
    return 0;
}

static int fuzz_compress_z(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const Bytef *source = (const Bytef *)Data;
    z_size_t sourceLen = Size;
    
    // Calculate maximum possible compressed size
    z_size_t destLen = compressBound(sourceLen);
    if (destLen == 0) return 0;
    
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) return 0;
    
    int ret = compress_z(dest, &destLen, source, sourceLen);
    
    free(dest);
    return 0;
}

static int fuzz_uncompress(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    size_t split = Size / 2;
    if (split == 0) split = 1;
    
    const Bytef *source = (const Bytef *)(Data + split);
    uLong sourceLen = (uLong)(Size - split);
    
    Bytef *dest = NULL;
    uLongf destLen = 0;
    
    // First call to get required size
    int ret = uncompress(NULL, &destLen, source, sourceLen);
    if (ret != Z_OK && ret != Z_BUF_ERROR && ret != Z_DATA_ERROR) {
        return 0;
    }
    
    if (destLen > 0) {
        dest = (Bytef *)malloc(destLen);
        if (!dest) return 0;
        
        ret = uncompress(dest, &destLen, source, sourceLen);
        free(dest);
    }
    return 0;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz uncompress2_z
    fuzz_uncompress2_z(Data, Size);
    
    // Fuzz uncompress_z
    fuzz_uncompress_z(Data, Size);
    
    // Fuzz uncompress2
    fuzz_uncompress2(Data, Size);
    
    // Fuzz compress2_z
    fuzz_compress2_z(Data, Size);
    
    // Fuzz compress_z
    fuzz_compress_z(Data, Size);
    
    // Fuzz uncompress
    fuzz_uncompress(Data, Size);
    
    return 0;
}
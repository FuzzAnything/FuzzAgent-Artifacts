// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
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
    uLongf dest_len = compressBound(size);
    if (dest_len == 0) return;
    
    Bytef *dest = (Bytef *)malloc(dest_len);
    if (!dest) return;
    
    compress(dest, &dest_len, data, size);
    
    free(dest);
}

static void test_compress_z(const uint8_t *data, size_t size) {
    z_size_t source_len = (z_size_t)size;
    z_size_t dest_len = compressBound_z(source_len);
    if (dest_len == (z_size_t)-1) return;
    
    Bytef *dest = (Bytef *)malloc(dest_len);
    if (!dest) return;
    
    compress_z(dest, &dest_len, data, source_len);
    
    free(dest);
}

static void test_compress2_z(const uint8_t *data, size_t size) {
    z_size_t source_len = (z_size_t)size;
    z_size_t dest_len = compressBound_z(source_len);
    if (dest_len == (z_size_t)-1) return;
    
    Bytef *dest = (Bytef *)malloc(dest_len);
    if (!dest) return;
    
    int level = size > 0 ? data[0] % 10 : 0;
    compress2_z(dest, &dest_len, data, source_len, level);
    
    free(dest);
}

static void test_uncompress_z(const uint8_t *data, size_t size) {
    z_size_t source_len = (z_size_t)size;
    z_size_t dest_len = source_len * 2 + 100;
    
    Bytef *dest = (Bytef *)malloc(dest_len);
    if (!dest) return;
    
    uncompress_z(dest, &dest_len, data, source_len);
    
    free(dest);
}

static void test_uncompress2_z(const uint8_t *data, size_t size) {
    z_size_t source_len = (z_size_t)size;
    z_size_t dest_len = source_len * 2 + 100;
    
    Bytef *dest = (Bytef *)malloc(dest_len);
    if (!dest) return;
    
    uncompress2_z(dest, &dest_len, data, &source_len);
    
    free(dest);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_compress(Data, Size);
    test_compress_z(Data, Size);
    test_compress2_z(Data, Size);
    test_uncompress_z(Data, Size);
    test_uncompress2_z(Data, Size);
    
    return 0;
}
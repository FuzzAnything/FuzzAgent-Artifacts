// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define MAX_ALLOC_SIZE (10 * 1024 * 1024)

static int fuzz_compress2_z(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Allocate source and destination buffers
    Bytef *source = (Bytef *)malloc(size);
    if (!source) return 1;
    memcpy(source, data, size);
    
    // Destination buffer: compress can expand data slightly in edge cases, but usually shrinks.
    // zlib guarantees compressed size <= source size + overhead. 
    // We use a safe upper bound: source size + 12% + 1000 bytes.
    z_size_t dest_size = size + (size / 10) + 1000;
    if (dest_size > MAX_ALLOC_SIZE) dest_size = MAX_ALLOC_SIZE;
    
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) {
        free(source);
        return 1;
    }
    
    z_size_t destLen = dest_size;
    int level = (data[0] % 10) - 1; // -1 to 8
    
    int ret = compress2_z(dest, &destLen, source, size, level);
    
    // Try with NULL dest to check size calculation if supported (though spec says dest required if destLen>0)
    // We skip NULL dest tests here as per spec "requires dest if destLen > 0"
    
    free(source);
    free(dest);
    return 0;
}

static int fuzz_compress_z(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    Bytef *source = (Bytef *)malloc(size);
    if (!source) return 1;
    memcpy(source, data, size);
    
    z_size_t dest_size = size + (size / 10) + 1000;
    if (dest_size > MAX_ALLOC_SIZE) dest_size = MAX_ALLOC_SIZE;
    
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) {
        free(source);
        return 1;
    }
    
    z_size_t destLen = dest_size;
    
    int ret = compress_z(dest, &destLen, source, size);
    
    free(source);
    free(dest);
    return 0;
}

static int fuzz_uncompress(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    Bytef *source = (Bytef *)malloc(size);
    if (!source) return 1;
    memcpy(source, data, size);
    
    // Destination buffer: decompression can expand significantly.
    // We limit to MAX_ALLOC_SIZE to prevent OOM.
    uLong dest_size = MAX_ALLOC_SIZE;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) {
        free(source);
        return 1;
    }
    
    uLongf destLen = dest_size;
    
    int ret = uncompress(dest, &destLen, source, size);
    
    free(source);
    free(dest);
    return 0;
}

static int fuzz_uncompress_z(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    Bytef *source = (Bytef *)malloc(size);
    if (!source) return 1;
    memcpy(source, data, size);
    
    z_size_t dest_size = MAX_ALLOC_SIZE;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) {
        free(source);
        return 1;
    }
    
    z_size_t destLen = dest_size;
    
    int ret = uncompress_z(dest, &destLen, source, size);
    
    free(source);
    free(dest);
    return 0;
}

static int fuzz_uncompress2(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    Bytef *source = (Bytef *)malloc(size);
    if (!source) return 1;
    memcpy(source, data, size);
    
    uLong sourceLen = size;
    
    z_size_t dest_size = MAX_ALLOC_SIZE;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) {
        free(source);
        return 1;
    }
    
    uLongf destLen = dest_size;
    
    int ret = uncompress2(dest, &destLen, source, &sourceLen);
    
    free(source);
    free(dest);
    return 0;
}

static int fuzz_uncompress2_z(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    Bytef *source = (Bytef *)malloc(size);
    if (!source) return 1;
    memcpy(source, data, size);
    
    z_size_t sourceLen = size;
    
    z_size_t dest_size = MAX_ALLOC_SIZE;
    Bytef *dest = (Bytef *)malloc(dest_size);
    if (!dest) {
        free(source);
        return 1;
    }
    
    z_size_t destLen = dest_size;
    
    int ret = uncompress2_z(dest, &destLen, source, &sourceLen);
    
    free(source);
    free(dest);
    return 0;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz compress2_z
    fuzz_compress2_z(Data, Size);
    
    // Fuzz compress_z
    fuzz_compress_z(Data, Size);
    
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
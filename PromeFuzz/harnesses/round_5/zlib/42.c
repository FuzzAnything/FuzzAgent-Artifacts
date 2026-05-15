// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to determine compression level (0-9, with 10 mapping to Z_DEFAULT_COMPRESSION)
    int level = Data[0] % 11;
    if (level == 10) {
        level = Z_DEFAULT_COMPRESSION;
    }
    
    // Use remaining data as source
    const Bytef* source = Data + 1;
    uLong sourceLen = Size - 1;
    if (sourceLen == 0) {
        return 0;
    }

    // Test compressBound
    uLong bound = compressBound(sourceLen);
    if (bound == (uLong)-1) {
        // Overflow case, skip further tests
        return 0;
    }

    // Allocate destination buffers
    Bytef* dest = (Bytef*)malloc(bound);
    if (!dest) {
        return 0;
    }
    
    Bytef* dest2 = (Bytef*)malloc(bound);
    if (!dest2) {
        free(dest);
        return 0;
    }

    // Test compress
    uLongf destLen = bound;
    int ret = compress(dest, &destLen, source, sourceLen);
    if (ret == Z_OK) {
        // Test uncompress with the compressed data
        uLongf uncompressedLen = sourceLen;
        Bytef* uncompressed = (Bytef*)malloc(sourceLen);
        if (uncompressed) {
            int uncompressRet = uncompress(uncompressed, &uncompressedLen, dest, destLen);
            free(uncompressed);
        }
    }

    // Test compress2
    destLen = bound;
    ret = compress2(dest, &destLen, source, sourceLen, level);
    if (ret == Z_OK) {
        // Test uncompress with the compressed data
        uLongf uncompressedLen = sourceLen;
        Bytef* uncompressed = (Bytef*)malloc(sourceLen);
        if (uncompressed) {
            int uncompressRet = uncompress(uncompressed, &uncompressedLen, dest, destLen);
            free(uncompressed);
        }
    }

    // Test compress_z (if sourceLen fits in z_size_t)
    if (sourceLen <= (uLong)(z_size_t)-1) {
        z_size_t destLen_z = (z_size_t)bound;
        z_size_t sourceLen_z = (z_size_t)sourceLen;
        ret = compress_z(dest2, &destLen_z, source, sourceLen_z);
        // Note: We don't test uncompress here as it expects uLong parameters
    }

    // Test compress2_z (if sourceLen fits in z_size_t)
    if (sourceLen <= (uLong)(z_size_t)-1) {
        z_size_t destLen_z = (z_size_t)bound;
        z_size_t sourceLen_z = (z_size_t)sourceLen;
        ret = compress2_z(dest2, &destLen_z, source, sourceLen_z, level);
    }

    // Clean up
    free(dest);
    free(dest2);
    
    return 0;
}
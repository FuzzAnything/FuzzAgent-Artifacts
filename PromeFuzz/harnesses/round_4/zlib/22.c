// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress at compress.c:82:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int compare_buffers(const Bytef* buf1, const Bytef* buf2, size_t len) {
    return memcmp(buf1, buf2, len);
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test compressBound
    uLong bound = compressBound((uLong)Size);
    if (bound == (uLong)-1) {
        // Input too large, skip further processing
        return 0;
    }

    // Allocate destination buffers
    Bytef* dest = (Bytef*)malloc(bound);
    Bytef* dest_z = (Bytef*)malloc(bound);
    Bytef* uncompressed = (Bytef*)malloc(Size);
    
    if (!dest || !dest_z || !uncompressed) {
        free(dest);
        free(dest_z);
        free(uncompressed);
        return 0;
    }

    // Test compress (default compression)
    uLongf destLen = bound;
    int ret = compress(dest, &destLen, Data, (uLong)Size);
    if (ret == Z_OK) {
        // Try to uncompress the compressed data
        uLongf uncompressedLen = Size;
        int unret = uncompress(uncompressed, &uncompressedLen, dest, destLen);
        if (unret == Z_OK) {
            // Verify decompressed data matches original
            compare_buffers(Data, uncompressed, Size);
        }
    }

    // Test compress2 with different compression levels
    int levels[] = {Z_NO_COMPRESSION, Z_BEST_SPEED, Z_DEFAULT_COMPRESSION, Z_BEST_COMPRESSION};
    for (int i = 0; i < 4; i++) {
        destLen = bound;
        ret = compress2(dest, &destLen, Data, (uLong)Size, levels[i]);
        if (ret == Z_OK) {
            uLongf uncompressedLen = Size;
            uncompress(uncompressed, &uncompressedLen, dest, destLen);
        }
    }

    // Test compress_z (z_size_t version)
    z_size_t destLen_z = bound;
    z_size_t sourceLen_z = Size;
    ret = compress_z(dest_z, &destLen_z, Data, sourceLen_z);
    if (ret == Z_OK) {
        // Convert back to uLong for uncompress
        uLongf uncompressedLen = Size;
        uLongf compressedLen = (uLongf)destLen_z;
        uncompress(uncompressed, &uncompressedLen, dest_z, compressedLen);
    }

    // Test compress2_z with different compression levels
    for (int i = 0; i < 4; i++) {
        destLen_z = bound;
        ret = compress2_z(dest_z, &destLen_z, Data, sourceLen_z, levels[i]);
        if (ret == Z_OK) {
            uLongf uncompressedLen = Size;
            uLongf compressedLen = (uLongf)destLen_z;
            uncompress(uncompressed, &uncompressedLen, dest_z, compressedLen);
        }
    }

    // Test uncompress with corrupted data (use original input as "compressed" data)
    uLongf corruptedLen = Size;
    if (Size > 0) {
        // This should typically fail with Z_DATA_ERROR
        uncompress(uncompressed, &corruptedLen, Data, (uLong)Size);
    }

    // Test with small destination buffer to trigger Z_BUF_ERROR
    if (Size > 10) {
        uLongf smallDestLen = Size / 2;
        Bytef* smallDest = (Bytef*)malloc(smallDestLen);
        if (smallDest) {
            compress(smallDest, &smallDestLen, Data, (uLong)Size);
            free(smallDest);
        }
    }

    // Cleanup
    free(dest);
    free(dest_z);
    free(uncompressed);

    return 0;
}
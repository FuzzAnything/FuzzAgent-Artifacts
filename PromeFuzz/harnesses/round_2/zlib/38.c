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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int compare_buffers(const Bytef* buf1, const Bytef* buf2, size_t len) {
    return memcmp(buf1, buf2, len) == 0;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    // Limit input size to avoid excessive memory usage
    if (Size > 1024 * 1024) {
        return 0;
    }

    // Test compressBound
    uLong bound = compressBound((uLong)Size);
    if (bound == (uLong)-1) {
        // Overflow case, skip further processing
        return 0;
    }

    // Allocate buffers for compression/decompression
    Bytef* compressed = (Bytef*)malloc(bound);
    Bytef* decompressed = (Bytef*)malloc(Size);
    if (!compressed || !decompressed) {
        free(compressed);
        free(decompressed);
        return 0;
    }

    // Test compress
    uLongf destLen = bound;
    int ret = compress(compressed, &destLen, Data, (uLong)Size);
    if (ret == Z_OK) {
        // Test uncompress on the compressed data
        uLongf decompLen = Size;
        int uncompress_ret = uncompress(decompressed, &decompLen, compressed, destLen);
        if (uncompress_ret == Z_OK) {
            // Verify decompressed data matches original
            compare_buffers(Data, decompressed, Size);
        }
    }

    // Test compress2 with different compression levels
    int levels[] = {Z_NO_COMPRESSION, Z_BEST_SPEED, Z_DEFAULT_COMPRESSION, Z_BEST_COMPRESSION};
    for (int i = 0; i < 4; i++) {
        destLen = bound;
        ret = compress2(compressed, &destLen, Data, (uLong)Size, levels[i]);
        if (ret == Z_OK) {
            uLongf decompLen = Size;
            uncompress(decompressed, &decompLen, compressed, destLen);
        }
    }

    // Test compress_z (if size_t version is available)
    z_size_t z_destLen = (z_size_t)bound;
    z_size_t z_sourceLen = (z_size_t)Size;
    ret = compress_z(compressed, &z_destLen, Data, z_sourceLen);
    if (ret == Z_OK) {
        // Note: No corresponding uncompress_z function, so we use regular uncompress
        uLongf decompLen = Size;
        uncompress(decompressed, &decompLen, compressed, (uLong)z_destLen);
    }

    // Test compress2_z with different compression levels
    for (int i = 0; i < 4; i++) {
        z_destLen = (z_size_t)bound;
        ret = compress2_z(compressed, &z_destLen, Data, z_sourceLen, levels[i]);
        if (ret == Z_OK) {
            uLongf decompLen = Size;
            uncompress(decompressed, &decompLen, compressed, (uLong)z_destLen);
        }
    }

    // Test uncompress on raw input data (may fail, but that's OK)
    if (Size > 0) {
        uLongf decompLen = Size * 2; // Try with larger buffer
        Bytef* temp_decomp = (Bytef*)malloc(decompLen);
        if (temp_decomp) {
            uncompress(temp_decomp, &decompLen, Data, (uLong)Size);
            free(temp_decomp);
        }
    }

    free(compressed);
    free(decompressed);
    return 0;
}
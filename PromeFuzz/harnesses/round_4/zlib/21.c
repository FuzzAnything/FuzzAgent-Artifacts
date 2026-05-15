// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "zlib.h"

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to control behavior
    uint8_t selector = Data[0];
    const uint8_t *source = Data + 1;
    size_t sourceLen = Size - 1;

    // Test compress2_z
    if (selector & 0x01) {
        z_size_t destLen = compressBound(sourceLen);
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            int level = selector % 11; // 0-10 compression levels
            int ret = compress2_z(dest, &destLen, source, sourceLen, level);
            free(dest);
        }
    }

    // Test uncompress2_z (needs compressed data first)
    if (selector & 0x02) {
        // First compress some data
        z_size_t compLen = compressBound(sourceLen);
        Bytef *compressed = (Bytef *)malloc(compLen);
        if (compressed) {
            int ret = compress2_z(compressed, &compLen, source, sourceLen, Z_DEFAULT_COMPRESSION);
            if (ret == Z_OK) {
                z_size_t uncompLen = sourceLen * 2; // Guess output size
                Bytef *uncompressed = (Bytef *)malloc(uncompLen);
                if (uncompressed) {
                    z_size_t consumed = compLen;
                    ret = uncompress2_z(uncompressed, &uncompLen, compressed, &consumed);
                    free(uncompressed);
                }
            }
            free(compressed);
        }
    }

    // Test uncompress_z
    if (selector & 0x04) {
        // First compress some data
        z_size_t compLen = compressBound(sourceLen);
        Bytef *compressed = (Bytef *)malloc(compLen);
        if (compressed) {
            int ret = compress2_z(compressed, &compLen, source, sourceLen, Z_DEFAULT_COMPRESSION);
            if (ret == Z_OK) {
                z_size_t uncompLen = sourceLen * 2;
                Bytef *uncompressed = (Bytef *)malloc(uncompLen);
                if (uncompressed) {
                    ret = uncompress_z(uncompressed, &uncompLen, compressed, compLen);
                    free(uncompressed);
                }
            }
            free(compressed);
        }
    }

    // Test compress (uLong version)
    if (selector & 0x08) {
        uLongf destLen = compressBound(sourceLen);
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            int ret = compress(dest, &destLen, source, sourceLen);
            free(dest);
        }
    }

    // Test uncompress (uLong version)
    if (selector & 0x10) {
        // First compress some data
        uLongf compLen = compressBound(sourceLen);
        Bytef *compressed = (Bytef *)malloc(compLen);
        if (compressed) {
            int ret = compress(compressed, &compLen, source, sourceLen);
            if (ret == Z_OK) {
                uLongf uncompLen = sourceLen * 2;
                Bytef *uncompressed = (Bytef *)malloc(uncompLen);
                if (uncompressed) {
                    ret = uncompress(uncompressed, &uncompLen, compressed, compLen);
                    free(uncompressed);
                }
            }
            free(compressed);
        }
    }

    // Test compress_z
    if (selector & 0x20) {
        z_size_t destLen = compressBound(sourceLen);
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            int ret = compress_z(dest, &destLen, source, sourceLen);
            free(dest);
        }
    }

    // Also test with zero-length input
    if (Size == 1) {
        // Test with empty source
        z_size_t destLen = compressBound(0);
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            int ret = compress2_z(dest, &destLen, NULL, 0, Z_DEFAULT_COMPRESSION);
            free(dest);
        }
    }

    return 0;
}
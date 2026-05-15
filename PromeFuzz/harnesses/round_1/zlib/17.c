// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2 at compress.c:67:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Ensure minimum size for meaningful fuzzing
    if (Size < 1) {
        return 0;
    }

    // Allocate source buffer
    Bytef *source = (Bytef *)malloc(Size);
    if (!source) {
        return 0;
    }
    memcpy(source, Data, Size);

    // Test compress
    {
        uLongf destLen = compressBound(Size);
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            int ret = compress(dest, &destLen, source, (uLong)Size);
            // Optionally test uncompress on the compressed data
            if (ret == Z_OK) {
                uLongf uncompLen = Size * 2; // Guess original size
                Bytef *uncomp = (Bytef *)malloc(uncompLen);
                if (uncomp) {
                    uncompress(uncomp, &uncompLen, dest, destLen);
                    free(uncomp);
                }
            }
            free(dest);
        }
    }

    // Test compress_z
    {
        z_size_t destLen = (z_size_t)compressBound(Size);
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            int ret = compress_z(dest, &destLen, source, (z_size_t)Size);
            free(dest);
        }
    }

    // Test compress2
    {
        uLongf destLen = compressBound(Size);
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            // Derive level from first byte (0-9 valid, but also allow out-of-range)
            int level = (Data[0] % 11) - 1; // -1 to 9
            int ret = compress2(dest, &destLen, source, (uLong)Size, level);
            free(dest);
        }
    }

    // Test compress2_z
    {
        z_size_t destLen = (z_size_t)compressBound(Size);
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            int level = (Data[0] % 11) - 1; // -1 to 9
            int ret = compress2_z(dest, &destLen, source, (z_size_t)Size, level);
            free(dest);
        }
    }

    // Test uncompress (on raw fuzzer input, which may be invalid)
    {
        uLongf destLen = Size * 4; // Arbitrary guess
        if (destLen > 1024 * 1024) destLen = 1024 * 1024; // Cap at 1MB
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            uncompress(dest, &destLen, source, (uLong)Size);
            free(dest);
        }
    }

    // Test uncompress2_z
    {
        z_size_t destLen = Size * 4;
        if (destLen > 1024 * 1024) destLen = 1024 * 1024;
        z_size_t sourceLen = (z_size_t)Size;
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            uncompress2_z(dest, &destLen, source, &sourceLen);
            free(dest);
        }
    }

    free(source);
    return 0;
}
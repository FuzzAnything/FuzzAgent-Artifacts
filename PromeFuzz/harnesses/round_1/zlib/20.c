// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    uLongf destLenCompress = compressBound((uLong)Size);
    Bytef *destCompress = (Bytef *)malloc(destLenCompress);
    if (!destCompress) {
        return 0;
    }

    z_size_t destLenCompressZ = (z_size_t)compressBound((uLong)Size);
    Bytef *destCompressZ = (Bytef *)malloc(destLenCompressZ);
    if (!destCompressZ) {
        free(destCompress);
        return 0;
    }

    {
        uLongf destLen = destLenCompress;
        int ret = compress(destCompress, &destLen, Data, (uLong)Size);
        if (ret == Z_OK) {
            uLongf destLenUncomp = Size * 2;
            Bytef *destUncomp = (Bytef *)malloc(destLenUncomp);
            if (destUncomp) {
                uncompress(destUncomp, &destLenUncomp, destCompress, destLen);
                free(destUncomp);
            }
        }
    }

    {
        z_size_t destLen = destLenCompressZ;
        int ret = compress_z(destCompressZ, &destLen, Data, (z_size_t)Size);
        if (ret == Z_OK) {
            z_size_t destLenUncomp = (z_size_t)Size * 2;
            Bytef *destUncomp = (Bytef *)malloc(destLenUncomp);
            if (destUncomp) {
                uncompress_z(destUncomp, &destLenUncomp, destCompressZ, destLen);
                free(destUncomp);
            }
        }
    }

    {
        int level = (Data[0] % 11) - 1;
        z_size_t destLen = destLenCompressZ;
        int ret = compress2_z(destCompressZ, &destLen, Data, (z_size_t)Size, level);
        if (ret == Z_OK) {
            z_size_t destLenUncomp = (z_size_t)Size * 2;
            z_size_t sourceLen = destLen;
            Bytef *destUncomp = (Bytef *)malloc(destLenUncomp);
            if (destUncomp) {
                uncompress2_z(destUncomp, &destLenUncomp, destCompressZ, &sourceLen);
                free(destUncomp);
            }
        }
    }

    {
        uLongf destLen = Size * 4;
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            uncompress(dest, &destLen, Data, (uLong)Size);
            free(dest);
        }
    }

    {
        z_size_t destLen = (z_size_t)Size * 4;
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            uncompress_z(dest, &destLen, Data, (z_size_t)Size);
            free(dest);
        }
    }

    {
        z_size_t destLen = (z_size_t)Size * 4;
        z_size_t sourceLen = (z_size_t)Size;
        Bytef *dest = (Bytef *)malloc(destLen);
        if (dest) {
            uncompress2_z(dest, &destLen, Data, &sourceLen);
            free(dest);
        }
    }

    free(destCompress);
    free(destCompressZ);
    
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateBound at deflate.c:929:15 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateBound_z at deflate.c:856:18 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_z_stream(z_stream* strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
    strm->msg = Z_NULL;
    strm->data_type = Z_BINARY;
    strm->adler = 0;
    strm->reserved = 0;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* 1. Test deflateBound */
    {
        z_stream strm;
        initialize_z_stream(&strm);
        
        int level = Data[0] % 10; /* Valid levels are 0-9 */
        int windowBits = 15;
        int memLevel = 8;
        
        if (deflateInit2(&strm, level, Z_DEFLATED, windowBits, memLevel, Z_DEFAULT_STRATEGY) == Z_OK) {
            uLong sourceLen = Size;
            uLong bound = deflateBound(&strm, sourceLen);
            (void)bound; /* Use result to avoid unused variable warning */
            deflateEnd(&strm);
        }
    }

    /* 2. Test compress2_z */
    {
        z_size_t sourceLen = Size;
        z_size_t destLen = compressBound_z(sourceLen);
        
        if (destLen != (z_size_t)-1 && destLen > 0) {
            Bytef* dest = (Bytef*)malloc(destLen);
            if (dest) {
                int level = Data[0] % 10;
                int ret = compress2_z(dest, &destLen, Data, sourceLen, level);
                (void)ret; /* Use result to avoid unused variable warning */
                free(dest);
            }
        }
    }

    /* 3. Test deflateBound_z */
    {
        z_stream strm;
        initialize_z_stream(&strm);
        
        int level = Data[0] % 10;
        int windowBits = 15;
        int memLevel = 8;
        
        if (deflateInit2(&strm, level, Z_DEFLATED, windowBits, memLevel, Z_DEFAULT_STRATEGY) == Z_OK) {
            z_size_t sourceLen = Size;
            z_size_t bound = deflateBound_z(&strm, sourceLen);
            (void)bound; /* Use result to avoid unused variable warning */
            deflateEnd(&strm);
        }
    }

    /* 4. Test compress_z */
    {
        z_size_t sourceLen = Size;
        z_size_t destLen = compressBound_z(sourceLen);
        
        if (destLen != (z_size_t)-1 && destLen > 0) {
            Bytef* dest = (Bytef*)malloc(destLen);
            if (dest) {
                int ret = compress_z(dest, &destLen, Data, sourceLen);
                (void)ret; /* Use result to avoid unused variable warning */
                free(dest);
            }
        }
    }

    /* 5. Test compressBound_z */
    {
        z_size_t sourceLen = Size;
        z_size_t bound = compressBound_z(sourceLen);
        (void)bound; /* Use result to avoid unused variable warning */
    }

    /* 6. Test compressBound */
    {
        uLong sourceLen = Size;
        uLong bound = compressBound(sourceLen);
        (void)bound; /* Use result to avoid unused variable warning */
    }

    return 0;
}
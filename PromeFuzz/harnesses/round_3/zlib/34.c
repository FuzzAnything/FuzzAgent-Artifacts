// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress at compress.c:82:13 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateBound at deflate.c:929:15 in zlib.h
// deflateBound_z at deflate.c:856:18 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_z_stream(z_stream *strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    /* Test compressBound */
    uLong bound = compressBound((uLong)Size);
    
    /* Test compressBound_z */
    z_size_t bound_z = compressBound_z((z_size_t)Size);
    
    /* Test compress */
    uLongf destLen = compressBound((uLong)Size);
    Bytef *dest = (Bytef *)malloc(destLen);
    if (dest) {
        int ret = compress(dest, &destLen, (const Bytef *)Data, (uLong)Size);
        free(dest);
    }
    
    /* Test compress2_z */
    z_size_t destLen_z = compressBound_z((z_size_t)Size);
    Bytef *dest_z = (Bytef *)malloc(destLen_z);
    if (dest_z) {
        int level = Z_DEFAULT_COMPRESSION;
        if (Size > 0) {
            level = Data[0] % 10;  /* Use first byte to determine level */
        }
        int ret = compress2_z(dest_z, &destLen_z, (const Bytef *)Data, 
                             (z_size_t)Size, level);
        free(dest_z);
    }
    
    /* Test deflateBound and deflateBound_z */
    z_stream strm;
    initialize_z_stream(&strm);
    
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        /* Test deflateBound */
        uLong def_bound = deflateBound(&strm, (uLong)Size);
        
        /* Test deflateBound_z */
        z_size_t def_bound_z = deflateBound_z(&strm, (z_size_t)Size);
        
        deflateEnd(&strm);
    }
    
    return 0;
}
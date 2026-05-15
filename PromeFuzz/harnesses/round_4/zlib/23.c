// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateBound at deflate.c:929:15 in zlib.h
// deflateBound_z at deflate.c:856:18 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_z_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test compressBound
    uLong bound = compressBound((uLong)Size);
    
    // Test compressBound_z
    z_size_t bound_z = compressBound_z((z_size_t)Size);
    
    // Test compress2_z
    if (bound_z != (z_size_t)-1 && bound_z > 0) {
        Bytef *dest = (Bytef *)malloc(bound_z);
        if (dest) {
            z_size_t destLen = bound_z;
            int level = Data[0] % 10; // Vary compression level 0-9
            int ret = compress2_z(dest, &destLen, Data, (z_size_t)Size, level);
            free(dest);
        }
    }
    
    // Test compress_z
    if (bound_z != (z_size_t)-1 && bound_z > 0) {
        Bytef *dest = (Bytef *)malloc(bound_z);
        if (dest) {
            z_size_t destLen = bound_z;
            int ret = compress_z(dest, &destLen, Data, (z_size_t)Size);
            free(dest);
        }
    }
    
    // Test deflateBound and deflateBound_z
    z_stream strm;
    init_z_stream(&strm);
    
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Test deflateBound
        uLong def_bound = deflateBound(&strm, (uLong)Size);
        
        // Test deflateBound_z
        z_size_t def_bound_z = deflateBound_z(&strm, (z_size_t)Size);
        
        deflateEnd(&strm);
    }
    
    return 0;
}
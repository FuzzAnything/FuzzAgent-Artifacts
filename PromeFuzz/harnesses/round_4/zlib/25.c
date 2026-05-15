// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
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

static void init_z_stream(z_stream *strm) {
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

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Test compressBound
    uLong bound = compressBound((uLong)Size);
    
    // Test compressBound_z
    z_size_t bound_z = compressBound_z((z_size_t)Size);
    
    // Test compress_z and compress2_z with different compression levels
    if (bound_z != (z_size_t)-1 && bound_z > 0) {
        Bytef *dest = (Bytef *)malloc(bound_z);
        if (dest) {
            z_size_t destLen = bound_z;
            int level = Data[0] % 10;  // Use first byte to determine compression level (0-9)
            
            // Test compress_z
            int ret_compress_z = compress_z(dest, &destLen, (const Bytef *)Data, (z_size_t)Size);
            
            // Reset destLen for compress2_z
            destLen = bound_z;
            int ret_compress2_z = compress2_z(dest, &destLen, (const Bytef *)Data, (z_size_t)Size, level);
            
            free(dest);
        }
    }
    
    // Test deflateBound and deflateBound_z with initialized z_stream
    z_stream strm;
    init_z_stream(&strm);
    
    // Initialize deflate with default compression
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Test deflateBound
        uLong deflate_bound = deflateBound(&strm, (uLong)Size);
        
        // Test deflateBound_z
        z_size_t deflate_bound_z = deflateBound_z(&strm, (z_size_t)Size);
        
        deflateEnd(&strm);
    }
    
    return 0;
}
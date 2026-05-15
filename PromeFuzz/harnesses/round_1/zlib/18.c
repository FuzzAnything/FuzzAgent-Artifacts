// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
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

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Test compressBound
    uLong sourceLen = Size;
    uLong bound = compressBound(sourceLen);
    
    // Test compressBound_z
    z_size_t sourceLen_z = Size;
    z_size_t bound_z = compressBound_z(sourceLen_z);
    
    // Test compress_z if we have enough data
    if (Size > 0 && bound != (uLong)-1 && bound_z != (z_size_t)-1) {
        // Allocate destination buffers
        Bytef *dest = NULL;
        Bytef *dest_z = NULL;
        z_size_t destLen_z = bound_z;
        
        if (bound > 0 && bound < 100 * 1024 * 1024) { // Limit allocation to 100MB
            dest = (Bytef *)malloc(bound);
            if (dest) {
                z_size_t destLen = bound;
                int ret = compress_z(dest, &destLen, Data, sourceLen_z);
                free(dest);
            }
        }
        
        if (bound_z > 0 && bound_z < 100 * 1024 * 1024) { // Limit allocation to 100MB
            dest_z = (Bytef *)malloc(bound_z);
            if (dest_z) {
                z_size_t actualDestLen = bound_z;
                int ret = compress2_z(dest_z, &actualDestLen, Data, sourceLen_z, Z_DEFAULT_COMPRESSION);
                free(dest_z);
            }
        }
    }
    
    // Test deflateBound and deflateBound_z
    if (Size > 0) {
        z_stream strm;
        initialize_z_stream(&strm);
        
        // Initialize deflate with random windowBits from first byte
        int windowBits = 15;
        if (Size >= 1) {
            windowBits = (Data[0] % 15) + 8; // Range 8-22
        }
        
        int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                              windowBits, 8, Z_DEFAULT_STRATEGY);
        
        if (ret == Z_OK) {
            // Test deflateBound
            uLong deflate_bound = deflateBound(&strm, sourceLen);
            
            // Test deflateBound_z
            z_size_t deflate_bound_z = deflateBound_z(&strm, sourceLen_z);
            
            deflateEnd(&strm);
        }
    }
    
    return 0;
}
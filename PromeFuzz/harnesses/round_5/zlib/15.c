// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compressBound at compress.c:96:15 in zlib.h
// compressBound_z at compress.c:91:18 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateBound at deflate.c:929:15 in zlib.h
// deflateBound_z at deflate.c:856:18 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateBound at deflate.c:929:15 in zlib.h
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

static void init_z_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Limit input size to avoid excessive memory usage */
    size_t max_input_size = 1024 * 1024; /* 1MB */
    if (Size > max_input_size) {
        Size = max_input_size;
    }
    
    /* Test compressBound */
    uLong sourceLen = (uLong)Size;
    uLong bound = compressBound(sourceLen);
    
    /* Test compressBound_z */
    z_size_t sourceLen_z = (z_size_t)Size;
    z_size_t bound_z = compressBound_z(sourceLen_z);
    
    /* Test deflateBound and deflateBound_z */
    z_stream strm;
    init_z_stream(&strm);
    
    /* Initialize deflate with default parameters */
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        uLong deflate_bound = deflateBound(&strm, sourceLen);
        z_size_t deflate_bound_z = deflateBound_z(&strm, sourceLen_z);
        deflateEnd(&strm);
    }
    
    /* Test compress_z */
    if (bound_z != (z_size_t)-1 && bound_z > 0) {
        Bytef *dest = (Bytef *)malloc(bound_z);
        if (dest) {
            z_size_t destLen = bound_z;
            int ret = compress_z(dest, &destLen, Data, sourceLen_z);
            free(dest);
        }
    }
    
    /* Test compress2_z with different compression levels */
    if (bound_z != (z_size_t)-1 && bound_z > 0) {
        Bytef *dest = (Bytef *)malloc(bound_z);
        if (dest) {
            z_size_t destLen = bound_z;
            
            /* Try multiple compression levels */
            int levels[] = {Z_NO_COMPRESSION, Z_BEST_SPEED, Z_DEFAULT_COMPRESSION, Z_BEST_COMPRESSION};
            for (int i = 0; i < 4; i++) {
                destLen = bound_z;
                compress2_z(dest, &destLen, Data, sourceLen_z, levels[i]);
            }
            
            /* Also try a level derived from input data */
            if (Size >= 2) {
                destLen = bound_z;
                int level = Data[0] % 10; /* 0-9 */
                compress2_z(dest, &destLen, Data, sourceLen_z, level);
            }
            
            free(dest);
        }
    }
    
    /* Test with a smaller destination buffer to trigger Z_BUF_ERROR */
    if (bound_z != (z_size_t)-1 && bound_z > 10) {
        Bytef *dest = (Bytef *)malloc(bound_z / 2);
        if (dest) {
            z_size_t destLen = bound_z / 2;
            compress_z(dest, &destLen, Data, sourceLen_z);
            free(dest);
        }
    }
    
    /* Test with different flush options for deflateBound */
    init_z_stream(&strm);
    if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     MAX_WBITS, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        uLong deflate_bound = deflateBound(&strm, sourceLen);
        deflateEnd(&strm);
    }
    
    return 0;
}
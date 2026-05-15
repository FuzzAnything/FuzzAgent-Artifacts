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

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Test compressBound (function 4)
    uLong bound_result = compressBound((uLong)Size);
    
    // Test compressBound_z (function 3)
    z_size_t bound_z_result = compressBound_z((z_size_t)Size);
    
    // Allocate destination buffer using the larger of the two bounds
    z_size_t dest_len = (bound_z_result != (z_size_t)-1) ? bound_z_result : (z_size_t)bound_result;
    if (dest_len == 0 || dest_len > 1024*1024) {
        dest_len = 1024*1024;
    }
    
    Bytef *dest = (Bytef *)malloc(dest_len);
    if (!dest) {
        return 0;
    }
    
    // Test compress_z (function 6)
    z_size_t dest_len_z = dest_len;
    int compress_z_result = compress_z(dest, &dest_len_z, Data, (z_size_t)Size);
    
    // Test compress2_z (function 1) with different compression levels
    int level = Data[0] % 11; // 0-10, including Z_DEFAULT_COMPRESSION
    z_size_t dest_len2 = dest_len;
    int compress2_z_result = compress2_z(dest, &dest_len2, Data, (z_size_t)Size, level);
    
    // Test deflateBound and deflateBound_z with initialized z_stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize with default allocator
    int init_result = deflateInit2(&strm, level, Z_DEFLATED, MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);
    if (init_result == Z_OK) {
        // Test deflateBound (function 2)
        uLong deflate_bound = deflateBound(&strm, (uLong)Size);
        
        // Test deflateBound_z (function 5)
        z_size_t deflate_bound_z = deflateBound_z(&strm, (z_size_t)Size);
        
        deflateEnd(&strm);
    }
    
    free(dest);
    return 0;
}
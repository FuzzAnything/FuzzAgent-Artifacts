// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
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

static void* dummy_alloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return malloc(items * size);
}

static void dummy_free(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    z_stream strm1, strm2;
    int ret;
    
    // Initialize strm1 with dummy allocators
    strm1.zalloc = dummy_alloc;
    strm1.zfree = dummy_free;
    strm1.opaque = Z_NULL;
    strm1.next_in = Z_NULL;
    strm1.avail_in = 0;
    
    // Use first byte to determine compression level
    int level = (Data[0] % 11) - 1;  // Range -1 to 9 (Z_DEFAULT_COMPRESSION is -1)
    
    // Initialize with deflateInit_
    ret = deflateInit_(&strm1, level, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        // If deflateInit_ fails, try deflateInit2_ instead
        int windowBits = 15;
        int memLevel = 8;
        int strategy = Z_DEFAULT_STRATEGY;
        
        ret = deflateInit2_(&strm1, level, Z_DEFLATED, windowBits, memLevel,
                          strategy, ZLIB_VERSION, sizeof(z_stream));
        if (ret != Z_OK) {
            return 0;
        }
    }
    
    // Call deflateTune with parameters derived from input
    if (Size >= 5) {
        int good_length = Data[1] % 258;
        int max_lazy = Data[2] % 258;
        int nice_length = Data[3] % 258;
        int max_chain = Data[4] % 4096;
        
        deflateTune(&strm1, good_length, max_lazy, nice_length, max_chain);
    }
    
    // Call deflateSetDictionary if we have enough data
    if (Size >= 6) {
        size_t dict_size = Size - 5;
        if (dict_size > 32768) dict_size = 32768;  // Reasonable limit
        
        deflateSetDictionary(&strm1, Data + 5, (uInt)dict_size);
    }
    
    // Initialize strm2 for deflateCopy test
    strm2.zalloc = dummy_alloc;
    strm2.zfree = dummy_free;
    strm2.opaque = Z_NULL;
    strm2.next_in = Z_NULL;
    strm2.avail_in = 0;
    
    // Try deflateCopy
    ret = deflateCopy(&strm2, &strm1);
    if (ret == Z_OK) {
        // Clean up strm2 if copy succeeded
        deflateEnd(&strm2);
    }
    
    // Try deflateResetKeep
    deflateResetKeep(&strm1);
    
    // Clean up
    deflateEnd(&strm1);
    
    return 0;
}
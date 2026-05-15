// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
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

static void *custom_zalloc(void *opaque, unsigned items, unsigned size) {
    (void)opaque;
    return malloc(items * size);
}

static void custom_zfree(void *opaque, void *address) {
    (void)opaque;
    free(address);
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    int bits;
    
    // Initialize stream with custom allocators
    strm.zalloc = custom_zalloc;
    strm.zfree = custom_zfree;
    strm.opaque = Z_NULL;
    
    // Initialize deflate with default settings
    ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Set dictionary if we have enough data
    if (Size >= 32) {
        uInt dict_len = (uInt)(Size > 32768 ? 32768 : Size);
        deflateSetDictionary(&strm, Data, dict_len);
    }
    
    // Prepare input/output buffers
    size_t out_size = Size * 2 + 1024; // Conservative output size
    Bytef *out_buf = (Bytef *)malloc(out_size);
    if (!out_buf) {
        deflateEnd(&strm);
        return 0;
    }
    
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_size;
    
    // Perform compression with various flush modes
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH};
    int mode_idx = Size % 4;
    
    do {
        ret = deflate(&strm, flush_modes[mode_idx]);
        
        // If output buffer is full, reset and continue
        if (strm.avail_out == 0) {
            strm.next_out = out_buf;
            strm.avail_out = (uInt)out_size;
        }
        
        // Try changing parameters mid-stream
        if (Size > 10 && ret == Z_OK) {
            int level = Data[0] % 10; // 0-9
            int strategy = Data[1] % 5; // 0-4
            deflateParams(&strm, level, strategy);
        }
        
        // Check used bits
        deflateUsed(&strm, &bits);
        
    } while (ret == Z_OK);
    
    // Reset stream and try again
    deflateReset(&strm);
    
    // Second compression pass with remaining data
    if (Size > 0) {
        strm.next_in = (Bytef *)Data;
        strm.avail_in = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
        strm.next_out = out_buf;
        strm.avail_out = (uInt)out_size;
        
        ret = deflate(&strm, Z_FINISH);
        while (ret == Z_OK) {
            ret = deflate(&strm, Z_NO_FLUSH);
        }
    }
    
    // Cleanup
    free(out_buf);
    deflateEnd(&strm);
    
    return 0;
}
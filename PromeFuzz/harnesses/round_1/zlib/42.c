// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
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

static void init_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    z_stream strm1, strm2;
    int ret;
    
    // Initialize first stream
    init_stream(&strm1);
    ret = deflateInit_(&strm1, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Use fuzzer data to set parameters for deflateTune
    if (Size >= 4) {
        int good_length = Data[0] % 256;
        int max_lazy = Data[1] % 256;
        int nice_length = Data[2] % 256;
        int max_chain = Data[3] % 256;
        deflateTune(&strm1, good_length, max_lazy, nice_length, max_chain);
    }
    
    // Use fuzzer data for dictionary
    if (Size > 0) {
        size_t dict_size = Size > 65535 ? 65535 : Size;
        Bytef* dictionary = (Bytef*)malloc(dict_size);
        if (dictionary) {
            memcpy(dictionary, Data, dict_size);
            deflateSetDictionary(&strm1, dictionary, (uInt)dict_size);
            free(dictionary);
        }
    }
    
    // Test deflateResetKeep
    deflateResetKeep(&strm1);
    
    // Test deflateReset
    deflateReset(&strm1);
    
    // Initialize second stream for deflateCopy - but don't call deflateInit_ yet
    init_stream(&strm2);
    
    // Copy state from strm1 to strm2 - deflateCopy will initialize strm2
    int copy_ret = deflateCopy(&strm2, &strm1);
    if (copy_ret == Z_OK) {
        // Clean up strm2 if copy succeeded
        deflateEnd(&strm2);
    } else {
        // If copy failed, we still need to clean up strm2 if it was initialized
        // But since we didn't call deflateInit_, we just need to make sure
        // any potential partial initialization is cleaned up
        // deflateEnd can handle partially initialized streams
        deflateEnd(&strm2);
    }
    
    // Clean up strm1
    deflateEnd(&strm1);
    
    return 0;
}
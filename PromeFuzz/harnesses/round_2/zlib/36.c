// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
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
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    z_stream strm1, strm2;
    int ret;
    unsigned char out_buffer[1024];
    
    /* Initialize first stream */
    init_z_stream(&strm1);
    ret = deflateInit_(&strm1, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) return 0;
    
    /* Use first byte to control flush behavior */
    int flush_mode = (Data[0] % 4 == 0) ? Z_NO_FLUSH : 
                     (Data[0] % 4 == 1) ? Z_SYNC_FLUSH : 
                     (Data[0] % 4 == 2) ? Z_FULL_FLUSH : Z_FINISH;
    
    /* Set up input data */
    strm1.next_in = (Bytef*)(Data + 1);
    strm1.avail_in = (uInt)(Size > 1 ? Size - 1 : 0);
    strm1.next_out = out_buffer;
    strm1.avail_out = sizeof(out_buffer);
    
    /* Call deflate with various flush modes */
    ret = deflate(&strm1, flush_mode);
    
    /* Try deflateTune with parameters derived from input data */
    if (Size > 5) {
        int good_length = Data[1] % 256;
        int max_lazy = Data[2] % 256;
        int nice_length = Data[3] % 256;
        int max_chain = Data[4] % 256;
        deflateTune(&strm1, good_length, max_lazy, nice_length, max_chain);
    }
    
    /* Try deflateReset or deflateResetKeep based on input */
    if (Size > 10) {
        if (Data[5] % 2 == 0) {
            deflateReset(&strm1);
        } else {
            deflateResetKeep(&strm1);
        }
    }
    
    /* Try deflateCopy with a second stream */
    if (Size > 20) {
        init_z_stream(&strm2);
        /* Don't initialize strm2 with deflateInit_ to avoid memory leak */
        ret = deflateCopy(&strm2, &strm1);
        if (ret == Z_OK) {
            deflateEnd(&strm2);
        }
    }
    
    /* Clean up */
    deflateEnd(&strm1);
    
    return 0;
}
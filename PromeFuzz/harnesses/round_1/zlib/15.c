// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_init_stream(z_stream *strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
}

static void fuzz_cleanup_stream(z_stream *strm) {
    deflateEnd(strm);
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    Bytef out_buf[4096];
    
    if (Size < 1) return 0;
    
    /* Test deflateInit_ */
    fuzz_init_stream(&strm);
    ret = deflateInit_(&strm, Data[0] % 10, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        /* Test deflateSetDictionary with part of input as dictionary */
        if (Size > 10) {
            size_t dict_size = Size > 100 ? 100 : Size - 1;
            deflateSetDictionary(&strm, Data + 1, dict_size);
        }
        
        /* Test deflate with some data */
        if (Size > 20) {
            size_t compress_size = Size > 100 ? 100 : Size - 10;
            strm.next_in = (Bytef *)(Data + 10);
            strm.avail_in = compress_size;
            strm.next_out = out_buf;
            strm.avail_out = sizeof(out_buf);
            
            deflate(&strm, Z_FINISH);
        }
        
        fuzz_cleanup_stream(&strm);
    }
    
    /* Test deflateInit2_ */
    fuzz_init_stream(&strm);
    ret = deflateInit2_(&strm, 
                       Data[0] % 10,                    /* level */
                       Z_DEFLATED,                      /* method */
                       (Data[1] % 15) + 8,              /* windowBits */
                       (Data[2] % 9) + 1,               /* memLevel */
                       Data[3] % 4,                     /* strategy */
                       ZLIB_VERSION, 
                       sizeof(z_stream));
    
    if (ret == Z_OK) {
        /* Test deflateParams */
        if (Size > 30) {
            deflateParams(&strm, Data[4] % 10, Data[5] % 4);
        }
        
        /* Test deflatePrime for raw deflate */
        if (Size > 40 && (Data[1] % 15) + 8 == -15) {
            deflatePrime(&strm, Data[6] % 17, Data[7]);
        }
        
        /* Test deflate with various flush modes */
        if (Size > 50) {
            size_t compress_size = Size > 150 ? 150 : Size - 30;
            strm.next_in = (Bytef *)(Data + 30);
            strm.avail_in = compress_size;
            strm.next_out = out_buf;
            strm.avail_out = sizeof(out_buf);
            
            /* Try different flush modes */
            int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH};
            int mode_idx = Data[8] % 4;
            
            do {
                strm.avail_in = compress_size > 50 ? 50 : compress_size;
                compress_size -= strm.avail_in;
                
                ret = deflate(&strm, compress_size > 0 ? flush_modes[mode_idx] : Z_FINISH);
                
                if (ret == Z_BUF_ERROR) {
                    strm.avail_out = sizeof(out_buf);
                    strm.next_out = out_buf;
                }
            } while (ret == Z_OK && compress_size > 0);
        }
        
        fuzz_cleanup_stream(&strm);
    }
    
    return 0;
}
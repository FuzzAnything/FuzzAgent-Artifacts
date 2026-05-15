// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
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

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    z_stream strm1, strm2;
    int ret;
    Bytef out_buf[4096];
    const Bytef *dict_data;
    uInt dict_len;
    size_t consumed;

    if (Size < 20) return 0;

    /* Initialize strm1 with deflateInit2_ */
    strm1.zalloc = custom_zalloc;
    strm1.zfree = custom_zfree;
    strm1.opaque = Z_NULL;
    
    /* Extract parameters from fuzz data */
    int level = Data[0] % 11 - 1;           /* -1 to 9 */
    int method = Z_DEFLATED;                /* Only DEFLATED is supported */
    int windowBits = (Data[1] % 16) + 8;    /* 8 to 23 */
    int memLevel = (Data[2] % 9) + 1;       /* 1 to 9 */
    int strategy = Data[3] % 5;             /* 0 to 4 */
    
    ret = deflateInit2_(&strm1, level, method, windowBits, memLevel,
                       strategy, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    /* deflateTune with parameters from fuzz data */
    int good_length = Data[4] % 258;
    int max_lazy = Data[5] % 258;
    int nice_length = Data[6] % 258;
    int max_chain = Data[7] % 4096;
    deflateTune(&strm1, good_length, max_lazy, nice_length, max_chain);

    /* deflateSetDictionary with part of fuzz data */
    dict_len = (Size > 256) ? 256 : (uInt)Size;
    dict_data = Data + 8;
    if (dict_len > 8) {
        dict_len -= 8;
        deflateSetDictionary(&strm1, dict_data, dict_len);
    }

    /* Compress data using deflate */
    strm1.next_in = Data + 8 + dict_len;
    strm1.avail_in = (uInt)(Size - 8 - dict_len);
    if (strm1.avail_in > Size) strm1.avail_in = 0;
    
    strm1.next_out = out_buf;
    strm1.avail_out = sizeof(out_buf);
    
    consumed = 0;
    while (strm1.avail_in > 0) {
        int flush = (consumed % 3 == 0) ? Z_SYNC_FLUSH : 
                   (consumed % 3 == 1) ? Z_PARTIAL_FLUSH : Z_NO_FLUSH;
        
        ret = deflate(&strm1, flush);
        if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
            deflateEnd(&strm1);
            return 0;
        }
        
        if (strm1.avail_out == 0) {
            strm1.next_out = out_buf;
            strm1.avail_out = sizeof(out_buf);
        }
        consumed++;
    }
    
    /* Finish compression */
    do {
        strm1.avail_out = sizeof(out_buf);
        strm1.next_out = out_buf;
        ret = deflate(&strm1, Z_FINISH);
    } while (ret == Z_OK);
    
    /* deflateResetKeep */
    deflateResetKeep(&strm1);
    
    /* Prepare strm2 for deflateCopy without initializing */
    strm2.zalloc = custom_zalloc;
    strm2.zfree = custom_zfree;
    strm2.opaque = Z_NULL;
    
    /* deflateCopy from strm1 to strm2 */
    ret = deflateCopy(&strm2, &strm1);
    if (ret != Z_OK) {
        deflateEnd(&strm1);
        return 0;
    }
    
    /* Clean up both streams */
    deflateEnd(&strm1);
    deflateEnd(&strm2);
    
    return 0;
}
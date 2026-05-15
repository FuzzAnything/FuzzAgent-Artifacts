// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

#ifdef __cplusplus
extern "C" {
#endif

static void initialize_stream(z_streamp strm, int level) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    deflateInit_(strm, level, ZLIB_VERSION, sizeof(z_stream));
}

static void use_stream(z_streamp strm, const uint8_t *Data, size_t Size) {
    if (Size < 10) return;
    
    size_t out_size = Size * 2 + 100;
    Bytef *out_buf = (Bytef *)malloc(out_size);
    strm->next_in = (Bytef *)Data;
    strm->avail_in = Size - 10;
    strm->next_out = out_buf;
    strm->avail_out = out_size;
    
    deflate(strm, Z_NO_FLUSH);
    
    free(out_buf);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    z_stream strm1, strm2;
    int level = Data[0] % 10;
    int flush_mode = Data[0] % 5;
    
    initialize_stream(&strm1, level);
    
    strm1.next_in = (Bytef *)Data;
    strm1.avail_in = Size > 1 ? Size - 1 : 0;
    
    size_t out_size = Size * 2 + 100;
    Bytef *out_buf = (Bytef *)malloc(out_size);
    strm1.next_out = out_buf;
    strm1.avail_out = out_size;
    
    int flush_values[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK};
    int flush = flush_values[flush_mode % 5];
    
    int ret = deflate(&strm1, flush);
    if (ret == Z_STREAM_ERROR) {
        deflateEnd(&strm1);
        free(out_buf);
        return 0;
    }
    
    if (Size >= 5) {
        int good_length = Data[1];
        int max_lazy = Data[2];
        int nice_length = Data[3];
        int max_chain = Data[4];
        deflateTune(&strm1, good_length, max_lazy, nice_length, max_chain);
    }
    
    deflateReset(&strm1);
    deflateResetKeep(&strm1);
    
    memset(&strm2, 0, sizeof(z_stream));
    strm2.zalloc = Z_NULL;
    strm2.zfree = Z_NULL;
    strm2.opaque = Z_NULL;
    
    int copy_ret = deflateCopy(&strm2, &strm1);
    if (copy_ret == Z_OK) {
        use_stream(&strm2, Data, Size);
        deflateEnd(&strm2);
    } else {
        deflateInit2(&strm2, level, Z_DEFLATED, MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
        use_stream(&strm2, Data, Size);
        deflateEnd(&strm2);
    }
    
    deflateEnd(&strm1);
    free(out_buf);
    
    return 0;
}

#ifdef __cplusplus
}
#endif
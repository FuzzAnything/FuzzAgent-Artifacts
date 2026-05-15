// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    unsigned char out[4096];
    
    /* Initialize zlib stream */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* First deflate call */
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)(Size > 100 ? 100 : Size);
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* First deflateParams call */
    ret = deflateParams(&strm, Z_BEST_SPEED, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Second deflate call */
    if (Size > 100) {
        strm.next_in = (Bytef *)Data + 100;
        strm.avail_in = (uInt)(Size > 200 ? 100 : Size - 100);
    } else {
        strm.avail_in = 0;
    }
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Second deflateParams call */
    ret = deflateParams(&strm, Z_BEST_COMPRESSION, Z_FILTERED);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Third deflate call */
    if (Size > 200) {
        strm.next_in = (Bytef *)Data + 200;
        strm.avail_in = (uInt)(Size > 300 ? 100 : Size - 200);
    } else {
        strm.avail_in = 0;
    }
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Fourth deflate call (with remaining data) */
    if (Size > 300) {
        strm.next_in = (Bytef *)Data + 300;
        strm.avail_in = (uInt)(Size - 300);
    }
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    
    ret = deflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Clean up */
    deflateEnd(&strm);
    
    return 0;
}
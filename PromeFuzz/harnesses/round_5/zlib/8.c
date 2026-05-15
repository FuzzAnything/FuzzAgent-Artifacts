// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    unsigned char out[4096];
    
    /* Initialize zlib stream */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    /* deflateInit_ */
    ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* Prepare input data */
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)(Size > 1024 ? 1024 : Size);
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    
    /* First deflate */
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* First deflateParams */
    ret = deflateParams(&strm, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Update input for second deflate */
    if (Size > 1024) {
        strm.next_in = (Bytef *)Data + 1024;
        strm.avail_in = (uInt)(Size > 2048 ? 1024 : Size - 1024);
    } else {
        strm.avail_in = 0;
    }
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    
    /* Second deflate */
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Second deflateParams */
    ret = deflateParams(&strm, Z_BEST_SPEED, Z_HUFFMAN_ONLY);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Update input for third deflate */
    if (Size > 2048) {
        strm.next_in = (Bytef *)Data + 2048;
        strm.avail_in = (uInt)(Size > 3072 ? 1024 : Size - 2048);
    } else {
        strm.avail_in = 0;
    }
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    
    /* Third deflate */
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Update input for fourth deflate - use remaining data */
    if (Size > 3072) {
        strm.next_in = (Bytef *)Data + 3072;
        strm.avail_in = (uInt)(Size - 3072);
    }
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    
    /* Fourth deflate with Z_FINISH */
    ret = deflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Cleanup */
    deflateEnd(&strm);
    
    return 0;
}
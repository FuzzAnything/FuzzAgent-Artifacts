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
    
    /* Initialize z_stream structure */
    memset(&strm, 0, sizeof(z_stream));
    
    /* Initialize deflate stream */
    ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* Set input data */
    strm.next_in = (Bytef *)Data;
    strm.avail_in = Size > 1024 ? 1024 : Size;
    
    /* First deflate call with Z_NO_FLUSH */
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
    
    /* Second deflate call with Z_NO_FLUSH */
    if (Size > 1024 && strm.avail_in == 0) {
        strm.next_in = (Bytef *)(Data + 1024);
        strm.avail_in = Size - 1024 > 1024 ? 1024 : Size - 1024;
    }
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Second deflateParams call */
    ret = deflateParams(&strm, Z_BEST_COMPRESSION, Z_HUFFMAN_ONLY);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Third deflate call with Z_SYNC_FLUSH */
    if (Size > 2048 && strm.avail_in == 0) {
        strm.next_in = (Bytef *)(Data + 2048);
        strm.avail_in = Size - 2048 > 1024 ? 1024 : Size - 2048;
    }
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    ret = deflate(&strm, Z_SYNC_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Fourth deflate call with Z_FINISH */
    if (Size > 3072 && strm.avail_in == 0) {
        strm.next_in = (Bytef *)(Data + 3072);
        strm.avail_in = Size - 3072;
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
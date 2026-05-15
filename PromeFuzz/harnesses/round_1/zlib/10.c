// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    unsigned char out_buffer[4096];
    
    /* Initialize z_stream structure */
    memset(&strm, 0, sizeof(z_stream));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    /* Initialize deflate with default settings */
    ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* Set input data for compression */
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)(Size > 1024 ? 1024 : Size);
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    /* First deflate call with Z_NO_FLUSH */
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* First deflateParams call - adjust compression level and strategy */
    int level1 = Z_DEFAULT_COMPRESSION;
    int strategy1 = Z_DEFAULT_STRATEGY;
    if (Size >= 1) {
        level1 = (Data[0] % 10); /* 0-9 compression level */
    }
    if (Size >= 2) {
        strategy1 = (Data[1] % 5); /* 0-4 strategy */
    }
    ret = deflateParams(&strm, level1, strategy1);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Update input pointer for second deflate call */
    size_t consumed = strm.total_in;
    if (consumed < Size) {
        strm.next_in = (Bytef *)Data + consumed;
        strm.avail_in = (uInt)((Size - consumed) > 512 ? 512 : (Size - consumed));
    } else {
        strm.next_in = (Bytef *)Data;
        strm.avail_in = 0;
    }
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    /* Second deflate call with Z_NO_FLUSH */
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Second deflateParams call - adjust compression level and strategy again */
    int level2 = Z_DEFAULT_COMPRESSION;
    int strategy2 = Z_DEFAULT_STRATEGY;
    if (Size >= 3) {
        level2 = (Data[2] % 10); /* 0-9 compression level */
    }
    if (Size >= 4) {
        strategy2 = (Data[3] % 5); /* 0-4 strategy */
    }
    ret = deflateParams(&strm, level2, strategy2);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Update input pointer for third deflate call */
    consumed = strm.total_in;
    if (consumed < Size) {
        strm.next_in = (Bytef *)Data + consumed;
        strm.avail_in = (uInt)((Size - consumed) > 256 ? 256 : (Size - consumed));
    } else {
        strm.next_in = (Bytef *)Data;
        strm.avail_in = 0;
    }
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    /* Third deflate call with Z_NO_FLUSH */
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Update input pointer for fourth deflate call */
    consumed = strm.total_in;
    if (consumed < Size) {
        strm.next_in = (Bytef *)Data + consumed;
        strm.avail_in = (uInt)(Size - consumed);
    } else {
        strm.next_in = (Bytef *)Data;
        strm.avail_in = 0;
    }
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    /* Fourth deflate call with Z_FINISH */
    ret = deflate(&strm, Z_FINISH);
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Clean up */
    ret = deflateEnd(&strm);
    if (ret != Z_OK && ret != Z_DATA_ERROR) {
        return 0;
    }
    
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    z_stream deflate_stream;
    z_stream inflate_stream;
    int ret;
    
    /* Initialize deflate stream */
    deflate_stream.zalloc = Z_NULL;
    deflate_stream.zfree = Z_NULL;
    deflate_stream.opaque = Z_NULL;
    deflate_stream.next_in = Z_NULL;
    deflate_stream.avail_in = 0;
    
    ret = deflateInit_(&deflate_stream, Z_DEFAULT_COMPRESSION,
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* Immediately end deflate stream */
    deflateEnd(&deflate_stream);
    
    /* Initialize inflate stream */
    inflate_stream.zalloc = Z_NULL;
    inflate_stream.zfree = Z_NULL;
    inflate_stream.opaque = Z_NULL;
    inflate_stream.next_in = Z_NULL;
    inflate_stream.avail_in = 0;
    
    ret = inflateInit_(&inflate_stream, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* Reset deflate stream (should fail since it was ended) */
    deflateReset(&deflate_stream);
    
    /* Reset inflate stream */
    inflateReset(&inflate_stream);
    
    /* Reset deflate stream again */
    deflateReset(&deflate_stream);
    
    /* End inflate stream */
    inflateEnd(&inflate_stream);
    
    /* End deflate stream again */
    deflateEnd(&deflate_stream);
    
    return 0;
}
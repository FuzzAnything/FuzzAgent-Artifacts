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
    strm->msg = Z_NULL;
    strm->data_type = 0;
    strm->adler = 0;
    strm->reserved = 0;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    z_stream def_stream, inf_stream;
    int level = Z_DEFAULT_COMPRESSION;
    const char *version = ZLIB_VERSION;
    int stream_size = sizeof(z_stream);
    
    /* Initialize compression stream */
    init_z_stream(&def_stream);
    deflateInit_(&def_stream, level, version, stream_size);
    
    /* Immediately end compression stream */
    deflateEnd(&def_stream);
    
    /* Initialize decompression stream */
    init_z_stream(&inf_stream);
    inflateInit_(&inf_stream, version, stream_size);
    
    /* Reset compression stream (after it was ended) */
    deflateReset(&def_stream);
    
    /* Reset decompression stream */
    inflateReset(&inf_stream);
    
    /* Reset compression stream again */
    deflateReset(&def_stream);
    
    /* End decompression stream */
    inflateEnd(&inf_stream);
    
    /* End compression stream again */
    deflateEnd(&def_stream);
    
    return 0;
}
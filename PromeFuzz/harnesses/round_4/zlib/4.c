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

static void init_z_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
    strm->msg = Z_NULL;
    strm->state = Z_NULL;
    strm->data_type = 0;
    strm->adler = 0;
    strm->reserved = 0;
    strm->total_in = 0;
    strm->total_out = 0;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    z_stream deflate_strm, inflate_strm;
    int level = Z_DEFAULT_COMPRESSION;
    const char *version = ZLIB_VERSION;
    int stream_size = sizeof(z_stream);
    
    if (Size > 0) {
        level = Data[0] % 10;
    }
    
    init_z_stream(&deflate_strm);
    init_z_stream(&inflate_strm);
    
    deflateInit_(&deflate_strm, level, version, stream_size);
    deflateEnd(&deflate_strm);
    inflateInit_(&inflate_strm, version, stream_size);
    deflateReset(&deflate_strm);
    inflateReset(&inflate_strm);
    deflateReset(&deflate_strm);
    inflateEnd(&inflate_strm);
    deflateEnd(&deflate_strm);
    
    return 0;
}
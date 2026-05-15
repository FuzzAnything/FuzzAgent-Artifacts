// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void init_z_stream(z_streamp strm, const uint8_t *data, size_t size) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = (Bytef *)data;
    strm->avail_in = size < UINT_MAX ? (uInt)size : UINT_MAX;
    strm->total_in = 0;
    strm->next_out = NULL;
    strm->avail_out = 0;
    strm->total_out = 0;
    strm->msg = NULL;
    strm->data_type = Z_BINARY;
    strm->adler = 0;
    strm->reserved = 0;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    z_stream strm1, strm2;
    Bytef out_buf[1024];
    int ret;
    size_t param_offset = 0;

    // Extract parameters from fuzzed input
    int level = Data[param_offset] % 12 - 1;
    param_offset = (param_offset + 1) % Size;
    int good_length = Data[param_offset];
    param_offset = (param_offset + 1) % Size;
    int max_lazy = Data[param_offset];
    param_offset = (param_offset + 1) % Size;
    int nice_length = Data[param_offset];
    param_offset = (param_offset + 1) % Size;
    int max_chain = Data[param_offset];
    param_offset = (param_offset + 1) % Size;
    int flush = Data[param_offset] % 4;

    // Initialize strm1 with deflateInit_
    init_z_stream(&strm1, Data, Size);
    ret = deflateInit_(&strm1, level, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    // Set up output buffer
    strm1.next_out = out_buf;
    strm1.avail_out = sizeof(out_buf);

    // Call deflate with various flush modes
    deflate(&strm1, flush);

    // Call deflateTune with extracted parameters
    deflateTune(&strm1, good_length, max_lazy, nice_length, max_chain);

    // Call deflateResetKeep
    ret = deflateResetKeep(&strm1);
    if (ret != Z_OK) {
        deflateEnd(&strm1);
        return 0;
    }

    // Re-initialize output for next deflate call
    strm1.next_out = out_buf;
    strm1.avail_out = sizeof(out_buf);
    strm1.next_in = (Bytef *)Data;
    strm1.avail_in = Size < UINT_MAX ? (uInt)Size : UINT_MAX;

    // Call deflate again with Z_FINISH
    deflate(&strm1, Z_FINISH);

    // Initialize strm2 for deflateCopy - only set allocators, don't call deflateInit_
    memset(&strm2, 0, sizeof(z_stream));
    strm2.zalloc = Z_NULL;
    strm2.zfree = Z_NULL;
    strm2.opaque = Z_NULL;

    // Call deflateCopy without initializing strm2 first
    ret = deflateCopy(&strm2, &strm1);
    if (ret == Z_OK) {
        // Only end strm2 if deflateCopy succeeded
        deflateEnd(&strm2);
    }

    // Call deflateReset on strm1
    ret = deflateReset(&strm1);
    if (ret != Z_OK) {
        deflateEnd(&strm1);
        return 0;
    }

    // Clean up
    deflateEnd(&strm1);

    return 0;
}
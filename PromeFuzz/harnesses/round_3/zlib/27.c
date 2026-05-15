// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_z_stream(z_streamp strm, const uint8_t *data, size_t size) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = (Bytef *)data;
    strm->avail_in = size > UINT_MAX ? UINT_MAX : (uInt)size;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    z_stream strm1, strm2;
    int ret;
    uint8_t *out_buf = NULL;
    size_t out_size = 0;

    // Initialize first stream with deflateInit_
    init_z_stream(&strm1, Data, Size);
    ret = deflateInit_(&strm1, Data[0] % 10, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&strm1);
        return 0;
    }

    // Allocate output buffer for compression
    out_size = Size * 2 + 1024;
    out_buf = (uint8_t *)malloc(out_size);
    if (!out_buf) {
        deflateEnd(&strm1);
        return 0;
    }
    strm1.next_out = out_buf;
    strm1.avail_out = out_size > UINT_MAX ? UINT_MAX : (uInt)out_size;

    // Perform some compression to change stream state
    deflate(&strm1, Z_FINISH);
    deflateReset(&strm1);

    // Test deflateTune with fuzzer-derived parameters
    if (Size >= 5) {
        deflateTune(&strm1, 
                   Data[1] % 256,
                   Data[2] % 256,
                   Data[3] % 256,
                   Data[4] % 256);
    }

    // Test deflateResetKeep
    deflateResetKeep(&strm1);

    // Initialize second stream with deflateInit2_ for copy test
    // Use safe parameter extraction with bounds checking
    init_z_stream(&strm2, Data, Size);
    
    int level = Data[0] % 10;
    int windowBits = 15;  // Default value
    int memLevel = 8;     // Default value
    int strategy = Z_DEFAULT_STRATEGY;  // Default value
    
    if (Size >= 2) windowBits = Data[1] % 16 + 8;
    if (Size >= 3) memLevel = Data[2] % 9 + 1;
    if (Size >= 4) strategy = Data[3] % 5;
    
    ret = deflateInit2_(&strm2,
                       level,
                       Z_DEFLATED,
                       windowBits,
                       memLevel,
                       strategy,
                       ZLIB_VERSION,
                       sizeof(z_stream));
    if (ret == Z_OK) {
        // Test deflateCopy - properly end strm2 first to avoid leak
        deflateEnd(&strm2);
        ret = deflateCopy(&strm2, &strm1);
        if (ret == Z_OK) {
            deflateEnd(&strm2);
        }
    }

    // Clean up
    deflateEnd(&strm1);
    free(out_buf);

    return 0;
}
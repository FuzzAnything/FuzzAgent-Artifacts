// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
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

static void init_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    z_stream strm1, strm2;
    int ret;
    uint8_t *dict = NULL;
    uInt dict_len = 0;

    /* Initialize strm1 for compression */
    init_stream(&strm1);
    ret = deflateInit_(&strm1, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    /* Use part of input as dictionary if enough data */
    if (Size > 10) {
        dict_len = (uInt)(Size > 65535 ? 65535 : Size - 10);
        dict = (uint8_t *)Data + 10;
        deflateSetDictionary(&strm1, dict, dict_len);
    }

    /* Call deflateTune with parameters derived from input */
    if (Size >= 5) {
        int good_length = Data[0] % 256;
        int max_lazy = Data[1] % 256;
        int nice_length = Data[2] % 256;
        int max_chain = Data[3] % 256;
        deflateTune(&strm1, good_length, max_lazy, nice_length, max_chain);
    }

    /* Call deflateReset */
    deflateReset(&strm1);

    /* Call deflateResetKeep */
    deflateResetKeep(&strm1);

    /* Initialize strm2 for deflateCopy */
    init_stream(&strm2);
    ret = deflateInit_(&strm2, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        /* Properly end strm2 before copying to avoid leak */
        deflateEnd(&strm2);
        init_stream(&strm2);
        
        /* Copy state from strm1 to strm2 */
        ret = deflateCopy(&strm2, &strm1);
        if (ret == Z_OK) {
            /* Clean up copied stream */
            deflateEnd(&strm2);
        }
    }

    /* Clean up strm1 */
    deflateEnd(&strm1);

    return 0;
}
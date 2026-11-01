// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// zlibVersion at zutil.c:27:22 in zlib.h
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

/* Fuzz driver for zlib compression APIs */

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    const uint8_t *in_buf = NULL;
    uInt in_len = 0;
    uInt out_len = 0;
    int ret;
    const char *version;
    uLong flags;
    int level, method, windowBits, memLevel, strategy;

    /* 1. Prepare environment */
    if (Size < 1) {
        return 0;
    }

    /* Use input data to seed parameters */
    level = Data[0] % 10; /* 0-9 */
    method = (Data[0] % 2) == 0 ? Z_DEFLATED : Z_RLE; /* Z_DEFLATED or Z_RLE */
    windowBits = (Data[0] % 16) + 8; /* 8-23 range */
    memLevel = (Data[0] % 9) + 1; /* 1-9 */
    strategy = Data[0] % 4; /* Z_DEFAULT_STRATEGY to Z_FIXED */

    /* Allocate output buffer (2x input size + overhead) */
    out_len = Size * 2 + 1024;
    out_buf = (uint8_t *)malloc(out_len);
    if (!out_buf) {
        return 0;
    }

    /* Set input buffer */
    in_buf = Data;
    in_len = (uInt)Size;

    /* 2. Test zlibVersion */
    version = zlibVersion();
    if (version == NULL) {
        free(out_buf);
        return 0;
    }

    /* 3. Test zlibCompileFlags */
    flags = zlibCompileFlags();
    (void)flags; /* Use the result to avoid unused variable warning */

    /* 4. Test deflateInit_ with correct version */
    ret = deflateInit_(&strm, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        /* Try with wrong version to trigger Z_VERSION_ERROR */
        ret = deflateInit_(&strm, level, "1.0.0", (int)sizeof(z_stream));
        if (ret != Z_VERSION_ERROR) {
            free(out_buf);
            return 0;
        }
        /* Re-initialize with correct version */
        ret = deflateInit_(&strm, level, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret != Z_OK) {
            free(out_buf);
            return 0;
        }
    }

    /* 5. Test deflateInit2_ with custom parameters */
    /* First, clean up the current stream */
    deflateEnd(&strm);
    
    /* Re-initialize with custom parameters */
    ret = deflateInit2_(&strm, level, method, windowBits, memLevel, strategy, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        /* Test deflateSetDictionary if input is large enough */
        if (Size > 10) {
            const Bytef *dict = &Data[0];
            uInt dictLen = (uInt)(Size / 2);
            if (dictLen > 0) {
                ret = deflateSetDictionary(&strm, dict, dictLen);
                (void)ret; /* Ignore return for fuzzing exploration */
            }
        }
    } else {
        /* If deflateInit2_ failed, fall back to deflateInit_ */
        ret = deflateInit_(&strm, level, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret != Z_OK) {
            free(out_buf);
            return 0;
        }
    }

    /* 6. Test deflate with various flush modes */
    strm.next_in = in_buf;
    strm.avail_in = in_len;
    strm.next_out = out_buf;
    strm.avail_out = out_len;

    /* Try Z_NO_FLUSH */
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
        /* Ignore errors, continue to explore */
    }

    /* Try Z_SYNC_FLUSH */
    ret = deflate(&strm, Z_SYNC_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
        /* Ignore errors */
    }

    /* Try Z_FULL_FLUSH */
    ret = deflate(&strm, Z_FULL_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
        /* Ignore errors */
    }

    /* Try Z_FINISH */
    ret = deflate(&strm, Z_FINISH);
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
        /* Ignore errors */
    }

    /* 7. Cleanup */
    deflateEnd(&strm);
    free(out_buf);

    return 0;
}
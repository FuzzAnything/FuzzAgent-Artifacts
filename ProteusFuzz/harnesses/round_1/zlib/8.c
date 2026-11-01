// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret;
    int level;
    int strategy;

    /* Allocate output buffer */
    size_t out_size = Size + 1024;
    out_buf = (uint8_t *)malloc(out_size);
    if (!out_buf) {
        return 0;
    }

    /* Initialize z_stream */
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_size;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    /* Derive level and strategy from input data to explore diverse states */
    level = (Data[0] % 10);
    strategy = (Data[1] % 4);

    /* 1. deflateInit_ */
    ret = deflateInit_(&strm, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    /* 2. deflate (first call) */
    ret = deflate(&strm, Z_NO_FLUSH);
    (void)ret;

    /* 3. deflateParams */
    ret = deflateParams(&strm, level, strategy);
    (void)ret;

    /* 4. deflate (second call) */
    ret = deflate(&strm, Z_NO_FLUSH);
    (void)ret;

    /* 5. deflateParams (second call with different params) */
    int new_level = (Data[2] % 10);
    int new_strategy = (Data[3] % 4);
    ret = deflateParams(&strm, new_level, new_strategy);
    (void)ret;

    /* 6. deflate (third call) */
    ret = deflate(&strm, Z_NO_FLUSH);
    (void)ret;

    /* 7. deflate (final call with Z_FINISH) */
    ret = deflate(&strm, Z_FINISH);
    (void)ret;

    /* 8. deflateEnd */
    ret = deflateEnd(&strm);
    (void)ret;

    free(out_buf);
    return 0;
}
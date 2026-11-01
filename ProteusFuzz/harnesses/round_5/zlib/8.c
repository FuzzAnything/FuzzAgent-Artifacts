// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret;

    // Allocate output buffer
    out_buf = (uint8_t *)malloc(Size + 1024);
    if (!out_buf) {
        return 0;
    }

    // Initialize the stream
    ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Set input data
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(Size + 1024);

    // First deflate call
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        free(out_buf);
        return 0;
    }

    // First deflateParams call
    int level = (Size > 0) ? (Data[0] % 10) : 6;
    int strategy = (Size > 1) ? (Data[1] % 4) : Z_DEFAULT_STRATEGY;
    ret = deflateParams(&strm, level, strategy);

    // Second deflate call
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        free(out_buf);
        return 0;
    }

    // Second deflateParams call
    level = (Size > 2) ? (Data[2] % 10) : 1;
    strategy = (Size > 3) ? (Data[3] % 4) : Z_HUFFMAN_ONLY;
    ret = deflateParams(&strm, level, strategy);

    // Third deflate call
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        free(out_buf);
        return 0;
    }

    // Fourth deflate call (finish)
    ret = deflate(&strm, Z_FINISH);

    // Cleanup
    deflateEnd(&strm);
    free(out_buf);

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    z_stream strm = {0};
    z_stream strm_copy = {0};
    unsigned char *in_buf = NULL;
    unsigned char *out_buf = NULL;
    unsigned pending;
    int bits;
    int ret;

    // Allocate buffers
    size_t in_size = (Size > 0) ? Size : 1;
    size_t out_size = in_size + (in_size / 10) + 128;
    
    in_buf = (unsigned char *)malloc(in_size);
    out_buf = (unsigned char *)malloc(out_size);
    
    if (!in_buf || !out_buf) {
        free(in_buf);
        free(out_buf);
        return 0;
    }

    // Copy fuzz input to input buffer
    memcpy(in_buf, Data, in_size);

    // Initialize stream
    strm.next_in = in_buf;
    strm.avail_in = in_size;
    strm.next_out = out_buf;
    strm.avail_out = out_size;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        free(in_buf);
        free(out_buf);
        return 0;
    }

    // Perform some compression to establish state
    ret = deflate(&strm, Z_NO_FLUSH);
    (void)ret;

    // Fuzz deflateResetKeep
    ret = deflateResetKeep(&strm);
    (void)ret;

    // Fuzz deflatePending
    ret = deflatePending(&strm, &pending, &bits);
    (void)ret;

    // Fuzz deflateUsed
    ret = deflateUsed(&strm, &bits);
    (void)ret;

    // Fuzz deflateTune with fuzzed parameters
    int good_len = (Data[0] % 20) + 1;
    int max_lazy = (Data[1] % 20) + 1;
    int nice_len = (Data[2] % 20) + 1;
    int max_chain = (Data[3] % 20) + 1;
    ret = deflateTune(&strm, good_len, max_lazy, nice_len, max_chain);
    (void)ret;

    // Fuzz deflateCopy
    ret = deflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        // Perform some operations on the copied stream
        ret = deflate(&strm_copy, Z_NO_FLUSH);
        (void)ret;
        
        // Cleanup copied stream
        deflateEnd(&strm_copy);
    }

    // Fuzz deflateReset
    ret = deflateReset(&strm);
    (void)ret;

    // Cleanup
    deflateEnd(&strm);
    free(in_buf);
    free(out_buf);

    return 0;
}
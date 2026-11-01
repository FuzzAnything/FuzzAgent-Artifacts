// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream copy_strm = {0};
    unsigned char out_buf[4096];
    unsigned char in_buf[4096];
    unsigned pending;
    int bits;
    int ret;

    if (Size == 0) {
        return 0;
    }

    // Prepare input data
    size_t in_len = (Size < sizeof(in_buf)) ? Size : sizeof(in_buf);
    memcpy(in_buf, Data, in_len);

    // Initialize stream
    strm.next_in = in_buf;
    strm.avail_in = (uInt)in_len;
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    // Fuzz deflatePending
    ret = deflatePending(&strm, &pending, &bits);
    (void)ret; // Suppress unused variable warning

    // Fuzz deflateUsed
    ret = deflateUsed(&strm, &bits);
    (void)ret;

    // Fuzz deflatePrime with various bit counts and values derived from input
    if (Size >= 2) {
        int prime_bits = Data[0] % 17; // 0-16
        int prime_val = Data[1];
        ret = deflatePrime(&strm, prime_bits, prime_val);
        (void)ret;
    }

    // Fuzz deflateTune with parameters derived from input
    if (Size >= 4) {
        int good_length = Data[2] % 20;
        int max_lazy = Data[3] % 20;
        int nice_length = (Data[2] + Data[3]) % 20;
        int max_chain = (Data[2] + Data[3]) % 20;
        ret = deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
        (void)ret;
    }

    // Perform some compression to ensure state is valid for copy/reset
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END) {
        deflateEnd(&strm);
        return 0;
    }

    // Fuzz deflateCopy
    copy_strm.zalloc = Z_NULL;
    copy_strm.zfree = Z_NULL;
    copy_strm.opaque = Z_NULL;
    ret = deflateCopy(&copy_strm, &strm);
    if (ret == Z_OK) {
        // Test pending/used on copied stream
        ret = deflatePending(&copy_strm, &pending, &bits);
        (void)ret;
        ret = deflateUsed(&copy_strm, &bits);
        (void)ret;
        
        // Cleanup copy stream
        deflateEnd(&copy_strm);
    }

    // Fuzz deflateResetKeep
    ret = deflateResetKeep(&strm);
    (void)ret;

    // Cleanup
    deflateEnd(&strm);

    return 0;
}
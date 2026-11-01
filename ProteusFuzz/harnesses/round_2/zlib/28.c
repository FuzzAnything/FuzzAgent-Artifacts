// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    z_stream strm = {0};
    z_stream strm_copy = {0};
    Bytef *out_buf = NULL;
    unsigned pending = 0;
    int bits = 0;
    int ret;

    // Allocate output buffer
    out_buf = (Bytef *)malloc(Size + 256);
    if (!out_buf) {
        return 0;
    }

    // Initialize stream
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Set input data
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = Size;

    // Set output buffer
    strm.next_out = out_buf;
    strm.avail_out = Size + 256;

    // Fuzz deflateTune
    // Parameters are derived from input data to explore various states
    int good_length = (Size > 0) ? (Data[0] % 512) : 0;
    int max_lazy = (Size > 1) ? (Data[1] % 256) : 0;
    int nice_length = (Size > 2) ? (Data[2] % 1024) : 0;
    int max_chain = (Size > 3) ? (Data[3] % 256) : 0;

    ret = deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
    (void)ret; // Ignore return value for fuzzing exploration

    // Fuzz deflateUsed
    ret = deflateUsed(&strm, &bits);
    (void)ret;

    // Fuzz deflatePending
    ret = deflatePending(&strm, &pending, &bits);
    (void)ret;

    // Fuzz deflatePrime
    int prime_bits = (Size > 4) ? (Data[4] % 17) : 0;
    int prime_value = (Size > 5) ? (Data[5] % 65536) : 0;
    ret = deflatePrime(&strm, prime_bits, prime_value);
    (void)ret;

    // Perform a partial compression to ensure state is valid for other calls
    if (strm.avail_in > 0 && strm.avail_out > 0) {
        deflate(&strm, Z_NO_FLUSH);
    }

    // Fuzz deflateResetKeep
    ret = deflateResetKeep(&strm);
    (void)ret;

    // Fuzz deflateCopy
    // Initialize dest stream minimally before copy
    ret = deflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        // Clean up copied stream
        deflateEnd(&strm_copy);
    }

    // Cleanup
    deflateEnd(&strm);
    free(out_buf);

    return 0;
}
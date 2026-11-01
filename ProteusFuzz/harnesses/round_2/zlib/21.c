// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static void setup_stream(z_stream *strm, const uint8_t *data, size_t size) {
    if (strm == NULL) return;
    memset(strm, 0, sizeof(z_stream));
    if (data != NULL && size > 0) {
        strm->next_in = (const Bytef *)data;
        strm->avail_in = (uInt)size;
    }
}

static void cleanup_stream(z_stream *strm) {
    if (strm != NULL && strm->state != NULL) {
        deflateEnd(strm);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream strm_copy = {0};
    Bytef out_buf[4096];
    int bits;
    int ret;

    // Ensure we have enough data to safely access indices 0, 1, and 2
    if (Size < 3) {
        return 0;
    }

    // Extract parameters from input data to diversify states
    int good_length = Data[0] & 0x3F; // 0-63
    int max_lazy = Data[1] & 0x1F;    // 0-31
    int nice_length = (Data[0] >> 6) | (Data[1] << 2);
    int max_chain = Data[1] & 0xFF;

    // Ensure some minimum values to avoid immediate failures if logic requires
    if (good_length < 1) good_length = 1;
    if (nice_length < 1) nice_length = 1;

    // Setup stream
    setup_stream(&strm, Data, Size);
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    // 1. Initialize with deflateInit2_
    // Use ZLIB_VERSION and sizeof(z_stream) for valid initialization
    ret = deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15, 8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    // 2. Test deflateTune
    ret = deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
    (void)ret; // Ignore return for fuzzing, just exercise the path

    // 3. Test deflatePrime
    // bits: 0-16, value: 0-65535
    int prime_bits = Data[0] % 17;
    int prime_value = Data[1] * 256 + Data[2];
    ret = deflatePrime(&strm, prime_bits, prime_value);
    (void)ret;

    // 4. Test deflateUsed
    ret = deflateUsed(&strm, &bits);
    (void)ret;
    ret = deflateUsed(&strm, NULL);
    (void)ret;

    // 5. Test deflateResetKeep
    ret = deflateResetKeep(&strm);
    (void)ret;

    // 6. Test deflateCopy
    // Initialize dest stream minimally
    memset(&strm_copy, 0, sizeof(z_stream));
    ret = deflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        // Perform a small deflate operation on the copy to ensure state is valid
        strm_copy.next_out = out_buf;
        strm_copy.avail_out = sizeof(out_buf);
        deflate(&strm_copy, Z_NO_FLUSH);
        cleanup_stream(&strm_copy);
    }

    // Cleanup source stream
    cleanup_stream(&strm);

    return 0;
}
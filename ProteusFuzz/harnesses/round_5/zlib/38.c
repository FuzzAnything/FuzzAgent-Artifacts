// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream strm_copy = {0};
    z_stream strm_prime = {0};
    z_stream strm_tune = {0};
    z_stream strm_used = {0};
    Bytef *in_buf = NULL;
    Bytef *out_buf = NULL;
    int ret;
    int bits;
    int level, method, windowBits, memLevel, strategy;
    int good_length, max_lazy, nice_length, max_chain;
    int bits_to_prime, value_to_prime;

    if (Size < 1) {
        return 0;
    }

    // Parse parameters from input data if possible, otherwise use defaults
    level = (Size > 0) ? (Data[0] % 10) : 6;
    method = (Size > 1) ? (Data[1] % 2) + Z_DEFLATED : Z_DEFLATED;
    windowBits = (Size > 2) ? (Data[2] % 16) + 8 : 15;
    memLevel = (Size > 3) ? (Data[3] % 10) + 1 : 8;
    strategy = (Size > 4) ? (Data[4] % 5) : Z_DEFAULT_STRATEGY;
    
    good_length = (Size > 5) ? (Data[5] % 256) : 258;
    max_lazy = (Size > 6) ? (Data[6] % 256) : 258;
    nice_length = (Size > 7) ? (Data[7] % 256) : 258;
    max_chain = (Size > 8) ? (Data[8] % 256) : 4096;

    bits_to_prime = (Size > 9) ? (Data[9] % 17) : 8;
    value_to_prime = (Size > 10) ? (Data[10] % 256) : 0;

    // Allocate buffers
    size_t input_size = Size > 10 ? Size - 10 : 0;
    if (input_size > 0) {
        in_buf = (Bytef *)malloc(input_size);
        if (!in_buf) return 0;
        memcpy(in_buf, Data + 10, input_size);
    } else {
        in_buf = (Bytef *)malloc(1);
        if (!in_buf) return 0;
        in_buf[0] = 0;
        input_size = 1;
    }

    size_t output_size = input_size * 2 + 1024;
    out_buf = (Bytef *)malloc(output_size);
    if (!out_buf) {
        free(in_buf);
        return 0;
    }

    // 1. Test deflateInit2_
    ret = deflateInit2_(&strm, level, method, windowBits, memLevel, strategy, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        // Expected to fail if version mismatch or invalid params, but we handle it
    } else {
        strm.next_in = in_buf;
        strm.avail_in = input_size;
        strm.next_out = out_buf;
        strm.avail_out = output_size;

        // Perform some compression to establish state
        ret = deflate(&strm, Z_NO_FLUSH);
        if (ret == Z_OK || ret == Z_STREAM_END) {
            // 2. Test deflateResetKeep
            ret = deflateResetKeep(&strm);
            if (ret == Z_OK) {
                // Reset successful, try compressing again
                strm.next_in = in_buf;
                strm.avail_in = input_size;
                strm.next_out = out_buf;
                strm.avail_out = output_size;
                deflate(&strm, Z_FINISH);
            }

            // 3. Test deflateCopy
            ret = deflateCopy(&strm_copy, &strm);
            if (ret == Z_OK) {
                // Use the copied stream
                strm_copy.next_in = in_buf;
                strm_copy.avail_in = input_size;
                strm_copy.next_out = out_buf;
                strm_copy.avail_out = output_size;
                deflate(&strm_copy, Z_FINISH);
                deflateEnd(&strm_copy);
            }

            // 4. Test deflatePrime
            ret = deflatePrime(&strm, bits_to_prime, value_to_prime);
            // Ignore return value as it might fail due to buffer constraints

            // 5. Test deflateUsed
            ret = deflateUsed(&strm, &bits);
            (void)ret; // Just checking functionality

            // 6. Test deflateTune
            ret = deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
            // Ignore return value, might fail if stream state is invalid
        }

        // Cleanup primary stream
        deflateEnd(&strm);
    }

    // Additional test: Initialize streams specifically for other functions to ensure coverage
    // Test deflateUsed on a fresh stream
    ret = deflateInit2_(&strm_used, level, method, windowBits, memLevel, strategy, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        deflateUsed(&strm_used, NULL);
        deflateUsed(&strm_used, &bits);
        deflateEnd(&strm_used);
    }

    // Test deflateTune on a fresh stream
    ret = deflateInit2_(&strm_tune, level, method, windowBits, memLevel, strategy, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        deflateTune(&strm_tune, good_length, max_lazy, nice_length, max_chain);
        deflateEnd(&strm_tune);
    }

    // Test deflatePrime on a fresh stream
    ret = deflateInit2_(&strm_prime, level, method, windowBits, memLevel, strategy, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        deflatePrime(&strm_prime, bits_to_prime, value_to_prime);
        deflateEnd(&strm_prime);
    }

    // Cleanup
    free(in_buf);
    free(out_buf);

    return 0;
}
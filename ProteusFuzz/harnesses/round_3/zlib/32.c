// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    z_stream dest = {0};
    z_stream source = {0};
    uint8_t *input_buf = NULL;
    uint8_t *output_buf = NULL;
    int ret;
    int bits;

    if (Size < 4) {
        return 0;
    }

    input_buf = malloc(Size);
    if (!input_buf) return 0;
    memcpy(input_buf, Data, Size);

    output_buf = malloc(Size + 1024);
    if (!output_buf) {
        free(input_buf);
        return 0;
    }

    // Initialize source stream
    ret = deflateInit_(&source, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(input_buf);
        free(output_buf);
        return 0;
    }

    // Prepare input for source
    source.next_in = input_buf;
    source.avail_in = Size;
    source.next_out = output_buf;
    source.avail_out = Size + 1024;

    // Perform some compression to establish state
    ret = deflate(&source, Z_NO_FLUSH);
    (void)ret;

    // Test 1: deflateUsed
    ret = deflateUsed(&source, &bits);
    (void)ret;
    ret = deflateUsed(&source, NULL);
    (void)ret;

    // Test 2: deflateTune
    // Use fuzzed values clamped to reasonable ranges to avoid immediate crashes from bad params
    int good_length = (Data[0] % 100);
    int max_lazy = (Data[1] % 100);
    int nice_length = (Data[2] % 100);
    int max_chain = (Data[3] % 100);
    ret = deflateTune(&source, good_length, max_lazy, nice_length, max_chain);
    (void)ret;

    // Test 3: deflateCopy
    ret = deflateCopy(&dest, &source);
    if (ret == Z_OK) {
        // Test deflateUsed on destination
        ret = deflateUsed(&dest, &bits);
        (void)ret;

        // Test deflateResetKeep on destination
        ret = deflateResetKeep(&dest);
        (void)ret;
    }

    // Test 4: deflateReset on source
    ret = deflateReset(&source);
    (void)ret;

    // Test 5: deflateResetKeep on source
    ret = deflateResetKeep(&source);
    (void)ret;

    // Cleanup
    deflateEnd(&source);
    if (ret == Z_OK) {
        deflateEnd(&dest);
    }

    free(input_buf);
    free(output_buf);

    return 0;
}
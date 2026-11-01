// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    uint8_t *output_buffer = NULL;
    int ret;
    int level;
    int strategy;

    if (Size < 4) {
        return 0;
    }

    // Initialize compression level and strategy from input data to diversify states
    level = Data[0] % 10;
    strategy = (Data[1] % 3); // Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY

    // Allocate output buffer (size can be larger than input to handle compression overhead)
    size_t out_size = Size + 100;
    output_buffer = (uint8_t *)malloc(out_size);
    if (!output_buffer) {
        return 0;
    }

    // 1. Initialize the stream
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = output_buffer;
    strm.avail_out = (uInt)out_size;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ret = deflateInit_(&strm, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(output_buffer);
        return 0;
    }

    // 2. First deflate call
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret == Z_STREAM_ERROR) {
        deflateEnd(&strm);
        free(output_buffer);
        return 0;
    }

    // 3. Change parameters
    int new_level = (Data[2] % 10);
    int new_strategy = (Data[3] % 3);
    ret = deflateParams(&strm, new_level, new_strategy);
    if (ret == Z_STREAM_ERROR) {
        deflateEnd(&strm);
        free(output_buffer);
        return 0;
    }

    // 4. Second deflate call
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret == Z_STREAM_ERROR) {
        deflateEnd(&strm);
        free(output_buffer);
        return 0;
    }

    // 5. Change parameters again
    int new_level_2 = (Data[0] + Data[1]) % 10;
    int new_strategy_2 = (Data[2] + Data[3]) % 3;
    ret = deflateParams(&strm, new_level_2, new_strategy_2);
    if (ret == Z_STREAM_ERROR) {
        deflateEnd(&strm);
        free(output_buffer);
        return 0;
    }

    // 6. Third deflate call
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret == Z_STREAM_ERROR) {
        deflateEnd(&strm);
        free(output_buffer);
        return 0;
    }

    // 7. Fourth deflate call (finish)
    ret = deflate(&strm, Z_FINISH);
    if (ret == Z_STREAM_ERROR) {
        deflateEnd(&strm);
        free(output_buffer);
        return 0;
    }

    // 8. Finalize
    ret = deflateEnd(&strm);
    (void)ret; // Ignore return value for cleanup, just ensure resources are freed

    free(output_buffer);
    return 0;
}
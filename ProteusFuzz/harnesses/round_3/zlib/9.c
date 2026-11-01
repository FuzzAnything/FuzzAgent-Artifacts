// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    z_stream strm;
    uint8_t *input_buf;
    uint8_t *output_buf;
    int ret;
    int level;
    int strategy;
    int new_strategy;

    // Ensure we have enough data to safely access indices 0, 1, and 2
    if (Size < 3) {
        return 0;
    }

    // Allocate buffers for input and output
    size_t input_size = Size;
    // Output buffer needs to be larger to handle compression overhead
    // zlib recommends at least input_size + (input_size / 10) + 128 + 3
    size_t output_size = input_size + (input_size / 10) + 128 + 3;
    
    input_buf = (uint8_t *)malloc(input_size);
    output_buf = (uint8_t *)malloc(output_size);
    
    if (!input_buf || !output_buf) {
        free(input_buf);
        free(output_buf);
        return 0;
    }

    memcpy(input_buf, Data, input_size);

    // Initialize stream
    memset(&strm, 0, sizeof(strm));
    strm.next_in = (Bytef *)input_buf;
    strm.avail_in = (uInt)input_size;
    strm.next_out = output_buf;
    strm.avail_out = (uInt)output_size;

    // Derive level and strategy from input data to explore diverse states
    // Level: 0 to 9
    level = Data[0] % 10;
    // Strategy: Z_DEFAULT_STRATEGY (0), Z_FILTERED (1), Z_HUFFMAN_ONLY (2), Z_RLE (3), Z_FIXED (4)
    strategy = Data[1] % 5;

    // 1. deflateInit_
    ret = deflateInit_(&strm, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(input_buf);
        free(output_buf);
        return 0;
    }

    // 2. deflate (First call)
    ret = deflate(&strm, Z_NO_FLUSH);
    // Handle potential errors gracefully, continue to next steps if stream is still valid
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        free(input_buf);
        free(output_buf);
        return 0;
    }

    // 3. deflateParams (First call)
    ret = deflateParams(&strm, level, strategy);
    // Continue even if it returns Z_BUF_ERROR or Z_STREAM_ERROR to test recovery paths

    // 4. deflate (Second call)
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        free(input_buf);
        free(output_buf);
        return 0;
    }

    // 5. deflateParams (Second call) - maybe change strategy again
    new_strategy = Data[2] % 5;
    ret = deflateParams(&strm, level, new_strategy);

    // 6. deflate (Third call)
    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
        deflateEnd(&strm);
        free(input_buf);
        free(output_buf);
        return 0;
    }

    // 7. deflate (Fourth call - Finish)
    ret = deflate(&strm, Z_FINISH);
    // We expect Z_STREAM_END or Z_BUF_ERROR here, but handle others gracefully

    // 8. deflateEnd
    ret = deflateEnd(&strm);

    free(input_buf);
    free(output_buf);
    
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret;
    int level, strategy;
    int level2, strategy2;

    if (Size < 4) {
        return 0;
    }

    // Extract parameters from input data to diversify fuzzing
    level = Data[0] % 10; // 0-9
    strategy = Data[1] % 5; // 0-4
    level2 = Data[2] % 10;
    strategy2 = Data[3] % 5;

    // Allocate output buffer (generous size to avoid Z_BUF_ERROR during initial tests)
    out_buf = (uint8_t *)malloc(Size * 2 + 1024);
    if (out_buf == NULL) {
        return 0;
    }

    // 1. deflateInit_
    // Initialize with ZLIB_VERSION and sizeof(z_stream) for ABI check
    ret = deflateInit_(&strm, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Setup input
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = Size * 2 + 1024;

    // 2. deflate (First call)
    ret = deflate(&strm, Z_NO_FLUSH);
    // Ignore return value for fuzzing exploration, but ensure stream is valid

    // 3. deflateParams (First adjustment)
    ret = deflateParams(&strm, level2, strategy2);
    // Handle potential errors gracefully, continue fuzzing

    // 4. deflate (Second call)
    // Reset input pointer if we want to process remaining data or reuse
    // For fuzzing, we often let it run with current state
    ret = deflate(&strm, Z_NO_FLUSH);

    // 5. deflateParams (Second adjustment)
    // Try to change back or to new values
    ret = deflateParams(&strm, level, strategy);

    // 6. deflate (Third call)
    ret = deflate(&strm, Z_NO_FLUSH);

    // 7. deflate (Finalize)
    // Try Z_FINISH to complete the stream
    ret = deflate(&strm, Z_FINISH);
    
    // If Z_STREAM_END was not reached, try flushing
    if (ret == Z_OK && strm.avail_out == 0) {
        // Extend buffer if needed (simplified for fuzzing)
        ret = deflate(&strm, Z_FINISH);
    }

    // 8. deflateEnd
    ret = deflateEnd(&strm);

    free(out_buf);
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    size_t out_buf_size;
    int ret;

    // Allocate output buffer, ensuring it's large enough
    out_buf_size = Size * 2 + 1024;
    out_buf = (uint8_t *)malloc(out_buf_size);
    if (!out_buf) {
        return 0;
    }

    // Initialize the stream with default parameters
    // We use deflateInit2_ to test the specific API requested
    ret = deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        -15, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Set input data
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_buf_size;

    // Fuzz deflateSetDictionary
    // Use a portion of input data as dictionary if available
    if (Size > 4) {
        uInt dict_len = Size > 100 ? 100 : Size - 4;
        ret = deflateSetDictionary(&strm, Data, dict_len);
        // Ignore return value, just exercising the path
    }

    // Fuzz deflatePrime
    // Try inserting some bits, ensuring we have enough data
    if (Size >= 2) {
        int bits = (Data[0] % 17); // 0-16
        int value = (Data[1] % 65536); // 0-65535
        ret = deflatePrime(&strm, bits, value);
    }

    // Fuzz deflateUsed
    int bits_used = 0;
    ret = deflateUsed(&strm, &bits_used);

    // Fuzz deflate with various flush modes
    // Cycle through flush modes based on input data
    int flush_mode = Z_NO_FLUSH;
    if (Size > 2) {
        switch (Data[2] % 4) {
            case 0: flush_mode = Z_NO_FLUSH; break;
            case 1: flush_mode = Z_SYNC_FLUSH; break;
            case 2: flush_mode = Z_FULL_FLUSH; break;
            case 3: flush_mode = Z_FINISH; break;
        }
    }

    ret = deflate(&strm, flush_mode);

    // If we finished or got an error, try to reset and continue
    if (ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
        // Reset the stream keeping dictionary
        ret = deflateResetKeep(&strm);
        if (ret == Z_OK) {
            // Re-feed some data to test reset path
            strm.next_in = (const Bytef *)Data;
            strm.avail_in = (uInt)Size;
            strm.next_out = out_buf;
            strm.avail_out = (uInt)out_buf_size;
            ret = deflate(&strm, Z_NO_FLUSH);
        }
    }

    // Cleanup
    deflateEnd(&strm);
    free(out_buf);

    return 0;
}
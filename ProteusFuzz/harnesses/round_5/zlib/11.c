// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret;

    // Need at least 7 bytes to safely access Data[0] to Data[6]
    if (Size < 7) {
        return 0;
    }

    // Allocate output buffer
    out_buf = (uint8_t *)malloc(Size + 256);
    if (!out_buf) {
        return 0;
    }

    // Strategy selection based on input data
    int level = (Data[0] % 10);
    int strategy = (Data[0] % 3); // Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY
    int method = Z_DEFLATED;
    int windowBits = (Data[1] % 16) + 8; // 8 to 23
    int memLevel = (Data[2] % 9) + 1;    // 1 to 9

    // Initialize stream
    ret = deflateInit2_(&strm, level, method, windowBits, memLevel, strategy,
                        ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Prepare input and output
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = out_buf;
    strm.avail_out = Size + 256;

    // Fuzz deflateSetDictionary
    // Ensure we have enough data for dictionary (start at index 4, need at least 1 byte)
    if (Size > 4) {
        const Bytef *dict = Data + 4;
        uInt dictLen = (Size - 4) > 1000 ? 1000 : (Size - 4);
        if (dictLen > 0) {
            deflateSetDictionary(&strm, dict, dictLen);
        }
    }

    // Fuzz deflatePrime
    // Ensure we have enough data for bits and value (indices 5 and 6)
    if (Size > 6) {
        int bits = (Data[5] % 17); // 0 to 16
        int value = (int)(Data[6] % 65536);
        deflatePrime(&strm, bits, value);
    }

    // Fuzz deflate with various flush modes
    int flush_mode = (Data[0] % 4); // Z_NO_FLUSH, Z_PARTIAL_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH
    ret = deflate(&strm, flush_mode);

    // Try to finish compression
    if (ret == Z_OK || ret == Z_STREAM_END) {
        ret = deflate(&strm, Z_FINISH);
    }

    // Cleanup
    deflateEnd(&strm);
    free(out_buf);

    return 0;
}
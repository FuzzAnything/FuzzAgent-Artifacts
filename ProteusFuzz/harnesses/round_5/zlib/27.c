// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// zlibVersion at zutil.c:27:22 in zlib.h
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static void cleanup_stream(z_stream *strm) {
    if (strm && strm->state) {
        deflateEnd(strm);
    }
    memset(strm, 0, sizeof(z_stream));
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    z_stream strm = {0};
    z_stream inflate_strm = {0};
    uint8_t *compressed_buf = NULL;
    uint8_t *decompressed_buf = NULL;
    int ret;

    // Allocate buffers for compression and decompression
    // Use a safe upper bound for compressed size
    size_t compress_size = Size + (Size / 10) + 128;
    compressed_buf = (uint8_t *)malloc(compress_size);
    decompressed_buf = (uint8_t *)malloc(Size + 128);

    if (!compressed_buf || !decompressed_buf) {
        free(compressed_buf);
        free(decompressed_buf);
        return 0;
    }

    // 1. Test zlibVersion and zlibCompileFlags
    const char *ver = zlibVersion();
    (void)ver; // Use to avoid unused warning
    uLong flags = zlibCompileFlags();
    (void)flags; // Use to avoid unused warning

    // 2. Test deflateInit_
    ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 3. Test deflateInit2_ with diverse parameters
    // We reuse the stream, so we must end the previous init first
    deflateEnd(&strm);
    memset(&strm, 0, sizeof(z_stream));

    // Extract parameters from input data to diversify
    // Ensure we have enough data before accessing indices
    int level = (Data[0] % 10);
    int method = Z_DEFLATED; // Only DEFLATED is standard
    int windowBits = (Data[1] % 16) + 8; // 8 to 23
    int memLevel = (Data[2] % 9) + 1;    // 1 to 9
    int strategy = (Data[3] % 3);        // 0, 1, 2

    ret = deflateInit2_(&strm, level, method, windowBits, memLevel, strategy,
                        ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        // Invalid parameters are expected sometimes in fuzzing, just continue
        goto cleanup;
    }

    // 4. Test deflate
    strm.next_in = (Bytef *)Data;
    strm.avail_in = Size;
    strm.next_out = compressed_buf;
    strm.avail_out = (uInt)compress_size;

    ret = deflate(&strm, Z_NO_FLUSH);
    if (ret != Z_OK && ret != Z_STREAM_END) {
        // Error or stream end, continue to cleanup
    }

    // Finish the compression
    ret = deflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        // Might fail if buffer too small, handled by cleanup
    }

    // 5. Test inflateInit_
    inflate_strm.next_in = NULL;
    inflate_strm.avail_in = 0;
    inflate_strm.next_out = NULL;
    inflate_strm.avail_out = 0;

    ret = inflateInit_(&inflate_strm, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 6. Test inflate (decompress)
    if (strm.total_out > 0) {
        inflate_strm.next_in = compressed_buf;
        inflate_strm.avail_in = (uInt)strm.total_out;
        inflate_strm.next_out = decompressed_buf;
        inflate_strm.avail_out = (uInt)(Size + 128);

        ret = inflate(&inflate_strm, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            // Decompression error, expected for malformed data
        }
        
        ret = inflate(&inflate_strm, Z_FINISH);
        (void)ret;
    }

cleanup:
    deflateEnd(&strm);
    inflateEnd(&inflate_strm);
    free(compressed_buf);
    free(decompressed_buf);
    return 0;
}
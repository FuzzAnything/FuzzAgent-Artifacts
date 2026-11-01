// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// zlibVersion at zutil.c:27:22 in zlib.h
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static void cleanup_stream(z_stream *strm) {
    if (strm != NULL) {
        if (strm->zalloc != NULL && strm->zfree != NULL && strm->opaque != NULL) {
            deflateEnd(strm);
            inflateEnd(strm);
        }
        memset(strm, 0, sizeof(z_stream));
    }
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    z_stream strm = {0};
    int ret;
    const char *version = zlibVersion();
    uLong flags = zlibCompileFlags();
    (void)version;
    (void)flags;

    // Test 1: deflateInit_
    ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Provide some input data if available
        if (Size > 0) {
            strm.next_in = Data;
            strm.avail_in = Size;
        }
        // Set dictionary if we have enough data
        if (Size > 10) {
            ret = deflateSetDictionary(&strm, Data, (uInt)(Size > 1000 ? 1000 : Size - 10));
            (void)ret;
        }
        cleanup_stream(&strm);
    }

    // Test 2: deflateInit2_ with varied parameters
    // Ensure we have enough data before accessing indices
    int level = (Size > 0) ? (Data[0] % 10) : 6;
    int method = Z_DEFLATED; // Only DEFLATED is supported
    int windowBits = (Size > 1) ? ((Data[1] % 16) + 8) : 15; // Range 8-23
    int memLevel = (Size > 2) ? ((Data[2] % 9) + 1) : 8;     // Range 1-9
    int strategy = (Size > 3) ? (Data[3] % 3) : 0;           // Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY

    ret = deflateInit2_(&strm, level, method, windowBits, memLevel, strategy, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        if (Size > 0) {
            strm.next_in = Data;
            strm.avail_in = Size;
        }
        if (Size > 20) {
            ret = deflateSetDictionary(&strm, Data + 10, (uInt)(Size > 1010 ? 1000 : Size - 20));
            (void)ret;
        }
        cleanup_stream(&strm);
    }

    // Test 3: inflateInit_
    ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        if (Size > 0) {
            strm.next_in = Data;
            strm.avail_in = Size;
        }
        cleanup_stream(&strm);
    }

    // Test 4: Invalid version/size checks (should fail gracefully)
    ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, "1.0.0", (int)sizeof(z_stream));
    (void)ret;
    ret = inflateInit_(&strm, "1.0.0", (int)sizeof(z_stream));
    (void)ret;
    ret = deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY, "1.0.0", (int)sizeof(z_stream));
    (void)ret;

    // Test 5: deflateSetDictionary on uninitialized stream (should return Z_STREAM_ERROR)
    ret = deflateSetDictionary(&strm, Data, Size);
    (void)ret;

    // Test 6: deflateSetDictionary with NULL dictionary
    ret = deflateSetDictionary(&strm, NULL, 0);
    (void)ret;

    return 0;
}
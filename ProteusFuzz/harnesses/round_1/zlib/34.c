// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// zlibVersion at zutil.c:27:22 in zlib.h
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test zlibVersion
    const char *version = zlibVersion();
    (void)version; // Suppress unused variable warning

    // Test zlibCompileFlags
    uLong flags = zlibCompileFlags();
    (void)flags; // Suppress unused variable warning

    z_stream strm = {0};
    int ret;

    // Fuzz deflateInit_
    int level = (Data[0] % 10); // 0-9
    ret = deflateInit_(&strm, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Fuzz deflateInit2_ (should fail as stream is already initialized, but tests error path)
        // Ensure we have enough data before accessing indices
        int method = Z_DEFLATED;
        int windowBits = 15;
        int memLevel = 8;
        int strategy = Z_DEFAULT_STRATEGY;

        if (Size > 1) method = (Data[1] % 2) ? Z_DEFLATED : Z_DEFLATED;
        if (Size > 2) windowBits = ((Data[2] % 16) - 8);
        if (Size > 3) memLevel = ((Data[3] % 10) + 1);
        if (Size > 4) strategy = (Data[4] % 3);

        ret = deflateInit2_(&strm, level, method, windowBits, memLevel, strategy, ZLIB_VERSION, (int)sizeof(z_stream));
        (void)ret;

        // Cleanup deflate stream
        deflateEnd(&strm);
    }

    // Re-initialize for inflate test
    memset(&strm, 0, sizeof(z_stream));

    // Fuzz inflateInit_
    ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Provide some input data for inflate
        if (Size > 0) {
            strm.next_in = Data;
            strm.avail_in = (uInt)(Size > 65535 ? 65535 : Size);
        }

        // Allocate output buffer
        uInt out_size = 1024;
        Bytef *out_buf = (Bytef *)malloc(out_size);
        if (out_buf) {
            strm.next_out = out_buf;
            strm.avail_out = out_size;

            // Perform a single inflate step to explore state
            inflate(&strm, Z_NO_FLUSH);

            free(out_buf);
        }

        // Cleanup inflate stream
        inflateEnd(&strm);
    }

    // Test with invalid version string to trigger Z_VERSION_ERROR
    memset(&strm, 0, sizeof(z_stream));
    ret = deflateInit_(&strm, 1, "invalid_version", (int)sizeof(z_stream));
    (void)ret;

    ret = inflateInit_(&strm, "invalid_version", (int)sizeof(z_stream));
    (void)ret;

    // Test deflateEnd on uninitialized stream (should return Z_STREAM_ERROR)
    memset(&strm, 0, sizeof(z_stream));
    ret = deflateEnd(&strm);
    (void)ret;

    // Test inflateEnd on uninitialized stream (should return Z_STREAM_ERROR)
    ret = inflateEnd(&strm);
    (void)ret;

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// zlibVersion at zutil.c:27:22 in zlib.h
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // 1. Prepare environment
    z_stream strm = {0};
    const char *version_str;
    uLong compile_flags;
    int ret;
    int level;
    int method;
    int windowBits;
    int memLevel;
    int strategy;

    // Fuzz zlibVersion (no args, safe)
    version_str = zlibVersion();
    (void)version_str;

    // Fuzz zlibCompileFlags (no args, safe)
    compile_flags = zlibCompileFlags();
    (void)compile_flags;

    // Extract parameters from input data
    if (Size >= 1) {
        level = Data[0] % 10; // 0-9
    } else {
        level = 6;
    }

    if (Size >= 2) {
        method = Data[1] % 3; // 0, 1, 2 (DEFLATED is 8, but we use mod for fuzzing)
        if (method != 8) method = 8; // Force DEFLATED if not valid
    } else {
        method = 8;
    }

    if (Size >= 3) {
        windowBits = (Data[2] % 20) - 8; // -8 to 11
    } else {
        windowBits = 15;
    }

    if (Size >= 4) {
        memLevel = (Data[3] % 10) + 1; // 1-9
    } else {
        memLevel = 8;
    }

    if (Size >= 5) {
        strategy = Data[4] % 3; // 0, 1, 2
    } else {
        strategy = 0;
    }

    // Test deflateInit_
    ret = deflateInit_(&strm, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Test deflateEnd
        deflateEnd(&strm);
    }

    // Test deflateInit2_
    ret = deflateInit2_(&strm, level, method, windowBits, memLevel, strategy,
                        ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Test deflateEnd
        deflateEnd(&strm);
    }

    // Test inflateInit_
    ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Test inflateEnd (cleanup for inflate)
        inflateEnd(&strm);
    }

    // Test with invalid version string to trigger Z_VERSION_ERROR
    const char *bad_version = "9.9.9";
    ret = deflateInit_(&strm, level, bad_version, (int)sizeof(z_stream));
    (void)ret;

    ret = deflateInit2_(&strm, level, method, windowBits, memLevel, strategy,
                        bad_version, (int)sizeof(z_stream));
    (void)ret;

    ret = inflateInit_(&strm, bad_version, (int)sizeof(z_stream));
    (void)ret;

    // Test with wrong stream size
    ret = deflateInit_(&strm, level, ZLIB_VERSION, 999);
    (void)ret;

    ret = deflateInit2_(&strm, level, method, windowBits, memLevel, strategy,
                        ZLIB_VERSION, 999);
    (void)ret;

    ret = inflateInit_(&strm, ZLIB_VERSION, 999);
    (void)ret;

    // Test with NULL stream (should return Z_STREAM_ERROR)
    ret = deflateInit_(NULL, level, ZLIB_VERSION, (int)sizeof(z_stream));
    (void)ret;

    ret = deflateInit2_(NULL, level, method, windowBits, memLevel, strategy,
                        ZLIB_VERSION, (int)sizeof(z_stream));
    (void)ret;

    ret = inflateInit_(NULL, ZLIB_VERSION, (int)sizeof(z_stream));
    (void)ret;

    // Test deflateEnd on uninitialized stream
    ret = deflateEnd(&strm);
    (void)ret;

    return 0;
}
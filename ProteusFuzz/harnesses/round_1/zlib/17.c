// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    unsigned char window[32768];
    int ret;
    int windowBits;
    int bits;
    int value;

    if (Size < 3) {
        return 0;
    }

    // Extract parameters from input data
    windowBits = (int)Data[0] & 0xFF;
    bits = (int)Data[1] & 0xFF;
    value = (int)Data[2] & 0xFF;

    // Adjust windowBits to valid range for inflateInit2_ (usually -15 to 15)
    // For inflateBackInit_, valid range is 8-15
    if (windowBits > 15) windowBits = 15;
    if (windowBits < -15) windowBits = -15;

    // Adjust bits for inflatePrime (0-16)
    if (bits > 16) bits = 16;

    // Test 1: inflateInit2_
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Try inflatePrime
        ret = inflatePrime(&strm, bits, value);
        // Cleanup
        inflateEnd(&strm);
    }

    // Test 2: inflateInit_
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Try inflatePrime
        ret = inflatePrime(&strm, bits, value);
        // Cleanup
        inflateEnd(&strm);
    }

    // Test 3: inflateBackInit_
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    // Ensure windowBits is within 8-15 for inflateBackInit_
    int ib_windowBits = ((int)Data[0] % 8) + 8; 
    ret = inflateBackInit_(&strm, ib_windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Cleanup
        ret = inflateBackEnd(&strm);
    }

    // Test 4: zError with various codes
    const char *msg1 = zError(Z_OK);
    const char *msg2 = zError(Z_STREAM_ERROR);
    const char *msg3 = zError(Z_VERSION_ERROR);
    const char *msg4 = zError(-1);
    (void)msg1; (void)msg2; (void)msg3; (void)msg4;

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    z_stream strm = {0};
    unsigned char window[32768];
    int ret;
    int windowBits;
    int bits;
    int value;

    // Initialize basic stream fields
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    strm.avail_out = 0;
    strm.next_out = Z_NULL;

    // Extract parameters from input data if available
    if (Size > 0) {
        windowBits = (int)Data[0];
    } else {
        windowBits = 15;
    }

    if (Size > 1) {
        bits = (int)Data[1];
    } else {
        bits = 8;
    }

    if (Size > 2) {
        value = (int)Data[2];
    } else {
        value = 0;
    }

    // Test 1: inflateInit_
    ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Test inflatePrime
        ret = inflatePrime(&strm, bits, value);
        // Cleanup
        inflateEnd(&strm);
    }

    // Test 2: inflateInit2_ with various windowBits
    // Reset stream
    memset(&strm, 0, sizeof(z_stream));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    // Try raw windowBits from input (might be invalid, testing error paths)
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        inflateEnd(&strm);
    }

    // Try valid windowBits range
    for (int wb = -15; wb <= 15; wb++) {
        memset(&strm, 0, sizeof(z_stream));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        ret = inflateInit2_(&strm, wb, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret == Z_OK) {
            // Try inflatePrime with valid state
            ret = inflatePrime(&strm, bits, value);
            inflateEnd(&strm);
        }
    }

    // Test 3: inflateBackInit_
    // Requires windowBits between 8 and 15
    memset(&strm, 0, sizeof(z_stream));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_out = window;
    strm.avail_out = sizeof(window);

    // Try raw windowBits (might be invalid)
    ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        inflateBackEnd(&strm);
    }

    // Try valid windowBits range for inflateBack
    for (int wb = 8; wb <= 15; wb++) {
        memset(&strm, 0, sizeof(z_stream));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.next_out = window;
        strm.avail_out = sizeof(window);

        ret = inflateBackInit_(&strm, wb, window, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret == Z_OK) {
            inflateBackEnd(&strm);
        }
    }

    // Test 4: zError with various codes
    // Use input data to generate error codes to test
    for (size_t i = 0; i < Size && i < 256; i++) {
        int code = (int)Data[i];
        const char *msg = zError(code);
        (void)msg; // Avoid unused variable warning
    }

    // Test 5: Edge cases with null pointers or invalid sizes
    // inflateInit_ with wrong version
    memset(&strm, 0, sizeof(z_stream));
    ret = inflateInit_(&strm, "1.0.0", (int)sizeof(z_stream));
    (void)ret;

    // inflateInit_ with wrong size
    ret = inflateInit_(&strm, ZLIB_VERSION, 0);
    (void)ret;

    // inflateInit2_ with wrong version
    memset(&strm, 0, sizeof(z_stream));
    ret = inflateInit2_(&strm, 15, "1.0.0", (int)sizeof(z_stream));
    (void)ret;

    // inflateBackInit_ with wrong version
    memset(&strm, 0, sizeof(z_stream));
    ret = inflateBackInit_(&strm, 15, window, "1.0.0", (int)sizeof(z_stream));
    (void)ret;

    // inflateBackEnd with null stream
    ret = inflateBackEnd(NULL);
    (void)ret;

    // inflatePrime with null stream
    ret = inflatePrime(NULL, 8, 0);
    (void)ret;

    return 0;
}
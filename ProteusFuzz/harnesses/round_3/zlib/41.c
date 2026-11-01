// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test zError with various codes derived from input
    int error_code = 0;
    if (Size >= 1) {
        error_code = Data[0];
        const char *msg = zError(error_code);
        (void)msg; // Use the result to avoid unused warnings
    }
    // Test with some known constants
    (void)zError(Z_OK);
    (void)zError(Z_STREAM_ERROR);
    (void)zError(Z_VERSION_ERROR);
    (void)zError(Z_MEM_ERROR);
    (void)zError(Z_BUF_ERROR);
    (void)zError(-1);
    (void)zError(100);

    // Prepare for inflateBack tests
    z_stream strm_back = {0};
    unsigned char window[32768];
    int ret;

    // Test inflateBackInit_
    // Valid windowBits range is 8-15. We fuzz it but expect errors for invalid.
    int windowBits = 15;
    if (Size >= 2) {
        windowBits = Data[1] & 0xFF; // Fuzz windowBits
    }
    
    // Initialize stream manually for safety
    strm_back.zalloc = Z_NULL;
    strm_back.zfree = Z_NULL;
    strm_back.opaque = Z_NULL;
    strm_back.avail_in = 0;
    strm_back.next_in = Z_NULL;
    strm_back.avail_out = 0;
    strm_back.next_out = Z_NULL;

    ret = inflateBackInit_(&strm_back, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
    
    // If init succeeded, we must end it
    if (ret == Z_OK) {
        ret = inflateBackEnd(&strm_back);
        (void)ret;
    }

    // Test inflateInit_
    z_stream strm_init = {0};
    strm_init.zalloc = Z_NULL;
    strm_init.zfree = Z_NULL;
    strm_init.opaque = Z_NULL;
    strm_init.avail_in = 0;
    strm_init.next_in = Z_NULL;
    strm_init.avail_out = 0;
    strm_init.next_out = Z_NULL;

    ret = inflateInit_(&strm_init, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Test inflatePrime if init succeeded
        int bits = 0;
        int value = 0;
        if (Size >= 4) {
            bits = Data[2] & 0xFF;
            value = Data[3] & 0xFF;
        }
        // Fuzz bits to be negative or large to test error handling
        if (Size >= 5) {
            bits = (int)Data[4] - 128; // Range -128 to 127
        }
        ret = inflatePrime(&strm_init, bits, value);
        (void)ret;

        // Cleanup
        ret = inflateEnd(&strm_init);
        (void)ret;
    }

    // Test inflateInit2_
    z_stream strm_init2 = {0};
    strm_init2.zalloc = Z_NULL;
    strm_init2.zfree = Z_NULL;
    strm_init2.opaque = Z_NULL;
    strm_init2.avail_in = 0;
    strm_init2.next_in = Z_NULL;
    strm_init2.avail_out = 0;
    strm_init2.next_out = Z_NULL;

    int windowBits2 = 15;
    if (Size >= 6) {
        windowBits2 = (int)Data[5] & 0xFF;
    }

    ret = inflateInit2_(&strm_init2, windowBits2, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Test inflatePrime again
        int bits2 = 0;
        int value2 = 0;
        if (Size >= 8) {
            bits2 = (int)Data[6] - 128;
            value2 = (int)Data[7] & 0xFF;
        }
        ret = inflatePrime(&strm_init2, bits2, value2);
        (void)ret;

        // Cleanup
        ret = inflateEnd(&strm_init2);
        (void)ret;
    }

    return 0;
}
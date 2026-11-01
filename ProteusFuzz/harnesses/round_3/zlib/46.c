// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// zError at zutil.c:139:22 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void cleanup_stream(z_stream *strm) {
    if (strm != NULL) {
        inflateEnd(strm);
        inflateBackEnd(strm);
        memset(strm, 0, sizeof(z_stream));
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    z_stream strm = {0};
    unsigned char window[32768];
    int ret;
    int i;

    /* Test zError with various error codes derived from input */
    for (i = 0; i < Size && i < 100; i++) {
        int error_code = (int)Data[i];
        const char *msg = zError(error_code);
        (void)msg; /* Suppress unused variable warning */
    }

    /* Test inflateInit_ */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        inflateEnd(&strm);
    }

    /* Test inflateInit2_ with various windowBits */
    for (i = 0; i < Size && i < 30; i++) {
        int windowBits = (int)Data[i] % 32 - 15; /* Range: -15 to 16 */
        strm = (z_stream){0};
        ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret == Z_OK) {
            inflateEnd(&strm);
        }
    }

    /* Test inflateBackInit_ */
    for (i = 0; i < Size && i < 30; i++) {
        int windowBits = (int)Data[i] % 8 + 8; /* Range: 8 to 15 */
        strm = (z_stream){0};
        ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret == Z_OK) {
            inflateBackEnd(&strm);
        }
    }

    /* Test inflatePrime */
    for (i = 0; i < Size && i < 100; i++) {
        int bits = (int)Data[i] % 17; /* 0 to 16 */
        int value = (int)Data[(i + 1) % Size];
        strm = (z_stream){0};
        ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret == Z_OK) {
            ret = inflatePrime(&strm, bits, value);
            (void)ret;
            inflateEnd(&strm);
        }
        /* Test with negative bits to clear buffer */
        strm = (z_stream){0};
        ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret == Z_OK) {
            ret = inflatePrime(&strm, -1, 0);
            (void)ret;
            inflateEnd(&strm);
        }
    }

    /* Test edge cases for inflateBackInit_ with invalid windowBits */
    strm = (z_stream){0};
    ret = inflateBackInit_(&strm, 7, window, ZLIB_VERSION, (int)sizeof(z_stream));
    (void)ret;
    ret = inflateBackInit_(&strm, 16, window, ZLIB_VERSION, (int)sizeof(z_stream));
    (void)ret;

    /* Test with NULL stream pointers */
    ret = inflateBackEnd(NULL);
    (void)ret;

    /* Test with corrupted version string */
    strm = (z_stream){0};
    ret = inflateInit_(&strm, "1.0.0", (int)sizeof(z_stream));
    (void)ret;
    ret = inflateInit2_(&strm, 15, "1.0.0", (int)sizeof(z_stream));
    (void)ret;
    ret = inflateBackInit_(&strm, 15, window, "1.0.0", (int)sizeof(z_stream));
    (void)ret;

    /* Test with wrong stream size */
    strm = (z_stream){0};
    ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream) - 1);
    (void)ret;
    ret = inflateInit2_(&strm, 15, ZLIB_VERSION, (int)sizeof(z_stream) + 1);
    (void)ret;
    ret = inflateBackInit_(&strm, 15, window, ZLIB_VERSION, (int)sizeof(z_stream) - 1);
    (void)ret;

    return 0;
}
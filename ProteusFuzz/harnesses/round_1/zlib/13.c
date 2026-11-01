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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static int fuzz_inflate_init2(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret;
    int windowBits;

    if (size > 0) {
        windowBits = (int)data[0] & 0x1F; // 0 to 31
    } else {
        windowBits = 15;
    }

    // Test with various windowBits values including negative for raw deflate
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Try to inject some bits if initialized
        if (size > 1) {
            int bits = (int)data[1] & 0x0F; // 0 to 15
            int value = (int)data[2 % size];
            inflatePrime(&strm, bits, value);
        }
        inflateEnd(&strm);
    }
    return 0;
}

static int fuzz_inflate_init(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    int ret;

    ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Try to inject bits
        if (size > 1) {
            int bits = (int)data[1] & 0x0F;
            int value = (int)data[2 % size];
            inflatePrime(&strm, bits, value);
        }
        inflateEnd(&strm);
    }
    return 0;
}

static int fuzz_inflate_back_init(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    unsigned char window[32768];
    int ret;
    int windowBits;

    if (size > 0) {
        windowBits = (int)data[0] & 0x0F; // 0 to 15
    } else {
        windowBits = 15;
    }

    ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        inflateBackEnd(&strm);
    }
    return 0;
}

static int fuzz_zerror(const uint8_t *data, size_t size) {
    if (size > 0) {
        int code = (int)data[0];
        const char *msg = zError(code);
        (void)msg; // Use the result to avoid unused variable warning
    }
    return 0;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz inflateInit2_
    fuzz_inflate_init2(Data, Size);

    // Fuzz inflateInit_
    fuzz_inflate_init(Data, Size);

    // Fuzz inflateBackInit_
    fuzz_inflate_back_init(Data, Size);

    // Fuzz zError
    fuzz_zerror(Data, Size);

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// zError at zutil.c:139:22 in zlib.h
// inflateBackInit_ at infback.c:25:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateBackEnd at infback.c:572:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    // Fuzz zError with various error codes derived from input
    int error_code = (int)Data[0] - 128; // Range roughly -128 to 127
    const char *msg = zError(error_code);
    (void)msg; // Suppress unused variable warning

    // Prepare z_stream for inflate functions
    z_stream strm = {0};
    unsigned char window[32768]; // Window buffer for inflateBack
    int ret;

    // Fuzz inflateBackInit_
    // windowBits: 8 to 15 are valid, others test error paths
    int windowBits = (int)(Data[1] % 20) + 4; // 4 to 23
    ret = inflateBackInit_(&strm, windowBits, window, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Perform some dummy inflateBack operations if initialized
        // We don't have a full inflateBack implementation here, so we just end it
        inflateBackEnd(&strm);
    }

    // Fuzz inflateInit_
    memset(&strm, 0, sizeof(strm));
    ret = inflateInit_(&strm, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        inflateEnd(&strm);
    }

    // Fuzz inflateInit2_
    memset(&strm, 0, sizeof(strm));
    // windowBits can be negative for raw deflate, positive for zlib/gzip
    int customWindowBits = (int)(Data[2] % 35) - 15; // -15 to 19
    ret = inflateInit2_(&strm, customWindowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // Fuzz inflatePrime if initialized
        int bits = (int)(Data[3] % 18) - 1; // -1 to 16
        int value = (int)Data[4];
        inflatePrime(&strm, bits, value);
        inflateEnd(&strm);
    }

    // Fuzz inflatePrime on uninitialized stream (should error)
    memset(&strm, 0, sizeof(strm));
    inflatePrime(&strm, 8, 0xFF);

    // Fuzz inflateBackEnd on null stream
    inflateBackEnd(NULL);

    // Fuzz inflateEnd on null stream
    inflateEnd(NULL);

    return 0;
}
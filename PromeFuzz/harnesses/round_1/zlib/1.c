// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    z_stream stream;
    int level;
    int ret;

    /* Initialize stream structure */
    memset(&stream, 0, sizeof(stream));
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    /* Use fuzz data to determine compression level */
    if (Size > 0) {
        level = Data[0] % 11; /* 0-10, including Z_DEFAULT_COMPRESSION */
    } else {
        level = Z_DEFAULT_COMPRESSION;
    }

    /* 1. deflateInit_ */
    ret = deflateInit_(&stream, level, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        /* If initialization fails, clean up and return */
        deflateEnd(&stream);
        return 0;
    }

    /* 2. deflateEnd */
    deflateEnd(&stream);

    /* Reinitialize stream for inflation */
    memset(&stream, 0, sizeof(stream));
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;

    /* 3. inflateInit_ */
    ret = inflateInit_(&stream, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&stream);
        return 0;
    }

    /* 4. deflateReset - This should fail since stream is in inflation state */
    deflateReset(&stream);

    /* 5. inflateReset */
    inflateReset(&stream);

    /* 6. deflateReset - This should fail since stream is still in inflation state */
    deflateReset(&stream);

    /* 7. inflateEnd */
    inflateEnd(&stream);

    /* 8. deflateEnd - This should fail since stream is already ended */
    deflateEnd(&stream);

    return 0;
}
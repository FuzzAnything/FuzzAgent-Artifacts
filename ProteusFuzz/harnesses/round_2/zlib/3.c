// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    z_stream strm_deflate = {0};
    z_stream strm_inflate = {0};
    unsigned char *out_buf = NULL;
    int ret;

    // Allocate output buffer for deflate
    out_buf = (unsigned char *)malloc(Size + 1024);
    if (!out_buf) {
        return 0;
    }

    // 1. deflateInit_
    ret = deflateInit_(&strm_deflate, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Setup input for deflate to ensure valid state before reset
    if (Size > 0) {
        strm_deflate.next_in = (const Bytef *)Data;
        strm_deflate.avail_in = (uInt)Size;
    } else {
        static const uint8_t dummy = 0;
        strm_deflate.next_in = &dummy;
        strm_deflate.avail_in = 1;
    }

    strm_deflate.next_out = out_buf;
    strm_deflate.avail_out = Size + 1024;

    // Perform a single deflate step to ensure state is valid before reset
    (void)deflate(&strm_deflate, Z_NO_FLUSH);

    // 2. deflateEnd
    (void)deflateEnd(&strm_deflate);

    // Re-initialize deflate stream to satisfy the call to deflateReset later
    ret = deflateInit_(&strm_deflate, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // 3. inflateInit_
    ret = inflateInit_(&strm_inflate, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        (void)deflateEnd(&strm_deflate);
        free(out_buf);
        return 0;
    }

    // 4. deflateReset
    (void)deflateReset(&strm_deflate);

    // 5. inflateReset
    (void)inflateReset(&strm_inflate);

    // 6. deflateReset
    (void)deflateReset(&strm_deflate);

    // 7. inflateEnd
    (void)inflateEnd(&strm_inflate);

    // 8. deflateEnd
    (void)deflateEnd(&strm_deflate);

    free(out_buf);
    return 0;
}
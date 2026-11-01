// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    z_stream deflate_stream = {0};
    z_stream inflate_stream = {0};
    Bytef *deflate_buf = NULL;
    Bytef *inflate_buf = NULL;
    int ret;

    // Allocate buffers if data is available
    if (Size > 0) {
        deflate_buf = (Bytef *)malloc(Size);
        inflate_buf = (Bytef *)malloc(Size);
        if (!deflate_buf || !inflate_buf) {
            free(deflate_buf);
            free(inflate_buf);
            return 0;
        }
        memcpy(deflate_buf, Data, Size);
        memcpy(inflate_buf, Data, Size);
    }

    // 1. deflateInit_
    deflate_stream.next_in = deflate_buf;
    deflate_stream.avail_in = Size;
    deflate_stream.next_out = deflate_buf;
    deflate_stream.avail_out = Size;
    deflate_stream.zalloc = Z_NULL;
    deflate_stream.zfree = Z_NULL;
    deflate_stream.opaque = Z_NULL;

    ret = deflateInit_(&deflate_stream, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 2. deflateEnd
    ret = deflateEnd(&deflate_stream);

    // 3. inflateInit_
    inflate_stream.next_in = inflate_buf;
    inflate_stream.avail_in = Size;
    inflate_stream.next_out = inflate_buf;
    inflate_stream.avail_out = Size;
    inflate_stream.zalloc = Z_NULL;
    inflate_stream.zfree = Z_NULL;
    inflate_stream.opaque = Z_NULL;

    ret = inflateInit_(&inflate_stream, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 4. deflateReset
    // Note: deflate_stream was ended in step 2, so this might return Z_STREAM_ERROR.
    ret = deflateReset(&deflate_stream);

    // 5. inflateReset
    ret = inflateReset(&inflate_stream);

    // 6. deflateReset (again)
    ret = deflateReset(&deflate_stream);

    // 7. inflateEnd
    ret = inflateEnd(&inflate_stream);

    // 8. deflateEnd (again)
    ret = deflateEnd(&deflate_stream);

cleanup:
    free(deflate_buf);
    free(inflate_buf);
    return 0;
}
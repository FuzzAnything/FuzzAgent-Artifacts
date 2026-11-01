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
    uint8_t *output_buffer = NULL;
    int ret;

    // Allocate output buffer if data is provided
    if (Size > 0) {
        output_buffer = (uint8_t *)malloc(Size + 1024);
        if (!output_buffer) {
            return 0;
        }
    }

    // 1. deflateInit_
    deflate_stream.zalloc = Z_NULL;
    deflate_stream.zfree = Z_NULL;
    deflate_stream.opaque = Z_NULL;
    deflate_stream.next_in = (Bytef *)Data;
    deflate_stream.avail_in = (uInt)Size;
    deflate_stream.next_out = output_buffer;
    deflate_stream.avail_out = (uInt)(Size + 1024);

    ret = deflateInit_(&deflate_stream, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 2. deflateEnd
    ret = deflateEnd(&deflate_stream);
    // We proceed even if error to test subsequent calls on invalid stream state
    (void)ret;

    // 3. inflateInit_
    inflate_stream.zalloc = Z_NULL;
    inflate_stream.zfree = Z_NULL;
    inflate_stream.opaque = Z_NULL;
    inflate_stream.next_in = (Bytef *)Data;
    inflate_stream.avail_in = (uInt)Size;
    inflate_stream.next_out = output_buffer;
    inflate_stream.avail_out = (uInt)(Size + 1024);

    ret = inflateInit_(&inflate_stream, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 4. deflateReset (on the deflate stream which was ended, might be invalid)
    ret = deflateReset(&deflate_stream);
    (void)ret;

    // 5. inflateReset
    ret = inflateReset(&inflate_stream);
    (void)ret;

    // 6. deflateReset (again)
    ret = deflateReset(&deflate_stream);
    (void)ret;

    // 7. inflateEnd
    ret = inflateEnd(&inflate_stream);
    (void)ret;

    // 8. deflateEnd (again)
    ret = deflateEnd(&deflate_stream);
    (void)ret;

cleanup:
    if (output_buffer) {
        free(output_buffer);
    }
    return 0;
}
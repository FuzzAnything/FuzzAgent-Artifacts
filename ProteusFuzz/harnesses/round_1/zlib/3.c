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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    z_stream deflate_stream = {0};
    z_stream inflate_stream = {0};
    int ret;

    // Allocate output buffers to avoid NULL pointer issues during init/reset
    uint8_t *out_buf = malloc(1024);
    if (!out_buf) return 0;

    // 1. deflateInit_
    // Initialize compression stream
    ret = deflateInit_(&deflate_stream, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Setup input/output for potential compression (though we focus on init/reset/end)
    deflate_stream.next_in = (Bytef*)Data;
    deflate_stream.avail_in = Size;
    deflate_stream.next_out = out_buf;
    deflate_stream.avail_out = 1024;

    // 2. deflateEnd
    // Clean up the deflate stream immediately as per sequence
    ret = deflateEnd(&deflate_stream);
    // We don't return on error here to allow testing subsequent steps with potentially invalid states

    // 3. inflateInit_
    // Initialize decompression stream
    ret = inflateInit_(&inflate_stream, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }
    
    inflate_stream.next_in = (Bytef*)Data;
    inflate_stream.avail_in = Size;
    inflate_stream.next_out = out_buf;
    inflate_stream.avail_out = 1024;

    // 4. deflateReset
    // Note: The task asks to call deflateReset on the deflate_stream.
    // Since we called deflateEnd on it in step 2, the state is likely invalid.
    // This is intentional to test error handling paths (Z_STREAM_ERROR).
    ret = deflateReset(&deflate_stream);
    (void)ret; // Ignore return for fuzzing exploration

    // 5. inflateReset
    // Reset the inflate stream
    ret = inflateReset(&inflate_stream);
    (void)ret;

    // 6. deflateReset
    // Call deflateReset again. The stream is still likely invalid from step 2.
    ret = deflateReset(&deflate_stream);
    (void)ret;

    // 7. inflateEnd
    // Clean up the inflate stream
    ret = inflateEnd(&inflate_stream);
    (void)ret;

    // 8. deflateEnd
    // Final cleanup for deflate stream (idempotent or error handling)
    ret = deflateEnd(&deflate_stream);
    (void)ret;

    free(out_buf);
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    z_stream deflate_stream = {0};
    z_stream inflate_stream = {0};
    int ret;

    // Prepare buffers for deflate
    uint8_t *deflate_out = NULL;
    uint8_t *deflate_in = NULL;
    uint8_t *inflate_in = NULL;
    uint8_t *inflate_out = NULL;

    if (Size > 0) {
        deflate_in = (uint8_t *)malloc(Size);
        if (!deflate_in) return 0;
        memcpy(deflate_in, Data, Size);
    }

    // Allocate output buffers
    size_t deflate_out_size = Size > 0 ? Size + Size / 10 + 12 + 5 + 5 + 256 : 256;
    deflate_out = (uint8_t *)malloc(deflate_out_size);
    inflate_in = (uint8_t *)malloc(deflate_out_size);
    inflate_out = (uint8_t *)malloc(Size > 0 ? Size + 256 : 256);

    if (!deflate_out || !inflate_in || !inflate_out) {
        free(deflate_in);
        free(deflate_out);
        free(inflate_in);
        free(inflate_out);
        return 0;
    }

    // 1. deflateInit_
    ret = deflateInit_(&deflate_stream, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(deflate_in);
        free(deflate_out);
        free(inflate_in);
        free(inflate_out);
        return 0;
    }

    // 2. deflateEnd
    ret = deflateEnd(&deflate_stream);
    // We expect Z_OK, but if it fails (e.g., invalid state), we continue to test other paths
    // However, for the sequence to make sense, we usually want success, but fuzzing explores errors too.
    // If deflateEnd failed because stream was invalid, we might not be able to proceed with reset logic properly.
    // But the task asks to invoke in order. Let's re-init if needed or just proceed.
    // To make the sequence valid, we should re-init if the first end was premature or failed.
    // However, the prompt says "invoke in order". If deflateEnd frees the state, subsequent calls on the same stream
    // might be invalid. Let's re-init to ensure valid state for the next steps if the first end was successful.
    // Actually, the prompt implies a specific sequence. If deflateEnd is called immediately after init, 
    // the stream becomes invalid. Then calling deflateReset on it would be an error case.
    // Let's follow the sequence strictly.
    
    // Re-init to allow deflateReset to work if the first deflateEnd succeeded and cleared the stream.
    // If the first deflateEnd failed, the stream might still be in a state where reset is attempted.
    // To maximize coverage, let's try to re-init if the first end was successful, 
    // OR just proceed to the next step which might be an error path.
    // Given the strict order, we will just proceed. But to make deflateReset meaningful, 
    // we need a valid stream. So we re-init here.
    if (ret == Z_OK) {
        ret = deflateInit_(&deflate_stream, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret != Z_OK) {
             // If re-init fails, we can't do much more with deflate stream
        }
    }

    // 3. inflateInit_
    ret = inflateInit_(&inflate_stream, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(deflate_in);
        free(deflate_out);
        free(inflate_in);
        free(inflate_out);
        return 0;
    }

    // 4. deflateReset
    // This requires a valid deflate stream. If we re-init above, it's valid.
    ret = deflateReset(&deflate_stream);

    // 5. inflateReset
    ret = inflateReset(&inflate_stream);

    // 6. deflateReset (again)
    ret = deflateReset(&deflate_stream);

    // 7. inflateEnd
    ret = inflateEnd(&inflate_stream);

    // 8. deflateEnd (again)
    ret = deflateEnd(&deflate_stream);

    // Cleanup
    free(deflate_in);
    free(deflate_out);
    free(inflate_in);
    free(inflate_out);

    return 0;
}
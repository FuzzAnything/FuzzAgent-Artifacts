// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    z_stream inflate_stream = {0};
    z_stream deflate_stream = {0};
    uint8_t *input_buf = NULL;
    uint8_t *output_buf = NULL;
    uLong crc;
    int ret;
    int window_bits;
    int bits;
    int value;

    if (Size < 16) {
        return 0;
    }

    input_buf = (uint8_t *)malloc(Size);
    output_buf = (uint8_t *)malloc(Size * 2 + 256);
    if (!input_buf || !output_buf) {
        free(input_buf);
        free(output_buf);
        return 0;
    }
    memcpy(input_buf, Data, Size);

    // Extract parameters from input to diversify states
    window_bits = (int)Data[0] % 16 + 8; // 8 to 23 range roughly
    bits = (int)Data[1] % 17; // 0 to 16
    value = (int)Data[2] % 65536;

    // 1. inflateInit2_
    ret = inflateInit2_(&inflate_stream, window_bits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 2. crc32 (initial)
    crc = crc32(0, input_buf, Size);

    // 3. inflate
    inflate_stream.next_in = input_buf;
    inflate_stream.avail_in = Size;
    inflate_stream.next_out = output_buf;
    inflate_stream.avail_out = Size * 2 + 256;

    ret = inflate(&inflate_stream, Z_NO_FLUSH);
    // Try different flush modes if space permits
    if (ret == Z_OK && Size > 20) {
        inflate_stream.next_out = output_buf + inflate_stream.total_out;
        inflate_stream.avail_out = Size * 2 + 256 - inflate_stream.total_out;
        ret = inflate(&inflate_stream, Z_SYNC_FLUSH);
    }
    // Final flush
    if (ret == Z_OK && Size > 20) {
        inflate_stream.next_out = output_buf + inflate_stream.total_out;
        inflate_stream.avail_out = Size * 2 + 256 - inflate_stream.total_out;
        ret = inflate(&inflate_stream, Z_FINISH);
    }

    // 4. crc32 (post-inflate)
    if (inflate_stream.total_out > 0) {
        crc = crc32(crc, output_buf, (uInt)inflate_stream.total_out);
    }

    // 5. inflateEnd
    inflateEnd(&inflate_stream);

    // 6. deflateInit2_
    ret = deflateInit2_(&deflate_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        window_bits, 8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 7. deflateSetDictionary
    // Use a portion of the input as dictionary
    uInt dict_len = Size > 100 ? 100 : Size;
    const Bytef *dict = (const Bytef *)input_buf;
    ret = deflateSetDictionary(&deflate_stream, dict, dict_len);

    // 8. deflatePrime
    ret = deflatePrime(&deflate_stream, bits, value);

    // Cleanup deflate stream to prevent memory leaks
    deflateEnd(&deflate_stream);

cleanup:
    free(input_buf);
    free(output_buf);
    return 0;
}
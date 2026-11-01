// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    z_stream inflate_strm = {0};
    z_stream deflate_strm = {0};
    uint8_t *in_buf = NULL;
    uint8_t *out_buf = NULL;
    uint8_t *dict_buf = NULL;
    uInt dict_len;
    int bits, value;
    int ret;

    // Parse input for parameters
    bits = Data[0] & 0x0F; // 0-15, safe for deflatePrime
    value = (Data[1] << 8) | Data[2];
    dict_len = Data[3] & 0xFF;

    // Allocate buffers
    size_t in_size = Size - 4;
    if (in_size > 0) {
        in_buf = (uint8_t *)malloc(in_size);
        if (!in_buf) return 0;
        memcpy(in_buf, Data + 4, in_size);
    }

    size_t out_size = Size > 1024 ? Size : 1024;
    out_buf = (uint8_t *)malloc(out_size);
    if (!out_buf) {
        free(in_buf);
        return 0;
    }

    if (dict_len > 0 && in_size > dict_len) {
        dict_buf = (uint8_t *)malloc(dict_len);
        if (!dict_buf) {
            free(in_buf);
            free(out_buf);
            return 0;
        }
        memcpy(dict_buf, in_buf, dict_len);
    }

    // 1. inflateInit2_
    ret = inflateInit2_(&inflate_strm, -15, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 2. crc32 (before inflate)
    uLong crc = crc32(0L, NULL, 0);
    if (in_buf && in_size > 0) {
        crc = crc32(crc, in_buf, (uInt)in_size);
    }

    // 3. inflate
    inflate_strm.next_in = in_buf;
    inflate_strm.avail_in = (uInt)in_size;
    inflate_strm.next_out = out_buf;
    inflate_strm.avail_out = (uInt)out_size;

    ret = inflate(&inflate_strm, Z_NO_FLUSH);
    // Ignore return value for fuzzing exploration, just ensure state is valid

    // 4. crc32 (after inflate)
    if (inflate_strm.total_out > 0) {
        crc = crc32(crc, out_buf, (uInt)inflate_strm.total_out);
    }

    // 5. inflateEnd
    ret = inflateEnd(&inflate_strm);
    if (ret != Z_OK) {
        // Stream might be invalid, continue to deflate tests
    }

    // 6. deflateInit2_
    ret = deflateInit2_(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        -15, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 7. deflateSetDictionary
    if (dict_buf && dict_len > 0) {
        ret = deflateSetDictionary(&deflate_strm, dict_buf, dict_len);
        // Ignore return, just explore state
    }

    // 8. deflatePrime
    // Ensure bits is within 0-16 range as per spec
    if (bits > 16) bits = 16;
    ret = deflatePrime(&deflate_strm, bits, value);
    // Ignore return, just explore state

    // Cleanup deflate stream
    deflateEnd(&deflate_strm);

cleanup:
    free(in_buf);
    free(out_buf);
    free(dict_buf);
    return 0;
}
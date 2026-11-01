// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 7) {
        return 0;
    }

    z_stream inflate_strm = {0};
    z_stream deflate_strm = {0};

    Bytef *in_buf = NULL;
    Bytef *out_buf = NULL;
    Bytef *dict_buf = NULL;

    // Allocate buffers for fuzzing
    size_t in_size = Size;
    size_t out_size = Size * 2 + 256;
    size_t dict_size = Size > 1024 ? 1024 : Size;

    in_buf = (Bytef *)malloc(in_size);
    out_buf = (Bytef *)malloc(out_size);
    dict_buf = (Bytef *)malloc(dict_size);

    if (!in_buf || !out_buf || !dict_buf) {
        free(in_buf);
        free(out_buf);
        free(dict_buf);
        return 0;
    }

    memcpy(in_buf, Data, in_size);
    memcpy(dict_buf, Data, dict_size);

    // 1. inflateInit2_
    // Use windowBits from input to explore different states (e.g., raw, zlib, gzip)
    int windowBits = (Data[0] % 16) - 8; // Range: -8 to 7
    if (inflateInit2_(&inflate_strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream)) != Z_OK) {
        // Initialization failed, try to proceed or skip
        goto cleanup;
    }

    // 2. crc32 (before inflate)
    uLong crc = crc32(0L, NULL, 0);
    crc = crc32(crc, in_buf, in_size > 0 ? in_size : 1);

    // Setup inflate stream
    inflate_strm.next_in = in_buf;
    inflate_strm.avail_in = in_size;
    inflate_strm.next_out = out_buf;
    inflate_strm.avail_out = out_size;

    // 3. inflate
    int ret = inflate(&inflate_strm, Z_NO_FLUSH);
    // Ignore return value for fuzzing stability, just ensure we don't crash
    (void)ret;

    // 4. crc32 (after inflate)
    crc = crc32(crc, out_buf, inflate_strm.total_out);

    // 5. inflateEnd
    inflateEnd(&inflate_strm);

    // 6. deflateInit2_
    int level = Data[1] % 10; // 0-9
    int method = Data[1] % 2; // 0 or 1 (DEFLATED or STORED)
    int winBits = (Data[2] % 16) + 8; // 8-23
    int memLevel = (Data[3] % 9) + 1; // 1-9
    int strategy = Data[4] % 3; // 0-2

    if (deflateInit2_(&deflate_strm, level, method, winBits, memLevel, strategy, ZLIB_VERSION, (int)sizeof(z_stream)) != Z_OK) {
        goto cleanup;
    }

    // 7. deflateSetDictionary
    // Only set dictionary if we have enough data and it's not empty
    if (dict_size > 0) {
        deflateSetDictionary(&deflate_strm, dict_buf, (uInt)dict_size);
    }

    // 8. deflatePrime
    // Insert raw bits to explore bit buffer states
    int bits = Data[5] % 17; // 0-16
    int value = Data[6] % 65536; // 0-65535

    deflate_strm.next_out = out_buf;
    deflate_strm.avail_out = out_size;

    ret = deflatePrime(&deflate_strm, bits, value);
    (void)ret;

cleanup:
    free(in_buf);
    free(out_buf);
    free(dict_buf);
    return 0;
}
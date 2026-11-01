// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
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
#include "zlib.h"

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    z_stream inflate_strm = {0};
    z_stream deflate_strm = {0};
    uint8_t *in_buf = NULL;
    uint8_t *out_buf = NULL;
    uint8_t *dict_buf = NULL;
    int ret;

    // Allocate buffers
    size_t in_size = Size;
    size_t out_size = Size * 2 + 256;
    size_t dict_size = Size > 100 ? 100 : Size;

    in_buf = (uint8_t *)malloc(in_size);
    out_buf = (uint8_t *)malloc(out_size);
    dict_buf = (uint8_t *)malloc(dict_size);

    if (!in_buf || !out_buf || !dict_buf) {
        free(in_buf);
        free(out_buf);
        free(dict_buf);
        return 0;
    }

    memcpy(in_buf, Data, in_size);
    // Initialize dictionary with some data from input
    memcpy(dict_buf, Data, dict_size);

    // 1. inflateInit2_
    // Use windowBits = 15 (default) or -15 (raw) based on input byte to explore states
    int windowBits = (Data[0] % 32) - 15; // Range: -15 to 16
    ret = inflateInit2_(&inflate_strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        // If init fails, try to proceed with cleanup logic if needed, but usually we skip
        // However, for fuzzing, we might want to try anyway or just return
        // Let's proceed to cleanup and return if init fails to avoid undefined behavior
        goto cleanup_inflate;
    }

    // 2. crc32 (before inflate)
    uLong crc = crc32(0L, NULL, 0);
    crc = crc32(crc, in_buf, in_size);

    // 3. inflate
    inflate_strm.next_in = in_buf;
    inflate_strm.avail_in = in_size;
    inflate_strm.next_out = out_buf;
    inflate_strm.avail_out = out_size;

    ret = inflate(&inflate_strm, Z_NO_FLUSH);
    // Try another call with different flush if possible
    if (ret == Z_OK && inflate_strm.avail_in > 0) {
        ret = inflate(&inflate_strm, Z_SYNC_FLUSH);
    }

    // 4. crc32 (after inflate)
    // Compute CRC on the output data if any was produced
    if (inflate_strm.total_out > 0) {
        crc = crc32(crc, out_buf, (uInt)inflate_strm.total_out);
    }

    // 5. inflateEnd
cleanup_inflate:
    ret = inflateEnd(&inflate_strm);

    // 6. deflateInit2_
    // Use different level and strategy based on input
    int level = (Data[1] % 10);
    int method = Z_DEFLATED;
    int memLevel = (Data[1] % 9) + 1;
    int strategy = (Data[1] % 3); // Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY

    ret = deflateInit2_(&deflate_strm, level, method, windowBits, memLevel, strategy,
                        ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup_deflate;
    }

    // 7. deflateSetDictionary
    ret = deflateSetDictionary(&deflate_strm, dict_buf, (uInt)dict_size);
    // Ignore return value for fuzzing to explore error paths

    // 8. deflatePrime
    // Insert some bits based on input
    int bits = (Data[1] % 17); // 0 to 16
    int value = (int)(Data[1] << 8) | (Data[0] & 0xFF);
    ret = deflatePrime(&deflate_strm, bits, value);

cleanup_deflate:
    // Cleanup deflate stream
    deflateEnd(&deflate_strm);

    free(in_buf);
    free(out_buf);
    free(dict_buf);

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret;

    // Allocate output buffer
    out_buf = (uint8_t *)malloc(Size * 2 + 65536);
    if (!out_buf) {
        return 0;
    }

    // Initialize stream
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(Size * 2 + 65536);
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    // Step 1: inflateInit2_
    // Use -15 for raw deflate (no zlib header) to handle arbitrary binary input
    ret = inflateInit2_(&strm, -15, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Step 2: inflate
    // Try to decompress the input
    ret = inflate(&strm, Z_NO_FLUSH);
    (void)ret; // Ignore return value for fuzzing purposes, just trigger code paths

    // Step 3: crc32_combine
    // Combine two CRCs using values derived from the stream or input
    uLong crc1 = strm.adler;
    uLong crc2 = (uLong)Size;
    z_off_t len2 = (z_off_t)Size;
    uLong combined_crc = crc32_combine(crc1, crc2, len2);
    (void)combined_crc;

    // Step 4: inflateReset
    // Reset the stream to prepare for potential reuse (though we end immediately after)
    ret = inflateReset(&strm);
    if (ret != Z_OK) {
        // If reset fails, we still proceed to cleanup
    }

    // Step 5: inflateEnd
    ret = inflateEnd(&strm);
    (void)ret;

    free(out_buf);
    return 0;
}
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
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret;

    // Allocate output buffer
    if (Size > 0) {
        out_buf = (uint8_t *)malloc(Size * 2 + 1024);
        if (out_buf == NULL) {
            return 0;
        }
    } else {
        out_buf = (uint8_t *)malloc(1024);
        if (out_buf == NULL) {
            return 0;
        }
    }

    // 1. inflateInit2_
    // Use -15 to indicate raw deflate (no zlib header), or 15 for zlib header.
    // We try both based on input size parity to explore different states.
    int window_bits = (Size % 2 == 0) ? -15 : 15;
    ret = inflateInit2_(&strm, window_bits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Setup input
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.total_in = 0;

    // Setup output
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(Size * 2 + 1024);
    strm.total_out = 0;

    // 2. inflate
    // Loop to process input until stream ends or error occurs
    do {
        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break;
        }
        if (ret != Z_OK && ret != Z_BUF_ERROR) {
            // Error occurred, but we continue to cleanup
            break;
        }
    } while (strm.avail_in > 0 || strm.avail_out > 0);

    // 3. crc32_combine
    // We use the adler from the stream (or 0) and combine it with a dummy CRC.
    // Note: inflate uses Adler-32, crc32_combine expects CRC-32, but we call it
    // to exercise the code path regardless of semantic correctness of the values.
    uLong crc1 = strm.adler;
    uLong crc2 = 0xFFFFFFFF;
    z_off_t len2 = (z_off_t)strm.total_out;
    uLong combined_crc = crc32_combine(crc1, crc2, len2);
    (void)combined_crc; // Suppress unused variable warning

    // 4. inflateReset
    // Reset the stream to prepare for a hypothetical new stream
    ret = inflateReset(&strm);
    if (ret != Z_OK) {
        // If reset fails, we still proceed to end
    }

    // 5. inflateEnd
    // Cleanup resources
    ret = inflateEnd(&strm);
    (void)ret;

    free(out_buf);
    return 0;
}
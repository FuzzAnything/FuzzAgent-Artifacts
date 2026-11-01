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

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret;
    const char *version = ZLIB_VERSION;
    int stream_size = sizeof(z_stream);

    // Allocate output buffer
    out_buf = (uint8_t *)malloc(Size * 10 + 1024);
    if (!out_buf) {
        return 0;
    }

    // 1. inflateInit2_
    // Vary windowBits slightly based on input to explore states, but keep valid range
    int wb = -15; 
    if (Size > 1) {
        int mod = Data[0] % 5;
        if (mod == 0) wb = -15; // zlib
        else if (mod == 1) wb = 15; // raw deflate
        else if (mod == 2) wb = -15; // zlib
        else if (mod == 3) wb = 15; // raw deflate
        else wb = 15; // raw deflate
    }
    
    ret = inflateInit2_(&strm, wb, version, stream_size);
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Setup input
    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(Size * 10 + 1024);

    // 2. inflate
    // Run inflate loop until no more data or error
    while (strm.avail_in > 0) {
        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break;
        }
        if (ret == Z_OK) {
            // Continue processing
            if (strm.avail_out == 0) {
                // If buffer full, reset output pointer to simulate chunked output
                // In a real app we'd process data, here we just reset to keep fuzzing
                strm.next_out = out_buf;
                strm.avail_out = (uInt)(Size * 10 + 1024);
            }
        } else {
            // Error or other status (Z_DATA_ERROR, Z_NEED_DICT, etc.)
            // We stop processing this specific stream to avoid undefined behavior
            break;
        }
    }

    // 3. crc32_combine
    // Use the adler value from the stream (which acts as a checksum)
    // Combine it with a dummy CRC based on input size
    uLong crc1 = strm.adler;
    uLong crc2 = (uLong)(Size % 0xFFFFFFFF);
    z_off_t len2 = (z_off_t)(Size % 1000);
    
    uLong combined_crc = crc32_combine(crc1, crc2, len2);
    (void)combined_crc; // Suppress unused variable warning

    // 4. inflateReset
    // Reset the stream to test the reset logic
    ret = inflateReset(&strm);
    if (ret != Z_OK) {
        // If reset fails, we still proceed to cleanup
    }

    // 5. inflateEnd
    // Cleanup resources
    inflateEnd(&strm);

    free(out_buf);
    return 0;
}
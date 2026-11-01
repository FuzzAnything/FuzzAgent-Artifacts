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
    if (Size < 4) {
        return 0;
    }

    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret;

    // Allocate output buffer
    out_buf = (uint8_t *)malloc(Size * 2 + 1024);
    if (out_buf == NULL) {
        return 0;
    }

    // Step 1: inflateInit2_
    // Use a negative windowBits to skip header check (raw deflate) or positive for zlib
    // Fuzzing different window sizes can help explore states
    int windowBits = (Data[0] % 16) - 8; // Range: -8 to 7
    // Ensure we pass valid version and stream_size
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
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

    // Step 2: inflate
    // Try different flush modes to explore states
    int flush_mode = (Data[1] % 5) + Z_NO_FLUSH; 
    if (flush_mode > Z_BLOCK) flush_mode = Z_BLOCK;
    
    ret = inflate(&strm, flush_mode);
    
    // Step 3: crc32_combine
    // Combine the current Adler/CRC with a dummy CRC based on input
    uLong crc1 = strm.adler;
    uLong crc2 = (uLong)(Data[2] * 256 + Data[3]);
    z_off_t len2 = (z_off_t)(Size % 1000);
    uLong combined_crc = crc32_combine(crc1, crc2, len2);
    (void)combined_crc; // Suppress unused variable warning

    // Step 4: inflateReset
    // Reset the stream to prepare for potential reuse or just to test the reset logic
    ret = inflateReset(&strm);
    if (ret != Z_OK) {
        // Even if reset fails, we proceed to cleanup
    }

    // Step 5: inflateEnd
    // Cleanup resources
    ret = inflateEnd(&strm);
    (void)ret;

    free(out_buf);
    return 0;
}
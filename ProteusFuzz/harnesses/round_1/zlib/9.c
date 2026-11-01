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
    z_stream strm = {0};
    uint8_t *out_buf = NULL;
    int ret;
    uLong crc1, crc2;
    z_off_t len2;

    // Allocate output buffer
    size_t out_size = (Size > 0) ? (Size * 2 + 1024) : 1024;
    out_buf = (uint8_t *)malloc(out_size);
    if (out_buf == NULL) {
        return 0;
    }

    // Initialize stream
    strm.next_in = NULL;
    strm.avail_in = 0;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_size;
    strm.total_in = 0;
    strm.total_out = 0;
    strm.msg = NULL;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    // 1. inflateInit2_
    int windowBits = 15;
    if (Size > 0) {
        // Vary windowBits based on input data to explore different states
        // Valid ranges: -15 to 15 (zlib/raw), 31 to 47 (gzip)
        int val = Data[0] % 65; 
        windowBits = val - 15;
        if (windowBits > 15) {
            windowBits += 32; // Shift to gzip range if needed
        }
    }

    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buf);
        return 0;
    }

    // Set input data
    if (Size > 0) {
        strm.next_in = (const Bytef *)Data;
        strm.avail_in = (uInt)Size;
    }

    // 2. inflate
    ret = inflate(&strm, Z_NO_FLUSH);
    // We don't strictly need to loop here for a simple fuzz driver, 
    // but the state is explored by the single call.

    // 3. crc32_combine
    crc1 = 0;
    crc2 = (uLong)strm.adler; 
    len2 = (z_off_t)strm.total_out;
    
    uLong combined_crc = crc32_combine(crc1, crc2, len2);
    (void)combined_crc;

    // 4. inflateReset
    ret = inflateReset(&strm);
    (void)ret;

    // 5. inflateEnd
    ret = inflateEnd(&strm);
    (void)ret;

    free(out_buf);
    return 0;
}
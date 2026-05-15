// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "zlib.h"

static void initialize_z_stream(z_streamp strm, const uint8_t *data, size_t size) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = (Bytef *)data;
    strm->avail_in = (uInt)size;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
    strm->total_in = 0;
    strm->total_out = 0;
    strm->msg = Z_NULL;
    strm->data_type = 0;
    strm->adler = 0;
    strm->reserved = 0;
    strm->state = Z_NULL;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    uLong combined_crc;
    static Bytef out_buffer[4096];
    
    // Initialize z_stream
    initialize_z_stream(&strm, Data, Size);
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    // 1. Call inflateInit2_
    ret = inflateInit2_(&strm, 15, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        // If initialization fails, we can still try other functions with default values
        // but we need to clean up any partial initialization
        if (strm.state != Z_NULL) {
            inflateEnd(&strm);
        }
        // Continue to crc32_combine with default values
        goto crc_combine;
    }
    
    // 2. Call inflate
    // Try different flush modes to explore more states
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH};
    for (int i = 0; i < 4 && ret >= 0 && strm.avail_in > 0; i++) {
        ret = inflate(&strm, flush_modes[i]);
        if (ret == Z_STREAM_END || ret == Z_BUF_ERROR || ret < 0) {
            break;
        }
        // Reset output buffer if full
        if (strm.avail_out == 0) {
            strm.next_out = out_buffer;
            strm.avail_out = sizeof(out_buffer);
        }
    }
    
crc_combine:
    // 3. Call crc32_combine
    // Use fuzzer data to generate CRC values and length
    uLong crc1 = 0, crc2 = 0;
    z_off_t len2 = 0;
    
    if (Size >= sizeof(uLong) * 2) {
        memcpy(&crc1, Data, sizeof(uLong));
        memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
        // Use remaining size for len2, ensure non-negative
        len2 = (z_off_t)(Size - sizeof(uLong) * 2);
        if (len2 < 0) len2 = 0;
    } else if (Size >= sizeof(uLong)) {
        memcpy(&crc1, Data, sizeof(uLong));
        len2 = (z_off_t)(Size - sizeof(uLong));
        if (len2 < 0) len2 = 0;
    } else {
        len2 = (z_off_t)Size;
    }
    
    combined_crc = crc32_combine(crc1, crc2, len2);
    
    // 4. Call inflateReset (only if initialization succeeded)
    if (strm.state != Z_NULL) {
        ret = inflateReset(&strm);
    }
    
    // 5. Call inflateEnd (only if initialization succeeded)
    if (strm.state != Z_NULL) {
        ret = inflateEnd(&strm);
    }
    
    return 0;
}
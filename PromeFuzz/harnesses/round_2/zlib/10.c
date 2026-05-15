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
#include "zlib.h"

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    // Initialize z_stream structure
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Set input data for decompression
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)Size;
    
    // Prepare output buffer
    Bytef out_buffer[4096];
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    // Call inflateInit2_ with windowBits from input or default
    int windowBits = 15; // Default windowBits for zlib
    if (Size > 0) {
        windowBits = (Data[0] % 32) - 16; // Range: -16 to 15
    }
    int init_result = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    
    // Only proceed if initialization succeeded
    if (init_result == Z_OK) {
        // Call inflate with flush parameter from input or default
        int flush = Z_NO_FLUSH;
        if (Size > 1) {
            int flush_options[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK};
            flush = flush_options[Data[1] % 5];
        }
        int inflate_result = inflate(&strm, flush);
        
        // Call crc32_combine with values derived from input
        uLong crc1 = 0;
        uLong crc2 = 0;
        z_off_t len2 = 0;
        
        if (Size > 9) {
            crc1 = *(uLong*)(Data);
            crc2 = *(uLong*)(Data + 4);
            len2 = (z_off_t)(Data[8] | (Data[9] << 8));
            if (len2 < 0) len2 = -len2;
        }
        uLong combined_crc = crc32_combine(crc1, crc2, len2);
        
        // Call inflateReset
        int reset_result = inflateReset(&strm);
        
        // Call inflateEnd
        int end_result = inflateEnd(&strm);
    }
    
    return 0;
}
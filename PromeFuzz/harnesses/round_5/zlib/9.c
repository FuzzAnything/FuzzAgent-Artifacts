// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Initialize z_stream structure
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    // Set up input buffer
    strm.next_in = (Bytef *)Data;
    strm.avail_in = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
    
    // Allocate output buffer
    size_t out_size = Size * 2 + 1024;  // Reasonable output buffer size
    if (out_size == 0) out_size = 1024;
    Bytef *out_buffer = (Bytef *)malloc(out_size);
    if (!out_buffer) return 0;
    
    strm.next_out = out_buffer;
    strm.avail_out = (uInt)(out_size > UINT_MAX ? UINT_MAX : out_size);
    
    // Set default allocators
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // 1. Call inflateInit2_
    int windowBits = 15;  // Default window size
    int ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(out_buffer);
        return 0;
    }
    
    // 2. Call inflate
    // Try different flush modes to explore more states
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK};
    int flush_idx = Size % 5;
    int flush = flush_modes[flush_idx];
    
    ret = inflate(&strm, flush);
    // We don't check ret value as we want to continue fuzzing regardless
    
    // 3. Call crc32_combine
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Data, (uInt)(Size > UINT_MAX ? UINT_MAX : Size));
    z_off_t len2 = (z_off_t)(Size % 1000);  // Non-negative length
    uLong combined_crc = crc32_combine(crc1, crc2, len2);
    (void)combined_crc;  // Avoid unused variable warning
    
    // 4. Call inflateReset
    ret = inflateReset(&strm);
    // We don't check ret value as we want to continue fuzzing regardless
    
    // 5. Call inflateEnd
    ret = inflateEnd(&strm);
    // We don't check ret value as we want to continue fuzzing regardless
    
    // Cleanup
    free(out_buffer);
    
    return 0;
}
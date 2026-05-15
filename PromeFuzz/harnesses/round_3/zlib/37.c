// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void init_zstream(z_streamp strm, const uint8_t *data, size_t size) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = (Bytef*)data;
    strm->avail_in = size;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
    strm->msg = Z_NULL;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    z_stream strm1, strm2;
    int ret;
    
    // Initialize first stream with inflateInit2_
    init_zstream(&strm1, Data, Size);
    int windowBits = (Data[0] % 31) - 15;  // Range: -15 to 15
    ret = inflateInit2_(&strm1, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Try inflatePrime with bits derived from input
    int bits = Data[1] % 17;  // Bits must be ≤16
    int value = Data[2];
    ret = inflatePrime(&strm1, bits, value);
    // Negative bits to clear buffer
    if (Size > 10) {
        inflatePrime(&strm1, -((int)Data[3] % 17), 0);
    }
    
    // Prepare output buffer for inflate
    unsigned char out_buf[1024];
    strm1.next_out = out_buf;
    strm1.avail_out = sizeof(out_buf);
    
    // Try inflate with different flush modes
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH};
    int flush_idx = Data[3] % 4;
    
    ret = inflate(&strm1, flush_modes[flush_idx]);
    if (ret == Z_BLOCK) {
        // After Z_BLOCK, we can call inflatePrime again
        inflatePrime(&strm1, Data[4] % 9, Data[5]);
    }
    
    // Try inflateUndermine
    int undermine_val = Data[6] % 2;
    inflateUndermine(&strm1, undermine_val);
    
    // Get inflate codes used
    unsigned long codes_used = inflateCodesUsed(&strm1);
    (void)codes_used;  // Use result to avoid unused warning
    
    // Try inflateCopy to duplicate stream
    init_zstream(&strm2, Data, Size);
    strm2.zalloc = Z_NULL;
    strm2.zfree = Z_NULL;
    strm2.opaque = Z_NULL;
    
    ret = inflateCopy(&strm2, &strm1);
    if (ret == Z_OK) {
        // Use the copied stream
        unsigned char out_buf2[512];
        strm2.next_out = out_buf2;
        strm2.avail_out = sizeof(out_buf2);
        
        inflate(&strm2, Z_NO_FLUSH);
        inflateEnd(&strm2);
    }
    
    // Clean up first stream
    inflateEnd(&strm1);
    
    return 0;
}
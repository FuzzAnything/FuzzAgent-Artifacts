// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    z_stream source, dest;
    int ret;
    
    // Initialize source stream
    init_stream(&source);
    
    // Use first byte for windowBits parameter (-15..-8 for raw, 8..15 for zlib, plus other options)
    int windowBits = (Data[0] % 31) - 15;  // Range -15 to 15
    
    // Initialize with inflateInit2_
    ret = inflateInit2_(&source, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Set up input data for source stream
    if (Size > 1) {
        source.next_in = (Bytef*)(Data + 1);
        source.avail_in = (uInt)(Size - 1);
        if (source.avail_in > Size - 1) source.avail_in = (uInt)(Size - 1);
    }
    
    // Allocate output buffer
    #define OUT_SIZE 1024
    Bytef out[OUT_SIZE];
    source.next_out = out;
    source.avail_out = OUT_SIZE;
    
    // Try inflatePrime with bits from input
    if (Size > 3) {
        int bits = Data[1] % 17;  // 0-16 bits
        int value = Data[2];
        inflatePrime(&source, bits, value);
    }
    
    // Try inflate with different flush modes
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK};
    int flush_idx = (Size > 4) ? (Data[3] % 5) : 0;
    
    // First inflate call
    ret = inflate(&source, flush_modes[flush_idx]);
    
    // Try inflateUndermine if we have enough data
    if (Size > 5) {
        int undermine_val = Data[4] & 1;
        inflateUndermine(&source, undermine_val);
    }
    
    // Get codes used
    inflateCodesUsed(&source);
    
    // Try to copy the stream
    init_stream(&dest);
    ret = inflateCopy(&dest, &source);
    
    if (ret == Z_OK) {
        // If copy succeeded, try to use the copied stream
        Bytef dest_out[OUT_SIZE];
        dest.next_out = dest_out;
        dest.avail_out = OUT_SIZE;
        
        // Continue inflation with copied stream
        inflate(&dest, Z_NO_FLUSH);
        
        // Clean up destination stream
        inflateEnd(&dest);
    }
    
    // Try another inflate call on source with different flush
    if (Size > 6) {
        flush_idx = Data[5] % 5;
        inflate(&source, flush_modes[flush_idx]);
    }
    
    // Try inflatePrime again after Z_BLOCK if we got that return
    if (ret == Z_BLOCK && Size > 7) {
        int bits = Data[6] % 17;
        int value = Data[7];
        inflatePrime(&source, bits, value);
    }
    
    // Clean up source stream
    inflateEnd(&source);
    
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
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

static void init_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    z_stream strm;
    init_stream(&strm);
    
    // Use first byte for windowBits parameter
    int windowBits = (Data[0] % 32) - 16; // Range: -16 to 15
    // Use second byte for flush parameter
    int flush = Data[1] % 6; // 0-5 corresponds to Z_NO_FLUSH, Z_PARTIAL_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK
    // Use third byte for inflateValidate check parameter
    int check = Data[2] % 2; // 0 or 1
    // Use fourth byte for inflatePrime bits parameter
    int bits = Data[3] % 17; // 0-16
    
    // Initialize decompression
    int ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Set up input data (skip the first 4 bytes used for parameters)
    size_t data_offset = 4;
    size_t remaining = (Size > data_offset) ? Size - data_offset : 0;
    
    // Allocate output buffer
    Bytef out_buffer[4096];
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    if (remaining > 0) {
        strm.next_in = (Bytef*)Data + data_offset;
        strm.avail_in = remaining;
        
        // Call inflatePrime with bits parameter
        inflatePrime(&strm, bits, Data[3]);
        
        // Call inflateValidate
        inflateValidate(&strm, check);
        
        // Call inflate with flush parameter
        inflate(&strm, flush);
        
        // Try inflateSync
        inflateSync(&strm);
        
        // Reset and keep dictionary
        inflateResetKeep(&strm);
        
        // Try inflate again after reset
        if (strm.avail_in > 0 && strm.avail_out > 0) {
            inflate(&strm, Z_FINISH);
        }
    }
    
    // Clean up
    inflateEnd(&strm);
    
    return 0;
}
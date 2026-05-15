// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
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

static void initialize_z_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    z_stream strm;
    initialize_z_stream(&strm);
    
    // Extract parameters from fuzzer input
    int windowBits = (Data[0] % 96) - 15; // Range: -15 to 80
    int bits = (Data[1] % 17); // 0-16 bits for inflatePrime
    int value = Data[2];
    int check = Data[3] & 1; // 0 or 1 for inflateValidate
    
    // Initialize decompression
    int ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Set up input buffer with remaining data
    size_t input_size = Size - 4;
    if (input_size > 0) {
        uint8_t *input_copy = malloc(input_size);
        if (!input_copy) {
            inflateEnd(&strm);
            return 0;
        }
        memcpy(input_copy, Data + 4, input_size);
        
        strm.next_in = input_copy;
        strm.avail_in = input_size;
        
        // Set up output buffer
        uint8_t output[1024];
        strm.next_out = output;
        strm.avail_out = sizeof(output);
        
        // Try to insert bits before inflation
        inflatePrime(&strm, bits, value);
        
        // Enable/disable CRC validation
        inflateValidate(&strm, check);
        
        // Start decompression
        ret = inflate(&strm, Z_NO_FLUSH);
        
        // If we get a data error, try to sync
        if (ret == Z_DATA_ERROR) {
            inflateSync(&strm);
        }
        
        // Get codes used
        unsigned long codes_used = inflateCodesUsed(&strm);
        
        // Reset while keeping window
        inflateResetKeep(&strm);
        
        // Try decompression again with reset state
        strm.next_in = input_copy;
        strm.avail_in = input_size;
        strm.next_out = output;
        strm.avail_out = sizeof(output);
        inflate(&strm, Z_FINISH);
        
        free(input_copy);
    }
    
    inflateEnd(&strm);
    return 0;
}
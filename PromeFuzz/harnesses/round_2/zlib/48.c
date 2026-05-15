// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflate at deflate.c:981:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_stream(z_stream *strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    z_stream strm;
    initialize_stream(&strm);
    
    // Use first byte to determine windowBits for raw vs zlib format
    int windowBits = (Data[0] & 1) ? -15 : 15; // -15 for raw, 15 for zlib
    int level = Z_DEFAULT_COMPRESSION;
    int strategy = Z_DEFAULT_STRATEGY;
    
    // Initialize deflate with appropriate windowBits
    if (deflateInit2(&strm, level, Z_DEFLATED, windowBits, 8, strategy) != Z_OK) {
        return 0;
    }
    
    // Prepare input buffer (skip first byte used for configuration)
    size_t input_offset = 1;
    size_t input_size = (Size > input_offset) ? Size - input_offset : 0;
    
    // Output buffer
    Bytef out_buffer[4096];
    
    // Set input data
    strm.next_in = (Bytef*)(Data + input_offset);
    strm.avail_in = (uInt)(input_size > UINT_MAX ? UINT_MAX : input_size);
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    // For raw deflate, try deflatePrime before first deflate call
    if (windowBits == -15 && input_size >= 2) {
        int bits = Data[input_offset] % 17; // bits 0-16
        int value = Data[(input_offset + 1) % Size];
        deflatePrime(&strm, bits, value);
    }
    
    // Try deflateSetDictionary if we have enough data
    if (input_size >= 4) {
        uInt dict_length = Data[input_offset] | (Data[(input_offset + 1) % Size] << 8);
        dict_length = dict_length % 256; // Limit dictionary size
        if (dict_length > 0 && input_offset + 2 + dict_length <= Size) {
            deflateSetDictionary(&strm, Data + input_offset + 2, dict_length);
        }
    }
    
    // Main compression loop
    int flush = Z_NO_FLUSH;
    int ret = Z_OK;
    
    while (strm.avail_in > 0 && ret == Z_OK) {
        // Occasionally switch to Z_SYNC_FLUSH
        if (strm.total_in % 100 == 0) {
            flush = Z_SYNC_FLUSH;
        }
        
        ret = deflate(&strm, flush);
        
        // Test deflatePending when output buffer is full or after deflate
        if (strm.avail_out == 0 || ret == Z_OK) {
            unsigned pending;
            int bits;
            deflatePending(&strm, &pending, &bits);
        }
        
        // Test deflateUsed
        int used_bits;
        deflateUsed(&strm, &used_bits);
        
        // Occasionally try deflateParams
        if (strm.total_in % 50 == 0 && input_size >= 2) {
            int new_level = Data[(input_offset + strm.total_in) % Size] % 10;
            int new_strategy = Data[(input_offset + strm.total_in + 1) % Size] % 4;
            deflateParams(&strm, new_level, new_strategy);
        }
        
        // Reset output buffer if full
        if (strm.avail_out == 0) {
            strm.next_out = out_buffer;
            strm.avail_out = sizeof(out_buffer);
        }
        
        flush = Z_NO_FLUSH;
    }
    
    // Finish compression
    if (ret == Z_OK || ret == Z_BUF_ERROR) {
        strm.avail_in = 0;
        do {
            strm.next_out = out_buffer;
            strm.avail_out = sizeof(out_buffer);
            ret = deflate(&strm, Z_FINISH);
            
            // Test functions during finish
            unsigned pending;
            int bits;
            deflatePending(&strm, &pending, &bits);
            
            int used_bits;
            deflateUsed(&strm, &used_bits);
        } while (ret == Z_OK);
    }
    
    // Cleanup
    deflateEnd(&strm);
    
    return 0;
}
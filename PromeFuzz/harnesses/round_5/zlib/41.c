// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflate at deflate.c:981:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_stream(z_streamp strm, int window_bits) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    deflateInit2(strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 
                 window_bits, 8, Z_DEFAULT_STRATEGY);
}

static void cleanup_stream(z_streamp strm) {
    deflateEnd(strm);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    
    if (Size < 4) return 0;
    
    // Use first byte to determine window bits mode
    int window_bits_mode = Data[0] % 3;
    int window_bits;
    switch (window_bits_mode) {
        case 0: window_bits = 15; break;  // zlib format
        case 1: window_bits = 31; break;  // gzip format
        case 2: window_bits = -15; break; // raw deflate
        default: window_bits = 15;
    }
    
    init_stream(&strm, window_bits);
    
    // Prepare input data (skip first byte used for mode)
    size_t input_size = Size - 1;
    const uint8_t* input_data = Data + 1;
    
    // Set up input buffer
    strm.next_in = (Bytef*)input_data;
    strm.avail_in = input_size > 65536 ? 65536 : (uInt)input_size;
    
    // Prepare output buffer
    unsigned char out_buffer[4096];
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    // Use second byte to determine flush mode
    int flush_mode = Data[1] % 5;
    int flush;
    switch (flush_mode) {
        case 0: flush = Z_NO_FLUSH; break;
        case 1: flush = Z_PARTIAL_FLUSH; break;
        case 2: flush = Z_SYNC_FLUSH; break;
        case 3: flush = Z_FULL_FLUSH; break;
        case 4: flush = Z_FINISH; break;
        default: flush = Z_NO_FLUSH;
    }
    
    // Test deflatePrime for raw deflate mode before first deflate call
    if (window_bits == -15 && input_size > 4) {
        int bits = Data[2] % 17;  // bits must be ≤ 16
        int value = Data[3];
        deflatePrime(&strm, bits, value);
    }
    
    // Test deflateSetDictionary if we have enough data
    if (input_size > 100) {
        uInt dict_len = Data[4] % 256;
        if (dict_len > 0 && dict_len + 5 < input_size) {
            deflateSetDictionary(&strm, input_data + 5, dict_len);
        }
    }
    
    // Main deflate call
    ret = deflate(&strm, flush);
    
    // Test other functions regardless of deflate result
    if (ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
        // Test deflateUsed
        int used_bits;
        deflateUsed(&strm, &used_bits);
        
        // Test deflatePending
        unsigned pending;
        int pending_bits;
        deflatePending(&strm, &pending, &pending_bits);
        
        // Test deflateGetDictionary
        Bytef dictionary[256];
        uInt dict_length;
        deflateGetDictionary(&strm, dictionary, &dict_length);
        
        // Continue deflating if output buffer was full
        while (strm.avail_out == 0 && ret == Z_OK) {
            strm.next_out = out_buffer;
            strm.avail_out = sizeof(out_buffer);
            ret = deflate(&strm, Z_NO_FLUSH);
        }
    }
    
    cleanup_stream(&strm);
    
    // Reset and test with different flush modes
    if (Size > 100) {
        init_stream(&strm, 15);  // Use zlib format for second test
        
        // Use a portion of data for dictionary
        uInt dict_len = Data[0] % 100;
        if (dict_len > 0) {
            deflateSetDictionary(&strm, input_data, dict_len);
        }
        
        // Deflate with remaining data
        strm.next_in = (Bytef*)(input_data + dict_len);
        strm.avail_in = input_size > dict_len + 100 ? 100 : (uInt)(input_size - dict_len);
        strm.next_out = out_buffer;
        strm.avail_out = sizeof(out_buffer);
        
        // Try different flush modes in sequence
        int flushes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH};
        for (int i = 0; i < 4 && strm.avail_in > 0; i++) {
            ret = deflate(&strm, flushes[i]);
            
            // Test helper functions after each flush
            if (ret == Z_OK || ret == Z_STREAM_END) {
                int used_bits;
                deflateUsed(&strm, &used_bits);
                
                unsigned pending;
                int pending_bits;
                deflatePending(&strm, &pending, &pending_bits);
            }
            
            // Refill output buffer if needed
            if (strm.avail_out == 0) {
                strm.next_out = out_buffer;
                strm.avail_out = sizeof(out_buffer);
            }
        }
        
        cleanup_stream(&strm);
    }
    
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateParams at deflate.c:774:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    // Early exit for insufficient data
    if (Size < 16) return 0;

    z_stream strm;
    Bytef out_buffer[4096];
    int ret;
    
    // Initialize stream structure
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Use first byte to determine initialization method
    uint8_t init_type = Data[0] % 3;
    Data++; Size--;
    
    // Use next byte for level parameter
    int level = (Data[0] % 12) - 2;  // Range -2 to 9 (Z_DEFAULT_COMPRESSION=6)
    Data++; Size--;
    
    // Use next byte for windowBits parameter
    int windowBits = (Data[0] % 32) - 16;  // Range -16 to 15
    Data++; Size--;
    
    // Use next byte for memLevel parameter
    int memLevel = (Data[0] % 10) + 1;  // Range 1 to 9
    Data++; Size--;
    
    // Use next byte for strategy parameter
    int strategy = Data[0] % 5;  // 0-4 maps to Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY, Z_RLE, Z_FIXED
    Data++; Size--;
    
    // Map strategy values
    int strategy_map[] = {Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY, Z_RLE, Z_FIXED};
    strategy = strategy_map[strategy];
    
    // Initialize deflate stream
    if (init_type == 0) {
        // Use deflateInit_
        ret = deflateInit_(&strm, level, ZLIB_VERSION, sizeof(z_stream));
    } else if (init_type == 1) {
        // Use deflateInit2_ with custom parameters
        ret = deflateInit2_(&strm, level, Z_DEFLATED, windowBits, memLevel, 
                           strategy, ZLIB_VERSION, sizeof(z_stream));
    } else {
        // Use deflateInit2_ with raw deflate for deflatePrime testing
        ret = deflateInit2_(&strm, level, Z_DEFLATED, -15, memLevel, 
                           strategy, ZLIB_VERSION, sizeof(z_stream));
    }
    
    if (ret != Z_OK) {
        return 0;
    }
    
    // Set up input/output buffers
    strm.next_in = (Bytef*)Data;
    strm.avail_in = Size > 2048 ? 2048 : Size;
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    // Try deflatePrime if using raw deflate
    if (init_type == 2 && Size > 4) {
        int bits = Data[0] % 17;  // 0-16 bits
        int value = Data[1] | (Data[2] << 8);
        deflatePrime(&strm, bits, value);
    }
    
    // Try deflateSetDictionary if we have enough data
    if (Size > 100) {
        size_t dict_size = Data[0] % 100 + 1;
        if (dict_size < Size) {
            deflateSetDictionary(&strm, Data + 1, dict_size);
        }
    }
    
    // Perform compression with different flush modes
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH};
    int flush_idx = 0;
    
    while (strm.avail_in > 0) {
        int flush = flush_modes[flush_idx % 4];
        flush_idx++;
        
        ret = deflate(&strm, flush);
        if (ret == Z_STREAM_ERROR || ret == Z_BUF_ERROR) {
            break;
        }
        
        // Reset output buffer if full
        if (strm.avail_out == 0) {
            strm.next_out = out_buffer;
            strm.avail_out = sizeof(out_buffer);
        }
        
        // Try deflateParams occasionally
        if ((Data[0] % 16) == 0 && Size > 10) {
            int new_level = Data[1] % 12 - 2;
            int new_strategy_idx = Data[2] % 5;
            int new_strategy = strategy_map[new_strategy_idx];
            deflateParams(&strm, new_level, new_strategy);
        }
        
        // Provide more input if available
        if (strm.avail_in == 0 && Size > 2048) {
            size_t remaining = Size - 2048;
            size_t to_add = remaining > 512 ? 512 : remaining;
            strm.next_in = (Bytef*)Data + 2048;
            strm.avail_in = to_add;
        }
    }
    
    // Finish compression
    if (ret != Z_STREAM_END) {
        strm.avail_in = 0;
        do {
            strm.next_out = out_buffer;
            strm.avail_out = sizeof(out_buffer);
            ret = deflate(&strm, Z_FINISH);
        } while (ret == Z_OK);
    }
    
    // Clean up
    deflateEnd(&strm);
    
    return 0;
}
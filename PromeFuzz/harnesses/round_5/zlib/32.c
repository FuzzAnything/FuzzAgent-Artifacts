// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_stream(z_streamp strm, int windowBits) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    deflateInit2(strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, windowBits, 8, Z_DEFAULT_STRATEGY);
}

static void cleanup_stream(z_streamp strm) {
    if (strm && strm->state) {
        deflateEnd(strm);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    z_stream strm1, strm2;
    int ret;
    int bits_val;
    
    // Initialize first stream with raw deflate to allow deflatePrime
    init_stream(&strm1, -15);
    
    // Use first byte to determine operation mix
    uint8_t op_selector = Data[0];
    size_t data_offset = 1;
    
    // Set dictionary if we have enough data
    if (Size > data_offset + 32) {
        uInt dict_len = (Data[data_offset] % 32) + 1;
        if (dict_len > Size - data_offset - 1) {
            dict_len = Size - data_offset - 1;
        }
        if (dict_len > 0) {
            deflateSetDictionary(&strm1, Data + data_offset + 1, dict_len);
            data_offset += dict_len + 1;
        }
    }
    
    // Prepare input buffer for deflate operations
    size_t input_len = 0;
    if (Size > data_offset) {
        input_len = Size - data_offset;
    }
    
    // Allocate output buffer
    size_t out_buf_size = 4096;
    Bytef *out_buf = (Bytef *)malloc(out_buf_size);
    if (!out_buf) return 0;
    
    // Set up stream for deflate operations
    strm1.next_in = (Bytef *)(Data + data_offset);
    strm1.avail_in = input_len;
    strm1.next_out = out_buf;
    strm1.avail_out = out_buf_size;
    
    // Call deflatePrime if we have enough data and stream is in right state
    if (op_selector & 0x01 && Size > data_offset + 2) {
        int bits = (Data[data_offset] % 17); // bits must be ≤ 16
        int value = Data[data_offset + 1];
        deflatePrime(&strm1, bits, value);
    }
    
    // Perform deflate operations with different flush modes
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH};
    int flush_idx = 0;
    
    while (strm1.avail_in > 0 && strm1.avail_out > 0) {
        int flush = flush_modes[flush_idx % 4];
        flush_idx++;
        
        ret = deflate(&strm1, flush);
        if (ret == Z_STREAM_ERROR || ret == Z_BUF_ERROR) {
            break;
        }
        
        // Check pending bits after some operations
        if (flush_idx % 3 == 0) {
            deflateUsed(&strm1, &bits_val);
        }
        
        if (ret == Z_STREAM_END) {
            break;
        }
        
        // Reset output buffer if full
        if (strm1.avail_out == 0) {
            strm1.next_out = out_buf;
            strm1.avail_out = out_buf_size;
        }
    }
    
    // Test deflateResetKeep
    if (op_selector & 0x02) {
        deflateResetKeep(&strm1);
        
        // Use remaining data for another round
        if (input_len > 0) {
            strm1.next_in = (Bytef *)(Data + data_offset);
            strm1.avail_in = input_len;
            strm1.next_out = out_buf;
            strm1.avail_out = out_buf_size;
            
            ret = deflate(&strm1, Z_FINISH);
        }
    }
    
    // Test deflateCopy - initialize second stream properly
    if (op_selector & 0x04) {
        // Don't initialize strm2 - let deflateCopy do it
        memset(&strm2, 0, sizeof(z_stream));
        strm2.zalloc = Z_NULL;
        strm2.zfree = Z_NULL;
        strm2.opaque = Z_NULL;
        
        ret = deflateCopy(&strm2, &strm1);
        if (ret == Z_OK) {
            // Use the copied stream
            Bytef *out_buf2 = (Bytef *)malloc(out_buf_size);
            if (out_buf2) {
                strm2.next_out = out_buf2;
                strm2.avail_out = out_buf_size;
                deflate(&strm2, Z_FINISH);
                free(out_buf2);
            }
            cleanup_stream(&strm2);
        }
    }
    
    // Cleanup
    free(out_buf);
    cleanup_stream(&strm1);
    
    return 0;
}
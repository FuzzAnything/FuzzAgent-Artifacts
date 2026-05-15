// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflate at deflate.c:981:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
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

static void initialize_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
    strm->total_in = 0;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
    strm->total_out = 0;
    strm->msg = Z_NULL;
    strm->data_type = Z_BINARY;
    strm->adler = 0;
    strm->reserved = 0;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    unsigned pending;
    int bits;
    int used_bits;
    
    if (Size < 1) return 0;
    
    /* Initialize deflate stream with raw format (windowBits = -15) */
    initialize_stream(&strm);
    ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) return 0;
    
    /* Use first byte to control behavior */
    uint8_t control = Data[0];
    size_t data_offset = 1;
    
    /* Allocate output buffer */
    size_t out_size = Size * 2 + 1024;
    Bytef *out_buf = (Bytef*)malloc(out_size);
    if (!out_buf) {
        deflateEnd(&strm);
        return 0;
    }
    
    /* Set up input/output pointers */
    strm.next_in = (Bytef*)(Data + data_offset);
    strm.avail_in = (Size > data_offset) ? (uInt)(Size - data_offset) : 0;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_size;
    
    /* Call deflatePrime if control bit 0 is set and we have enough data */
    if ((control & 0x01) && data_offset + 3 <= Size) {
        int prime_bits = Data[data_offset] & 0x0F; /* 0-15 bits */
        int prime_value = Data[data_offset + 1] | (Data[data_offset + 2] << 8);
        deflatePrime(&strm, prime_bits, prime_value);
        data_offset += 3;
        strm.next_in = (Bytef*)(Data + data_offset);
        strm.avail_in = (Size > data_offset) ? (uInt)(Size - data_offset) : 0;
    }
    
    /* Call deflateSetDictionary if control bit 1 is set and we have data */
    if ((control & 0x02) && strm.avail_in > 0) {
        uInt dict_len = strm.avail_in > 32768 ? 32768 : strm.avail_in;
        deflateSetDictionary(&strm, strm.next_in, dict_len);
        /* Consume dictionary data */
        strm.next_in += dict_len;
        strm.avail_in -= dict_len;
    }
    
    /* Call deflateParams if control bit 2 is set */
    if (control & 0x04) {
        int level = (control >> 3) & 0x07; /* 0-7 */
        int strategy = control & 0x03; /* 0-3, map to Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY, Z_RLE */
        switch (strategy) {
            case 0: strategy = Z_DEFAULT_STRATEGY; break;
            case 1: strategy = Z_FILTERED; break;
            case 2: strategy = Z_HUFFMAN_ONLY; break;
            case 3: strategy = Z_RLE; break;
        }
        deflateParams(&strm, level, strategy);
    }
    
    /* Perform deflation with different flush modes */
    int flush_mode = Z_NO_FLUSH;
    if (control & 0x08) {
        flush_mode = Z_SYNC_FLUSH;
    } else if (control & 0x10) {
        flush_mode = Z_FULL_FLUSH;
    } else if (control & 0x20) {
        flush_mode = Z_FINISH;
    }
    
    /* Main deflation loop */
    do {
        ret = deflate(&strm, flush_mode);
        
        /* Call deflatePending after each deflate call */
        if (deflatePending(&strm, &pending, &bits) == Z_OK) {
            /* Use pending and bits values to avoid optimization removal */
            volatile unsigned dummy = pending;
            volatile int dummy_bits = bits;
            (void)dummy;
            (void)dummy_bits;
        }
        
        /* Call deflateUsed after each deflate call */
        if (deflateUsed(&strm, &used_bits) == Z_OK) {
            volatile int dummy_used = used_bits;
            (void)dummy_used;
        }
        
        /* If output buffer is full, reset it */
        if (strm.avail_out == 0) {
            strm.next_out = out_buf;
            strm.avail_out = (uInt)out_size;
        }
        
    } while (ret == Z_OK && strm.avail_in > 0);
    
    /* Clean up */
    free(out_buf);
    deflateEnd(&strm);
    
    return 0;
}
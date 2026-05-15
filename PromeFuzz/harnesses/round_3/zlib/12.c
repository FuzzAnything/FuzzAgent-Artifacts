// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    z_stream strm;
    Bytef *out_buffer = NULL;
    int ret;
    
    /* Initialize z_stream */
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    /* Use first byte to control behavior */
    uint8_t control = Data[0];
    size_t data_offset = 1;
    
    /* Try different initialization methods based on control byte */
    if ((control & 0x01) == 0) {
        /* Use deflateInit_ with default settings */
        ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, 
                          ZLIB_VERSION, sizeof(z_stream));
    } else {
        /* Use deflateInit2_ with parameters derived from input */
        int level = (control >> 1) & 0x7; /* 0-7 */
        int method = Z_DEFLATED;
        int windowBits = 15 + ((control >> 4) & 0x1); /* 15 or 16 */
        int memLevel = 8 + ((control >> 5) & 0x1); /* 8 or 9 */
        int strategy = Z_DEFAULT_STRATEGY;
        
        if ((control >> 6) & 0x1) {
            /* Try raw deflate for deflatePrime */
            windowBits = -15;
        }
        
        ret = deflateInit2_(&strm, level, method, windowBits, 
                           memLevel, strategy, ZLIB_VERSION, 
                           sizeof(z_stream));
    }
    
    if (ret != Z_OK) {
        /* Initialization failed, cleanup and return */
        deflateEnd(&strm);
        return 0;
    }
    
    /* Set dictionary if we have enough data */
    if (data_offset + 32 <= Size) {
        uInt dict_len = (Size - data_offset) > 32768 ? 32768 : (uInt)(Size - data_offset);
        deflateSetDictionary(&strm, Data + data_offset, dict_len);
        data_offset += dict_len;
    }
    
    /* Try deflatePrime if using raw deflate and we have enough data */
    if (((control >> 6) & 0x1) && data_offset + 2 <= Size) {
        int bits = Data[data_offset] % 17; /* 0-16 bits */
        int value = Data[data_offset + 1];
        deflatePrime(&strm, bits, value);
        data_offset += 2;
    }
    
    /* Prepare output buffer */
    size_t out_size = Size * 2 + 1024;
    out_buffer = (Bytef*)malloc(out_size);
    if (!out_buffer) {
        deflateEnd(&strm);
        return 0;
    }
    
    strm.next_out = out_buffer;
    strm.avail_out = (uInt)out_size;
    
    /* Process remaining input data */
    if (data_offset < Size) {
        size_t remaining = Size - data_offset;
        size_t chunk_size;
        int flush;
        
        while (data_offset < Size) {
            /* Determine chunk size and flush mode */
            chunk_size = (remaining > 1024) ? 1024 : remaining;
            flush = (data_offset + chunk_size >= Size) ? Z_FINISH : Z_NO_FLUSH;
            
            strm.next_in = (Bytef*)(Data + data_offset);
            strm.avail_in = (uInt)chunk_size;
            
            do {
                ret = deflate(&strm, flush);
                
                if (ret == Z_BUF_ERROR) {
                    /* Output buffer full, expand it */
                    size_t old_size = out_size;
                    out_size *= 2;
                    Bytef* new_buffer = (Bytef*)realloc(out_buffer, out_size);
                    if (!new_buffer) {
                        free(out_buffer);
                        deflateEnd(&strm);
                        return 0;
                    }
                    out_buffer = new_buffer;
                    strm.next_out = out_buffer + (strm.next_out - out_buffer);
                    strm.avail_out = (uInt)(out_size - old_size);
                } else if (ret != Z_OK && ret != Z_STREAM_END) {
                    /* Error occurred */
                    break;
                }
            } while (strm.avail_out == 0 && ret == Z_BUF_ERROR);
            
            data_offset += chunk_size;
            remaining -= chunk_size;
            
            if (ret != Z_OK && ret != Z_STREAM_END) {
                break;
            }
        }
        
        /* Try changing parameters mid-stream if we haven't finished */
        if (ret == Z_OK && data_offset + 2 <= Size) {
            int new_level = Data[data_offset] % 10; /* 0-9 */
            int new_strategy = Data[data_offset + 1] % 5; /* 0-4 */
            deflateParams(&strm, new_level, new_strategy);
        }
    }
    
    /* Cleanup */
    free(out_buffer);
    deflateEnd(&strm);
    
    return 0;
}
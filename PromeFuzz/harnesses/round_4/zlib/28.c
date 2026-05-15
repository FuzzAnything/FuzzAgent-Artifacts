// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void init_stream(z_streamp strm, int windowBits) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    deflateInit2(strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, windowBits, 8, Z_DEFAULT_STRATEGY);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    z_stream strm1, strm2;
    int ret;
    int bits;
    
    // Use first byte to determine which function to test
    uint8_t selector = Data[0];
    const uint8_t* func_data = Data + 1;
    size_t func_size = Size - 1;
    
    switch (selector % 6) {
        case 0: // deflateResetKeep
            memset(&strm1, 0, sizeof(z_stream));
            init_stream(&strm1, 15);
            
            // Compress some data first
            unsigned char out_buf[1024];
            strm1.next_in = (Bytef*)func_data;
            strm1.avail_in = func_size > 1024 ? 1024 : func_size;
            strm1.next_out = out_buf;
            strm1.avail_out = sizeof(out_buf);
            deflate(&strm1, Z_FINISH);
            
            ret = deflateResetKeep(&strm1);
            deflateEnd(&strm1);
            break;
            
        case 1: // deflateCopy
            memset(&strm1, 0, sizeof(z_stream));
            memset(&strm2, 0, sizeof(z_stream));
            init_stream(&strm1, 15);
            
            // Don't initialize strm2 - let deflateCopy initialize it
            strm2.zalloc = Z_NULL;
            strm2.zfree = Z_NULL;
            strm2.opaque = Z_NULL;
            
            ret = deflateCopy(&strm2, &strm1);
            
            if (ret == Z_OK) {
                deflateEnd(&strm1);
                deflateEnd(&strm2);
            } else {
                deflateEnd(&strm1);
            }
            break;
            
        case 2: // deflateSetDictionary
            memset(&strm1, 0, sizeof(z_stream));
            init_stream(&strm1, 15);
            
            ret = deflateSetDictionary(&strm1, func_data, 
                                      func_size > 65535 ? 65535 : func_size);
            
            deflateEnd(&strm1);
            break;
            
        case 3: // deflateUsed
            memset(&strm1, 0, sizeof(z_stream));
            init_stream(&strm1, 15);
            
            // Do some compression first
            unsigned char out_buf2[1024];
            strm1.next_in = (Bytef*)func_data;
            strm1.avail_in = func_size > 1024 ? 1024 : func_size;
            strm1.next_out = out_buf2;
            strm1.avail_out = sizeof(out_buf2);
            deflate(&strm1, Z_NO_FLUSH);
            
            ret = deflateUsed(&strm1, &bits);
            deflateEnd(&strm1);
            break;
            
        case 4: // deflate
            memset(&strm1, 0, sizeof(z_stream));
            init_stream(&strm1, 15);
            
            unsigned char out_buf3[1024];
            strm1.next_in = (Bytef*)func_data;
            strm1.avail_in = func_size > 1024 ? 1024 : func_size;
            strm1.next_out = out_buf3;
            strm1.avail_out = sizeof(out_buf3);
            
            // Test different flush modes
            int flush_mode = (func_size > 0) ? (func_data[0] % 3) : 0;
            switch (flush_mode) {
                case 0: ret = deflate(&strm1, Z_NO_FLUSH); break;
                case 1: ret = deflate(&strm1, Z_SYNC_FLUSH); break;
                case 2: ret = deflate(&strm1, Z_FINISH); break;
            }
            
            deflateEnd(&strm1);
            break;
            
        case 5: // deflatePrime
            memset(&strm1, 0, sizeof(z_stream));
            // Use raw deflate for deflatePrime
            init_stream(&strm1, -15);
            
            if (func_size >= 2) {
                int bits_to_insert = func_data[0] % 17; // 0-16 bits
                int value = func_data[1];
                ret = deflatePrime(&strm1, bits_to_insert, value);
            } else {
                ret = deflatePrime(&strm1, 8, 0xFF);
            }
            
            deflateEnd(&strm1);
            break;
    }
    
    return 0;
}
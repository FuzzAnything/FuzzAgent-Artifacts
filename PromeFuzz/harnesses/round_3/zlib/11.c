// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    z_stream strm;
    Bytef *out_buffer = NULL;
    int ret;
    
    /* Initialize z_stream */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    
    /* Extract parameters from fuzz data */
    int level = Data[0] % 11;                /* 0-10 */
    int method = Z_DEFLATED;                 /* Fixed */
    int windowBits = (Data[1] % 31) - 15;    /* -15 to 15 */
    int memLevel = (Data[2] % 9) + 1;        /* 1-9 */
    int strategy = Data[3] % 5;              /* 0-4 */
    int flush = Data[4] % 4;                 /* 0-3 */
    int bits = Data[5] % 17;                 /* 0-16 */
    int value = Data[6];
    
    /* Initialize deflate with parameters */
    ret = deflateInit2_(&strm, level, method, windowBits, memLevel, strategy,
                        ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) return 0;
    
    /* Set dictionary if we have enough data */
    if (Size > 50) {
        uInt dictLength = Data[7] % 32;
        if (dictLength > 0 && dictLength <= Size - 8) {
            deflateSetDictionary(&strm, Data + 8, dictLength);
        }
    }
    
    /* For raw deflate (windowBits = -15), try deflatePrime */
    if (windowBits == -15) {
        deflatePrime(&strm, bits, value);
    }
    
    /* Prepare input/output buffers */
    size_t in_offset = 8 + (Data[7] % 32);
    if (in_offset >= Size) in_offset = 8;
    
    strm.next_in = (Bytef*)(Data + in_offset);
    strm.avail_in = Size - in_offset;
    
    size_t out_size = Size * 2 + 100;
    out_buffer = (Bytef*)malloc(out_size);
    if (!out_buffer) {
        deflateEnd(&strm);
        return 0;
    }
    
    strm.next_out = out_buffer;
    strm.avail_out = out_size;
    
    /* First deflate call */
    ret = deflate(&strm, flush);
    
    /* Try deflateParams mid-stream if we have space */
    if (ret == Z_OK && strm.avail_out > 10) {
        int new_level = Data[8] % 11;
        int new_strategy = Data[9] % 5;
        deflateParams(&strm, new_level, new_strategy);
    }
    
    /* Continue deflating until done or buffer full */
    while (ret == Z_OK && strm.avail_in > 0 && strm.avail_out > 0) {
        ret = deflate(&strm, Z_FINISH);
    }
    
    /* Try deflateResetKeep */
    if (deflateResetKeep(&strm) == Z_OK) {
        /* Reset input/output for new round */
        strm.next_in = (Bytef*)(Data + in_offset);
        strm.avail_in = Size - in_offset;
        strm.next_out = out_buffer;
        strm.avail_out = out_size;
        
        /* Deflate again with reset stream */
        ret = deflate(&strm, Z_FINISH);
    }
    
    /* Cleanup */
    free(out_buffer);
    deflateEnd(&strm);
    
    return 0;
}
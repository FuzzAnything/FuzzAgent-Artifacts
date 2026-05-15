// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    unsigned char out[4096];
    
    if (Size < 1) return 0;
    
    /* Initialize z_stream structure */
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    /* Use first byte to determine windowBits parameter */
    int windowBits = (Data[0] % 32) + 8; /* Range 8-39 */
    if (windowBits < 8) windowBits = 8;
    
    /* Initialize decompression */
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        /* If initialization fails, try with different windowBits */
        windowBits = -((Data[0] % 8) + 8); /* Negative for raw inflate */
        ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
        if (ret != Z_OK) return 0;
    }
    
    /* Set up input buffer */
    strm.next_in = (Bytef *)(Data + 1);
    strm.avail_in = (Size > 1) ? (Size - 1) : 0;
    
    /* Set up output buffer */
    strm.next_out = out;
    strm.avail_out = sizeof(out);
    
    /* Try inflatePrime if we have enough data */
    if (Size >= 4) {
        int bits = Data[1] % 17; /* 0-16 bits */
        int value = Data[2];
        inflatePrime(&strm, bits, value);
    }
    
    /* Try inflate with different flush modes */
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK};
    int flush_idx = (Size > 3) ? (Data[3] % 5) : 0;
    
    /* First inflate call */
    ret = inflate(&strm, flush_modes[flush_idx]);
    
    /* Try inflateResetKeep */
    if (ret == Z_OK || ret == Z_BUF_ERROR || ret == Z_STREAM_END) {
        inflateResetKeep(&strm);
        
        /* Reset input/output for another round */
        strm.next_in = (Bytef *)(Data + 1);
        strm.avail_in = (Size > 1) ? (Size - 1) : 0;
        strm.next_out = out;
        strm.avail_out = sizeof(out);
        
        /* Try inflate again with different flush mode */
        flush_idx = (Size > 4) ? (Data[4] % 5) : 1;
        inflate(&strm, flush_modes[flush_idx]);
    }
    
    /* Try inflateSync if we got a data error */
    if (ret == Z_DATA_ERROR) {
        inflateSync(&strm);
    }
    
    /* Try inflateValidate if we have enough data */
    if (Size >= 6) {
        int check = Data[5] % 2; /* 0 or 1 */
        inflateValidate(&strm, check);
    }
    
    /* Clean up */
    inflateEnd(&strm);
    
    return 0;
}
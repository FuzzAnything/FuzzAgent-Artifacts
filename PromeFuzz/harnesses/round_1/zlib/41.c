// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateSyncPoint at inflate.c:1320:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateValidate at inflate.c:1385:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
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

static void init_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    z_stream strm;
    init_stream(&strm);
    
    /* Use first byte to control windowBits parameter */
    int windowBits = (Data[0] % 32) - 15; /* Range -15 to 16 */
    if (windowBits == 0) windowBits = 15; /* Default to 15 if 0 */
    
    /* Initialize decompression */
    int ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* Set up input buffer with remaining data */
    size_t input_offset = 1;
    if (input_offset >= Size) {
        inflateEnd(&strm);
        return 0;
    }
    
    strm.next_in = (Bytef *)(Data + input_offset);
    strm.avail_in = (uInt)(Size - input_offset);
    
    /* Allocate output buffer */
    unsigned char out_buf[1024];
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    
    /* Perform some inflation to get stream into a state */
    inflate(&strm, Z_NO_FLUSH);
    
    /* Call target functions based on data content */
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            inflateResetKeep(&strm);
            break;
        case 1:
            inflateSyncPoint(&strm);
            break;
        case 2:
            /* Re-initialize with different windowBits */
            inflateEnd(&strm);
            init_stream(&strm);
            windowBits = (Data[0] % 16) + 8; /* Range 8-23 */
            inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
            break;
        case 3:
            inflateSync(&strm);
            break;
        case 4:
            /* Enable/disable validation based on second byte */
            if (Size > 1) {
                int check = Data[1] & 1;
                inflateValidate(&strm, check);
            }
            break;
        case 5:
            /* Prime with bits from data */
            if (Size > 2) {
                int bits = Data[1] % 17; /* 0-16 bits */
                int value = Data[2];
                inflatePrime(&strm, bits, value);
            }
            break;
    }
    
    /* Try to inflate more data after operation */
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    inflate(&strm, Z_SYNC_FLUSH);
    
    /* Clean up */
    inflateEnd(&strm);
    
    return 0;
}
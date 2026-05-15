// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    z_stream strm;
    int ret;
    unsigned long codes_used;
    
    /* Initialize stream structure */
    memset(&strm, 0, sizeof(z_stream));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    /* Initialize decompression stream */
    ret = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* Set up input buffer from fuzzer data */
    strm.next_in = (Bytef *)Data;
    strm.avail_in = Size > UINT_MAX ? UINT_MAX : (uInt)Size;
    
    /* Allocate output buffer */
    Bytef out_buf[4096];
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);
    
    /* Try inflatePrime if we have enough data */
    if (Size >= 2) {
        int bits = Data[0] % 17;  /* Bits 0-16 */
        int value = Data[1];
        inflatePrime(&strm, bits, value);
    }
    
    /* Try inflateReset */
    inflateReset(&strm);
    
    /* Try inflateSync */
    inflateSync(&strm);
    
    /* Try inflateCodesUsed */
    codes_used = inflateCodesUsed(&strm);
    
    /* Try inflateResetKeep */
    inflateResetKeep(&strm);
    
    /* Clean up */
    inflateEnd(&strm);
    
    return 0;
}
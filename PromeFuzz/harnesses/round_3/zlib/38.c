// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateCodesUsed at inflate.c:1408:23 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_zstream(z_streamp strm) {
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
    strm->data_type = 0;
    strm->adler = 0;
    strm->reserved = 0;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    z_stream strm1, strm2;
    int ret;
    
    /* Initialize first stream */
    init_zstream(&strm1);
    init_zstream(&strm2);
    
    /* Use first byte for windowBits parameter */
    int windowBits = (Data[0] % 32) - 16; /* Range: -16 to 15 */
    
    /* Initialize decompression */
    ret = inflateInit2_(&strm1, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* Set up input buffer */
    strm1.next_in = (Bytef *)Data;
    strm1.avail_in = Size > UINT_MAX ? UINT_MAX : (uInt)Size;
    
    /* Allocate output buffer */
    size_t out_size = 4096;
    Bytef *out_buf = (Bytef *)malloc(out_size);
    if (!out_buf) {
        inflateEnd(&strm1);
        return 0;
    }
    
    strm1.next_out = out_buf;
    strm1.avail_out = out_size > UINT_MAX ? UINT_MAX : (uInt)out_size;
    
    /* Use second byte for flush parameter */
    int flush = Data[1] % 6; /* Z_NO_FLUSH, Z_PARTIAL_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK */
    
    /* Call inflate */
    ret = inflate(&strm1, flush);
    
    /* Use third byte for inflatePrime parameters */
    int bits = Data[2] % 17; /* Bits must be ≤16 */
    int value = Data[3];
    ret = inflatePrime(&strm1, bits, value);
    
    /* Call inflate again after prime */
    if (ret == Z_OK || ret == Z_BUF_ERROR) {
        inflate(&strm1, Z_NO_FLUSH);
    }
    
    /* Use fourth byte for inflateUndermine parameter */
    int undermine = Data[3] & 1;
    ret = inflateUndermine(&strm1, undermine);
    
    /* Get codes used */
    unsigned long codes_used = inflateCodesUsed(&strm1);
    
    /* Initialize second stream for copy */
    ret = inflateInit2_(&strm2, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        /* Free the state allocated by inflateInit2_ for strm2 because inflateCopy will allocate a new one. */
        inflateEnd(&strm2);
        /* Now strm2.state is Z_NULL, but the allocation functions are set. */
        /* Copy state if first stream is active */
        if (strm1.state != Z_NULL) {
            ret = inflateCopy(&strm2, &strm1);
            if (ret != Z_OK) {
                /* If copy fails, we don't have a state in strm2, so we don't need to do anything. */
            }
        }
        /* Call inflateEnd to free any state in strm2 (if copy was successful, it has a state; otherwise, it's Z_NULL). */
        inflateEnd(&strm2);
    }
    
    /* Cleanup */
    free(out_buf);
    inflateEnd(&strm1);
    
    return 0;
}
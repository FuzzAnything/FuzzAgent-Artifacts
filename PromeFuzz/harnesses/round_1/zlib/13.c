// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK 1024

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

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    z_stream strm1, strm2;
    int ret;
    unsigned pending;
    int bits;
    Bytef out[CHUNK];

    /* Use first byte to control windowBits for raw deflate */
    int windowBits = (Data[0] & 1) ? -15 : 15;
    
    /* Initialize first stream */
    init_stream(&strm1, windowBits);
    
    /* Set up input data */
    strm1.next_in = (Bytef *)Data;
    strm1.avail_in = Size > CHUNK ? CHUNK : Size;
    strm1.next_out = out;
    strm1.avail_out = CHUNK;

    /* Call deflate with different flush modes based on second byte */
    int flush_mode = Data[1] % 4;
    switch (flush_mode) {
        case 0: ret = deflate(&strm1, Z_NO_FLUSH); break;
        case 1: ret = deflate(&strm1, Z_SYNC_FLUSH); break;
        case 2: ret = deflate(&strm1, Z_FULL_FLUSH); break;
        case 3: ret = deflate(&strm1, Z_FINISH); break;
    }

    /* deflatePending - check pending output */
    if (deflatePending(&strm1, &pending, &bits) == Z_OK) {
        /* Can use pending and bits values */
    }

    /* deflateUsed - get pending bit count */
    if (deflateUsed(&strm1, &bits) == Z_OK) {
        /* bits contains the pending bit count */
    }

    /* deflatePrime - insert raw bits (only for raw deflate) */
    if (windowBits == -15 && Size > 3) {
        int prime_bits = Data[2] % 17;  /* bits 0-16 */
        int prime_value = Data[3];
        deflatePrime(&strm1, prime_bits, prime_value);
    }

    /* deflateResetKeep - reset stream while keeping dictionary */
    if (deflateResetKeep(&strm1) != Z_OK) {
        cleanup_stream(&strm1);
        return 0;
    }

    /* deflateCopy - copy stream state */
    /* Initialize second stream differently based on third byte */
    if (Data[2] & 0x80) {
        /* Case 1: strm2 uninitialized (zeroed) */
        memset(&strm2, 0, sizeof(z_stream));
    } else {
        /* Case 2: strm2 initialized then ended before copy */
        init_stream(&strm2, windowBits);
        deflateEnd(&strm2);
        memset(&strm2, 0, sizeof(z_stream));
    }
    
    /* Perform the copy */
    ret = deflateCopy(&strm2, &strm1);
    
    if (ret == Z_OK) {
        /* Both streams are now valid and must be cleaned up */
        cleanup_stream(&strm2);
    }

    /* Clean up first stream */
    cleanup_stream(&strm1);

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void initialize_stream(z_streamp strm, const uint8_t *data, size_t size) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = (Bytef *)data;
    strm->avail_in = size > UINT_MAX ? UINT_MAX : (uInt)size;
    strm->total_in = 0;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
    strm->total_out = 0;
    strm->msg = Z_NULL;
    strm->data_type = Z_BINARY;
    strm->adler = 0;
    strm->reserved = 0;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    z_stream strm1, strm2;
    int ret;
    unsigned pending;
    int bits;
    Bytef out_buffer[4096];

    // Initialize first stream with various parameters based on input
    int windowBits = (Data[0] & 0x01) ? 15 : -15;
    int memLevel = 1 + (Data[0] % 9);
    int strategy = Data[0] % 4;

    ret = deflateInit2(&strm1, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                      windowBits, memLevel, strategy);
    if (ret != Z_OK) return 0;

    // Set up input data
    strm1.next_in = (Bytef *)(Data + 1);
    strm1.avail_in = Size > 1 ? (Size - 1 > UINT_MAX ? UINT_MAX : (uInt)(Size - 1)) : 0;
    strm1.next_out = out_buffer;
    strm1.avail_out = sizeof(out_buffer);

    // Call deflate with various flush modes
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH};
    int flush = flush_modes[Data[0] % 4];
    
    ret = deflate(&strm1, flush);
    // Ignore return value for fuzzing purposes

    // Call deflatePending
    deflatePending(&strm1, &pending, &bits);

    // Call deflatePrime if using raw deflate
    if (windowBits == -15) {
        int prime_bits = (Data[0] % 16) + 1;
        int prime_value = Data[0];
        deflatePrime(&strm1, prime_bits, prime_value);
    }

    // Call deflateReset
    deflateReset(&strm1);

    // Initialize second stream for deflateCopy
    ret = deflateInit2(&strm2, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                      windowBits, memLevel, strategy);
    if (ret == Z_OK) {
        // End strm2 before copying to avoid memory leak
        deflateEnd(&strm2);
        initialize_stream(&strm2, Data, Size);
        
        // Call deflateCopy
        ret = deflateCopy(&strm2, &strm1);
        if (ret == Z_OK) {
            // Use the copied stream
            strm2.next_out = out_buffer;
            strm2.avail_out = sizeof(out_buffer);
            deflate(&strm2, Z_NO_FLUSH);
            deflatePending(&strm2, &pending, &bits);
            deflateEnd(&strm2);
        }
    }

    // Call deflateEnd on first stream
    deflateEnd(&strm1);

    return 0;
}
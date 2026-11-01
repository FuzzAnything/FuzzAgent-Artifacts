// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflatePending at deflate.c:722:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

/* 
 * Fuzz driver for zlib deflateTune, deflatePrime, deflateResetKeep, 
 * deflateCopy, deflateParams, and deflatePending.
 */

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    z_stream strm = {0};
    z_stream strm_copy = {0};
    unsigned char out_buf[4096];
    unsigned char in_buf[256];

    /* Prepare input data for compression */
    size_t input_len = Size > sizeof(in_buf) ? sizeof(in_buf) : Size;
    memcpy(in_buf, Data, input_len);

    /* Initialize the main stream */
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    /* Initialize output buffer */
    strm.next_out = out_buf;
    strm.avail_out = sizeof(out_buf);

    /* Initialize input buffer */
    strm.next_in = in_buf;
    strm.avail_in = (uInt)input_len;

    /* 
     * Extract parameters from fuzz input for various API calls
     * Ensure bounds checking to avoid undefined behavior
     */
    int good_length = Data[0] % 256;
    int max_lazy = Data[1] % 256;
    int nice_length = Data[2] % 256;
    int max_chain = Data[3] % 256;
    int bits = Data[4] % 17; /* 0-16 */
    int value = Data[5] % 65536; /* 0-65535 */
    int level = Data[6] % 10; /* 0-9 */
    int strategy = Data[7] % 5; /* 0-4 */

    /* 1. Call deflateTune */
    deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);

    /* 2. Call deflatePrime */
    deflatePrime(&strm, bits, value);

    /* 3. Call deflateParams */
    deflateParams(&strm, level, strategy);

    /* 4. Call deflatePending */
    unsigned pending;
    int bits_count;
    deflatePending(&strm, &pending, &bits_count);

    /* 5. Call deflateCopy */
    if (deflateCopy(&strm_copy, &strm) == Z_OK) {
        /* Perform a small operation on the copied stream to exercise state */
        strm_copy.next_out = out_buf;
        strm_copy.avail_out = sizeof(out_buf);
        deflatePrime(&strm_copy, bits, value);
        deflatePending(&strm_copy, &pending, &bits_count);
    }

    /* 6. Call deflateResetKeep */
    deflateResetKeep(&strm);

    /* Cleanup */
    deflateEnd(&strm);
    deflateEnd(&strm_copy);

    return 0;
}
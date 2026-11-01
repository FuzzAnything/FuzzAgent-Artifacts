// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Allocate buffers for input and output */
    uint8_t *in_buf = NULL;
    uint8_t *out_buf = NULL;
    uint8_t *dict_buf = NULL;
    uInt dict_len = 0;
    int ret;

    /* Allocate input buffer if data is available */
    if (Size > 0) {
        in_buf = (uint8_t *)malloc(Size);
        if (!in_buf) return 0;
        memcpy(in_buf, Data, Size);
    }

    /* Allocate output buffer (at least 64KB to be safe) */
    size_t out_size = 65536;
    out_buf = (uint8_t *)malloc(out_size);
    if (!out_buf) {
        free(in_buf);
        return 0;
    }

    /* Allocate dictionary buffer */
    size_t dict_size = 32768;
    dict_buf = (uint8_t *)malloc(dict_size);
    if (!dict_buf) {
        free(in_buf);
        free(out_buf);
        return 0;
    }

    /* Test inflateInit2_ and related inflate functions */
    {
        z_stream strm = {0};
        int windowBits;

        /* Determine windowBits from input data to explore different states */
        if (Size >= 1) {
            windowBits = (Data[0] % 28) - 14; /* Range: -14 to 13 */
            if (windowBits < -15) windowBits = -15;
            if (windowBits > 15) windowBits = 15;
        } else {
            windowBits = 15;
        }

        /* Initialize inflate stream */
        ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret == Z_OK) {
            /* Set input data if available */
            if (in_buf && Size > 0) {
                strm.next_in = in_buf;
                strm.avail_in = Size;
            }

            /* Set output buffer */
            strm.next_out = out_buf;
            strm.avail_out = out_size;

            /* Try to set a dictionary if available */
            if (Size > 1) {
                dict_len = (Data[1] % 32768) + 1;
                if (dict_len > Size) dict_len = Size;
                ret = inflateSetDictionary(&strm, in_buf, dict_len);
                (void)ret; /* Ignore return for fuzzing state exploration */
            }

            /* Try to get dictionary */
            uInt dict_len_out = 0;
            ret = inflateGetDictionary(&strm, dict_buf, &dict_len_out);
            (void)ret;

            /* Try to reset with different windowBits */
            int reset_bits = (Size >= 2) ? ((int)Data[1] % 28) - 14 : 15;
            if (reset_bits < -15) reset_bits = -15;
            if (reset_bits > 15) reset_bits = 15;
            ret = inflateReset2(&strm, reset_bits);
            (void)ret;

            /* Cleanup */
            inflateEnd(&strm);
        }
    }

    /* Test deflateInit2 and related deflate functions */
    {
        z_stream strm = {0};
        int windowBits;

        /* Determine windowBits from input data */
        if (Size >= 3) {
            windowBits = (Data[2] % 28) - 14;
            if (windowBits < -15) windowBits = -15;
            if (windowBits > 15) windowBits = 15;
        } else {
            windowBits = 15;
        }

        /* Initialize deflate stream */
        ret = deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, windowBits, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret == Z_OK) {
            /* Set input data if available */
            if (in_buf && Size > 0) {
                strm.next_in = in_buf;
                strm.avail_in = Size;
            }

            /* Set output buffer */
            strm.next_out = out_buf;
            strm.avail_out = out_size;

            /* Try to set a dictionary */
            if (Size > 2) {
                dict_len = (Data[2] % 32768) + 1;
                if (dict_len > Size) dict_len = Size;
                ret = deflateSetDictionary(&strm, in_buf, dict_len);
                (void)ret;
            }

            /* Try to get dictionary */
            uInt dict_len_out = 0;
            ret = deflateGetDictionary(&strm, dict_buf, &dict_len_out);
            (void)ret;

            /* Cleanup */
            deflateEnd(&strm);
        }
    }

    /* Free allocated buffers */
    free(in_buf);
    free(out_buf);
    free(dict_buf);

    return 0;
}
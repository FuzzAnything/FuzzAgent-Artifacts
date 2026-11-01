// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// zlibVersion at zutil.c:27:22 in zlib.h
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test zlibVersion and zlibCompileFlags which have no side effects */
    const char *ver = zlibVersion();
    (void)ver;
    uLong flags = zlibCompileFlags();
    (void)flags;

    z_stream strm_deflate = {0};
    z_stream strm_inflate = {0};

    /* Allocate buffers for compression and decompression */
    uint8_t *compressed_buf = NULL;
    uint8_t *decompressed_buf = NULL;
    size_t compressed_size = 0;
    size_t decompressed_size = 0;

    /* Determine buffer sizes based on input, ensuring minimum size */
    size_t input_size = Size;
    if (input_size > 1024 * 1024) {
        input_size = 1024 * 1024;
    }

    /* Allocate output buffer for compression (worst case: input + overhead) */
    compressed_buf = (uint8_t *)malloc(input_size + 1024);
    if (!compressed_buf) {
        return 0;
    }

    /* Allocate output buffer for decompression (worst case: large expansion) */
    decompressed_buf = (uint8_t *)malloc(input_size * 10 + 1024);
    if (!decompressed_buf) {
        free(compressed_buf);
        return 0;
    }

    /*
     * Strategy 1: Test deflateInit2_ with diverse parameters
     * Parameters derived from input data to explore state space
     * Ensure we have enough bytes before accessing Data[i]
     */
    int level = 6;
    int method = Z_DEFLATED;
    int windowBits = -15;
    int memLevel = 8;
    int strategy = Z_DEFAULT_STRATEGY;

    if (Size > 0) level = Data[0] % 10;
    if (Size > 1) method = (Data[1] % 2) ? Z_DEFLATED : Z_DEFLATED; 
    if (Size > 2) windowBits = (int)(Data[2] % 16) - 8; 
    if (Size > 3) memLevel = (int)(Data[3] % 9) + 1;    
    if (Size > 4) strategy = (int)(Data[4] % 3);         

    int ret = deflateInit2_(&strm_deflate, level, method, windowBits, memLevel, strategy,
                            ZLIB_VERSION, (int)sizeof(z_stream));

    if (ret == Z_OK) {
        strm_deflate.next_in = (Bytef *)Data;
        strm_deflate.avail_in = (uInt)input_size;
        strm_deflate.next_out = compressed_buf;
        strm_deflate.avail_out = (uInt)(input_size + 1024);

        int deflate_ret = deflate(&strm_deflate, Z_NO_FLUSH);
        if (deflate_ret == Z_OK || deflate_ret == Z_STREAM_END) {
            compressed_size = input_size + 1024 - strm_deflate.avail_out;

            /* Flush to finish */
            strm_deflate.avail_in = 0;
            deflate_ret = deflate(&strm_deflate, Z_FINISH);
            if (deflate_ret == Z_STREAM_END) {
                compressed_size = input_size + 1024 - strm_deflate.avail_out;
            }
        }
        deflateEnd(&strm_deflate);
    }

    /*
     * Strategy 2: Test deflateInit_ with default settings
     */
    ret = deflateInit_(&strm_deflate, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        strm_deflate.next_in = (Bytef *)Data;
        strm_deflate.avail_in = (uInt)input_size;
        strm_deflate.next_out = compressed_buf;
        strm_deflate.avail_out = (uInt)(input_size + 1024);

        int deflate_ret2 = deflate(&strm_deflate, Z_SYNC_FLUSH);
        if (deflate_ret2 == Z_OK || deflate_ret2 == Z_STREAM_END) {
            compressed_size = input_size + 1024 - strm_deflate.avail_out;
        }
        deflateEnd(&strm_deflate);
    }

    /*
     * Strategy 3: Test inflateInit_ and decompression
     * Only attempt if we have compressed data
     */
    if (compressed_size > 0) {
        ret = inflateInit_(&strm_inflate, ZLIB_VERSION, (int)sizeof(z_stream));
        if (ret == Z_OK) {
            strm_inflate.next_in = compressed_buf;
            strm_inflate.avail_in = (uInt)compressed_size;
            strm_inflate.next_out = decompressed_buf;
            strm_inflate.avail_out = (uInt)(input_size * 10 + 1024);

            int inflate_ret = inflate(&strm_inflate, Z_NO_FLUSH);
            while (inflate_ret == Z_OK) {
                if (strm_inflate.avail_out == 0) {
                    /* Buffer full, try to continue */
                    break;
                }
                inflate_ret = inflate(&strm_inflate, Z_NO_FLUSH);
            }
            inflateEnd(&strm_inflate);
        }
    }

    /* Cleanup */
    free(compressed_buf);
    free(decompressed_buf);

    return 0;
}
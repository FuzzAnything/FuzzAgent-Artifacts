// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    z_stream strm_deflate = {0};
    z_stream strm_inflate = {0};
    z_stream strm_copy = {0};
    gz_header head = {0};
    uint8_t *out_buf = NULL;
    uint8_t *in_buf = NULL;
    size_t out_size = 0;
    int ret;

    // Allocate buffers if input is available
    if (Size > 0) {
        in_buf = (uint8_t *)malloc(Size);
        if (!in_buf) return 0;
        memcpy(in_buf, Data, Size);
    }

    // Ensure we have output buffer space
    out_size = Size > 0 ? Size * 2 : 1024;
    if (out_size < 1024) out_size = 1024;
    out_buf = (uint8_t *)malloc(out_size);
    if (!out_buf) {
        free(in_buf);
        return 0;
    }

    // --- Test deflateSetHeader ---
    // Initialize deflate stream
    ret = deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION);
    if (ret == Z_OK) {
        // Setup gzip header
        head.text = 1;
        head.time = 0;
        head.xflags = 0;
        head.os = 0;
        head.extra = NULL;
        head.extra_len = 0;
        head.extra_max = 0;
        head.name = (Bytef *)"fuzz_test";
        head.name_max = 100;
        head.comment = NULL;
        head.comm_max = 0;
        head.hcrc = 0;
        head.done = 0;

        // Call deflateSetHeader
        deflateSetHeader(&strm_deflate, &head);

        // Prepare input for deflate
        strm_deflate.next_in = in_buf;
        strm_deflate.avail_in = Size;
        strm_deflate.next_out = out_buf;
        strm_deflate.avail_out = out_size;

        // Perform deflate
        deflate(&strm_deflate, Z_FINISH);
        deflateEnd(&strm_deflate);
    }

    // --- Test inflateGetHeader and inflatePrime ---
    // Initialize inflate stream
    ret = inflateInit(&strm_inflate);
    if (ret == Z_OK) {
        // Call inflateGetHeader before reset (valid usage)
        inflateGetHeader(&strm_inflate, &head);

        // Try inflatePrime with various bit counts
        inflatePrime(&strm_inflate, 0, 0); // No-op
        inflatePrime(&strm_inflate, 8, 0xFF); // 8 bits
        inflatePrime(&strm_inflate, -1, 0); // Clear buffer
        inflatePrime(&strm_inflate, 16, 0xFFFF); // 16 bits
        inflatePrime(&strm_inflate, 32, 0); // Should fail (too many bits)

        // Prepare input for inflate (using the compressed data if available)
        if (strm_deflate.total_out > 0) {
            strm_inflate.next_in = out_buf;
            strm_inflate.avail_in = strm_deflate.total_out;
        } else {
            // If deflate didn't produce output, try with raw input
            strm_inflate.next_in = in_buf;
            strm_inflate.avail_in = Size;
        }
        strm_inflate.next_out = out_buf;
        strm_inflate.avail_out = out_size;

        // Perform inflate
        inflate(&strm_inflate, Z_FINISH);

        // Test inflateReset
        inflateReset(&strm_inflate);

        // Try inflateGetHeader again after reset
        inflateGetHeader(&strm_inflate, &head);

        inflateEnd(&strm_inflate);
    }

    // --- Test inflateCopy ---
    // Re-initialize source stream for copy test
    ret = inflateInit(&strm_inflate);
    if (ret == Z_OK) {
        // Setup some state
        if (strm_deflate.total_out > 0) {
            strm_inflate.next_in = out_buf;
            strm_inflate.avail_in = strm_deflate.total_out;
        } else {
            strm_inflate.next_in = in_buf;
            strm_inflate.avail_in = Size;
        }
        strm_inflate.next_out = out_buf;
        strm_inflate.avail_out = out_size;

        // Perform a small amount of inflate to establish state
        inflate(&strm_inflate, Z_NO_FLUSH);

        // Copy the stream
        ret = inflateCopy(&strm_copy, &strm_inflate);
        if (ret == Z_OK) {
            // Try to use the copied stream
            inflate(&strm_copy, Z_FINISH);
            inflateEnd(&strm_copy);
        }

        inflateEnd(&strm_inflate);
    }

    // --- Test deflateReset ---
    // Re-initialize deflate stream for reset test
    ret = deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION);
    if (ret == Z_OK) {
        // Set header again
        deflateSetHeader(&strm_deflate, &head);

        // Prepare input
        strm_deflate.next_in = in_buf;
        strm_deflate.avail_in = Size;
        strm_deflate.next_out = out_buf;
        strm_deflate.avail_out = out_size;

        // Compress a bit
        deflate(&strm_deflate, Z_NO_FLUSH);

        // Reset the stream
        deflateReset(&strm_deflate);

        // Try to compress again with same or new data
        strm_deflate.next_in = in_buf;
        strm_deflate.avail_in = Size;
        strm_deflate.next_out = out_buf;
        strm_deflate.avail_out = out_size;
        deflate(&strm_deflate, Z_FINISH);

        deflateEnd(&strm_deflate);
    }

    // Cleanup
    free(in_buf);
    free(out_buf);

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    z_stream source_deflate = {0};
    z_stream dest_deflate = {0};
    z_stream source_inflate = {0};
    z_stream dest_inflate = {0};
    gz_header header = {0};
    int ret;

    // Initialize buffers
    uint8_t *input_buf = (uint8_t *)malloc(Size);
    uint8_t *output_buf = (uint8_t *)malloc(Size * 2 + 1024);
    if (!input_buf || !output_buf) {
        free(input_buf);
        free(output_buf);
        return 0;
    }
    memcpy(input_buf, Data, Size);

    // --- Test deflateSetHeader and deflateReset ---
    ret = deflateInit(&source_deflate, Z_DEFAULT_COMPRESSION);
    if (ret == Z_OK) {
        // Setup GZIP header
        header.text = 1;
        header.time = 0;
        header.xflags = 0;
        header.os = 0;
        header.extra = NULL;
        header.extra_len = 0;
        header.name = NULL;
        header.comment = NULL;
        header.hcrc = 0;
        header.done = 0;

        // Set wrap to GZIP (2) before setting header
        source_deflate.data_type = 2; 
        // Note: deflateSetHeader expects wrap=2 internally, usually set via deflateInit2 with Z_GZIP
        // But we can try to set it on an existing stream if supported, or just call it.
        // The API requires the stream to be in GZIP mode. Let's re-init with Z_GZIP for safety.
        deflateEnd(&source_deflate);
        
        ret = deflateInit2(&source_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
        if (ret == Z_OK) {
            ret = deflateSetHeader(&source_deflate, &header);
            (void)ret; // Ignore return for fuzzing state exploration

            // Prepare some input for deflate
            source_deflate.next_in = input_buf;
            source_deflate.avail_in = Size;
            source_deflate.next_out = output_buf;
            source_deflate.avail_out = Size * 2 + 1024;

            ret = deflate(&source_deflate, Z_NO_FLUSH);
            (void)ret;

            // Test deflateReset
            ret = deflateReset(&source_deflate);
            (void)ret;

            // Test deflateCopy
            ret = deflateCopy(&dest_deflate, &source_deflate);
            if (ret == Z_OK) {
                deflateEnd(&dest_deflate);
            }
        }
        deflateEnd(&source_deflate);
    }

    // --- Test inflateGetHeader, inflatePrime, inflateReset, inflateCopy ---
    ret = inflateInit2(&source_inflate, MAX_WBITS + 16); // GZIP mode
    if (ret == Z_OK) {
        // Setup header retrieval
        header.text = 0;
        header.time = 0;
        header.xflags = 0;
        header.os = 0;
        header.extra = NULL;
        header.extra_len = 0;
        header.name = NULL;
        header.comment = NULL;
        header.hcrc = 0;
        header.done = 0;

        ret = inflateGetHeader(&source_inflate, &header);
        (void)ret;

        // Test inflatePrime with various bit counts
        // Positive bits
        ret = inflatePrime(&source_inflate, 8, 0x55);
        (void)ret;
        ret = inflatePrime(&source_inflate, 16, 0x1234);
        (void)ret;
        // Negative bits to clear
        ret = inflatePrime(&source_inflate, -1, 0);
        (void)ret;
        // Zero bits (no-op)
        ret = inflatePrime(&source_inflate, 0, 0);
        (void)ret;
        // Invalid bits (should fail)
        ret = inflatePrime(&source_inflate, 17, 0);
        (void)ret;

        // Prepare input for inflate
        source_inflate.next_in = input_buf;
        source_inflate.avail_in = Size;
        source_inflate.next_out = output_buf;
        source_inflate.avail_out = Size * 2 + 1024;

        // Run inflate to process some data
        ret = inflate(&source_inflate, Z_NO_FLUSH);
        (void)ret;

        // Test inflateReset
        ret = inflateReset(&source_inflate);
        if (ret == Z_OK) {
            // Re-attach header retrieval after reset if needed
            ret = inflateGetHeader(&source_inflate, &header);
            (void)ret;
        }

        // Test inflateCopy
        ret = inflateCopy(&dest_inflate, &source_inflate);
        if (ret == Z_OK) {
            inflateEnd(&dest_inflate);
        }
    }
    inflateEnd(&source_inflate);

    free(input_buf);
    free(output_buf);
    return 0;
}
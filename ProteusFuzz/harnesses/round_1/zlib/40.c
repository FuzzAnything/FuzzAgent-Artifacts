// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    z_stream strm = {0};
    z_stream strm_copy = {0};
    gz_header header = {0};
    gz_header *head_ptr = &header;
    
    uint8_t *input_buf = NULL;
    uint8_t *output_buf = NULL;
    size_t input_len = 0;
    size_t output_len = 0;

    // Allocate buffers
    if (Size > 0) {
        input_buf = (uint8_t *)malloc(Size);
        if (!input_buf) return 0;
        memcpy(input_buf, Data, Size);
        input_len = Size;
    }

    output_len = Size * 2 + 1024; // Heuristic for decompression output size
    output_buf = (uint8_t *)malloc(output_len);
    if (!output_buf) {
        free(input_buf);
        return 0;
    }

    // Test 1: deflateSetHeader and deflateReset
    // Initialize for deflate with GZIP wrapper (wrap=2)
    if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        // Set header
        header.text = 1;
        header.time = 0;
        header.xflags = 0;
        header.os = 0;
        header.extra = NULL;
        header.extra_len = 0;
        header.name = (Bytef *)"fuzz_test";
        header.name_max = 20;
        header.comment = (Bytef *)"fuzz_comment";
        header.comm_max = 20;
        header.hcrc = 1;
        header.done = 0;

        deflateSetHeader(&strm, head_ptr);

        // Perform some compression to ensure state is active
        strm.next_in = input_buf;
        strm.avail_in = input_len;
        strm.next_out = output_buf;
        strm.avail_out = output_len;

        deflate(&strm, Z_NO_FLUSH);
        deflate(&strm, Z_FINISH);

        // Reset deflate stream
        deflateReset(&strm);
        
        // Cleanup deflate
        deflateEnd(&strm);
    }

    // Test 2: inflateGetHeader, inflate, inflateReset, inflateCopy
    // Reuse input buffer as compressed data (even if it's not valid gzip, we test error handling)
    
    // Initialize inflate with GZIP header support
    if (inflateInit2(&strm, 16 + MAX_WBITS) == Z_OK) {
        // Enable header retrieval
        inflateGetHeader(&strm, head_ptr);

        // Feed data
        strm.next_in = input_buf;
        strm.avail_in = input_len;
        strm.next_out = output_buf;
        strm.avail_out = output_len;

        // Try to inflate
        (void)inflate(&strm, Z_NO_FLUSH);
        
        // Try inflateReset
        inflateReset(&strm);

        // Try inflateCopy
        // Initialize dest stream
        if (inflateInit2(&strm_copy, 16 + MAX_WBITS) == Z_OK) {
            inflateGetHeader(&strm_copy, head_ptr);
            // Copy state from source to dest
            inflateCopy(&strm_copy, &strm);
            
            // Cleanup copy
            inflateEnd(&strm_copy);
        }

        // Cleanup inflate
        inflateEnd(&strm);
    }

    // Test 3: Edge cases with invalid states
    // Call functions on uninitialized or partially initialized streams to trigger error paths
    z_stream bad_stream = {0};
    deflateSetHeader(&bad_stream, head_ptr);
    inflateReset(&bad_stream);
    inflate(&bad_stream, Z_NO_FLUSH);
    deflateReset(&bad_stream);
    inflateGetHeader(&bad_stream, head_ptr);
    
    // Test inflateCopy with invalid source
    z_stream bad_dest = {0};
    inflateInit2(&bad_dest, 16 + MAX_WBITS);
    inflateCopy(&bad_dest, &bad_stream);
    inflateEnd(&bad_dest);

    // Cleanup
    free(input_buf);
    free(output_buf);

    return 0;
}
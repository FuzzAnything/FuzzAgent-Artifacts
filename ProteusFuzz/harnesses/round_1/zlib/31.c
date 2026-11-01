// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static void free_gz_header_fields(gz_header *head) {
    if (head == NULL) return;
    if (head->extra != NULL) {
        free(head->extra);
        head->extra = NULL;
    }
    if (head->name != NULL) {
        free(head->name);
        head->name = NULL;
    }
    if (head->comment != NULL) {
        free(head->comment);
        head->comment = NULL;
    }
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    z_stream strm = {0};
    z_stream strm_copy = {0};
    gz_header head = {0};
    gz_header head_copy = {0};
    uint8_t *input_buf = NULL;
    uint8_t *output_buf = NULL;
    int ret;

    // Allocate buffers
    size_t buf_size = Size > 65536 ? 65536 : Size;
    input_buf = (uint8_t *)malloc(buf_size);
    output_buf = (uint8_t *)malloc(buf_size);
    if (!input_buf || !output_buf) {
        free(input_buf);
        free(output_buf);
        return 0;
    }
    memcpy(input_buf, Data, buf_size);

    // Initialize gzip header fields with fuzzed data if available
    // Ensure we have enough data before accessing indices
    if (Size > 10) {
        head.text = Data[0] & 1;
        head.time = (uLong)(Data[1] << 24 | Data[2] << 16 | Data[3] << 8 | Data[4]);
        head.xflags = Data[5];
        head.os = Data[6];
        head.hcrc = (Data[7] & 1);
        
        // Allocate extra field
        if (buf_size > 8) {
            uInt extra_len = Data[8];
            if (extra_len > 1000) extra_len = 1000;
            // Ensure we don't read past the end of Data
            if (9 + extra_len <= Size) {
                head.extra = (Bytef *)malloc(extra_len);
                if (head.extra) {
                    memcpy(head.extra, Data + 9, extra_len);
                    head.extra_len = extra_len;
                    head.extra_max = extra_len;
                }
            }
        }

        // Allocate name field
        if (buf_size > 9) {
            uInt name_len = Data[9];
            if (name_len > 1000) name_len = 1000;
            // Ensure we don't read past the end of Data
            if (10 + name_len <= Size) {
                head.name = (Bytef *)malloc(name_len + 1);
                if (head.name) {
                    memcpy(head.name, Data + 10, name_len);
                    head.name[name_len] = '\0';
                    head.name_max = name_len;
                }
            }
        }

        // Allocate comment field
        if (buf_size > 10) {
            uInt comm_len = Data[10];
            if (comm_len > 1000) comm_len = 1000;
            // Ensure we don't read past the end of Data
            if (11 + comm_len <= Size) {
                head.comment = (Bytef *)malloc(comm_len + 1);
                if (head.comment) {
                    memcpy(head.comment, Data + 11, comm_len);
                    head.comment[comm_len] = '\0';
                    head.comm_max = comm_len;
                }
            }
        }
    }

    // Test deflateSetHeader
    // Initialize deflate stream for GZIP mode (wrap=2)
    if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15 + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        // Set header
        ret = deflateSetHeader(&strm, &head);
        (void)ret; // Ignore return for fuzzing exploration

        // Setup input for deflate
        strm.next_in = input_buf;
        strm.avail_in = buf_size;
        strm.next_out = output_buf;
        strm.avail_out = buf_size;

        // Perform compression
        ret = deflate(&strm, Z_FINISH);
        (void)ret;

        // Reset deflate stream
        ret = deflateReset(&strm);
        (void)ret;

        // Cleanup deflate
        deflateEnd(&strm);
    }

    // Test inflateGetHeader and inflate
    // Initialize inflate stream for GZIP mode
    if (inflateInit2(&strm, 16 + MAX_WBITS) == Z_OK) {
        // Setup header retrieval
        ret = inflateGetHeader(&strm, &head_copy);
        (void)ret;

        // Setup input for inflate (using the same fuzzed data as potential compressed stream)
        strm.next_in = input_buf;
        strm.avail_in = buf_size;
        strm.next_out = output_buf;
        strm.avail_out = buf_size;

        // Perform decompression
        ret = inflate(&strm, Z_NO_FLUSH);
        (void)ret;

        // Try inflateReset
        ret = inflateReset(&strm);
        (void)ret;

        // Try inflateCopy
        if (inflateInit2(&strm_copy, 16 + MAX_WBITS) == Z_OK) {
            ret = inflateCopy(&strm_copy, &strm);
            (void)ret;
            // Ensure strm_copy is cleaned up regardless of copy success
            inflateEnd(&strm_copy);
        }

        // Cleanup inflate
        inflateEnd(&strm);
    }

    // Cleanup allocated header fields
    free_gz_header_fields(&head);
    free_gz_header_fields(&head_copy);

    free(input_buf);
    free(output_buf);

    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void cleanup_stream(z_stream *strm) {
    if (strm) {
        if (strm->zalloc == NULL && strm->zfree == NULL) {
            // Only call destroy if we initialized it with default allocators
            // Check if state exists to avoid double free or invalid access
            if (strm->state != NULL) {
                deflateEnd(strm);
                inflateEnd(strm);
            }
        }
        // Free any manually allocated header buffers
        if (strm->opaque != NULL) {
            gz_header *head = (gz_header *)strm->opaque;
            if (head->extra != NULL) free(head->extra);
            if (head->name != NULL) free(head->name);
            if (head->comment != NULL) free(head->comment);
            free(head);
            strm->opaque = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    z_stream strm = {0};
    z_stream dest = {0};
    gz_header head = {0};
    gz_header *head_ptr = &head;
    
    // Allocate buffers for compression and decompression
    uint8_t *in_buf = NULL;
    uint8_t *out_buf = NULL;
    uint8_t *comp_buf = NULL;
    
    size_t in_size = Size;
    size_t out_size = Size * 2 + 1024;
    size_t comp_size = Size * 2 + 1024;

    // Allocate memory
    in_buf = (uint8_t *)malloc(in_size);
    out_buf = (uint8_t *)malloc(out_size);
    comp_buf = (uint8_t *)malloc(comp_size);

    if (!in_buf || !out_buf || !comp_buf) {
        free(in_buf); free(out_buf); free(comp_buf);
        return 0;
    }

    memcpy(in_buf, Data, in_size);

    // --- Test deflateSetHeader and deflateReset ---
    // Initialize deflate stream with GZIP header (wrap=2)
    if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        // Set GZIP header
        head.text = (Data[0] & 1);
        head.time = Data[1] + Data[2] * 256;
        head.xflags = Data[3];
        head.os = Data[4];
        head.hcrc = (Data[5] & 1);
        
        // Allocate extra, name, comment if indicated by data
        if (in_size > 6 && (Data[6] & 1)) {
            head.extra = (Bytef *)malloc(100);
            if (head.extra) {
                head.extra_len = 10;
                head.extra_max = 100;
                memcpy(head.extra, Data + 7, 10);
            }
        }
        if (in_size > 7 && (Data[7] & 1)) {
            head.name = (Bytef *)malloc(100);
            if (head.name) {
                head.name_max = 100;
                memset(head.name, 0, 100);
                size_t name_len = (Data[8] % 50) + 1;
                if (in_size > 8 + name_len) memcpy(head.name, Data + 9, name_len);
            }
        }
        if (in_size > 8 && (Data[8] & 1)) {
            head.comment = (Bytef *)malloc(100);
            if (head.comment) {
                head.comm_max = 100;
                memset(head.comment, 0, 100);
                size_t comm_len = (Data[9] % 50) + 1;
                if (in_size > 9 + comm_len) memcpy(head.comment, Data + 10, comm_len);
            }
        }

        // Call deflateSetHeader
        deflateSetHeader(&strm, head_ptr);

        // Prepare input
        strm.next_in = in_buf;
        strm.avail_in = in_size;
        strm.next_out = comp_buf;
        strm.avail_out = comp_size;

        // Compress
        deflate(&strm, Z_FINISH);

        // Test deflateReset
        deflateReset(&strm);

        // Cleanup deflate
        deflateEnd(&strm);
    }

    // --- Test inflateGetHeader, inflate, inflateCopy, inflateReset ---
    if (comp_size > 0 && comp_buf) {
        // Initialize inflate stream
        if (inflateInit2(&strm, -15) == Z_OK) { // -15 for raw gzip
            // Enable header retrieval
            inflateGetHeader(&strm, head_ptr);

            strm.next_in = comp_buf;
            strm.avail_in = comp_size; // Use full buffer, inflate will read what it needs
            strm.next_out = out_buf;
            strm.avail_out = out_size;

            // Perform decompression
            int ret = inflate(&strm, Z_NO_FLUSH);
            
            // Try to copy the stream
            if (ret == Z_OK || ret == Z_STREAM_END) {
                inflateCopy(&dest, &strm);
                
                // Try to reset the original stream
                inflateReset(&strm);
                
                // Cleanup copied stream
                inflateEnd(&dest);
            }
            
            // Cleanup inflate
            inflateEnd(&strm);
        }
    }

    // Cleanup allocated buffers
    free(in_buf);
    free(out_buf);
    free(comp_buf);
    
    // Cleanup header fields allocated in deflateSetHeader section
    if (head.extra) free(head.extra);
    if (head.name) free(head.name);
    if (head.comment) free(head.comment);

    return 0;
}
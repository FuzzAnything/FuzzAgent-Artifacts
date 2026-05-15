// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 32768
#define MAX_HEADER_SIZE 1024

static void init_z_stream(z_stream *strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
    strm->msg = Z_NULL;
    strm->data_type = 0;
    strm->adler = 0;
    strm->reserved = 0;
}

static void init_gz_header(gz_header *head) {
    head->text = 0;
    head->time = 0;
    head->xflags = 0;
    head->os = 0;
    head->extra = Z_NULL;
    head->extra_len = 0;
    head->extra_max = 0;
    head->name = Z_NULL;
    head->name_max = 0;
    head->comment = Z_NULL;
    head->comm_max = 0;
    head->hcrc = 0;
    head->done = 0;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    z_stream strm;
    gz_header head;
    int ret;
    size_t dict_size, header_size;
    uint8_t *dict_buf = NULL;
    uint8_t *header_buf = NULL;

    // Initialize structures
    init_z_stream(&strm);
    init_gz_header(&head);

    // Split input for different purposes
    size_t split = Size / 3;
    if (split > MAX_DICT_SIZE) split = MAX_DICT_SIZE;

    // Test inflateSetDictionary
    dict_size = split;
    if (dict_size > 0) {
        dict_buf = (uint8_t*)malloc(dict_size);
        if (dict_buf) {
            memcpy(dict_buf, Data, dict_size);
            // Initialize inflate stream
            if (inflateInit(&strm) == Z_OK) {
                // Set Adler-32 to match dictionary (simplified - in real use should compute from original data)
                strm.adler = adler32(0L, dict_buf, dict_size);
                inflateSetDictionary(&strm, dict_buf, dict_size);
                inflateEnd(&strm);
            }
            free(dict_buf);
        }
    }

    // Test deflateSetHeader
    header_size = (Size > split) ? (Size - split) / 2 : 0;
    if (header_size > MAX_HEADER_SIZE) header_size = MAX_HEADER_SIZE;
    
    if (header_size > 0) {
        header_buf = (uint8_t*)malloc(header_size);
        if (header_buf) {
            memcpy(header_buf, Data + split, header_size);
            // Initialize deflate stream with gzip wrapper
            if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 
                            MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
                // Set up header fields from fuzzer data
                head.extra = header_buf;
                head.extra_len = header_size > 255 ? 255 : header_size;
                deflateSetHeader(&strm, &head);
                deflateEnd(&strm);
            }
            free(header_buf);
        }
    }

    // Test inflateReset2
    if (inflateInit2(&strm, MAX_WBITS) == Z_OK) {
        // Use remaining bytes for windowBits parameter
        int windowBits = (Size > 0) ? (Data[Size-1] % 32) - 16 : MAX_WBITS;
        inflateReset2(&strm, windowBits);
        inflateEnd(&strm);
    }

    // Test inflatePrime
    if (inflateInit2(&strm, MAX_WBITS) == Z_OK) {
        int bits = (Size > 1) ? Data[0] % 17 : 8;  // bits 0-16
        int value = (Size > 2) ? Data[1] : 0;
        // Test both positive and negative bits
        inflatePrime(&strm, bits, value);
        if (bits > 0) {
            inflatePrime(&strm, -bits, 0);  // Clear buffer
        }
        inflateEnd(&strm);
    }

    // Test inflateGetHeader
    if (inflateInit2(&strm, MAX_WBITS + 16) == Z_OK) {  // gzip wrapper
        init_gz_header(&head);
        inflateGetHeader(&strm, &head);
        inflateEnd(&strm);
    }

    // Test inflateSync
    if (inflateInit2(&strm, MAX_WBITS) == Z_OK) {
        // Feed some data to cause potential error
        if (Size > 10) {
            strm.next_in = (Bytef*)Data;
            strm.avail_in = Size > 100 ? 100 : Size;
            uint8_t out_buf[1024];
            strm.next_out = out_buf;
            strm.avail_out = sizeof(out_buf);
            
            ret = inflate(&strm, Z_NO_FLUSH);
            if (ret == Z_DATA_ERROR) {
                inflateSync(&strm);
            }
        }
        inflateEnd(&strm);
    }

    return 0;
}
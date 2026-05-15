// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_z_stream(z_streamp strm) {
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

static void init_gz_header(gz_headerp head) {
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

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    z_stream strm;
    gz_header head;
    int ret;
    
    init_z_stream(&strm);
    init_gz_header(&head);

    // Use first byte for windowBits parameter (limited to meaningful range)
    int windowBits = (Data[0] % 32) - 16;  // Range: -16..15
    size_t data_offset = 1;

    // Initialize decompression stream
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    // Set up input data for inflate
    size_t input_size = Size - data_offset;
    if (input_size > 0) {
        strm.next_in = (Bytef*)(Data + data_offset);
        strm.avail_in = (uInt)input_size;
        
        // Allocate output buffer
        Bytef out_buf[4096];
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);

        // Try to get header if enough data
        if (input_size > 10) {
            inflateGetHeader(&strm, &head);
        }

        // Use second byte for inflatePrime bits (limited to 0-16)
        int prime_bits = Data[1] % 17;
        int prime_value = Data[2];
        if (prime_bits > 0) {
            inflatePrime(&strm, prime_bits, prime_value);
            data_offset += 3;
            if (data_offset < Size) {
                strm.next_in = (Bytef*)(Data + data_offset);
                strm.avail_in = (uInt)(Size - data_offset);
            }
        }

        // Use third byte for flush parameter
        int flush = Data[3] % 4;  // 0=NO_FLUSH, 1=SYNC_FLUSH, 2=FULL_FLUSH, 3=FINISH
        
        // Perform inflation
        ret = inflate(&strm, flush);
        
        // Try reset with different windowBits
        if (Size > 4) {
            int new_windowBits = (Data[4] % 32) - 16;
            inflateReset2(&strm, new_windowBits);
            
            // Try inflation again with reset stream
            if (data_offset < Size) {
                strm.next_in = (Bytef*)(Data + data_offset);
                strm.avail_in = (uInt)(Size - data_offset);
                strm.next_out = out_buf;
                strm.avail_out = sizeof(out_buf);
                inflate(&strm, Z_NO_FLUSH);
            }
        }
    }

    // Test deflateSetHeader with a separate stream
    if (Size > 20) {
        z_stream deflate_strm;
        init_z_stream(&deflate_strm);
        
        // Initialize deflate stream with gzip wrapper
        ret = deflateInit2_(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           windowBits + 16, 8, Z_DEFAULT_STRATEGY,
                           ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            // Set up gzip header
            head.time = (uLong)Data[5] << 24 | (uLong)Data[6] << 16 | 
                       (uLong)Data[7] << 8 | Data[8];
            head.os = Data[9] % 256;
            deflateSetHeader(&deflate_strm, &head);
            
            // Clean up deflate stream
            deflateEnd(&deflate_strm);
        }
    }

    // Clean up inflate stream
    inflateEnd(&strm);
    
    return 0;
}
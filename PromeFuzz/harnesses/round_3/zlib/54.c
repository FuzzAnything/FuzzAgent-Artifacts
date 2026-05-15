// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_z_stream(z_stream *strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
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

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to select different code paths
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    // Test deflateSetHeader
    if (selector % 6 == 0) {
        z_stream strm;
        gz_header head;
        
        init_z_stream(&strm);
        init_gz_header(&head);
        
        // Initialize deflate with gzip wrapper
        if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                         MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
            // Set some header fields from fuzz data if available
            if (fuzz_size > 0) {
                head.text = fuzz_data[0] & 1;
                head.time = (fuzz_size > 4) ? *(uint32_t*)fuzz_data : 0;
            }
            
            deflateSetHeader(&strm, &head);
            
            deflateEnd(&strm);
        }
    }
    
    // Test inflateSync
    else if (selector % 6 == 1) {
        z_stream strm;
        init_z_stream(&strm);
        
        // Initialize inflate with auto header detection
        if (inflateInit2(&strm, MAX_WBITS + 32) == Z_OK) {
            // Provide some input data
            if (fuzz_size > 0) {
                strm.next_in = (Bytef*)fuzz_data;
                strm.avail_in = (uInt)(fuzz_size > 1024 ? 1024 : fuzz_size);
                
                // Try to inflate to potentially get Z_DATA_ERROR
                Bytef out_buf[1024];
                strm.next_out = out_buf;
                strm.avail_out = sizeof(out_buf);
                
                int ret = inflate(&strm, Z_NO_FLUSH);
                if (ret == Z_DATA_ERROR) {
                    inflateSync(&strm);
                }
            }
            inflateEnd(&strm);
        }
    }
    
    // Test inflateReset2
    else if (selector % 6 == 2) {
        z_stream strm;
        init_z_stream(&strm);
        
        // Initialize with some windowBits
        int windowBits = (fuzz_size > 0) ? (fuzz_data[0] % 31) + 8 : MAX_WBITS;
        if (inflateInit2(&strm, windowBits) == Z_OK) {
            // Reset with different windowBits
            int new_windowBits = (fuzz_size > 1) ? (fuzz_data[1] % 31) + 8 : MAX_WBITS;
            inflateReset2(&strm, new_windowBits);
            
            inflateEnd(&strm);
        }
    }
    
    // Test inflateSetDictionary
    else if (selector % 6 == 3) {
        z_stream strm;
        init_z_stream(&strm);
        
        if (inflateInit(&strm) == Z_OK) {
            if (fuzz_size > 0) {
                // Use part of fuzz data as dictionary
                uInt dict_len = (uInt)(fuzz_size > 32768 ? 32768 : fuzz_size);
                inflateSetDictionary(&strm, fuzz_data, dict_len);
            }
            inflateEnd(&strm);
        }
    }
    
    // Test inflate
    else if (selector % 6 == 4) {
        z_stream strm;
        init_z_stream(&strm);
        
        // Initialize with raw inflate
        if (inflateInit2(&strm, -MAX_WBITS) == Z_OK) {
            if (fuzz_size > 0) {
                // Create a simple deflated buffer to test inflate
                z_stream def_strm;
                init_z_stream(&def_strm);
                
                if (deflateInit(&def_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
                    Bytef def_buf[1024];
                    def_strm.next_in = (Bytef*)fuzz_data;
                    def_strm.avail_in = (uInt)(fuzz_size > 512 ? 512 : fuzz_size);
                    def_strm.next_out = def_buf;
                    def_strm.avail_out = sizeof(def_buf);
                    
                    deflate(&def_strm, Z_FINISH);
                    
                    // Now inflate the compressed data
                    strm.next_in = def_buf;
                    strm.avail_in = (uInt)(sizeof(def_buf) - def_strm.avail_out);
                    
                    Bytef inf_buf[1024];
                    strm.next_out = inf_buf;
                    strm.avail_out = sizeof(inf_buf);
                    
                    inflate(&strm, Z_NO_FLUSH);
                    
                    deflateEnd(&def_strm);
                }
            }
            inflateEnd(&strm);
        }
    }
    
    // Test inflateGetHeader
    else if (selector % 6 == 5) {
        z_stream strm;
        gz_header head;
        
        init_z_stream(&strm);
        init_gz_header(&head);
        
        // Initialize inflate for gzip
        if (inflateInit2(&strm, MAX_WBITS + 16) == Z_OK) {
            inflateGetHeader(&strm, &head);
            
            // Try to inflate some data if available
            if (fuzz_size > 0) {
                strm.next_in = (Bytef*)fuzz_data;
                strm.avail_in = (uInt)(fuzz_size > 1024 ? 1024 : fuzz_size);
                
                Bytef out_buf[1024];
                strm.next_out = out_buf;
                strm.avail_out = sizeof(out_buf);
                
                inflate(&strm, Z_NO_FLUSH);
            }
            
            inflateEnd(&strm);
        }
    }
    
    return 0;
}
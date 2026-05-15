// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateResetKeep at inflate.c:100:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    // Initialize streams
    z_stream deflate_strm;
    z_stream inflate_strm;
    
    // Set up dummy buffers
    Bytef in_buf[1024];
    Bytef out_buf[1024];
    Bytef dict_buf[1024];
    uInt dict_len = 0;
    
    // Initialize deflate stream
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    // Initialize inflate stream
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    
    // Use input data for dictionary and stream content
    size_t data_used = 0;
    
    // 1. Test deflateGetDictionary and deflateSetDictionary
    if (deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     MAX_WBITS, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        
        // Prepare dictionary from input data
        size_t dict_size = Size > 256 ? 256 : Size;
        if (dict_size > 0) {
            memcpy(dict_buf, Data, dict_size);
            deflateSetDictionary(&deflate_strm, dict_buf, (uInt)dict_size);
            data_used += dict_size;
        }
        
        // Call deflateGetDictionary
        deflateGetDictionary(&deflate_strm, dict_buf, &dict_len);
        
        // Clean up deflate stream
        deflateEnd(&deflate_strm);
    }
    
    // 2. Test inflate functions
    if (inflateInit2(&inflate_strm, MAX_WBITS) == Z_OK) {
        
        // Prepare some compressed data for testing
        // We'll use a simple deflate stream to generate valid compressed data
        z_stream tmp_strm;
        tmp_strm.zalloc = Z_NULL;
        tmp_strm.zfree = Z_NULL;
        tmp_strm.opaque = Z_NULL;
        
        if (deflateInit(&tmp_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
            // Use remaining input data as source
            size_t remaining = Size - data_used;
            if (remaining > sizeof(in_buf)) {
                remaining = sizeof(in_buf);
            }
            
            if (remaining > 0 && data_used < Size) {
                memcpy(in_buf, Data + data_used, remaining);
                data_used += remaining;
                
                tmp_strm.next_in = in_buf;
                tmp_strm.avail_in = (uInt)remaining;
                tmp_strm.next_out = out_buf;
                tmp_strm.avail_out = sizeof(out_buf);
                
                // Deflate to get compressed data
                deflate(&tmp_strm, Z_FINISH);
                
                // Set up inflate stream with compressed data
                inflate_strm.next_in = out_buf;
                inflate_strm.avail_in = sizeof(out_buf) - tmp_strm.avail_out;
                inflate_strm.next_out = in_buf;  // Reuse in_buf for output
                inflate_strm.avail_out = sizeof(in_buf);
                
                // 3. Test inflateSetDictionary
                if (dict_len > 0) {
                    inflateSetDictionary(&inflate_strm, dict_buf, dict_len);
                }
                
                // 4. Test inflateUndermine
                inflateUndermine(&inflate_strm, Size & 1);  // Use LSB of Size as parameter
                
                // 5. Test inflateResetKeep
                inflateResetKeep(&inflate_strm);
                
                // 1. Test inflateGetDictionary (reusing the same stream)
                inflateGetDictionary(&inflate_strm, dict_buf, &dict_len);
            }
            
            deflateEnd(&tmp_strm);
        }
        
        // 6. Test deflateSetDictionary again with different parameters
        if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
            // Use any remaining data for dictionary
            size_t remaining = Size - data_used;
            if (remaining > sizeof(dict_buf)) {
                remaining = sizeof(dict_buf);
            }
            
            if (remaining > 0 && data_used < Size) {
                memcpy(dict_buf, Data + data_used, remaining);
                deflateSetDictionary(&deflate_strm, dict_buf, (uInt)remaining);
            }
            
            deflateEnd(&deflate_strm);
        }
        
        inflateEnd(&inflate_strm);
    }
    
    return 0;
}

#ifdef __cplusplus
}
#endif
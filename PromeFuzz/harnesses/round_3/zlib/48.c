// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// zlibVersion at zutil.c:27:22 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
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

static void init_z_stream(z_stream *strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // 1. zlibCompileFlags
    (void)zlibCompileFlags();
    
    // 2. zlibVersion
    const char *version = zlibVersion();
    (void)version;
    
    // 3. deflateInit_
    z_stream deflate_strm;
    init_z_stream(&deflate_strm);
    int ret = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, 
                          ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        deflateEnd(&deflate_strm);
    }
    
    // 4. inflateInit_
    z_stream inflate_strm;
    init_z_stream(&inflate_strm);
    ret = inflateInit_(&inflate_strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        inflateEnd(&inflate_strm);
    }
    
    // 5. deflateInit2_ - use fuzzer data to parameterize
    if (Size >= 5) {
        z_stream deflate_strm2;
        init_z_stream(&deflate_strm2);
        
        // Extract parameters from fuzzer input
        int level = Data[0] % 10;  // 0-9 compression levels
        int method = Z_DEFLATED;   // Only valid method
        int windowBits = 8 + (Data[1] % 8);  // 8-15 valid window bits
        int memLevel = 1 + (Data[2] % 9);    // 1-9 memory levels
        int strategy = Data[3] % 5;          // 0-4 strategies
        
        ret = deflateInit2_(&deflate_strm2, level, method, windowBits,
                           memLevel, strategy, ZLIB_VERSION, sizeof(z_stream));
        
        // 6. deflateSetDictionary - if deflateInit2_ succeeded
        if (ret == Z_OK && Size > 5) {
            size_t dict_size = Size - 5;
            if (dict_size > 0) {
                // Use remaining data as dictionary
                deflateSetDictionary(&deflate_strm2, Data + 5, 
                                    dict_size > 32768 ? 32768 : (uInt)dict_size);
            }
            deflateEnd(&deflate_strm2);
        } else if (ret == Z_OK) {
            deflateEnd(&deflate_strm2);
        }
    }
    
    // Try another deflateInit_ with different compression level
    if (Size > 0) {
        z_stream deflate_strm3;
        init_z_stream(&deflate_strm3);
        int level = Data[0] % 11;  // 0-10 (including Z_DEFAULT_COMPRESSION)
        if (level == 10) level = Z_DEFAULT_COMPRESSION;
        
        ret = deflateInit_(&deflate_strm3, level, 
                          ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            deflateEnd(&deflate_strm3);
        }
    }
    
    return 0;
}
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
// deflateInit2_ at deflate.c:387:13 in zlib.h
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

static void initialize_z_stream(z_streamp strm) {
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

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // Call zlibCompileFlags and zlibVersion (no input needed)
    uLong flags = zlibCompileFlags();
    const char* version = zlibVersion();
    
    // Test deflateInit_ with varying compression levels
    if (Size > 0) {
        z_stream strm;
        initialize_z_stream(&strm);
        
        // Use first byte to determine compression level (0-9, map 255 to default)
        int level = (Data[0] % 10);
        if (level == 255 % 10) level = Z_DEFAULT_COMPRESSION;
        
        int ret = deflateInit_(&strm, level, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            // Clean up
            deflateEnd(&strm);
        }
    }
    
    // Test inflateInit_
    {
        z_stream strm;
        initialize_z_stream(&strm);
        
        int ret = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            inflateEnd(&strm);
        }
    }
    
    // Test deflateInit2_ with parameters derived from input
    if (Size >= 5) {
        z_stream strm;
        initialize_z_stream(&strm);
        
        // Use input bytes to parameterize deflateInit2_
        int level = Data[0] % 10;  // 0-9
        int method = Z_DEFLATED;   // Only valid method
        int windowBits = 8 + (Data[1] % 9);  // 8-16
        int memLevel = 1 + (Data[2] % 9);    // 1-9
        int strategy = Data[3] % 4;          // 0-3
        
        int ret = deflateInit2_(&strm, level, method, windowBits, 
                               memLevel, strategy, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            // Test deflateSetDictionary if there's remaining data
            if (Size > 5) {
                size_t dict_size = Size - 5;
                if (dict_size > 0) {
                    const Bytef* dict = (const Bytef*)(Data + 5);
                    uInt dictLength = (uInt)(dict_size > UINT_MAX ? UINT_MAX : dict_size);
                    deflateSetDictionary(&strm, dict, dictLength);
                }
            }
            deflateEnd(&strm);
        }
    }
    
    // Additional test with different windowBits values
    if (Size > 0) {
        z_stream strm;
        initialize_z_stream(&strm);
        
        // Try raw deflate (negative windowBits)
        int windowBits = -((int)(Data[0] % 8) + 8);  // -8 to -15
        int ret = deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                               windowBits, 8, Z_DEFAULT_STRATEGY,
                               ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            deflateEnd(&strm);
        }
    }
    
    return 0;
}
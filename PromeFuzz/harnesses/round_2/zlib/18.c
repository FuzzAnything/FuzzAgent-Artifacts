// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// zlibVersion at zutil.c:27:22 in zlib.h
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void* custom_zalloc(void* opaque, unsigned int items, unsigned int size) {
    (void)opaque;
    return malloc(items * size);
}

static void custom_zfree(void* opaque, void* address) {
    (void)opaque;
    free(address);
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    // Test zlibVersion
    const char* version = zlibVersion();
    if (version == NULL) {
        // Handle error
    }
    
    // Test zlibCompileFlags
    unsigned long compile_flags = zlibCompileFlags();
    (void)compile_flags;
    
    // Initialize z_stream for deflate
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(z_stream));
    deflate_stream.zalloc = custom_zalloc;
    deflate_stream.zfree = custom_zfree;
    deflate_stream.opaque = Z_NULL;
    
    // Test deflateInit_ with random level
    int level = 0;
    if (Size > 0) {
        level = Data[0] % 11; // 0-10, where 10 maps to Z_DEFAULT_COMPRESSION
        if (level == 10) level = Z_DEFAULT_COMPRESSION;
    }
    
    int ret = deflateInit_(&deflate_stream, level, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // Test deflateSetDictionary if we have data
        if (Size > 1) {
            size_t dict_size = Size - 1;
            if (dict_size > 0) {
                const Bytef* dictionary = (const Bytef*)(Data + 1);
                uInt dict_length = (uInt)(dict_size > 65535 ? 65535 : dict_size);
                deflateSetDictionary(&deflate_stream, dictionary, dict_length);
            }
        }
        
        deflateEnd(&deflate_stream);
    }
    
    // Test deflateInit2_ with more parameters
    z_stream deflate_stream2;
    memset(&deflate_stream2, 0, sizeof(z_stream));
    deflate_stream2.zalloc = custom_zalloc;
    deflate_stream2.zfree = custom_zfree;
    deflate_stream2.opaque = Z_NULL;
    
    if (Size >= 5) {
        int level2 = Data[1] % 11;
        if (level2 == 10) level2 = Z_DEFAULT_COMPRESSION;
        
        int method = Z_DEFLATED;
        int windowBits = 8 + (Data[2] % 8); // 8-15
        int memLevel = 1 + (Data[3] % 9);   // 1-9
        int strategy = Data[4] % 5;         // 0-4
        
        ret = deflateInit2_(&deflate_stream2, level2, method, windowBits, 
                           memLevel, strategy, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            deflateEnd(&deflate_stream2);
        }
    }
    
    // Test inflateInit_
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(z_stream));
    inflate_stream.zalloc = custom_zalloc;
    inflate_stream.zfree = custom_zfree;
    inflate_stream.opaque = Z_NULL;
    
    ret = inflateInit_(&inflate_stream, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        inflateEnd(&inflate_stream);
    }
    
    return 0;
}
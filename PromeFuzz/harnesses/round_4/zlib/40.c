// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// zlibVersion at zutil.c:27:22 in zlib.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static voidpf dummy_alloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    (void)items;
    (void)size;
    return NULL;
}

static void dummy_free(voidpf opaque, voidpf address) {
    (void)opaque;
    (void)address;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Test zlibCompileFlags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags;

    // Test zlibVersion
    const char* version = zlibVersion();
    (void)version;

    // Initialize z_stream for compression
    z_stream comp_stream;
    memset(&comp_stream, 0, sizeof(z_stream));
    
    // Use dummy allocators to avoid actual memory allocation
    comp_stream.zalloc = dummy_alloc;
    comp_stream.zfree = dummy_free;
    comp_stream.opaque = Z_NULL;

    // Test deflateInit_ with varying levels
    if (Size > 0) {
        int level = Data[0] % 12 - 1; // Range -1 to 10 (Z_DEFAULT_COMPRESSION is -1)
        int ret = deflateInit_(&comp_stream, level, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            // Test deflateSetDictionary if initialization succeeded
            if (Size > 1) {
                uInt dict_len = (uInt)(Size - 1);
                if (dict_len > 32768) dict_len = 32768; // Reasonable limit
                deflateSetDictionary(&comp_stream, Data + 1, dict_len);
            }
            deflateEnd(&comp_stream);
        }
    }

    // Test deflateInit2_ with various parameters
    if (Size > 4) {
        memset(&comp_stream, 0, sizeof(z_stream));
        comp_stream.zalloc = dummy_alloc;
        comp_stream.zfree = dummy_free;
        comp_stream.opaque = Z_NULL;

        int level = Data[0] % 12 - 1;
        int method = Z_DEFLATED;
        int windowBits = 8 + (Data[1] % 8); // 8-15 for zlib wrapper
        int memLevel = 1 + (Data[2] % 9);   // 1-9
        int strategy = Data[3] % 5;         // 0-4 for different strategies
        
        int ret = deflateInit2_(&comp_stream, level, method, windowBits, 
                               memLevel, strategy, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            deflateEnd(&comp_stream);
        }
    }

    // Test inflateInit_
    z_stream decomp_stream;
    memset(&decomp_stream, 0, sizeof(z_stream));
    decomp_stream.zalloc = dummy_alloc;
    decomp_stream.zfree = dummy_free;
    decomp_stream.opaque = Z_NULL;
    
    int inflate_ret = inflateInit_(&decomp_stream, ZLIB_VERSION, sizeof(z_stream));
    if (inflate_ret == Z_OK) {
        inflateEnd(&decomp_stream);
    }

    return 0;
}
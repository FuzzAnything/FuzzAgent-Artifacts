// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// zlibVersion at zutil.c:27:22 in zlib.h
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static voidpf dummy_alloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return malloc(items * size);
}

static void dummy_free(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Test zlibVersion and zlibCompileFlags */
    const char *version = zlibVersion();
    uLong flags = zlibCompileFlags();
    (void)version;
    (void)flags;

    /* Initialize z_stream structures */
    z_stream comp_stream;
    z_stream decomp_stream;
    
    /* Set up allocation functions */
    comp_stream.zalloc = dummy_alloc;
    comp_stream.zfree = dummy_free;
    comp_stream.opaque = Z_NULL;
    
    decomp_stream.zalloc = dummy_alloc;
    decomp_stream.zfree = dummy_free;
    decomp_stream.opaque = Z_NULL;

    /* Use first byte for level parameter */
    int level = Data[0] % 12; /* 0-11, covering valid levels + some invalid */
    
    /* Test deflateInit_ */
    int ret = deflateInit_(&comp_stream, level, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        /* Test deflateSetDictionary if we have enough data */
        if (Size > 1) {
            size_t dict_size = Size - 1;
            if (dict_size > 32768) dict_size = 32768; /* Reasonable limit */
            deflateSetDictionary(&comp_stream, Data + 1, (uInt)dict_size);
        }
        deflateEnd(&comp_stream);
    }

    /* Test inflateInit_ */
    ret = inflateInit_(&decomp_stream, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        inflateEnd(&decomp_stream);
    }

    /* Test deflateInit2_ with various parameters derived from input */
    if (Size >= 6) {
        /* Use input bytes to parameterize deflateInit2_ */
        int method = Z_DEFLATED;
        int windowBits = (Data[1] % 16) + 8; /* 8-23 */
        int memLevel = (Data[2] % 9) + 1; /* 1-9 */
        int strategy = Data[3] % 5; /* 0-4 */
        
        /* Reset stream */
        comp_stream.zalloc = dummy_alloc;
        comp_stream.zfree = dummy_free;
        comp_stream.opaque = Z_NULL;
        
        ret = deflateInit2_(&comp_stream, level, method, windowBits,
                           memLevel, strategy, ZLIB_VERSION, sizeof(z_stream));
        if (ret == Z_OK) {
            /* Test deflateSetDictionary again with different parameters */
            if (Size > 6) {
                size_t dict_size = Size - 6;
                if (dict_size > 32768) dict_size = 32768;
                deflateSetDictionary(&comp_stream, Data + 6, (uInt)dict_size);
            }
            deflateEnd(&comp_stream);
        }
    }

    return 0;
}
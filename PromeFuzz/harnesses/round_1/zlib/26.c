// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// zlibVersion at zutil.c:27:22 in zlib.h
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void test_inflateInit_(const uint8_t *data, size_t size) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    if (size > 0) {
        strm.next_in = (Bytef *)data;
        strm.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
    }
    
    int ret = inflateInit_(&strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        inflateEnd(&strm);
    }
}

static void test_deflateInit_(const uint8_t *data, size_t size) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    if (size > 0) {
        strm.next_in = (Bytef *)data;
        strm.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
    }
    
    int level = Z_DEFAULT_COMPRESSION;
    if (size > 0) {
        level = data[0] % 11 - 1;
    }
    
    int ret = deflateInit_(&strm, level, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        deflateEnd(&strm);
    }
}

static void test_deflateInit2_(const uint8_t *data, size_t size) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    if (size > 0) {
        strm.next_in = (Bytef *)data;
        strm.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
    }
    
    int level = Z_DEFAULT_COMPRESSION;
    int method = Z_DEFLATED;
    int windowBits = MAX_WBITS;
    int memLevel = MAX_MEM_LEVEL;
    int strategy = Z_DEFAULT_STRATEGY;
    
    if (size >= 5) {
        level = data[0] % 11 - 1;
        windowBits = (data[1] % 31) - 15;
        memLevel = (data[2] % 9) + 1;
        strategy = data[3] % 5;
    }
    
    int ret = deflateInit2_(&strm, level, method, windowBits, memLevel,
                           strategy, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        deflateEnd(&strm);
    }
}

static void test_deflateSetDictionary(const uint8_t *data, size_t size) {
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    int ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return;
    }
    
    if (size > 0) {
        uInt dictLength = (uInt)(size > UINT_MAX ? UINT_MAX : size);
        ret = deflateSetDictionary(&strm, data, dictLength);
    }
    
    deflateEnd(&strm);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    const char *version = zlibVersion();
    uLong flags = zlibCompileFlags();
    
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    if (fuzz_size == 0) {
        fuzz_data = Data;
        fuzz_size = Size;
    }
    
    switch (selector % 4) {
        case 0:
            test_inflateInit_(fuzz_data, fuzz_size);
            break;
        case 1:
            test_deflateInit_(fuzz_data, fuzz_size);
            break;
        case 2:
            test_deflateInit2_(fuzz_data, fuzz_size);
            break;
        case 3:
            test_deflateSetDictionary(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}
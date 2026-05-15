// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void cleanup_stream(z_streamp strm) {
    if (strm && strm->state) {
        deflateEnd(strm);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    int level = Data[0] % 10;
    if (level == 0) level = Z_DEFAULT_COMPRESSION;
    
    z_stream strm1;
    memset(&strm1, 0, sizeof(z_stream));
    strm1.zalloc = Z_NULL;
    strm1.zfree = Z_NULL;
    strm1.opaque = Z_NULL;
    
    int ret = deflateInit_(&strm1, level, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    uint8_t op_selector = Size > 1 ? Data[1] : 0;
    size_t data_offset = 2;

    if ((op_selector & 0x01) && strm1.state) {
        deflateReset(&strm1);
    }

    if ((op_selector & 0x02) && strm1.state) {
        int good_length = 8;
        int max_lazy = 8;
        int nice_length = 16;
        int max_chain = 16;
        
        if (Size >= data_offset + 4) {
            good_length = Data[data_offset] % 256;
            max_lazy = Data[data_offset + 1] % 256;
            nice_length = Data[data_offset + 2] % 256;
            max_chain = Data[data_offset + 3] % 256;
            data_offset += 4;
        }
        deflateTune(&strm1, good_length, max_lazy, nice_length, max_chain);
    }

    if ((op_selector & 0x04) && strm1.state) {
        deflateResetKeep(&strm1);
    }

    if ((op_selector & 0x08) && strm1.state) {
        size_t dict_size = Size - data_offset;
        if (dict_size > 0) {
            deflateReset(&strm1);
            deflateSetDictionary(&strm1, Data + data_offset, 
                                dict_size > 65535 ? 65535 : (uInt)dict_size);
        }
    }

    if ((op_selector & 0x10) && strm1.state) {
        z_stream strm2;
        memset(&strm2, 0, sizeof(z_stream));
        strm2.zalloc = Z_NULL;
        strm2.zfree = Z_NULL;
        strm2.opaque = Z_NULL;
        
        int copy_ret = deflateCopy(&strm2, &strm1);
        if (copy_ret == Z_OK) {
            deflateEnd(&strm2);
        }
    }

    cleanup_stream(&strm1);

    return 0;
}
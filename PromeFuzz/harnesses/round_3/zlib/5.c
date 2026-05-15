// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
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

static void init_stream(z_stream *strm) {
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

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    z_stream inflate_strm, deflate_strm;
    int ret;
    uLong crc_val;
    
    if (Size < 32) {
        return 0;
    }
    
    // Initialize inflation stream
    init_stream(&inflate_strm);
    ret = inflateInit2_(&inflate_strm, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Set up input buffer for inflation
    inflate_strm.next_in = (Bytef *)Data;
    inflate_strm.avail_in = Size > 1024 ? 1024 : (uInt)Size;
    
    // Allocate output buffer for inflation
    Bytef *inflate_out = (Bytef *)malloc(4096);
    if (!inflate_out) {
        inflateEnd(&inflate_strm);
        return 0;
    }
    inflate_strm.next_out = inflate_out;
    inflate_strm.avail_out = 4096;
    
    // First crc32
    crc_val = crc32(0L, Z_NULL, 0);
    crc_val = crc32(crc_val, Data, Size > 256 ? 256 : (uInt)Size);
    
    // Inflate
    ret = inflate(&inflate_strm, Z_NO_FLUSH);
    
    // Second crc32
    crc_val = crc32(crc_val, inflate_out, 4096 - inflate_strm.avail_out);
    
    // Clean up inflation
    inflateEnd(&inflate_strm);
    free(inflate_out);
    
    // Initialize deflation stream
    init_stream(&deflate_strm);
    ret = deflateInit2_(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       -15, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Set dictionary
    uInt dict_len = Size > 512 ? 512 : (uInt)Size;
    ret = deflateSetDictionary(&deflate_strm, Data, dict_len);
    
    // deflatePrime
    int bits = Size > 0 ? Data[0] % 17 : 0;
    int value = Size > 1 ? Data[1] : 0;
    ret = deflatePrime(&deflate_strm, bits, value);
    
    // Clean up deflation
    deflateEnd(&deflate_strm);
    
    return 0;
}
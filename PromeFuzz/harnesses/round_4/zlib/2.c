// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    z_stream inflate_stream;
    z_stream deflate_stream;
    int ret;
    
    /* Initialize inflate stream */
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    inflate_stream.zalloc = Z_NULL;
    inflate_stream.zfree = Z_NULL;
    inflate_stream.opaque = Z_NULL;
    
    /* Call inflateInit2_ */
    ret = inflateInit2_(&inflate_stream, 15, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) return 0;
    
    /* First crc32 call */
    uLong crc1 = crc32(0L, Z_NULL, 0);
    size_t crc_len = Size > 100 ? 100 : Size;
    crc1 = crc32(crc1, Data, crc_len);
    
    /* Set up inflate input */
    inflate_stream.next_in = (Bytef*)Data;
    inflate_stream.avail_in = Size;
    
    /* Allocate output buffer for inflate */
    Bytef out_buf[4096];
    inflate_stream.next_out = out_buf;
    inflate_stream.avail_out = sizeof(out_buf);
    
    /* Call inflate */
    ret = inflate(&inflate_stream, Z_NO_FLUSH);
    
    /* Second crc32 call on inflated data if any */
    uLong crc2 = crc32(0L, Z_NULL, 0);
    if (inflate_stream.total_out > 0) {
        crc2 = crc32(crc2, out_buf, inflate_stream.total_out > sizeof(out_buf) ? sizeof(out_buf) : (uInt)inflate_stream.total_out);
    }
    
    /* Call inflateEnd */
    inflateEnd(&inflate_stream);
    
    /* Initialize deflate stream */
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    deflate_stream.zalloc = Z_NULL;
    deflate_stream.zfree = Z_NULL;
    deflate_stream.opaque = Z_NULL;
    
    /* Call deflateInit2_ */
    ret = deflateInit2_(&deflate_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) return 0;
    
    /* Call deflateSetDictionary */
    size_t dict_size = Size > 50 ? 50 : Size;
    deflateSetDictionary(&deflate_stream, Data, dict_size);
    
    /* Call deflatePrime */
    int bits = Data[0] % 17;
    int value = Data[1];
    deflatePrime(&deflate_stream, bits, value);
    
    /* Clean up deflate stream */
    deflateEnd(&deflate_stream);
    
    return 0;
}
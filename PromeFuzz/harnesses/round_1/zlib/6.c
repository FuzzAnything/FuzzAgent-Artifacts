// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit2_ at inflate.c:173:13 in zlib.h
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

static voidpf dummy_alloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return malloc(items * size);
}

static void dummy_free(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 20) return 0;
    
    z_stream strm_inflate, strm_deflate;
    int ret;
    uLong crc_val;
    
    /* Initialize inflate stream */
    memset(&strm_inflate, 0, sizeof(z_stream));
    strm_inflate.zalloc = dummy_alloc;
    strm_inflate.zfree = dummy_free;
    
    /* Use first byte for windowBits parameter */
    int windowBits_inflate = (Data[0] % 32) + 8;
    ret = inflateInit2_(&strm_inflate, windowBits_inflate, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* First crc32 call */
    size_t crc_len1 = Size > 100 ? 100 : Size / 2;
    crc_val = crc32(0L, Z_NULL, 0);
    crc_val = crc32(crc_val, Data, crc_len1);
    
    /* Setup inflate input */
    strm_inflate.next_in = (Bytef*)Data;
    strm_inflate.avail_in = Size > 1024 ? 1024 : Size;
    
    /* Allocate output buffer for inflate */
    Bytef out_buf[4096];
    strm_inflate.next_out = out_buf;
    strm_inflate.avail_out = sizeof(out_buf);
    
    /* Inflate call */
    ret = inflate(&strm_inflate, Z_NO_FLUSH);
    
    /* Second crc32 call */
    size_t crc_len2 = Size > 200 ? 200 : Size / 3;
    if (crc_len2 > Size) crc_len2 = Size;
    crc_val = crc32(crc_val, Data + (Size - crc_len2), crc_len2);
    
    /* Cleanup inflate */
    inflateEnd(&strm_inflate);
    
    /* Initialize deflate stream */
    memset(&strm_deflate, 0, sizeof(z_stream));
    strm_deflate.zalloc = dummy_alloc;
    strm_deflate.zfree = dummy_free;
    
    /* Use second byte for windowBits parameter */
    int windowBits_deflate = (Data[1] % 16) + 8;
    int level = Data[2] % 10;
    int memLevel = (Data[3] % 9) + 1;
    int strategy = Data[4] % 5;
    
    ret = deflateInit2_(&strm_deflate, level, Z_DEFLATED, windowBits_deflate,
                       memLevel, strategy, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* deflateSetDictionary */
    size_t dict_len = Size > 512 ? 512 : Size / 2;
    if (dict_len > 0) {
        deflateSetDictionary(&strm_deflate, Data, dict_len);
    }
    
    /* deflatePrime */
    int bits = Data[5] % 17;
    int value = Data[6];
    deflatePrime(&strm_deflate, bits, value);
    
    /* Cleanup deflate */
    deflateEnd(&strm_deflate);
    
    return 0;
}
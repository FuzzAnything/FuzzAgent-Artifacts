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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK 16384

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    static uint8_t out[CHUNK];
    static uint8_t dict[CHUNK];
    z_stream inflate_strm;
    z_stream deflate_strm;
    int ret;
    uLong crc_val;
    
    if (Size < 4) return 0;
    
    /* Initialize inflate stream */
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    inflate_strm.avail_in = 0;
    inflate_strm.next_in = Z_NULL;
    
    /* 1. inflateInit2_ */
    ret = inflateInit2_(&inflate_strm, (int)Data[0] % 31 - 15, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* 2. First crc32 call */
    crc_val = crc32(0L, Z_NULL, 0);
    if (Size > 0) {
        crc_val = crc32(crc_val, Data, Size > 100 ? 100 : (uInt)Size);
    }
    
    /* Prepare input for inflate */
    inflate_strm.avail_in = Size > CHUNK ? CHUNK : (uInt)Size;
    inflate_strm.next_in = (Bytef*)Data;
    inflate_strm.avail_out = CHUNK;
    inflate_strm.next_out = out;
    
    /* 3. inflate */
    ret = inflate(&inflate_strm, Z_NO_FLUSH);
    
    /* 4. Second crc32 call */
    if (inflate_strm.total_out > 0) {
        crc_val = crc32(crc_val, out, inflate_strm.total_out > CHUNK ? CHUNK : (uInt)inflate_strm.total_out);
    }
    
    /* 5. inflateEnd */
    inflateEnd(&inflate_strm);
    
    /* Initialize deflate stream */
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    /* 6. deflateInit2_ */
    ret = deflateInit2_(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       -15, 8, Z_DEFAULT_STRATEGY,
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    /* Prepare dictionary */
    size_t dict_size = Size > CHUNK ? CHUNK : Size;
    memcpy(dict, Data, dict_size);
    
    /* 7. deflateSetDictionary */
    deflateSetDictionary(&deflate_strm, dict, dict_size > 65535 ? 65535 : (uInt)dict_size);
    
    /* 8. deflatePrime */
    if (Size >= 4) {
        int bits = Data[1] % 17;
        int value = Data[2] << 8 | Data[3];
        deflatePrime(&deflate_strm, bits, value);
    }
    
    /* Clean up deflate stream */
    deflateEnd(&deflate_strm);
    
    return 0;
}
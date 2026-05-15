// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void init_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    z_stream inflate_strm, deflate_strm;
    int ret;
    
    /* Initialize streams */
    init_stream(&inflate_strm);
    init_stream(&deflate_strm);
    
    /* Extract parameters from fuzz input */
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    /* Prepare dictionary data if available */
    uInt dict_len = 0;
    const Bytef* dict_data = NULL;
    if (offset < Size) {
        dict_len = (uInt)Data[offset] % 256;
        offset++;
        if (dict_len > 0 && offset + dict_len <= Size) {
            dict_data = (const Bytef*)(Data + offset);
            offset += dict_len;
        } else {
            dict_len = 0;
        }
    }
    
    /* Prepare additional parameters */
    int windowBits = (selector & 0x0F) + 8;  /* 8-23 */
    int bits = (selector >> 4) & 0x0F;       /* 0-15 */
    int value = selector;
    int undermine = selector & 1;
    
    /* Test inflateInit2_ */
    ret = inflateInit2_(&inflate_strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        return 0;
    }
    
    /* Test deflateInit2 to have a deflate stream */
    ret = deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                      windowBits, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        return 0;
    }
    
    /* Test inflateSetDictionary if we have dictionary data */
    if (dict_len > 0) {
        inflateSetDictionary(&inflate_strm, dict_data, dict_len);
    }
    
    /* Test inflatePrime */
    inflatePrime(&inflate_strm, bits, value);
    
    /* Test inflateUndermine */
    inflateUndermine(&inflate_strm, undermine);
    
    /* Test inflateGetDictionary */
    uInt get_dict_len = 0;
    Bytef get_dict_buf[32768];  /* Maximum window size */
    if (selector % 2 == 0) {
        get_dict_len = sizeof(get_dict_buf);
        inflateGetDictionary(&inflate_strm, get_dict_buf, &get_dict_len);
    } else {
        inflateGetDictionary(&inflate_strm, NULL, &get_dict_len);
    }
    
    /* Test deflateGetDictionary */
    uInt deflate_dict_len = 0;
    Bytef deflate_dict_buf[32768];
    if (selector % 3 == 0) {
        deflate_dict_len = sizeof(deflate_dict_buf);
        deflateGetDictionary(&deflate_strm, deflate_dict_buf, &deflate_dict_len);
    } else {
        deflateGetDictionary(&deflate_strm, NULL, &deflate_dict_len);
    }
    
    /* Clean up */
    inflateEnd(&inflate_strm);
    deflateEnd(&deflate_strm);
    
    return 0;
}
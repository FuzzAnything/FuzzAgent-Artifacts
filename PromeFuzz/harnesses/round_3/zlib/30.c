// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
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

static void init_z_stream(z_streamp strm) {
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

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    z_stream deflate_strm;
    z_stream inflate_strm;
    int ret;
    uInt dict_len;
    Bytef dict_buf[32768]; /* Maximum window size */
    const char *version = ZLIB_VERSION;
    
    /* Initialize deflate stream */
    init_z_stream(&deflate_strm);
    ret = deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       (Data[0] % 15) + 8, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        deflateEnd(&deflate_strm);
        return 0;
    }
    
    /* Test deflateGetDictionary */
    dict_len = sizeof(dict_buf);
    deflateGetDictionary(&deflate_strm, dict_buf, &dict_len);
    
    /* Clean up deflate stream */
    deflateEnd(&deflate_strm);
    
    /* Initialize inflate stream with inflateInit2_ */
    init_z_stream(&inflate_strm);
    ret = inflateInit2_(&inflate_strm, 
                       (Size > 1 ? Data[1] : 0) % 15 + 8, 
                       version, 
                       sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        return 0;
    }
    
    /* Test inflateSetDictionary if we have enough data */
    if (Size > 8) {
        uInt set_dict_len = Size > 32768 ? 32768 : (uInt)Size;
        /* Set adler to match the dictionary for potential success */
        inflate_strm.adler = adler32(0L, Data, set_dict_len);
        inflateSetDictionary(&inflate_strm, Data, set_dict_len);
    }
    
    /* Test inflateUndermine */
    if (Size > 2) {
        inflateUndermine(&inflate_strm, Data[2] & 1);
    }
    
    /* Test inflateGetDictionary */
    dict_len = sizeof(dict_buf);
    inflateGetDictionary(&inflate_strm, dict_buf, &dict_len);
    
    /* Test inflatePrime with bounds checking */
    if (Size > 4) {
        int bits = Data[3] % 17;
        int value = Data[4] & 0xFFFF;
        inflatePrime(&inflate_strm, bits, value);
    } else if (Size > 3) {
        int bits = Data[3] % 17;
        inflatePrime(&inflate_strm, bits, 0);
    }
    
    /* Clean up inflate stream */
    inflateEnd(&inflate_strm);
    
    return 0;
}
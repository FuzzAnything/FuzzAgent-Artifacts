// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_z_stream(z_stream *strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    z_stream deflate_strm, inflate_strm;
    int ret;
    Bytef dict_buf[32768];
    uInt dict_len;
    Bytef out_buf[4096];

    /* Initialize deflate stream */
    initialize_z_stream(&deflate_strm);
    ret = deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) return 0;

    /* Initialize inflate stream */
    initialize_z_stream(&inflate_strm);
    ret = inflateInit2(&inflate_strm, -MAX_WBITS);
    if (ret != Z_OK) {
        deflateEnd(&deflate_strm);
        return 0;
    }

    /* Use first part of data as dictionary */
    size_t dict_size = Size / 4;
    if (dict_size > sizeof(dict_buf)) dict_size = sizeof(dict_buf);
    
    /* Test inflateSetDictionary */
    if (dict_size > 0) {
        inflate_strm.adler = adler32(0L, Z_NULL, 0);
        inflate_strm.adler = adler32(inflate_strm.adler, Data, dict_size);
        ret = inflateSetDictionary(&inflate_strm, Data, dict_size);
        /* Ignore return value - we're fuzzing */
    }

    /* Test deflateGetDictionary */
    dict_len = sizeof(dict_buf);
    ret = deflateGetDictionary(&deflate_strm, dict_buf, &dict_len);

    /* Test inflateGetDictionary */
    dict_len = sizeof(dict_buf);
    ret = inflateGetDictionary(&inflate_strm, dict_buf, &dict_len);

    /* Test inflatePrime with bits from data */
    if (Size > dict_size + 2) {
        int bits = Data[dict_size] & 0x0F; /* 0-15 bits */
        int value = Data[dict_size + 1];
        ret = inflatePrime(&inflate_strm, bits, value);
    }

    /* Test inflateUndermine */
    if (Size > dict_size + 3) {
        int undermine = Data[dict_size + 2] & 1;
        ret = inflateUndermine(&inflate_strm, undermine);
    }

    /* Test inflate with some compressed data */
    if (Size > dict_size + 100) {
        /* Create a simple deflated buffer to test inflate */
        z_stream tmp_strm;
        initialize_z_stream(&tmp_strm);
        deflateInit2(&tmp_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     -MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
        
        tmp_strm.next_in = (Bytef*)(Data + dict_size);
        tmp_strm.avail_in = Size - dict_size > 100 ? 100 : Size - dict_size;
        tmp_strm.next_out = out_buf;
        tmp_strm.avail_out = sizeof(out_buf);
        
        deflate(&tmp_strm, Z_FINISH);
        deflateEnd(&tmp_strm);
        
        /* Now inflate the compressed data */
        inflate_strm.next_in = out_buf;
        inflate_strm.avail_in = sizeof(out_buf) - tmp_strm.avail_out;
        inflate_strm.next_out = dict_buf;
        inflate_strm.avail_out = sizeof(dict_buf);
        
        ret = inflate(&inflate_strm, Z_NO_FLUSH);
        /* Try different flush modes */
        if (ret == Z_OK || ret == Z_BUF_ERROR) {
            ret = inflate(&inflate_strm, Z_SYNC_FLUSH);
        }
    }

    /* Cleanup */
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);

    return 0;
}
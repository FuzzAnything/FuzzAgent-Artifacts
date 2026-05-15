// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
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

static void init_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    z_stream deflate_strm, inflate_strm;
    int ret;
    uInt dict_len;
    Bytef *dictionary;
    
    /* Initialize streams */
    init_stream(&deflate_strm);
    init_stream(&inflate_strm);
    
    /* Initialize deflate */
    ret = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 0;
    
    /* Initialize inflate */
    ret = inflateInit(&inflate_strm);
    if (ret != Z_OK) {
        deflateEnd(&deflate_strm);
        return 0;
    }
    
    /* Allocate dictionary buffer */
    dict_len = (Size > 32768) ? 32768 : (uInt)Size;
    dictionary = (Bytef*)malloc(dict_len);
    if (!dictionary) {
        deflateEnd(&deflate_strm);
        inflateEnd(&inflate_strm);
        return 0;
    }
    
    /* Use first part of input as dictionary data */
    memcpy(dictionary, Data, dict_len);
    
    /* Test deflateGetDictionary */
    uInt get_dict_len = dict_len;
    ret = deflateGetDictionary(&deflate_strm, dictionary, &get_dict_len);
    /* Ignore return value - just testing the call */
    
    /* Test inflateSetDictionary */
    ret = inflateSetDictionary(&inflate_strm, dictionary, dict_len);
    /* Ignore return value - just testing the call */
    
    /* Test inflateGetDictionary */
    get_dict_len = dict_len;
    ret = inflateGetDictionary(&inflate_strm, dictionary, &get_dict_len);
    /* Ignore return value - just testing the call */
    
    /* Test inflatePrime with bits from input */
    int bits = (Size > 1) ? (Data[0] & 0x0F) : 0;  /* 0-15 bits */
    int value = (Size > 2) ? Data[1] : 0;
    ret = inflatePrime(&inflate_strm, bits, value);
    /* Ignore return value - just testing the call */
    
    /* Test inflateUndermine */
    int undermine = (Size > 3) ? (Data[2] & 1) : 0;
    ret = inflateUndermine(&inflate_strm, undermine);
    /* Ignore return value - just testing the call */
    
    /* Prepare some data for inflate */
    Bytef compressed[1024];
    Bytef uncompressed[1024];
    z_stream tmp_strm;
    
    init_stream(&tmp_strm);
    if (deflateInit(&tmp_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        tmp_strm.next_in = dictionary;
        tmp_strm.avail_in = dict_len > 100 ? 100 : dict_len;
        tmp_strm.next_out = compressed;
        tmp_strm.avail_out = sizeof(compressed);
        
        deflate(&tmp_strm, Z_FINISH);
        deflateEnd(&tmp_strm);
        
        /* Test inflate with the compressed data */
        inflate_strm.next_in = compressed;
        inflate_strm.avail_in = tmp_strm.total_out;
        inflate_strm.next_out = uncompressed;
        inflate_strm.avail_out = sizeof(uncompressed);
        
        ret = inflate(&inflate_strm, Z_NO_FLUSH);
        /* Ignore return value - just testing the call */
    }
    
    /* Cleanup */
    free(dictionary);
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    
    return 0;
}
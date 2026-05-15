// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// compress at compress.c:82:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// compress at compress.c:82:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
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

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    z_stream deflate_strm, inflate_strm;
    int ret;
    
    /* Initialize deflate stream */
    init_stream(&deflate_strm);
    ret = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 0;
    
    /* Initialize inflate stream */
    init_stream(&inflate_strm);
    ret = inflateInit(&inflate_strm);
    if (ret != Z_OK) {
        deflateEnd(&deflate_strm);
        return 0;
    }
    
    /* Split input data for different purposes */
    size_t dict_size = (Data[0] << 8) | Data[1];
    dict_size = dict_size % (Size / 2);
    if (dict_size == 0) dict_size = 1;
    
    size_t remaining = Size - dict_size;
    if (remaining < 8) {
        deflateEnd(&deflate_strm);
        inflateEnd(&inflate_strm);
        return 0;
    }
    
    /* Test deflateGetDictionary */
    {
        Bytef dict_buf[32768];
        uInt dict_len = sizeof(dict_buf);
        deflate_strm.next_in = (Bytef*)(Data + dict_size);
        deflate_strm.avail_in = remaining;
        
        /* Compress some data to populate dictionary */
        Bytef out_buf[1024];
        deflate_strm.next_out = out_buf;
        deflate_strm.avail_out = sizeof(out_buf);
        deflate(&deflate_strm, Z_FINISH);
        deflateReset(&deflate_strm);
        
        ret = deflateGetDictionary(&deflate_strm, dict_buf, &dict_len);
        (void)ret; /* Use result to avoid unused variable warning */
    }
    
    /* Test inflateSetDictionary */
    {
        /* Create a simple compressed data buffer for testing */
        unsigned char compressed[1024];
        uLongf comp_len = sizeof(compressed);
        
        /* Compress some data to use as dictionary */
        if (compress(compressed, &comp_len, Data + dict_size, remaining) == Z_OK) {
            /* Set dictionary for inflate stream */
            ret = inflateSetDictionary(&inflate_strm, Data, dict_size);
            (void)ret;
        }
    }
    
    /* Test inflateGetDictionary */
    {
        Bytef dict_buf[32768];
        uInt dict_len = sizeof(dict_buf);
        ret = inflateGetDictionary(&inflate_strm, dict_buf, &dict_len);
        (void)ret;
    }
    
    /* Test inflatePrime */
    {
        int bits = Data[2] % 17; /* 0-16 bits */
        int value = Data[3];
        ret = inflatePrime(&inflate_strm, bits, value);
        (void)ret;
        
        /* Test with negative bits to clear buffer */
        if (Data[4] % 2) {
            inflatePrime(&inflate_strm, -8, 0);
        }
    }
    
    /* Test inflate */
    {
        /* Create a simple compressed stream */
        unsigned char compressed[1024];
        uLongf comp_len = sizeof(compressed);
        
        if (compress(compressed, &comp_len, Data + dict_size, remaining) == Z_OK) {
            inflate_strm.next_in = compressed;
            inflate_strm.avail_in = comp_len;
            
            unsigned char out_buf[2048];
            inflate_strm.next_out = out_buf;
            inflate_strm.avail_out = sizeof(out_buf);
            
            ret = inflate(&inflate_strm, Z_NO_FLUSH);
            (void)ret;
            
            /* Try different flush modes */
            if (Data[5] % 2) {
                inflate(&inflate_strm, Z_SYNC_FLUSH);
            }
            if (Data[6] % 2) {
                inflate(&inflate_strm, Z_FINISH);
            }
            
            inflateReset(&inflate_strm);
        }
    }
    
    /* Test inflateUndermine */
    {
        int undermine = Data[7] % 2;
        ret = inflateUndermine(&inflate_strm, undermine);
        (void)ret;
    }
    
    /* Cleanup */
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    
    return 0;
}
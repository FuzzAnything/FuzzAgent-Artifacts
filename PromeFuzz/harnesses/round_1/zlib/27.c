// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
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
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    z_stream deflate_strm;
    z_stream inflate_strm;
    gz_header header;
    int ret;
    
    /* Test 1: deflateSetHeader */
    {
        init_z_stream(&deflate_strm);
        ret = deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
        if (ret == Z_OK) {
            memset(&header, 0, sizeof(header));
            
            /* Set some header fields from fuzz data with bounds checking */
            header.text = Data[0] & 1;
            
            /* Safely read time field */
            uLong time_val = 0;
            if (Size >= 1) time_val |= (uLong)Data[0] << 24;
            if (Size >= 2) time_val |= (uLong)Data[1] << 16;
            if (Size >= 3) time_val |= (uLong)Data[2] << 8;
            if (Size >= 4) time_val |= (uLong)Data[3];
            header.time = time_val;
            
            if (Size >= 5) header.xflags = Data[4];
            if (Size >= 6) header.os = Data[5];
            
            if (Size > 7) {
                size_t extra_len = Data[6] % 256;
                if (extra_len > 0 && Size > 7 + extra_len) {
                    header.extra = (Bytef*)(Data + 7);
                    header.extra_len = (uInt)extra_len;
                }
            }
            
            deflateSetHeader(&deflate_strm, &header);
            deflateEnd(&deflate_strm);
        }
    }
    
    /* Test 2 & 5 & 6: inflate, inflatePrime, inflateReset2 */
    {
        init_z_stream(&inflate_strm);
        
        /* Initialize with different windowBits values */
        int windowBits = MAX_WBITS;
        if (Size >= 1) {
            int choice = Data[0] % 3;
            if (choice == 0) windowBits = MAX_WBITS + 16;
            else if (choice == 1) windowBits = MAX_WBITS;
            else windowBits = -MAX_WBITS;
        }
        
        ret = inflateInit2(&inflate_strm, windowBits);
        if (ret == Z_OK) {
            /* Test inflatePrime */
            int bits = 0;
            int value = 0;
            if (Size >= 1) bits = Data[0] % 17;  /* Bits must be ≤16 */
            if (Size >= 2) value = Data[1];
            inflatePrime(&inflate_strm, bits, value);
            
            /* Test inflateGetHeader */
            memset(&header, 0, sizeof(header));
            inflateGetHeader(&inflate_strm, &header);
            
            /* Prepare input for inflate */
            Bytef *in_data = (Bytef*)malloc(Size);
            if (in_data) {
                memcpy(in_data, Data, Size);
                inflate_strm.next_in = in_data;
                inflate_strm.avail_in = (uInt)(Size > UINT_MAX ? UINT_MAX : Size);
                
                /* Prepare output buffer */
                Bytef out_buf[1024];
                inflate_strm.next_out = out_buf;
                inflate_strm.avail_out = sizeof(out_buf);
                
                /* Test inflate with different flush modes */
                int flush = Z_NO_FLUSH;
                if (Size >= 1) {
                    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FINISH};
                    flush = flush_modes[Data[0] % 3];
                }
                inflate(&inflate_strm, flush);
                
                /* Test inflateSetDictionary */
                if (Size > 100) {
                    uInt dict_len = 0;
                    if (Size >= 1) dict_len = (uInt)(Data[0] % 100);
                    if (dict_len > 0 && Size >= 50 + dict_len) {
                        inflateSetDictionary(&inflate_strm, Data + 50, dict_len);
                    }
                }
                
                /* Test inflateReset2 with different windowBits */
                int new_windowBits = MAX_WBITS;
                if (Size >= 2) {
                    int choice = Data[1] % 3;
                    if (choice == 0) new_windowBits = MAX_WBITS + 32;
                    else if (choice == 1) new_windowBits = MAX_WBITS + 8;
                    else new_windowBits = MAX_WBITS;
                }
                inflateReset2(&inflate_strm, new_windowBits);
                
                free(in_data);
            }
            
            inflateEnd(&inflate_strm);
        }
    }
    
    /* Test 3: inflateSetDictionary with separate stream */
    {
        z_stream dict_strm;
        init_z_stream(&dict_strm);
        
        ret = inflateInit(&dict_strm);
        if (ret == Z_OK) {
            if (Size >= 5) {
                uInt dict_len = 0;
                if (Size >= 1) dict_len = (uInt)(Data[0] % 20);
                if (dict_len > 0 && Size >= 5 + dict_len) {
                    /* Use Adler-32 from data if available */
                    uLong adler_val = 0;
                    if (Size >= 1) adler_val |= (uLong)Data[1] << 24;
                    if (Size >= 2) adler_val |= (uLong)Data[2] << 16;
                    if (Size >= 3) adler_val |= (uLong)Data[3] << 8;
                    if (Size >= 4) adler_val |= (uLong)Data[4];
                    dict_strm.adler = adler_val;
                    inflateSetDictionary(&dict_strm, Data + 5, dict_len);
                }
            }
            inflateEnd(&dict_strm);
        }
    }
    
    return 0;
}
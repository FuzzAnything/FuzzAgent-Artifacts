// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_z_stream(z_stream *strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Split input for various parameters */
    size_t dict_size = Size > 256 ? 256 : Size;
    size_t remaining = Size - dict_size;
    if (remaining < 1) return 0;
    
    /* Create dummy file if needed */
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    /* Test deflateGetDictionary */
    {
        z_stream strm;
        init_z_stream(&strm);
        
        /* Initialize deflate stream */
        if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                         MAX_WBITS, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
            
            /* Provide some input data */
            strm.next_in = (Bytef *)Data;
            strm.avail_in = dict_size > 1024 ? 1024 : dict_size;
            
            /* Compress a little to populate state */
            Bytef out_buf[1024];
            strm.next_out = out_buf;
            strm.avail_out = sizeof(out_buf);
            deflate(&strm, Z_SYNC_FLUSH);
            
            /* Now test target function */
            Bytef dictionary[32768];
            uInt dictLength = sizeof(dictionary);
            deflateGetDictionary(&strm, dictionary, &dictLength);
            
            deflateEnd(&strm);
        }
    }
    
    /* Test inflateSetDictionary and inflateGetDictionary */
    {
        z_stream strm;
        init_z_stream(&strm);
        
        /* Initialize inflate stream */
        if (inflateInit2(&strm, MAX_WBITS) == Z_OK) {
            Bytef dictionary[32768];
            uInt dictLength = dict_size;
            
            /* Copy dictionary data */
            if (dictLength > sizeof(dictionary)) dictLength = sizeof(dictionary);
            memcpy(dictionary, Data, dictLength);
            
            /* Test inflateSetDictionary */
            inflateSetDictionary(&strm, dictionary, dictLength);
            
            /* Test inflateGetDictionary */
            Bytef out_dict[32768];
            uInt out_len = sizeof(out_dict);
            inflateGetDictionary(&strm, out_dict, &out_len);
            
            inflateEnd(&strm);
        }
    }
    
    /* Test inflate with various flush modes */
    {
        z_stream strm;
        init_z_stream(&strm);
        
        /* Create a simple deflated stream for testing */
        z_stream deflate_strm;
        init_z_stream(&deflate_strm);
        
        if (deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                         MAX_WBITS, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
            
            Bytef compressed[1024];
            deflate_strm.next_in = (Bytef *)Data;
            deflate_strm.avail_in = dict_size > 512 ? 512 : dict_size;
            deflate_strm.next_out = compressed;
            deflate_strm.avail_out = sizeof(compressed);
            
            deflate(&deflate_strm, Z_FINISH);
            deflateEnd(&deflate_strm);
            
            /* Now test inflate */
            if (inflateInit2(&strm, MAX_WBITS) == Z_OK) {
                strm.next_in = compressed;
                strm.avail_in = deflate_strm.total_out;
                
                Bytef out_buf[1024];
                strm.next_out = out_buf;
                strm.avail_out = sizeof(out_buf);
                
                /* Test with different flush modes based on input byte */
                int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH, Z_BLOCK};
                int flush = flush_modes[Data[0] % 5];
                
                inflate(&strm, flush);
                inflateEnd(&strm);
            }
        }
    }
    
    /* Test inflateUndermine */
    {
        z_stream strm;
        init_z_stream(&strm);
        
        if (inflateInit2(&strm, MAX_WBITS) == Z_OK) {
            /* Use input byte to determine undermine value (0 or 1) */
            int undermine = Data[0] & 1;
            inflateUndermine(&strm, undermine);
            inflateEnd(&strm);
        }
    }
    
    /* Test inflatePrime */
    {
        z_stream strm;
        init_z_stream(&strm);
        
        /* Initialize for raw inflate */
        if (inflateInit2(&strm, -MAX_WBITS) == Z_OK) {
            /* Extract bits and value from input data */
            int bits = (Data[0] & 0x0F);  /* 0-15 bits */
            int value = (Data[0] >> 4) & 0x0F;
            
            /* Test inflatePrime */
            inflatePrime(&strm, bits, value);
            
            /* Also test with negative bits to clear buffer */
            if (Size > 1 && (Data[1] & 1)) {
                inflatePrime(&strm, -1, 0);
            }
            
            inflateEnd(&strm);
        }
    }
    
    return 0;
}
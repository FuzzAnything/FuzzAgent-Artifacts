// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateUndermine at inflate.c:1370:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateReset at inflate.c:125:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
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

static void init_z_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    z_stream inflate_strm, deflate_strm;
    int ret;
    
    /* Initialize inflate stream */
    init_z_stream(&inflate_strm);
    ret = inflateInit(&inflate_strm);
    if (ret != Z_OK) return 0;
    
    /* Initialize deflate stream */
    init_z_stream(&deflate_strm);
    ret = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        return 0;
    }
    
    /* Prepare buffers and variables */
    Bytef dictionary[32768];
    uInt dictLength;
    int bits, value, undermine;
    
    /* Use first byte to determine operation mode */
    uint8_t mode = Data[0] % 6;
    size_t offset = 1;
    
    /* Set up input data for streams */
    if (Size > offset) {
        size_t input_size = Size - offset;
        Bytef* input_copy = (Bytef*)malloc(input_size);
        if (!input_copy) goto cleanup;
        
        memcpy(input_copy, Data + offset, input_size);
        inflate_strm.next_in = input_copy;
        inflate_strm.avail_in = (uInt)input_size;
        
        deflate_strm.next_in = input_copy;
        deflate_strm.avail_in = (uInt)input_size;
        
        /* Allocate output buffers */
        Bytef out_buf[4096];
        inflate_strm.next_out = out_buf;
        inflate_strm.avail_out = sizeof(out_buf);
        
        Bytef deflate_out[4096];
        deflate_strm.next_out = deflate_out;
        deflate_strm.avail_out = sizeof(deflate_out);
        
        switch (mode) {
            case 0: /* Test inflateGetDictionary */
                dictLength = sizeof(dictionary);
                inflateGetDictionary(&inflate_strm, dictionary, &dictLength);
                break;
                
            case 1: /* Test inflate */
                inflate(&inflate_strm, Z_NO_FLUSH);
                break;
                
            case 2: /* Test inflateSetDictionary */
                if (Size > offset + 4) {
                    dictLength = (Data[offset] << 8) | Data[offset + 1];
                    if (dictLength > sizeof(dictionary)) dictLength = sizeof(dictionary);
                    if (offset + 2 + dictLength <= Size) {
                        memcpy(dictionary, Data + offset + 2, dictLength);
                        inflateSetDictionary(&inflate_strm, dictionary, dictLength);
                    }
                }
                break;
                
            case 3: /* Test inflatePrime */
                if (Size > offset + 2) {
                    bits = Data[offset] & 0x1F; /* Max 16 bits */
                    value = Data[offset + 1];
                    inflatePrime(&inflate_strm, bits, value);
                }
                break;
                
            case 4: /* Test inflateUndermine */
                if (Size > offset) {
                    undermine = Data[offset] & 1;
                    inflateUndermine(&inflate_strm, undermine);
                }
                break;
                
            case 5: /* Test deflateGetDictionary */
                dictLength = sizeof(dictionary);
                deflateGetDictionary(&deflate_strm, dictionary, &dictLength);
                break;
        }
        
        /* Try some additional operations in various states */
        if (mode != 1) {
            /* Try inflate with different flush modes */
            inflate(&inflate_strm, Z_SYNC_FLUSH);
            inflate(&inflate_strm, Z_FINISH);
        }
        
        /* Reset and try different combinations */
        inflateReset(&inflate_strm);
        deflateReset(&deflate_strm);
        
        free(input_copy);
    }
    
cleanup:
    inflateEnd(&inflate_strm);
    deflateEnd(&deflate_strm);
    
    return 0;
}
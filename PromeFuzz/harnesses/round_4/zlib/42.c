// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
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

static void initialize_z_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->avail_in = 0;
    strm->next_in = Z_NULL;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    z_stream deflate_strm, inflate_strm;
    int ret;
    
    // Initialize deflate stream
    initialize_z_stream(&deflate_strm);
    ret = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 0;
    
    // Initialize inflate stream
    initialize_z_stream(&inflate_strm);
    ret = inflateInit(&inflate_strm);
    if (ret != Z_OK) {
        deflateEnd(&deflate_strm);
        return 0;
    }
    
    // Use first byte to determine operation mode
    uint8_t mode = Data[0] % 6;
    size_t data_offset = 1;
    
    // Prepare buffers
    size_t dict_len = (Size > data_offset + 4) ? 
        (Data[data_offset] % 256) + 1 : 32;
    data_offset++;
    
    if (dict_len > Size - data_offset) {
        dict_len = Size - data_offset;
    }
    
    Bytef* dictionary = (Bytef*)malloc(dict_len);
    if (!dictionary) {
        deflateEnd(&deflate_strm);
        inflateEnd(&inflate_strm);
        return 0;
    }
    
    if (dict_len > 0 && data_offset < Size) {
        memcpy(dictionary, Data + data_offset, dict_len);
        data_offset += dict_len;
    }
    
    uInt actual_dict_len = 0;
    
    switch (mode) {
        case 0: // inflateGetDictionary
            if (data_offset < Size) {
                actual_dict_len = (Data[data_offset] % 256) + 1;
                Bytef* out_dict = (Bytef*)malloc(actual_dict_len);
                if (out_dict) {
                    inflateGetDictionary(&inflate_strm, out_dict, &actual_dict_len);
                    free(out_dict);
                }
            }
            break;
            
        case 1: // deflateGetDictionary
            if (data_offset < Size) {
                actual_dict_len = (Data[data_offset] % 256) + 1;
                Bytef* out_dict = (Bytef*)malloc(actual_dict_len);
                if (out_dict) {
                    deflateGetDictionary(&deflate_strm, out_dict, &actual_dict_len);
                    free(out_dict);
                }
            }
            break;
            
        case 2: // inflateSetDictionary
            if (dict_len > 0) {
                inflateSetDictionary(&inflate_strm, dictionary, dict_len);
            }
            break;
            
        case 3: // inflatePrime
            if (data_offset + 2 <= Size) {
                int bits = (Data[data_offset] % 17); // bits ≤ 16
                int value = Data[data_offset + 1];
                inflatePrime(&inflate_strm, bits, value);
            }
            break;
            
        case 4: // inflate
            if (data_offset < Size) {
                int flush = Data[data_offset] % 4; // Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_FINISH
                
                // Prepare some input data for inflation
                size_t in_size = (Size - data_offset > 1024) ? 1024 : Size - data_offset;
                Bytef* in_data = (Bytef*)malloc(in_size);
                Bytef* out_data = (Bytef*)malloc(4096);
                
                if (in_data && out_data && in_size > 0) {
                    memcpy(in_data, Data + data_offset, in_size);
                    inflate_strm.next_in = in_data;
                    inflate_strm.avail_in = in_size;
                    inflate_strm.next_out = out_data;
                    inflate_strm.avail_out = 4096;
                    
                    inflate(&inflate_strm, flush);
                    
                    free(in_data);
                    free(out_data);
                } else {
                    if (in_data) free(in_data);
                    if (out_data) free(out_data);
                }
            }
            break;
            
        case 5: // inflateUndermine
            if (data_offset < Size) {
                int subvert = Data[data_offset] % 2;
                inflateUndermine(&inflate_strm, subvert);
            }
            break;
    }
    
    // Cleanup
    free(dictionary);
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    
    return 0;
}
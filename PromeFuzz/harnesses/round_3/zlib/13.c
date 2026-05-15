// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSync at inflate.c:1264:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
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

static void init_z_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->avail_in = 0;
    strm->next_in = Z_NULL;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    z_stream strm;
    init_z_stream(&strm);
    
    int windowBits = (Data[0] % 31) - 15; // Range: -15..15
    if (windowBits == 0) windowBits = 15;
    
    int ret = inflateInit2(&strm, windowBits);
    if (ret != Z_OK) {
        return 0;
    }

    // Use first byte to determine which function to test
    uint8_t func_selector = Data[1] % 6;
    size_t data_offset = 2;

    switch (func_selector) {
        case 0: { // inflateSetDictionary
            if (Size - data_offset < 4) break;
            
            uInt dictLength = (Data[data_offset] << 8) | Data[data_offset + 1];
            dictLength = dictLength % 1024;
            data_offset += 2;
            
            if (Size - data_offset < dictLength) {
                dictLength = Size - data_offset;
            }
            
            if (dictLength > 0) {
                strm.adler = adler32(0L, Z_NULL, 0);
                strm.adler = adler32(strm.adler, Data + data_offset, dictLength);
                inflateSetDictionary(&strm, Data + data_offset, dictLength);
            }
            break;
        }
        
        case 1: { // inflateReset2
            if (Size - data_offset < 1) break;
            
            int new_windowBits = (Data[data_offset] % 31) - 15;
            if (new_windowBits == 0) new_windowBits = 15;
            data_offset++;
            
            inflateReset2(&strm, new_windowBits);
            break;
        }
        
        case 2: { // inflate
            if (Size - data_offset < 1) break;
            
            int flush_mode = Data[data_offset] % 5;
            data_offset++;
            
            // Set up input buffer
            size_t inflate_data_size = Size - data_offset;
            if (inflate_data_size > CHUNK) {
                inflate_data_size = CHUNK;
            }
            
            Bytef *in_buf = (Bytef*)malloc(inflate_data_size);
            if (!in_buf) break;
            
            memcpy(in_buf, Data + data_offset, inflate_data_size);
            strm.next_in = in_buf;
            strm.avail_in = (uInt)inflate_data_size;
            
            // Set up output buffer
            Bytef out_buf[CHUNK];
            strm.next_out = out_buf;
            strm.avail_out = CHUNK;
            
            int flush;
            switch (flush_mode) {
                case 0: flush = Z_NO_FLUSH; break;
                case 1: flush = Z_SYNC_FLUSH; break;
                case 2: flush = Z_FULL_FLUSH; break;
                case 3: flush = Z_FINISH; break;
                case 4: flush = Z_BLOCK; break;
                default: flush = Z_NO_FLUSH;
            }
            
            inflate(&strm, flush);
            
            free(in_buf);
            break;
        }
        
        case 3: { // inflateGetHeader
            gz_header header;
            memset(&header, 0, sizeof(header));
            
            // Allocate space for header fields
            if (Size - data_offset > 0) {
                header.extra_max = 256;
                header.name_max = 256;
                header.comm_max = 256;
                
                header.extra = (Bytef*)malloc(header.extra_max);
                header.name = (Bytef*)malloc(header.name_max);
                header.comment = (Bytef*)malloc(header.comm_max);
                
                if (header.extra && header.name && header.comment) {
                    inflateGetHeader(&strm, &header);
                }
                
                free(header.extra);
                free(header.name);
                free(header.comment);
            }
            break;
        }
        
        case 4: { // inflateSync
            // First try to inflate some data to potentially get into error state
            if (Size - data_offset > 0) {
                size_t sync_data_size = Size - data_offset;
                if (sync_data_size > CHUNK) {
                    sync_data_size = CHUNK;
                }
                
                Bytef *in_buf = (Bytef*)malloc(sync_data_size);
                if (!in_buf) break;
                
                memcpy(in_buf, Data + data_offset, sync_data_size);
                strm.next_in = in_buf;
                strm.avail_in = (uInt)sync_data_size;
                
                Bytef out_buf[CHUNK];
                strm.next_out = out_buf;
                strm.avail_out = CHUNK;
                
                // Try inflate first
                int inflate_ret = inflate(&strm, Z_SYNC_FLUSH);
                
                // If we got a data error or need sync, try inflateSync
                if (inflate_ret == Z_DATA_ERROR || inflate_ret == Z_NEED_DICT) {
                    inflateSync(&strm);
                }
                
                free(in_buf);
            }
            break;
        }
        
        case 5: { // inflatePrime
            if (Size - data_offset < 2) break;
            
            int bits = Data[data_offset] % 17; // 0-16 bits
            data_offset++;
            
            int value = Data[data_offset];
            data_offset++;
            
            inflatePrime(&strm, bits, value);
            break;
        }
    }

    inflateEnd(&strm);
    return 0;
}
// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
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
#include <stdio.h>

static void init_z_stream(z_stream *strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
    strm->next_out = Z_NULL;
    strm->avail_out = 0;
}

static void init_gz_header(gz_header *head) {
    head->text = 0;
    head->time = 0;
    head->xflags = 0;
    head->os = 0;
    head->extra = Z_NULL;
    head->extra_len = 0;
    head->extra_max = 0;
    head->name = Z_NULL;
    head->name_max = 0;
    head->comment = Z_NULL;
    head->comm_max = 0;
    head->hcrc = 0;
    head->done = 0;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    z_stream strm;
    gz_header head;
    int ret;
    
    // Use first byte to determine which function to test
    uint8_t selector = Data[0] % 6;
    size_t offset = 1;
    
    switch (selector) {
        case 0: { // deflateSetHeader
            init_z_stream(&strm);
            init_gz_header(&head);
            
            // Initialize deflate with gzip wrapper
            ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                               MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
            if (ret != Z_OK) {
                deflateEnd(&strm);
                return 0;
            }
            
            // Set some header fields from fuzzer data if available
            if (offset < Size) head.text = Data[offset++] & 1;
            if (offset < Size) head.os = Data[offset++] % 256;
            
            ret = deflateSetHeader(&strm, &head);
            
            deflateEnd(&strm);
            break;
        }
        
        case 1: { // inflateReset2
            init_z_stream(&strm);
            
            // Initialize inflate
            int windowBits = (Size > offset) ? (Data[offset] % 31) + 8 : MAX_WBITS;
            offset++;
            ret = inflateInit2(&strm, windowBits);
            if (ret != Z_OK) {
                inflateEnd(&strm);
                return 0;
            }
            
            // Reset with potentially different windowBits
            int newWindowBits = (Size > offset) ? (Data[offset] % 31) + 8 : windowBits;
            ret = inflateReset2(&strm, newWindowBits);
            
            inflateEnd(&strm);
            break;
        }
        
        case 2: { // inflateSetDictionary
            init_z_stream(&strm);
            
            // Initialize inflate
            ret = inflateInit2(&strm, MAX_WBITS);
            if (ret != Z_OK) {
                inflateEnd(&strm);
                return 0;
            }
            
            // Use remaining data as dictionary
            if (Size > offset) {
                uInt dictLength = (uInt)(Size - offset);
                if (dictLength > 32768) dictLength = 32768; // Reasonable limit
                
                const Bytef *dictionary = (const Bytef *)(Data + offset);
                ret = inflateSetDictionary(&strm, dictionary, dictLength);
            }
            
            inflateEnd(&strm);
            break;
        }
        
        case 3: { // inflate
            init_z_stream(&strm);
            
            // Initialize inflate with raw data
            ret = inflateInit2(&strm, -MAX_WBITS);
            if (ret != Z_OK) {
                inflateEnd(&strm);
                return 0;
            }
            
            // Set up input buffer
            if (Size > offset) {
                strm.next_in = (Bytef *)(Data + offset);
                strm.avail_in = (uInt)(Size - offset);
                if (strm.avail_in > 1024) strm.avail_in = 1024;
                
                // Allocate output buffer
                Bytef out[1024];
                strm.next_out = out;
                strm.avail_out = sizeof(out);
                
                // Try different flush modes
                int flush = (Size > offset) ? Data[offset] % 4 : Z_NO_FLUSH;
                if (flush == 0) flush = Z_NO_FLUSH;
                else if (flush == 1) flush = Z_SYNC_FLUSH;
                else if (flush == 2) flush = Z_FULL_FLUSH;
                else flush = Z_FINISH;
                
                ret = inflate(&strm, flush);
            }
            
            inflateEnd(&strm);
            break;
        }
        
        case 4: { // inflateGetHeader
            init_z_stream(&strm);
            init_gz_header(&head);
            
            // Initialize inflate for gzip
            ret = inflateInit2(&strm, MAX_WBITS + 16);
            if (ret != Z_OK) {
                inflateEnd(&strm);
                return 0;
            }
            
            ret = inflateGetHeader(&strm, &head);
            
            // Try to inflate a bit to see if header gets populated
            if (Size > offset) {
                strm.next_in = (Bytef *)(Data + offset);
                strm.avail_in = (uInt)(Size - offset);
                if (strm.avail_in > 1024) strm.avail_in = 1024;
                
                Bytef out[1024];
                strm.next_out = out;
                strm.avail_out = sizeof(out);
                
                inflate(&strm, Z_NO_FLUSH);
            }
            
            inflateEnd(&strm);
            break;
        }
        
        case 5: { // inflatePrime
            init_z_stream(&strm);
            
            // Initialize inflate for raw data
            ret = inflateInit2(&strm, -MAX_WBITS);
            if (ret != Z_OK) {
                inflateEnd(&strm);
                return 0;
            }
            
            // Extract bits and value from fuzzer data
            int bits = 0;
            int value = 0;
            
            if (Size > offset) {
                bits = Data[offset++] % 17; // 0-16 bits
                if (bits < 0) bits = 0;
            }
            
            if (Size > offset) {
                value = Data[offset++];
                if (Size > offset) {
                    value = (value << 8) | Data[offset++];
                }
                // Mask to keep only the specified number of bits
                if (bits > 0 && bits < 16) {
                    value &= (1 << bits) - 1;
                }
            }
            
            ret = inflatePrime(&strm, bits, value);
            
            // Try to inflate after priming
            if (Size > offset) {
                strm.next_in = (Bytef *)(Data + offset);
                strm.avail_in = (uInt)(Size - offset);
                if (strm.avail_in > 1024) strm.avail_in = 1024;
                
                Bytef out[1024];
                strm.next_out = out;
                strm.avail_out = sizeof(out);
                
                inflate(&strm, Z_NO_FLUSH);
            }
            
            inflateEnd(&strm);
            break;
        }
    }
    
    return 0;
}
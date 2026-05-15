// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Split input for different operations
    size_t split = Size / 2;
    const uint8_t *dict_data = Data;
    size_t dict_size = split;
    const uint8_t *comp_data = Data + split;
    size_t comp_size = Size - split;
    
    // Initialize zlib structures
    z_stream strm;
    gz_header head;
    
    // Test deflateSetHeader
    {
        z_stream def_strm;
        memset(&def_strm, 0, sizeof(def_strm));
        def_strm.zalloc = Z_NULL;
        def_strm.zfree = Z_NULL;
        def_strm.opaque = Z_NULL;
        
        // Initialize deflate with gzip wrapper
        if (deflateInit2(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                         15 + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
            memset(&head, 0, sizeof(head));
            
            // Set some header fields from fuzzer data
            if (Size > 0) {
                head.text = Data[0] & 1;
                head.time = (uLong)Data[0] << 16;
                head.os = Data[0] % 256;
                head.xflags = Data[0] & 0xFF;
            }
            
            // Call target function
            deflateSetHeader(&def_strm, &head);
            
            deflateEnd(&def_strm);
        }
    }
    
    // Test inflate functions
    {
        memset(&strm, 0, sizeof(strm));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        
        // Initialize inflate with raw mode to allow inflatePrime
        if (inflateInit2(&strm, -15) == Z_OK) {
            // Test inflateGetHeader
            memset(&head, 0, sizeof(head));
            head.extra_max = 65536;
            head.name_max = 65536;
            head.comm_max = 65536;
            inflateGetHeader(&strm, &head);
            
            // Test inflatePrime
            if (comp_size > 2) {
                int bits = comp_data[0] % 17;  // 0-16 bits
                int value = comp_data[1];
                inflatePrime(&strm, bits, value);
            }
            
            // Test inflateSetDictionary
            if (dict_size > 0) {
                // Use first 4 bytes as Adler-32 if available
                if (dict_size >= 4) {
                    strm.adler = *(uLong*)dict_data;
                }
                inflateSetDictionary(&strm, dict_data, 
                    dict_size > 65535 ? 65535 : (uInt)dict_size);
            }
            
            // Test inflate
            if (comp_size > 0) {
                Bytef out_buf[4096];
                strm.next_in = (Bytef*)comp_data;
                strm.avail_in = comp_size > 65535 ? 65535 : (uInt)comp_size;
                strm.next_out = out_buf;
                strm.avail_out = sizeof(out_buf);
                
                // Try different flush modes
                int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FINISH};
                for (int i = 0; i < 3 && strm.avail_in > 0; i++) {
                    inflate(&strm, flush_modes[i]);
                }
            }
            
            // Test inflateReset2 with different windowBits
            if (Size > 0) {
                int windowBits = (Data[0] % 31) - 15;  // Range -15 to 15
                inflateReset2(&strm, windowBits);
            }
            
            inflateEnd(&strm);
        }
    }
    
    return 0;
}
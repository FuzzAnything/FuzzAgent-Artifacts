// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateParams at deflate.c:774:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Initialize z_stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;

    // Use raw deflate for deflatePrime compatibility
    int windowBits = -15;  // raw deflate
    int memLevel = 8;      // default
    int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 
                          windowBits, memLevel, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        return 0;
    }

    // Prepare input data
    if (Size > 0) {
        // Safely extract control parameters with bounds checking
        int level = Z_DEFAULT_COMPRESSION;
        int strategy = Z_DEFAULT_STRATEGY;
        int bits = 0;
        int value = 0;
        
        if (Size > 0) level = Data[0] % 10;          // 0-9
        if (Size > 1) strategy = Data[1] % 4;        // 0-3
        if (Size > 2) bits = Data[2] % 17;           // 0-16
        if (Size > 3) value = Data[3] % 65536;       // 0-65535
        
        // Set up input buffer with remaining data
        size_t input_offset = (Size > 4) ? 4 : Size;
        
        strm.next_in = (Bytef*)(Data + input_offset);
        strm.avail_in = (uInt)(Size - input_offset);
        if (strm.avail_in > 0xFFFFFFFF) strm.avail_in = 0xFFFFFFFF;
        
        // Prepare output buffer
        size_t out_size = (Size - input_offset) * 2 + 1024;
        if (out_size < 1024) out_size = 1024;
        Bytef* out_buf = (Bytef*)malloc(out_size);
        if (!out_buf) {
            deflateEnd(&strm);
            return 0;
        }
        
        strm.next_out = out_buf;
        strm.avail_out = (uInt)out_size;
        if (strm.avail_out > 0xFFFFFFFF) strm.avail_out = 0xFFFFFFFF;

        // Test deflatePrime (only valid before first deflate call)
        if (bits > 0) {
            deflatePrime(&strm, bits, value);
        }

        // Test deflate with Z_NO_FLUSH
        int flush = Z_NO_FLUSH;
        while (strm.avail_in > 0 && ret == Z_OK) {
            if (strm.avail_out == 0) {
                // Expand output buffer
                size_t old_size = out_size;
                out_size *= 2;
                Bytef* new_buf = (Bytef*)realloc(out_buf, out_size);
                if (!new_buf) {
                    free(out_buf);
                    deflateEnd(&strm);
                    return 0;
                }
                out_buf = new_buf;
                strm.next_out = out_buf + (old_size - (strm.next_out - out_buf));
                strm.avail_out = (uInt)(out_size - old_size);
            }
            
            // Test deflateParams at random points
            if (strm.total_in % 100 == 0 && strm.avail_out > 100) {
                deflateParams(&strm, level, strategy);
            }
            
            ret = deflate(&strm, flush);
            
            // Switch to Z_SYNC_FLUSH occasionally
            if (strm.total_in % 200 == 0) {
                flush = Z_SYNC_FLUSH;
            } else if (strm.total_in % 300 == 0) {
                flush = Z_NO_FLUSH;
            }
        }

        // Finish compression
        if (ret == Z_OK) {
            do {
                if (strm.avail_out == 0) {
                    size_t old_size = out_size;
                    out_size *= 2;
                    Bytef* new_buf = (Bytef*)realloc(out_buf, out_size);
                    if (!new_buf) {
                        free(out_buf);
                        deflateEnd(&strm);
                        return 0;
                    }
                    out_buf = new_buf;
                    strm.next_out = out_buf + (old_size - (strm.next_out - out_buf));
                    strm.avail_out = (uInt)(out_size - old_size);
                }
                ret = deflate(&strm, Z_FINISH);
            } while (ret == Z_OK);
        }

        // Test deflateUsed
        int used_bits;
        deflateUsed(&strm, &used_bits);

        // Test deflateReset
        if (ret == Z_STREAM_END) {
            deflateReset(&strm);
            
            // Try compressing again with reset stream
            strm.next_in = (Bytef*)(Data + input_offset);
            strm.avail_in = (uInt)(Size - input_offset);
            if (strm.avail_in > 0xFFFFFFFF) strm.avail_in = 0xFFFFFFFF;
            
            strm.next_out = out_buf;
            strm.avail_out = (uInt)out_size;
            
            ret = deflate(&strm, Z_FINISH);
        }

        free(out_buf);
    }

    // Clean up
    deflateEnd(&strm);
    
    return 0;
}
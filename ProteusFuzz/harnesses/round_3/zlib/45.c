// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// zlibVersion at zutil.c:27:22 in zlib.h
// zlibCompileFlags at zutil.c:31:15 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test zlibVersion
    const char *version = zlibVersion();
    (void)version;

    // Test zlibCompileFlags
    uLong flags = zlibCompileFlags();
    (void)flags;

    // Prepare buffers for compression and decompression
    // We use a portion of the input data as the dictionary and input stream
    uInt dict_len = 0;
    const Bytef *dict = NULL;
    if (Size > 10) {
        dict = Data;
        dict_len = Size - 10;
    }

    // Prepare input for compression
    uInt input_len = (Size > dict_len) ? (Size - dict_len) : 1;
    const Bytef *input_data = (Size > dict_len) ? (Data + dict_len) : Data;
    if (input_len == 0) input_len = 1;

    // Allocate output buffer. 
    // A safe upper bound for zlib output is roughly input + overhead.
    size_t output_buf_size = input_len + (input_len / 10) + 128 + 1024; 
    Byte *output_buf = (Byte *)malloc(output_buf_size);
    if (!output_buf) return 0;

    // Initialize deflate stream
    z_stream strm = {0};
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    int ret = deflateInit_(&strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(strm));
    if (ret != Z_OK) {
        free(output_buf);
        return 0;
    }

    // Test deflateInit2_ with various parameters derived from input
    z_stream strm2 = {0};
    strm2.zalloc = Z_NULL;
    strm2.zfree = Z_NULL;
    strm2.opaque = Z_NULL;
    
    int level = (Size > 0) ? (Data[0] % 10) : 6;
    int method = Z_DEFLATED; // Only Z_DEFLATED is valid for standard deflate
    int windowBits = (Size > 2) ? ((Data[2] % 16) + 8) : 15; // 8 to 23 range roughly
    int memLevel = (Size > 3) ? ((Data[3] % 9) + 1) : 8;
    int strategy = (Size > 4) ? (Data[4] % 3) : 0; // Z_DEFAULT_STRATEGY, Z_FILTERED, Z_HUFFMAN_ONLY
    
    ret = deflateInit2_(&strm2, level, method, windowBits, memLevel, strategy, ZLIB_VERSION, (int)sizeof(strm2));
    if (ret == Z_OK) {
        deflateEnd(&strm2);
    }

    // Set dictionary if available
    if (dict_len > 0 && dict != NULL) {
        ret = deflateSetDictionary(&strm, dict, dict_len);
        (void)ret; // Ignore error for fuzzing state exploration
    }

    // Setup input
    strm.next_in = (Bytef *)input_data;
    strm.avail_in = input_len;
    strm.next_out = output_buf;
    strm.avail_out = (uInt)output_buf_size;

    // Perform compression
    ret = deflate(&strm, Z_FINISH);
    (void)ret;

    // Cleanup deflate
    deflateEnd(&strm);

    // Prepare inflate stream
    z_stream strm_inflate = {0};
    strm_inflate.zalloc = Z_NULL;
    strm_inflate.zfree = Z_NULL;
    strm_inflate.opaque = Z_NULL;

    ret = inflateInit_(&strm_inflate, ZLIB_VERSION, (int)sizeof(strm_inflate));
    if (ret != Z_OK) {
        free(output_buf);
        return 0;
    }

    // Setup inflate input from compressed data
    uInt compressed_len = strm.total_out;
    if (compressed_len > 0) {
        strm_inflate.next_in = output_buf;
        strm_inflate.avail_in = compressed_len;
        
        // Allocate buffer for decompressed data
        size_t inflate_out_size = output_buf_size * 2; // Heuristic for decompression
        Byte *inflate_out = (Byte *)malloc(inflate_out_size);
        if (inflate_out) {
            strm_inflate.next_out = inflate_out;
            strm_inflate.avail_out = (uInt)inflate_out_size;
            
            ret = inflate(&strm_inflate, Z_FINISH);
            (void)ret;
            
            inflateEnd(&strm_inflate);
            free(inflate_out);
        }
    }
    
    inflateEnd(&strm_inflate);
    free(output_buf);

    return 0;
}
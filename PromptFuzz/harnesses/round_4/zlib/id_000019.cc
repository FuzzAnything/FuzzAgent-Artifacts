#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <zlib.h>
}
//<ID> 38
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 40
//<Quality> {"density":8,"unique_branches":{"deflate_rle":[[2094,13,2094,38,0,0,4,1],[2096,17,2096,42,0,0,4,1],[2099,17,2099,34,0,0,4,1],[2104,13,2104,38,0,0,4,0],[2104,13,2104,38,0,0,4,1],[2104,42,2104,57,0,0,4,0],[2104,42,2104,57,0,0,4,1],[2107,17,2107,32,0,0,4,0],[2107,17,2107,32,0,0,4,1],[2107,36,2107,51,0,0,4,0],[2107,36,2107,51,0,0,4,1],[2107,55,2107,70,0,0,4,0],[2107,55,2107,70,0,0,4,1],[2110,26,2110,41,0,0,4,0],[2110,26,2110,41,0,0,4,1],[2110,45,2110,60,0,0,4,0],[2110,45,2110,60,0,0,4,1],[2111,26,2111,41,0,0,4,0],[2111,26,2111,41,0,0,4,1],[2111,45,2111,60,0,0,4,0],[2111,45,2111,60,0,0,4,1],[2112,26,2112,41,0,0,4,0],[2112,26,2112,41,0,0,4,1],[2112,45,2112,60,0,0,4,0],[2112,45,2112,60,0,0,4,1],[2113,26,2113,41,0,0,4,0],[2113,26,2113,41,0,0,4,1],[2113,45,2113,60,0,0,4,0],[2113,45,2113,60,0,0,4,1],[2114,26,2114,39,0,0,4,0],[2114,26,2114,39,0,0,4,1],[2116,21,2116,51,0,0,4,0],[2116,21,2116,51,0,0,4,1],[2124,13,2124,41,0,0,4,0],[2124,13,2124,41,0,0,4,1],[2139,13,2139,19,0,0,4,0],[2139,13,2139,19,0,0,4,1],[1644,8,1644,31,10,0,4,1],[321,5,321,17,16,0,4,0],[1631,24,1631,44,19,0,4,0]]},"library_calls":["inflateInit2_","inflateEnd","inflate","inflateMark","inflateCodesUsed","inflateResetKeep","inflate","deflateInit2_","deflateSetDictionary","deflateParams","deflateBound","deflate","deflateEnd","inflateBackInit_","inflateBackEnd","inflateEnd"],"critical_calls":["inflateInit2_","inflate","inflateMark","inflateCodesUsed","inflateResetKeep","inflate","deflateInit2_","deflateSetDictionary","deflateParams","deflateBound","deflate","deflateEnd","inflateBackInit_","inflateBackEnd","inflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Return early if input is too small for meaningful processing
    if (size < 10) {
	return 0;
	}
	
    
    z_stream strm;
    Bytef *out_buffer = nullptr;
    gz_header header;
    int ret;
    long mark_value;
    unsigned long codes_used;
    
    // Initialize inflate stream
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = (Bytef *)data;
    strm.avail_in = size;
    
    if (inflateInit2(&strm, 15 + 32) != Z_OK) {
        return 0;
    }
    
    // Allocate output buffer for decompression
    size_t out_size = size * 4; // Reasonable expansion factor
    out_buffer = (Bytef *)malloc(out_size);
    if (!out_buffer) {
        inflateEnd(&strm);
        return 0;
    }
    
    strm.next_out = out_buffer;
    strm.avail_out = out_size;
    
    // Decompress some data first
    ret = inflate(&strm, fuzz_int32_t_1);
    
    // Get mark position after some decompression
    mark_value = inflateMark(&strm);
    
    // Get codes used so far
    codes_used = inflateCodesUsed(&strm);
    
    // Reset the stream while keeping the window for potential reuse
    if (inflateResetKeep(&strm) == Z_OK) {
        // Re-set input/output for continued processing
        strm.next_in = (Bytef *)data;
        strm.avail_in = size;
        strm.next_out = out_buffer;
        strm.avail_out = out_size;
        
        // Continue decompression from reset state
        inflate(&strm, fuzz_int32_t_2);
    }
    
    // Now set up for deflate to demonstrate deflateParams
    // We'll use the decompressed data if any was produced
    z_stream def_strm;
    Bytef *compressed_buffer = nullptr;
    
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    if (deflateInit2(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     15 + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        
        // Use some of the input data as dictionary if available
        if (size > 100) {
            deflateSetDictionary(&def_strm, data, 100);
        }
        
        // Set compression parameters dynamically
        // Randomly choose level and strategy based on input data
        int level = (data[0] % 10) + 1; // 1-10
        int strategy = data[1] % 4; // 0-3
        
        deflateParams(&def_strm, level, strategy);
        
        // Allocate buffer for compressed data
        size_t comp_size = deflateBound(&def_strm, size);
        compressed_buffer = (Bytef *)malloc(comp_size);
        
        if (compressed_buffer) {
            def_strm.next_in = (Bytef *)data;
            def_strm.avail_in = size;
            def_strm.next_out = compressed_buffer;
            def_strm.avail_out = comp_size;
            
            // Compress the data
            deflate(&def_strm, fuzz_int32_t_3);
            
            free(compressed_buffer);
        }
        
        deflateEnd(&def_strm);
    }
    
    // Set up for inflateBack to demonstrate inflateBackEnd
    // We need window buffer and callbacks
    unsigned char window[1 << 15]; // 32K window
    z_stream back_strm;
    
    back_strm.zalloc = Z_NULL;
    back_strm.zfree = Z_NULL;
    back_strm.opaque = Z_NULL;
    
    // Initialize for inflateBack
    if (inflateBackInit(&back_strm, 15, window) == Z_OK) {
        // We would need proper callback functions here, but for fuzzing
        // we'll just initialize and immediately clean up
        inflateBackEnd(&back_strm);
    }
    
    // Clean up resources
    free(out_buffer);
    inflateEnd(&strm);
    
    return 0;
}
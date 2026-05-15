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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 50
//<Quality> {"density":21,"unique_branches":{"deflate":[[1219,18,1219,38,0,0,4,0]],"deflateParams":[[786,22,786,31,0,0,4,0]],"deflate_rle":[[2094,13,2094,38,0,0,4,0],[2094,13,2094,38,0,0,4,1],[2096,17,2096,42,0,0,4,0],[2096,17,2096,42,0,0,4,1],[2096,46,2096,65,0,0,4,1],[2099,17,2099,34,0,0,4,0],[2099,17,2099,34,0,0,4,1],[2104,13,2104,38,0,0,4,0],[2104,13,2104,38,0,0,4,1],[2104,42,2104,57,0,0,4,0],[2104,42,2104,57,0,0,4,1],[2107,17,2107,32,0,0,4,0],[2107,17,2107,32,0,0,4,1],[2107,36,2107,51,0,0,4,0],[2107,36,2107,51,0,0,4,1],[2107,55,2107,70,0,0,4,0],[2107,55,2107,70,0,0,4,1],[2110,26,2110,41,0,0,4,0],[2110,26,2110,41,0,0,4,1],[2110,45,2110,60,0,0,4,0],[2110,45,2110,60,0,0,4,1],[2111,26,2111,41,0,0,4,0],[2111,26,2111,41,0,0,4,1],[2111,45,2111,60,0,0,4,0],[2111,45,2111,60,0,0,4,1],[2112,26,2112,41,0,0,4,0],[2112,26,2112,41,0,0,4,1],[2112,45,2112,60,0,0,4,0],[2112,45,2112,60,0,0,4,1],[2113,26,2113,41,0,0,4,0],[2113,26,2113,41,0,0,4,1],[2113,45,2113,60,0,0,4,0],[2113,45,2113,60,0,0,4,1],[2114,26,2114,39,0,0,4,0],[2114,26,2114,39,0,0,4,1],[2116,21,2116,51,0,0,4,0],[2116,21,2116,51,0,0,4,1],[2124,13,2124,41,0,0,4,0],[2124,13,2124,41,0,0,4,1],[2139,13,2139,19,0,0,4,0],[2139,13,2139,19,0,0,4,1],[2142,9,2142,26,0,0,4,0],[1644,8,1644,31,10,0,4,1],[1644,8,1644,31,12,0,4,1],[321,5,321,17,16,0,4,0],[1631,24,1631,44,19,0,4,0],[1631,24,1631,44,20,0,4,0],[1631,24,1631,44,20,0,4,1]]},"library_calls":["zlibVersion","deflateInit_","inflateInit_","deflateEnd","inflateEnd","deflateEnd","compressBound_z","inflateEnd","deflateEnd","compress2_z","inflateEnd","deflateEnd","uncompress2_z","gzdopen","gzbuffer","gzclose","deflateParams","deflate","deflateReset","inflate","inflateCodesUsed","inflateMark","inflateCopy","inflateReset2","inflateEnd","inflateEnd","deflateEnd"],"critical_calls":["zlibVersion","deflateInit_","inflateInit_","compressBound_z","compress2_z","uncompress2_z","gzdopen","gzbuffer","gzclose","deflateParams","deflate","deflateReset","inflate","inflateCodesUsed","inflateMark","inflateCopy","inflateReset2","inflateEnd","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check minimum size requirement
    if (size < 100) {
        return 0;
    }

    // First, check zlib version
    const char* version = zlibVersion();

    // Step 1: Create input and output files
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Prepare zlib streams
    z_stream deflate_strm;
    z_stream inflate_strm;
    z_stream inflate_copy_strm;
    
    memset(&deflate_strm, 0, sizeof(z_stream));
    memset(&inflate_strm, 0, sizeof(z_stream));
    memset(&inflate_copy_strm, 0, sizeof(z_stream));

    // Step 3: Initialize deflate with default parameters
    int ret = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, version, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 4: Initialize inflate with default parameters
    ret = inflateInit_(&inflate_strm, version, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&deflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 5: Use part of input data for compression
    size_t compress_data_len = size / 3;
    Bytef* compress_source = (Bytef*)malloc(compress_data_len);
    if (!compress_source) {
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    memcpy(compress_source, data, compress_data_len);
    
    // Determine compressed size bound
    z_size_t compressed_bound = compressBound_z(compress_data_len);
    Bytef* compressed_data = (Bytef*)malloc(compressed_bound);
    if (!compressed_data) {
        free(compress_source);
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    z_size_t compressed_len = compressed_bound;
    
    // Perform compression with compress2_z
    ret = compress2_z(compressed_data, &compressed_len, compress_source, 
                      compress_data_len, Z_BEST_COMPRESSION);
    
    // Step 6: Use another part of input data for uncompress2_z
    size_t uncompress_source_len = size / 4;
    const Bytef* uncompress_source = data + size/2;
    
    z_size_t uncompressed_bound = compressed_len * 4;  // Estimate
    Bytef* uncompressed_data = (Bytef*)malloc(uncompressed_bound);
    if (!uncompressed_data) {
        free(compressed_data);
        free(compress_source);
        inflateEnd(&inflate_strm);
        deflateEnd(&deflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    z_size_t uncompressed_len = uncompressed_bound;
    z_size_t uncompress_source_len_var = uncompress_source_len;
    
    // Perform uncompression with uncompress2_z
    ret = uncompress2_z(uncompressed_data, &uncompressed_len, 
                       uncompress_source, &uncompress_source_len_var);

    // Step 7: Configure gzFile buffer
    int fd = fuzz_fileno(out_file);
    gzFile gz_file = gzdopen(fd, "wb");
    if (gz_file) {
        gzbuffer(gz_file, 8192);  // Set buffer size
        gzclose(gz_file);
    }

    // Step 8: Set deflate parameters using part of input data
    int level = (data[0] % 10) + 1;  // Level between 1-10
    int strategy = data[1] % 4;      // Strategy between 0-3
    deflateParams(&deflate_strm, level, strategy);

    // Step 9: Process data through deflate stream
    size_t deflate_input_len = size / 5;
    Bytef* deflate_input = (Bytef*)malloc(deflate_input_len);
    Bytef* deflate_output = (Bytef*)malloc(deflate_input_len * 2);
    
    if (deflate_input && deflate_output) {
        memcpy(deflate_input, data + size/3, deflate_input_len);
        
        deflate_strm.next_in = deflate_input;
        deflate_strm.avail_in = deflate_input_len;
        deflate_strm.next_out = deflate_output;
        deflate_strm.avail_out = deflate_input_len * 2;
        
        ret = deflate(&deflate_strm, Z_FINISH);
        deflateReset(&deflate_strm);  // Reset for potential reuse
    }

    // Step 10: Process through inflate stream
    if (compressed_len > 0 && compressed_data) {
        inflate_strm.next_in = compressed_data;
        inflate_strm.avail_in = compressed_len;
        
        Bytef* inflate_output = (Bytef*)malloc(compress_data_len * 2);
        if (inflate_output) {
            inflate_strm.next_out = inflate_output;
            inflate_strm.avail_out = compress_data_len * 2;
            
            ret = inflate(&inflate_strm, Z_FINISH);
            
            // Get codes used by inflate
            unsigned long codes_used = inflateCodesUsed(&inflate_strm);
            
            // Mark current position
            long mark_pos = inflateMark(&inflate_strm);
            
            // Copy inflate stream
            ret = inflateCopy(&inflate_copy_strm, &inflate_strm);
            
            // Reset inflate with different window bits
            inflateReset2(&inflate_strm, 15 + 32);  // Enable gzip decoding
            
            free(inflate_output);
        }
    }

    // Step 11: Clean up resources
    free(compress_source);
    free(compressed_data);
    free(uncompressed_data);
    free(deflate_input);
    free(deflate_output);
    
    inflateEnd(&inflate_strm);
    inflateEnd(&inflate_copy_strm);
    deflateEnd(&deflate_strm);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
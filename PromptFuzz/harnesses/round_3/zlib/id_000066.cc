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
//<ID> 142
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"deflate":[[1246,21,1246,42,0,0,4,0],[1248,25,1248,42,0,0,4,0]]},"library_calls":["gzdopen","gztell64","gzclose","deflateInit2_","gzclose","inflateInit2_","deflateEnd","gzclose","gzfread","compress2_z","inflate","inflateUndermine","deflateResetKeep","deflateSetDictionary","deflate","deflatePending","inflateResetKeep","inflate","deflateGetDictionary","inflateSetDictionary","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gztell64","deflateInit2_","inflateInit2_","gzfread","compress2_z","inflate","inflateUndermine","deflateResetKeep","deflateSetDictionary","deflate","deflatePending","inflateResetKeep","inflate","deflateGetDictionary","inflateSetDictionary","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<91) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
	int fuzz_fd_0 = fuzz_fileno(in_file);

    
    // Create gzFile for reading compressed data
    gzFile gz_in = gzdopen(fuzz_fd_0, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // Get current position in gzFile
    off64_t current_pos = gztell64(gz_in);
    (void)current_pos; // Use to avoid unused variable warning
    
    // Allocate buffers for compression/decompression
    size_t buf_size = 1024 * 1024; // 1MB buffer
    Bytef *compress_buf = (Bytef *)malloc(buf_size);
    Bytef *decompress_buf = (Bytef *)malloc(buf_size);
    Bytef *dict_buf = (Bytef *)malloc(32768); // Dictionary buffer
    
    if (!compress_buf || !decompress_buf || !dict_buf) {
        free(compress_buf);
        free(decompress_buf);
        free(dict_buf);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // Initialize z_stream for compression
    z_stream comp_strm;
    memset(&comp_strm, 0, sizeof(z_stream));
    int ret = deflateInit2_(&comp_strm, fuzz_int32_t_3, fuzz_int32_t_4,
                           fuzz_int32_t_5, fuzz_int32_t_6, fuzz_int32_t_7,
                           fuzz_str_2, sizeof(z_stream));
    if (ret != Z_OK) {
        free(compress_buf);
        free(decompress_buf);
        free(dict_buf);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // Initialize z_stream for decompression
    z_stream decomp_strm;
    memset(&decomp_strm, 0, sizeof(z_stream));
    ret = inflateInit2_(&decomp_strm, fuzz_int32_t_9, fuzz_str_8, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&comp_strm);
        free(compress_buf);
        free(decompress_buf);
        free(dict_buf);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // Read data from gzFile for compression
    size_t read_size = gzfread(compress_buf, fuzz_uint64_t_10, buf_size, gz_in);
    
    // Use compress2_z for compression
    z_size_t comp_len = buf_size;
    if (read_size > 0) {
        ret = compress2_z(compress_buf, &comp_len, compress_buf, read_size, fuzz_int32_t_11);
        (void)ret; // Result may be Z_OK or Z_BUF_ERROR
    }
    
    // Set up decompression stream with compressed data
    if (comp_len > 0 && comp_len < buf_size) {
        decomp_strm.next_in = compress_buf;
        decomp_strm.avail_in = comp_len;
        decomp_strm.next_out = decompress_buf;
        decomp_strm.avail_out = buf_size;
        
        // Inflate the data
        ret = inflate(&decomp_strm, fuzz_int32_t_12);
        (void)ret; // Result may be Z_OK, Z_STREAM_END, or error
        
        // Use inflateUndermine to test corruption
        if (decomp_strm.state) {
            ret = inflateUndermine(&decomp_strm, fuzz_int32_t_13);
            (void)ret; // Result may be Z_STREAM_ERROR or Z_DATA_ERROR
        }
    }
    
    // Reset compression stream
    ret = deflateResetKeep(&comp_strm);
    if (ret == Z_OK) {
        // Configure compression stream
        comp_strm.next_in = decompress_buf;
        comp_strm.avail_in = decomp_strm.total_out;
        comp_strm.next_out = compress_buf;
        comp_strm.avail_out = buf_size;
        
        // Set dictionary if we have data
        if (decomp_strm.total_out > 32) {
            deflateSetDictionary(&comp_strm, decompress_buf, 32);
        }
        
        // Compress with deflate
        ret = deflate(&comp_strm, fuzz_int32_t_14);
        (void)ret;
        
        // Check for pending output
        unsigned int pending;
        int bits;
        ret = deflatePending(&comp_strm, &pending, &bits);
        (void)ret;
        (void)pending;
        (void)bits;
    }
    
    // Reset decompression stream
    ret = inflateResetKeep(&decomp_strm);
    if (ret == Z_OK && comp_strm.total_out > 0) {
        // Try to decompress the compressed data again
        decomp_strm.next_in = compress_buf;
        decomp_strm.avail_in = comp_strm.total_out;
        decomp_strm.next_out = decompress_buf;
        decomp_strm.avail_out = buf_size;
        
        ret = inflate(&decomp_strm, fuzz_int32_t_15);
        (void)ret;
    }
    
    // Test dictionary operations
    if (decomp_strm.total_out > 0) {
        uInt dict_len = 0;
        Bytef *test_dict = (Bytef *)malloc(32768);
        if (test_dict) {
            ret = deflateGetDictionary(&comp_strm, test_dict, &dict_len);
            (void)ret;
            if (dict_len > 0) {
                inflateSetDictionary(&decomp_strm, test_dict, dict_len);
            }
            free(test_dict);
        }
    }
    
    // Clean up
    deflateEnd(&comp_strm);
    inflateEnd(&decomp_strm);
    free(compress_buf);
    free(decompress_buf);
    free(dict_buf);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
}
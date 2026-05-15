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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 6
//<Quality> {"density":16,"unique_branches":{"updatewindow":[[291,17,291,45,0,0,4,0]],"inflate":[[575,9,575,18,0,0,4,0],[584,9,584,16,0,0,4,0],[595,9,595,19,0,0,4,0],[676,9,676,18,0,0,4,0],[1097,9,1097,20,0,0,4,0]]},"library_calls":["get_crc_table","inflateInit2_","inflateReset2","inflateEnd","inflateResetKeep","inflateEnd","inflate","deflateInit2_","inflateEnd","deflateSetDictionary","deflateParams","deflateBound","deflate","inflateBackInit_","deflateEnd","inflateEnd","inflateBackEnd","gzdopen","gzread","gzerror","gzclose","deflateEnd","inflateEnd"],"critical_calls":["get_crc_table","inflateInit2_","inflateReset2","inflateResetKeep","inflate","deflateInit2_","deflateSetDictionary","deflateParams","deflateBound","deflate","inflateBackInit_","inflateBackEnd","gzdopen","gzread","gzerror","gzclose","deflateEnd","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<88) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create input and output files
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Get CRC table (required API)
    const z_crc_t *crc_table = get_crc_table();
    (void)crc_table; // Prevent unused variable warning
    
    // Initialize decompression stream for input data
    z_stream decomp_stream;
    memset(&decomp_stream, 0, sizeof(decomp_stream));
    
    // Use windowBits = 15+32 to enable gzip decoding
    int ret = inflateInit2_(&decomp_stream, 15 + 32, fuzz_str_1, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Reset with different windowBits
    ret = inflateReset2(&decomp_stream, 15 + 16); // 15+16 for gzip
    if (ret != Z_OK) {
        inflateEnd(&decomp_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Reset keep to test
    ret = inflateResetKeep(&decomp_stream);
    if (ret != Z_OK) {
        inflateEnd(&decomp_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Decompress the input
    Bytef decomp_buffer[4096];
    decomp_stream.next_in = (Bytef *)data;
    decomp_stream.avail_in = size;
    
    do {
        decomp_stream.next_out = decomp_buffer;
        decomp_stream.avail_out = sizeof(decomp_buffer);
        ret = inflate(&decomp_stream, fuzz_int32_t_2);
        
        if (ret == Z_OK || ret == Z_STREAM_END || ret == Z_BUF_ERROR) {
            size_t have = sizeof(decomp_buffer) - decomp_stream.avail_out;
            if (have > 0) {
                // We'll use this decompressed data for recompression
            }
        }
    } while (ret == Z_OK);
    
    // Initialize compression stream for recompression
    z_stream comp_stream;
    memset(&comp_stream, 0, sizeof(comp_stream));
    
    // Initialize compression with specific parameters
    ret = deflateInit2_(&comp_stream, fuzz_int32_t_4, fuzz_int32_t_5,
                        15 + 16, fuzz_int32_t_6, fuzz_int32_t_7,
                        fuzz_str_3, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&decomp_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set dictionary from part of decompressed data
    if (decomp_stream.total_out > 0) {
        size_t dict_size = decomp_stream.total_out > 256 ? 256 : decomp_stream.total_out;
        ret = deflateSetDictionary(&comp_stream, decomp_buffer, dict_size);
        (void)ret; // Ignore error for fuzzing
    }
    
    // Change compression parameters
    ret = deflateParams(&comp_stream, fuzz_int32_t_8, fuzz_int32_t_9);
    (void)ret; // Ignore error for fuzzing
    
    // Calculate bound for compression
    uLong bound = deflateBound(&comp_stream, decomp_stream.total_out);
    (void)bound; // Prevent unused variable warning
    
    // Recompress the decompressed data
    Bytef comp_buffer[8192];
    comp_stream.next_in = decomp_buffer;
    comp_stream.avail_in = decomp_stream.total_out > sizeof(decomp_buffer) ? 
                          sizeof(decomp_buffer) : decomp_stream.total_out;
    
    do {
        comp_stream.next_out = comp_buffer;
        comp_stream.avail_out = sizeof(comp_buffer);
        ret = deflate(&comp_stream, fuzz_int32_t_10);
    } while (ret == Z_OK);
    
    // Initialize inflateBack for another decompression pass
    z_stream back_stream;
    memset(&back_stream, 0, sizeof(back_stream));
    
    unsigned char window[32768]; // 32K window
    ret = inflateBackInit_(&back_stream, 15, window, 
                           fuzz_str_11, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&comp_stream);
        inflateEnd(&decomp_stream);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Note: inflateBack requires custom input/output callbacks
    // For simplicity in fuzzing, we'll just initialize and end it
    
    // Clean up inflateBack
    inflateBackEnd(&back_stream);
    
    // Clean up gzFile operations
    gzFile gz_in = gzdopen(in_fd, fuzz_str_12);
    if (gz_in) {
        // Read some data to trigger potential errors
        char gz_buffer[128];
        gzread(gz_in, gz_buffer, sizeof(gz_buffer) > size ? size : sizeof(gz_buffer));
        
        // Check for errors
        int errnum;
        const char *err_msg = gzerror(gz_in, &errnum);
        (void)err_msg; // Prevent unused variable warning
        
        gzclose(gz_in);
    } else {
        // If gzdopen failed, we still need to close the original file
        assert_file_closed(&in_file);;
        in_file = NULL; // Mark as closed
    }
    
    // Clean up all streams
    deflateEnd(&comp_stream);
    inflateEnd(&decomp_stream);
    
    // Close files
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
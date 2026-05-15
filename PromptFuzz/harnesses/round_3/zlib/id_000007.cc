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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 2
//<Quality> {"density":6,"unique_branches":{"inflateStateCheck":[[94,9,94,24,0,0,4,0]],"inflateEnd":[[1157,9,1157,32,0,0,4,0]]},"library_calls":["zlibCompileFlags","gzdopen","gzungetc","gzclose","compressBound","gzclose","compress2_z","deflateInit_","deflatePrime","deflatePending","deflateEnd","inflateBackInit_","inflateValidate","inflateCodesUsed","inflateBackEnd","inflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","gzungetc","compressBound","compress2_z","deflateInit_","deflatePrime","deflatePending","deflateEnd","inflateBackInit_","inflateValidate","inflateCodesUsed","inflateBackEnd","inflateEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Return early if input is too small for meaningful testing
    if (size < 10) {
	return 0;
	}
	
    
    // First, use zlibCompileFlags() - informational only
    unsigned long compile_flags = zlibCompileFlags();
    
    // Use fmemopen to create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create gzFile from file descriptor for gzungetc
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Use gzungetc to push back the first character
    int first_byte = gzgetc(gz_file);
    if (first_byte != -1) {
        gzungetc(first_byte, gz_file);
    }
    
    // Create output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Allocate buffers for compression/decompression
    size_t original_size = size;
    uLong bound = compressBound(original_size);
    Bytef *compressed = (Bytef*)malloc(bound);
    Bytef *decompressed = (Bytef*)malloc(original_size);
    
    if (!compressed || !decompressed) {
        free(compressed);
        free(decompressed);
        assert_file_closed(&out_file);;
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Compress the input data using compress2_z
    z_size_t compressed_len = bound;
    int compress_result = compress2_z(compressed, &compressed_len, data, 
                                      original_size, fuzz_int32_t_2);
    
    // Initialize z_stream for deflate operations
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(z_stream));
    
    // Initialize deflate
    if (deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Use deflatePrime to insert bits
        if (size > 0) {
            deflatePrime(&deflate_stream, fuzz_int32_t_3, data[0] & 0xFF);
        }
        
        // Check pending output
        unsigned int pending;
        int bits;
        deflatePending(&deflate_stream, &pending, &bits);
        
        // Clean up deflate stream
        deflateEnd(&deflate_stream);
    }
    
    // Initialize z_stream for inflate operations
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(z_stream));
    
    // Allocate window for inflateBack
    unsigned char *window = (unsigned char*)malloc(32768);
    if (window) {
        // Initialize inflateBack
        int back_result = inflateBackInit_(&inflate_stream, 15, window, 
                                          fuzz_str_4, sizeof(z_stream));
        
        if (back_result == Z_OK) {
            // Enable validation
            inflateValidate(&inflate_stream, fuzz_int32_t_5);
            
            // Get code usage statistics
            unsigned long codes_used = inflateCodesUsed(&inflate_stream);
            
            // Clean up inflateBack
            inflateBackEnd(&inflate_stream);
        }
        
        free(window);
    }
    
    // Clean up inflate stream
    inflateEnd(&inflate_stream);
    
    // Write compressed data to output file
    if (compress_result == Z_OK) {
        fwrite(compressed, 1, compressed_len, out_file);
    }
    
    // Clean up all allocated resources
    free(compressed);
    free(decompressed);
    assert_file_closed(&out_file);;
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
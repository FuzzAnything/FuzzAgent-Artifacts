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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"deflateStateCheck":[[540,9,540,23,0,0,4,0]]},"library_calls":["gzdopen","gzoffset","gzrewind","gzclearerr","deflateBound","gzclose","compress2_z","deflateInit_","deflateTune","deflateGetDictionary","deflateEnd","gzopen","gzread","gzclose","gzclose"],"critical_calls":["gzdopen","gzoffset","gzrewind","gzclearerr","deflateBound","compress2_z","deflateInit_","deflateTune","deflateGetDictionary","deflateEnd","gzopen","gzread","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Step 1: Create a FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Step 3: Open compressed data stream with gzdopen
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Step 4: Get current offset in gzFile
    off_t offset = gzoffset(gz_in);

    // Step 5: Try to rewind the gzFile
    int rewind_result = gzrewind(gz_in);

    // Step 6: Clear any error state
    gzclearerr(gz_in);

    // Step 7: Prepare for compression using compress2_z
    // Use first byte as compression level (0-9)
    int level = data[0] % 10;
    
    // Calculate maximum compressed size
    uLong bound = deflateBound(nullptr, size);
    
    // Allocate destination buffer
    Bytef *dest = (Bytef *)malloc(bound);
    if (!dest) {
        gzclose(gz_in);  // This also closes the underlying fd
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    z_size_t destLen = bound;
    
    // Step 8: Attempt to compress the data
    int compress_result = compress2_z(dest, &destLen, data, size, level);
    
    // Step 9: Initialize z_stream for deflate operations
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    // Initialize deflate with compression level
    int init_result = deflateInit(&strm, level);
    if (init_result == Z_OK) {
        // Step 10: Tune deflate parameters using random values from input
        int good_length = (size > 0) ? (data[0] % 256) : 8;
        int max_lazy = (size > 1) ? (data[1] % 256) : 16;
        int nice_length = (size > 2) ? (data[2] % 256) : 32;
        int max_chain = (size > 3) ? (data[3] % 256) : 64;
        
        deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
        
        // Step 11: Try to get dictionary (will likely fail as none is set)
        Bytef dict_buf[256];
        uInt dict_len = 256;
        int dict_result = deflateGetDictionary(&strm, dict_buf, &dict_len);
        
        // Clean up deflate stream
        deflateEnd(&strm);
    }
    
    // Step 12: Open output file for writing compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (compress_result == Z_OK && destLen > 0) {
            fwrite(dest, 1, destLen, out_file);
        }
        assert_file_closed(&out_file);;
    }

    // Step 13: Open the output file with gzopen to read it back
    gzFile gz_out = gzopen("output_file", "rb");
    if (gz_out) {
        // Read some data from the gz file
        Bytef read_buf[256];
        int bytes_read = gzread(gz_out, read_buf, sizeof(read_buf));
        gzclose(gz_out);
    }

    // Step 14: Clean up resources
    free(dest);
    gzclose(gz_in);  // This also closes the underlying fd and FILE*
    // Note: in_file is already closed by gzclose(gz_in), so we don't need to close it again
    
    // Remove the temporary output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
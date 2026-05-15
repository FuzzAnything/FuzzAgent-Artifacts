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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"inflateStateCheck":[[95,9,95,27,0,0,4,0]],"inflateSyncPoint":[[1323,9,1323,32,0,0,4,0]]},"library_calls":["gzdopen","deflateInit_","gzclose","gzgets","gztell64","inflateSyncPoint","deflateResetKeep","deflateEnd","gzclose","deflateEnd","gzclose","deflateEnd","gzclose","compressBound_z","compress_z","deflateEnd","gzclose"],"critical_calls":["gzdopen","deflateInit_","gzgets","gztell64","inflateSyncPoint","deflateResetKeep","compressBound_z","compress_z","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // Create input file from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for gzopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Open compressed file using gzdopen
    gzFile gz_file = gzdopen(fd, fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Initialize deflate stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Use deflateInit_ to initialize deflate stream
    int ret = deflateInit_(&deflate_strm, fuzz_int32_t_3, 
                          fuzz_str_2, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Read from gz file using gzgets
    char buffer[1024];
    if (gzgets(gz_file, buffer, sizeof(buffer))) {
        // Get current position in gz file using gztell64
        off64_t pos = gztell64(gz_file);
        (void)pos; // Use pos to avoid unused variable warning
    }
    
    // Check sync point in deflate stream
    int sync_point = inflateSyncPoint(&deflate_strm);
    (void)sync_point; // Use sync_point to avoid unused variable warning
    
    // Reset deflate stream keeping dictionary
    ret = deflateResetKeep(&deflate_strm);
    if (ret != Z_OK) {
        deflateEnd(&deflate_strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Create output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        deflateEnd(&deflate_strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Get file descriptor for output
    int out_fd = fuzz_fileno(out_file);
    if (out_fd < 0) {
        assert_file_closed(&out_file);;
        deflateEnd(&deflate_strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Use compress_z to compress the input data
    Bytef *compressed_data = nullptr;
    z_size_t compressed_len = 0;
    z_size_t source_len = size;
    
    // First get the bound for compression
    compressed_len = compressBound_z(source_len);
    if (compressed_len > 0) {
        compressed_data = (Bytef *)malloc(compressed_len);
        if (compressed_data) {
            // Compress the data
            ret = compress_z(compressed_data, &compressed_len, 
                           (const Bytef *)data, source_len);
            
            if (ret == Z_OK && compressed_len > 0) {
                // Write compressed data to output file
                fwrite(compressed_data, 1, compressed_len, out_file);
            }
            
            free(compressed_data);
        }
    }
    
    // Clean up all resources
    deflateEnd(&deflate_strm);
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}
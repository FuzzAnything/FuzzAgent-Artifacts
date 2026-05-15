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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"gzvprintf":[[423,9,423,32,0,0,4,0]]},"library_calls":["gzdopen","inflateBackInit_","gzclose","gzfread","gzclearerr","gztell64","gzseek","gzclearerr","compress_z","compress2_z","gzprintf","inflateBackEnd","gzclose"],"critical_calls":["gzdopen","inflateBackInit_","gzfread","gzclearerr","gztell64","gzseek","gzclearerr","compress_z","compress2_z","gzprintf","inflateBackEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);

    // Step 1: Open compressed input file with gzopen64 (using gzdopen approach)
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 2: Initialize inflateBack for decompression
    z_stream strm;
    unsigned char window[32768]; // 32K window
    int windowBits = 15; // MAX_WBITS
    int ret = inflateBackInit_(&strm, windowBits, window, 
                               ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 3: Read compressed data using gzfread
    Bytef read_buffer[4096];
    z_size_t items_read;
    
    // Try to read some data
    items_read = gzfread(read_buffer, 1, sizeof(read_buffer), gz_in);
    
    // Use gzclearerr to clear any error state
    gzclearerr(gz_in);

    // Step 4: Get current position with gztell64
    off64_t current_pos = gztell64(gz_in);

    // Step 5: Seek to beginning with gzseek
    off_t seek_ret = gzseek(gz_in, 0, SEEK_SET);
    if (seek_ret == -1) {
        // Seek failed, handle gracefully
        gzclearerr(gz_in);
    }

    // Step 6: Read a character with gzgetc
    int first_char = gzgetc(gz_in);
    
    // Step 7: Use compress_z on the data we read
    Bytef compressed_buffer1[8192];
    z_size_t compressed_len1 = sizeof(compressed_buffer1);
    
    if (items_read > 0) {
        // Try to compress the data we read
        int compress_ret = compress_z(compressed_buffer1, &compressed_len1, 
                                      read_buffer, items_read);
        // Ignore result for fuzzing
    }

    // Step 8: Use compress2_z with different compression level
    Bytef compressed_buffer2[8192];
    z_size_t compressed_len2 = sizeof(compressed_buffer2);
    
    if (items_read > 0) {
        // Try with maximum compression
        int compress2_ret = compress2_z(compressed_buffer2, &compressed_len2,
                                        read_buffer, items_read, Z_BEST_COMPRESSION);
        // Ignore result for fuzzing
    }

    // Step 9: Use gzprintf to write some diagnostic info
    gzprintf(gz_in, "Fuzzing input size: %zu\n", size);

    // Clean up inflateBack
    inflateBackEnd(&strm);

    // Close gzFile
    gzclose(gz_in);
    
    // Note: gzclose also closes the underlying file descriptor,
    // so we shouldn't close in_file separately

    // Close output file
    assert_file_closed(&out_file);;

    // Remove output file if created
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","compressBound_z","uncompress_z","gzputc","gzrewind","gzgets","gztell","inflateInit2_","inflate","inflateReset2","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","compressBound_z","uncompress_z","gzputc","gzrewind","gzgets","gztell","inflateInit2_","inflate","inflateReset2","inflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Step 1: Create input FILE* from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Create output FILE* for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 3: Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Step 4: Open gzFile handles for reading and writing
    gzFile gz_in = gzdopen(in_fd, fuzz_str_1);
    gzFile gz_out = gzdopen(out_fd, fuzz_str_2);
    
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Step 5: Use uncompress_z to decompress the input data
    // First, we need to read the compressed data from the gzFile
    // We'll use a buffer to read chunks
    Bytef *compressed_buf = nullptr;
    Bytef *decompressed_buf = nullptr;
    
    // Try to read the entire compressed data
    fseek(in_file, 0, SEEK_END);
    long compressed_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (compressed_size > 0) {
        compressed_buf = (Bytef *)malloc(compressed_size);
        if (compressed_buf) {
            fread(compressed_buf, 1, compressed_size, in_file);
            
            // Estimate decompressed size (use compressBound for estimation)
            z_size_t dest_len = compressBound_z(compressed_size);
            decompressed_buf = (Bytef *)malloc(dest_len);
            
            if (decompressed_buf) {
                // Try to decompress using uncompress_z
                int uncompress_result = uncompress_z(
                    decompressed_buf, &dest_len,
                    compressed_buf, compressed_size
                );
                
                // If successful, write decompressed data to gz_out using gzputc
                if (uncompress_result == Z_OK) {
                    for (z_size_t i = 0; i < dest_len; i++) {
                        gzputc(gz_out, decompressed_buf[i]);
                    }
                }
                
                free(decompressed_buf);
            }
            free(compressed_buf);
        }
    }

    // Step 6: Reset file position and use gzgets
    gzrewind(gz_in);
    char read_buf[256];
    while (gzgets(gz_in, read_buf, sizeof(read_buf)) != nullptr) {
        // Get current position using gztell
        off_t pos = gztell(gz_in);
        (void)pos; // Use pos to avoid unused variable warning
    }

    // Step 7: Create and use z_stream with inflateReset2
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize for decompression
    if (inflateInit2(&strm, 15 + 32) == Z_OK) { // 15+32 for gzip decoding
        // Set up input buffer with some data
        Bytef test_in[100];
        Bytef test_out[200];
        
        if (size > 0) {
            size_t test_size = size > sizeof(test_in) ? sizeof(test_in) : size;
            memcpy(test_in, data, test_size);
            
            strm.next_in = test_in;
            strm.avail_in = test_size;
            strm.next_out = test_out;
            strm.avail_out = sizeof(test_out);
            
            // Try to inflate
            inflate(&strm, fuzz_int32_t_3);
            
            // Reset the stream with different windowBits
            inflateReset2(&strm, fuzz_int32_t_4); // Reset to zlib format
        }
        
        inflateEnd(&strm);
    }

    // Step 8: Clean up all resources
    gzclose(gz_in);  // This also closes the underlying file descriptor
    gzclose(gz_out); // This also closes the underlying file descriptor
    
    // Note: fclose is not needed because gzdopen took ownership of the file descriptors
    // But we need to close the FILE* handles that were already closed by gzclose
    // Actually, gzdopen duplicates the file descriptor, so we still need to close the FILE*
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
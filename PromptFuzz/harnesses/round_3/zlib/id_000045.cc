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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 3, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["gzopen64","compressBound","compress","gzopen64","gzeof","inflateInit2_","inflateReset2","deflateInit_","deflateReset","compressBound","compress2","uncompress2","gzopen64","gzwrite","gzflush","gzwrite","gzflush","gzclose","gzread","gzeof","inflateEnd","deflateEnd","gzclose"],"critical_calls":["gzopen64","compressBound","compress","gzopen64","gzeof","inflateInit2_","inflateReset2","deflateInit_","deflateReset","compressBound","compress2","uncompress2","gzopen64","gzwrite","gzflush","gzwrite","gzflush","gzclose","gzread","gzeof","inflateEnd","deflateEnd","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to achieve decompression and manipulation of compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<31) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	
    
    // Create temporary files
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
    
    // Open compressed data file for reading with gzopen64
    gzFile gz_input = gzopen64("input_file", fuzz_str_1);
    if (!gz_input) {
        // Create a compressed version of input data for testing
        // First, write data to a temp file
        FILE *tmp_in = fopen("input_file", "wb");
        if (tmp_in) {
            fwrite(data, 1, size, tmp_in);
            assert_file_closed(&tmp_in);;
            
            // Compress the file using zlib APIs
            uLongf compressed_size = compressBound(size);
            Bytef *compressed = (Bytef *)malloc(compressed_size);
            if (compressed) {
                compress(compressed, &compressed_size, data, size);
                
                // Write compressed data to input_file
                tmp_in = fopen("input_file", "wb");
                if (tmp_in) {
                    fwrite(compressed, 1, compressed_size, tmp_in);
                    assert_file_closed(&tmp_in);;
                }
                free(compressed);
            }
            
            // Reopen for reading
            gz_input = gzopen64("input_file", fuzz_str_2);
        }
    }
    
    if (gz_input) {
        // Test gzeof - check if at end of file (should be false initially)
        int at_eof = gzeof(gz_input);
        
        // Setup z_stream for inflation
        z_stream strm_inflate;
        memset(&strm_inflate, 0, sizeof(z_stream));
        
        // Initialize inflation with default window bits
        inflateInit2(&strm_inflate, MAX_WBITS);
        
        // Reset inflation stream with different window bits
        // Using negative window bits for raw deflate data
        inflateReset2(&strm_inflate, fuzz_int32_t_3);
        
        // Setup z_stream for deflation
        z_stream strm_deflate;
        memset(&strm_deflate, 0, sizeof(z_stream));
        
        // Initialize deflation
        deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION);
        
        // Reset deflation stream
        deflateReset(&strm_deflate);
        
        // Test uncompress2 with the input data
        // First need to compress some data to test uncompress2
        uLongf dest_len = size * 2; // Estimate decompressed size
        Bytef *dest = (Bytef *)malloc(dest_len);
        uLong source_len = size;
        
        if (dest) {
            // Create compressed version of input
            uLongf compressed_len = compressBound(size);
            Bytef *compressed_data = (Bytef *)malloc(compressed_len);
            
            if (compressed_data) {
                // Compress the input
                int compress_result = compress2(compressed_data, &compressed_len, 
                                              data, size, fuzz_int32_t_4);
                
                if (compress_result == Z_OK) {
                    // Now test uncompress2
                    uLong actual_source_len = compressed_len;
                    int uncompress_result = uncompress2(dest, &dest_len, 
                                                       compressed_data, &actual_source_len);
                    
                    // Even if it fails, we've called the API
                }
                
                free(compressed_data);
            }
            
            free(dest);
        }
        
        // Test gzflush on output
        gzFile gz_output = gzopen64("output_file.gz", fuzz_str_5);
        if (gz_output) {
            // Write some data
            gzwrite(gz_output, data, size > 1024 ? 1024 : size);
            
            // Flush the output
            gzflush(gz_output, fuzz_int32_t_6);
            
            // Write more data
            if (size > 1024) {
                gzwrite(gz_output, data + 1024, size - 1024 > 1024 ? 1024 : size - 1024);
            }
            
            // Flush again with different parameter
            gzflush(gz_output, fuzz_int32_t_7);
            
            gzclose(gz_output);
        }
        
        // Read from gz_input to exercise gzeof
        Bytef read_buffer[1024];
        int bytes_read;
        do {
            bytes_read = gzread(gz_input, read_buffer, sizeof(read_buffer));
            // Check eof status
            at_eof = gzeof(gz_input);
        } while (bytes_read > 0);
        
        // Cleanup inflation stream
        inflateEnd(&strm_inflate);
        
        // Cleanup deflation stream
        deflateEnd(&strm_deflate);
        
        // Close gzip file
        gzclose(gz_input);
    }
    
    // Cleanup temporary files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    remove("output_file.gz");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file.gz");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
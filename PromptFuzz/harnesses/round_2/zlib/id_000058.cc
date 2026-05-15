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
//<ID> 157
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 3
//<Quality> {"density":17,"unique_branches":{"deflate":[[995,9,995,29,0,0,4,0]],"compress2_z":[[61,14,61,25,0,0,4,0],[65,12,65,31,0,0,4,1]]},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","gzeof","gzfread","crc32_z","gzputc","gzungetc","gzclose","gzclose","crc32_z","crc32_combine","compressBound","compress","compressBound_z","compress_z","inflateInit2_","inflateValidate","inflateEnd"],"critical_calls":["gzdopen","gzdopen","gzeof","gzfread","crc32_z","gzputc","gzungetc","gzclose","gzclose","crc32_z","crc32_combine","compressBound","compress","compressBound_z","compress_z","inflateInit2_","inflateValidate","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Create gzFile from file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    gzFile gz_out = gzdopen(fd_out, "wb");
    
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Read and process compressed data using gzfread
    const size_t BUFFER_SIZE = 4096;
    Bytef buffer[BUFFER_SIZE];
    z_size_t total_read = 0;
    uLong crc1 = 0;
    uLong crc2 = 0;
    
    while (!gzeof(gz_in)) {
        z_size_t items_read = gzfread(buffer, sizeof(Bytef), BUFFER_SIZE / sizeof(Bytef), gz_in);
        if (items_read > 0) {
            total_read += items_read;
            
            // Compute CRC32 using crc32_z
            crc1 = crc32_z(crc1, buffer, items_read);
            
            // Write to output file using gzputc
            for (z_size_t i = 0; i < items_read; i++) {
                gzputc(gz_out, buffer[i]);
            }
            
            // Push back last character using gzungetc
            if (items_read > 0) {
                gzungetc(buffer[items_read - 1], gz_in);
            }
        }
    }
    
    // Close gz files to flush buffers
    gzclose(gz_in);
    gzclose(gz_out);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Re-open output file for reading
    FILE *output_check = fopen("output_file", "rb");
    if (!output_check) {
	assert_file_closed(&output_check);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
	}
	
    
    fseek(output_check, 0, SEEK_END);
    long output_size = ftell(output_check);
    fseek(output_check, 0, SEEK_SET);
    
    Bytef *output_data = (Bytef*)malloc(output_size);
    if (!output_data) {
        assert_file_closed(&output_check);;
        assert_file_closed(&output_check);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    fread(output_data, 1, output_size, output_check);
    assert_file_closed(&output_check);;
    
    // Compute CRC32 of output data
    crc2 = crc32_z(0, output_data, output_size);
    
    // Combine CRCs using crc32_combine
    uLong combined_crc = crc32_combine(crc1, crc2, total_read);
    
    // Prepare for compression
    uLongf compressed_size = compressBound(total_read);
    Bytef *compressed_data = (Bytef*)malloc(compressed_size);
    Bytef *uncompressed_data = (Bytef*)malloc(total_read);
    
    if (!compressed_data || !uncompressed_data) {
        free(compressed_data);
        free(uncompressed_data);
        free(output_data);
        assert_file_closed(&output_check);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Use compress API
    int compress_result = compress(compressed_data, &compressed_size, output_data, output_size);
    
    // Use compress_z API
    z_size_t z_compressed_size = compressBound_z(output_size);
    Bytef *z_compressed_data = (Bytef*)malloc(z_compressed_size);
    if (z_compressed_data) {
        int compress_z_result = compress_z(z_compressed_data, &z_compressed_size, output_data, output_size);
        free(z_compressed_data);
    }
    
    // Create z_stream for inflateValidate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    // Initialize for decompression
    if (inflateInit2(&strm, 15 + 32) == Z_OK) { // 15+32 for gzip decoding
        strm.next_in = compressed_data;
        strm.avail_in = compressed_size;
        
        // Use inflateValidate
        int validate_result = inflateValidate(&strm, 1);
        
        inflateEnd(&strm);
    }
    
    // Clean up
    free(compressed_data);
    free(uncompressed_data);
    free(output_data);
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&output_check);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
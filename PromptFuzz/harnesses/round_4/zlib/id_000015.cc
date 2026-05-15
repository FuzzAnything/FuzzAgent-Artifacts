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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.5, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["gzdopen","gzdirect","gztell64","gzclose","inflateInit_","gzclose","inflateValidate","inflateEnd","gzclose","inflate","deflateInit_","inflateEnd","gzclose","deflateUsed","compressBound","deflateEnd","inflateEnd","gzclose","compress2_z","gzdopen","gzfwrite","gzclose","crc32","crc32","crc32","crc32","crc32_combine64","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gztell64","inflateInit_","inflateValidate","inflate","deflateInit_","deflateUsed","compressBound","compress2_z","gzdopen","gzfwrite","gzclose","crc32","crc32","crc32","crc32","crc32_combine64","deflateEnd","inflateEnd","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to create a comprehensive fuzzing scenario:

*/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<38) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 16) {
	return 0;
	}
	
    
    // Create input FILE* from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output FILE*
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
    
    // 1. Use gzdirect - open file for reading and check if it's direct
    gzFile gz_in = gzdopen(in_fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    int is_direct = gzdirect(gz_in);
    
    // 2. Use gztell64 - get current position
    off64_t pos = gztell64(gz_in);
    
    // Read compressed data for inflate operations
    fseek(in_file, 0, SEEK_SET);
    
    // Allocate buffers
    size_t compressed_size = size;
    Bytef *compressed_data = (Bytef *)malloc(compressed_size);
    if (!compressed_data) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    fread(compressed_data, 1, compressed_size, in_file);
    
    // 3. Set up inflate stream for validation
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    strm_inflate.next_in = compressed_data;
    strm_inflate.avail_in = compressed_size;
    
    // Initialize inflate
    if (inflateInit(&strm_inflate) != Z_OK) {
        free(compressed_data);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 4. Use inflateValidate
    int validate_result = inflateValidate(&strm_inflate, fuzz_int32_t_2);
    
    // Create output buffer for decompression
    size_t decompressed_size = size * 4; // Estimate
    Bytef *decompressed_data = (Bytef *)malloc(decompressed_size);
    if (!decompressed_data) {
        inflateEnd(&strm_inflate);
        free(compressed_data);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    strm_inflate.next_out = decompressed_data;
    strm_inflate.avail_out = decompressed_size;
    
    // Try to decompress
    int inflate_result = inflate(&strm_inflate, fuzz_int32_t_3);
    
    size_t actual_decompressed_size = decompressed_size - strm_inflate.avail_out;
    
    // 5. Set up deflate stream for deflateUsed
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    if (deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(decompressed_data);
        inflateEnd(&strm_inflate);
        free(compressed_data);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Use deflateUsed
    int used_bits = 0;
    int deflate_used_result = deflateUsed(&strm_deflate, &used_bits);
    
    // 7. Use compress2_z for compression
    size_t compressed2_size = compressBound(actual_decompressed_size);
    Bytef *compressed2_data = (Bytef *)malloc(compressed2_size);
    if (!compressed2_data) {
        deflateEnd(&strm_deflate);
        free(decompressed_data);
        inflateEnd(&strm_inflate);
        free(compressed_data);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    z_size_t dest_len = compressed2_size;
    int compress_result = compress2_z(compressed2_data, &dest_len, 
                                      decompressed_data, actual_decompressed_size, 
                                      fuzz_int32_t_4);
    
    // 8. Use gzfwrite to write decompressed data to gz output file
    gzFile gz_out = gzdopen(out_fd, fuzz_str_5);
    if (gz_out) {
        z_size_t write_result = gzfwrite(decompressed_data, 1, actual_decompressed_size, gz_out);
        gzclose(gz_out);
    }
    
    // 9. Use crc32_combine64 - calculate CRC32 for different parts of data
    uLong crc1 = crc32(fuzz_uint64_t_6, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_7, Z_NULL, 0);
    
    if (actual_decompressed_size > 0) {
        // Split data into two parts for CRC combination
        size_t split_point = actual_decompressed_size / 2;
        
        if (split_point > 0) {
            crc1 = crc32(crc1, decompressed_data, split_point);
        }
        
        if (actual_decompressed_size - split_point > 0) {
            crc2 = crc32(crc2, decompressed_data + split_point, 
                        actual_decompressed_size - split_point);
        }
        
        // Combine the two CRCs
        off64_t len2 = actual_decompressed_size - split_point;
        uLong combined_crc = crc32_combine64(crc1, crc2, len2);
    }
    
    // Clean up
    deflateEnd(&strm_deflate);
    free(compressed2_data);
    free(decompressed_data);
    inflateEnd(&strm_inflate);
    free(compressed_data);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 3
//<Quality> {"density":16,"unique_branches":{"inflateSyncPoint":[[1325,12,1325,33,0,0,4,0],[1325,37,1325,53,0,0,4,0],[1325,37,1325,53,0,0,4,1]]},"library_calls":["gzdopen","zlibCompileFlags","inflateInit_","gzclose","inflateEnd","gzclose","inflate","inflateSync","inflateSyncPoint","inflateReset","inflateReset2","inflate","inflateEnd","deflateInit_","gzclose","deflateBound_z","deflateEnd","gzclose","deflateParams","deflate","deflateEnd","crc32","crc32","crc32_combine_gen64","gzclose"],"critical_calls":["gzdopen","zlibCompileFlags","inflateInit_","inflate","inflateSync","inflateSyncPoint","inflateReset","inflateReset2","inflate","inflateEnd","deflateInit_","deflateBound_z","deflateParams","deflate","deflateEnd","crc32","crc32","crc32_combine_gen64","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<71) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    // Create a FILE* from input data for gzopen64
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for gzopen64
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Use gzopen64 to open the file descriptor
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Get zlib compile flags
    uLong flags = zlibCompileFlags();
    
    // Initialize decompression stream
    z_stream d_stream;
    memset(&d_stream, 0, sizeof(z_stream));
    
    // Initialize inflate with zlib version
    int ret = inflateInit_(&d_stream, fuzz_str_2, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set input for decompression
    d_stream.next_in = (Bytef *)data;
    d_stream.avail_in = size;
    
    // Allocate output buffer for decompression
    size_t decomp_size = size * 4; // Reasonable guess
    Bytef *decomp_buf = (Bytef *)malloc(decomp_size);
    if (!decomp_buf) {
        inflateEnd(&d_stream);
        gzclose(gz_in);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    d_stream.next_out = decomp_buf;
    d_stream.avail_out = decomp_size;
    
    // Try to decompress with inflate
    ret = inflate(&d_stream, fuzz_int32_t_3);
    
    // Try inflateSync if we had an error
    if (ret == Z_BUF_ERROR || ret == Z_DATA_ERROR) {
        inflateSync(&d_stream);
    }
    
    // Check sync point
    inflateSyncPoint(&d_stream);
    
    // Reset inflate stream
    inflateReset(&d_stream);
    
    // Reset with specific window bits
    inflateReset2(&d_stream, 15 + 32); // Add 32 for gzip decoding
    
    // Try decompression again
    d_stream.next_in = (Bytef *)data;
    d_stream.avail_in = size;
    d_stream.next_out = decomp_buf;
    d_stream.avail_out = decomp_size;
    
    ret = inflate(&d_stream, fuzz_int32_t_4);
    
    // Get actual decompressed size
    size_t actual_decomp_size = d_stream.total_out;
    
    // End decompression
    inflateEnd(&d_stream);
    
    // Initialize compression stream
    z_stream c_stream;
    memset(&c_stream, 0, sizeof(z_stream));
    
    // Initialize deflate with zlib version
    ret = deflateInit_(&c_stream, fuzz_int32_t_6, fuzz_str_5, sizeof(z_stream));
    if (ret != Z_OK) {
        free(decomp_buf);
        gzclose(gz_in);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Calculate maximum compressed size using deflateBound_z
    z_size_t max_comp_size = deflateBound_z(&c_stream, actual_decomp_size);
    
    // Allocate compression buffer
    Bytef *comp_buf = (Bytef *)malloc(max_comp_size);
    if (!comp_buf) {
        deflateEnd(&c_stream);
        free(decomp_buf);
        gzclose(gz_in);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set compression input
    c_stream.next_in = decomp_buf;
    c_stream.avail_in = actual_decomp_size;
    c_stream.next_out = comp_buf;
    c_stream.avail_out = max_comp_size;
    
    // Change compression parameters
    deflateParams(&c_stream, fuzz_int32_t_7, fuzz_int32_t_8);
    
    // Compress the data
    ret = deflate(&c_stream, fuzz_int32_t_9);
    
    // Get actual compressed size
    size_t actual_comp_size = c_stream.total_out;
    
    // End compression
    deflateEnd(&c_stream);
    
    // Compute CRC32 of original data
    uLong crc_orig = crc32(fuzz_uint64_t_10, Z_NULL, 0);
    crc_orig = crc32(crc_orig, data, size);
    
    // Generate CRC32 combine operator for 64-bit offset
    uLong combine_op = crc32_combine_gen64(actual_decomp_size);
    
    // Create output file for writing compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(comp_buf, 1, actual_comp_size, out_file);
        assert_file_closed(&out_file);;
    }
    
    // Close gzip file
    gzclose(gz_in);
    
    // Clean up allocated resources
    free(decomp_buf);
    free(comp_buf);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
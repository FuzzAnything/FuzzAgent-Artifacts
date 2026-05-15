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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 3
//<Quality> {"density":13,"unique_branches":{"deflateParams":[[797,31,797,76,0,0,4,0]],"gz_vacate":[[386,9,386,66,0,0,4,1],[389,9,389,28,0,0,4,0]]},"library_calls":["gzdopen","deflateInit_","gzclose","adler32","adler32","adler32","adler32","deflate","deflatePending","gzprintf","gzwrite","deflateBound","deflateParams","deflate","gzwrite","adler32_combine","gzprintf","gzclearerr","deflateEnd","gzclose"],"critical_calls":["gzdopen","deflateInit_","adler32","adler32","adler32","deflate","deflatePending","gzprintf","gzwrite","deflateBound","deflateParams","deflate","gzwrite","adler32_combine","gzprintf","gzclearerr","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
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

    // Open gzipped file for writing
    gzFile gz_out = gzdopen(out_fd, fuzz_str_1);
    if (!gz_out) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Initialize zlib structures
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    // Initialize deflate with default parameters
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Read and process input data
    unsigned char in_buf[4096];
    unsigned char out_buf[4096];
    size_t total_read = 0;
    
    // Use some data for Adler-32 calculation
    uLong adler1 = adler32(fuzz_uint64_t_2, Z_NULL, 0);
    uLong adler2 = adler32(fuzz_uint64_t_3, Z_NULL, 0);
    
    // Process in chunks
    while (total_read < size) {
        size_t to_read = (size - total_read > sizeof(in_buf)) ? 
                         sizeof(in_buf) : (size - total_read);
        
        // Read data
        size_t bytes_read = fread(in_buf, 1, to_read, in_file);
        if (bytes_read == 0) {
            break;
        }
        total_read += bytes_read;
        
        // Calculate Adler-32 for first half
        if (total_read <= size/2) {
            adler1 = adler32(adler1, in_buf, bytes_read);
        } else {
            adler2 = adler32(adler2, in_buf, bytes_read);
        }
        
        // Set up deflate input
        strm.next_in = in_buf;
        strm.avail_in = bytes_read;
        
        // Deflate loop
        do {
            strm.next_out = out_buf;
            strm.avail_out = sizeof(out_buf);
            
            int ret = deflate(&strm, fuzz_int32_t_4);
            if (ret != Z_OK && ret != Z_STREAM_END) {
                break;
            }
            
            // Check pending output
            unsigned int pending;
            int bits;
            deflatePending(&strm, &pending, &bits);
            
            // Write compressed data to gzipped file
            size_t have = sizeof(out_buf) - strm.avail_out;
            if (have > 0) {
                // Use gzprintf to write some formatted output
                gzprintf(gz_out, "Chunk: %lu bytes\n", (unsigned long)have);
                gzwrite(gz_out, out_buf, have);
            }
            
        } while (strm.avail_out == 0);
    }
    
    // Get deflate bound for remaining data
    uLong bound = deflateBound(&strm, strm.avail_in);
    
    // Change compression parameters dynamically
    int level = (data[0] % 10) + 1;  // 1-10 based on first byte
    int strategy = data[0] % 4;      // 0-3 based on first byte
    
    deflateParams(&strm, level, strategy);
    
    // Finish deflation
    int flush_ret;
    do {
        strm.next_out = out_buf;
        strm.avail_out = sizeof(out_buf);
        
        flush_ret = deflate(&strm, fuzz_int32_t_5);
        
        size_t have = sizeof(out_buf) - strm.avail_out;
        if (have > 0) {
            gzwrite(gz_out, out_buf, have);
        }
    } while (flush_ret == Z_OK);
    
    // Combine Adler-32 checksums
    off_t len2 = size - size/2;
    uLong combined_adler = adler32_combine(adler1, adler2, len2);
    
    // Write combined Adler-32 to gzipped file
    gzprintf(gz_out, "Combined Adler-32: %lu\n", combined_adler);
    
    // Clear any errors on gzipped file
    gzclearerr(gz_out);
    
    // Clean up
    deflateEnd(&strm);
    gzclose(gz_out);  // This also closes the underlying file descriptor
    
    // Note: gzclose already closed out_fd, so we don't need to fclose out_file
    // But we need to close in_file
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
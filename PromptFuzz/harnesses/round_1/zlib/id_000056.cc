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
//<ID> 124
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["compressBound","compress2","zlibVersion","inflateInit_","uncompress2_z","inflateEnd","gzdopen","inflateEnd","gzwrite","gzclose","inflateEnd","gzputc","gzclose","inflateEnd","gzseek","gzclose","inflateEnd","gzclose","inflateEnd","gzdopen","gzclose_r","inflateEnd"],"critical_calls":["compressBound","compress2","zlibVersion","inflateInit_","uncompress2_z","gzdopen","gzwrite","gzputc","gzseek","gzclose","gzdopen","gzclose_r","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if not enough data
    if (size < 4) {
	return 0;
	}
	
    
    // Step 1: Use compressBound to allocate buffer for compression
    uLong bound = compressBound(size);
    if (bound == 0) {
	return 0;
	}
	
    
    Bytef *compressed = (Bytef*)malloc(bound);
    uLongf compressed_len = bound;
    
    // Step 2: Compress input data using compress2
    int compress_level = data[0] % 10;  // Random level from 0-9
    int compress_result = compress2(compressed, &compressed_len, 
                                    data, size, compress_level);
    if (compress_result != Z_OK) {
        free(compressed);
        return 0;
    }
    
    // Step 3: Initialize z_stream for inflation
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    const char *version = zlibVersion();
    int init_result = inflateInit_(&strm, version, sizeof(z_stream));
    if (init_result != Z_OK) {
        free(compressed);
        return 0;
    }
    
    // Step 4: Uncompress the compressed data using uncompress2_z
    // We'll use a buffer for decompression
    Bytef *decompressed = (Bytef*)malloc(size * 2);  // Extra space
    z_size_t decompressed_len = size * 2;
    z_size_t source_len = compressed_len;
    
    // Note: uncompress2_z modifies source_len to indicate consumed bytes
    int uncompress_result = uncompress2_z(decompressed, &decompressed_len,
                                          compressed, &source_len);
    
    // Clean up compression buffer
    free(compressed);
    free(decompressed);
    
    // Step 5: Create a temporary file for gzip operations
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        inflateEnd(&strm);
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Step 6: Open gzip file using gzdopen
    int fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        inflateEnd(&strm);
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 7: Write some data to gzip file using gzwrite
    // Use a subset of input data for writing
    size_t write_size = size > 100 ? 100 : size;
    int gzwrite_result = gzwrite(gz_out, data, write_size);
    if (gzwrite_result <= 0) {
        gzclose(gz_out);
        inflateEnd(&strm);
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 8: Write a character using gzputc
    int gzputc_result = gzputc(gz_out, 'X');
    if (gzputc_result == -1) {
        gzclose(gz_out);
        inflateEnd(&strm);
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 9: Seek in gzip file using gzseek
    off_t seek_result = gzseek(gz_out, 10, SEEK_SET);
    if (seek_result == -1) {
        gzclose(gz_out);
        inflateEnd(&strm);
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 10: Close gzip files using both gzclose_r and gzclose
    // Since we opened for writing, use gzclose
    int close_result = gzclose(gz_out);
    
    // Step 11: Also test gzclose_r by opening a read stream
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        inflateEnd(&strm);
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int read_fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(read_fd, "rb");
    if (gz_in) {
        // Read one byte
        int c = gzgetc(gz_in);
        int close_r_result = gzclose_r(gz_in);
    } else {
        assert_file_closed(&in_file);;
    }
    
    // Step 12: End inflation stream
    inflateEnd(&strm);
    
    // Clean up temporary file
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(read_fd);
	assert_fd_closed(fd);
	return 0;
}
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
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 2
//<Quality> {"density":22,"unique_branches":{"gz_write":[[230,13,230,33,0,0,4,0],[230,37,230,69,0,0,4,1]]},"library_calls":["zlibCompileFlags","gzdopen","inflateInit2_","gzclose","inflateEnd","gzclose","inflateCopy","inflateEnd","gzclose","inflate","deflateInit2_","inflateEnd","inflateEnd","gzclose","deflateSetHeader","deflateEnd","inflateEnd","inflateEnd","gzclose","gzwrite","gzputc","gzwrite","gzoffset","inflateBackInit_","inflateBackEnd","deflateEnd","inflateEnd","inflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","inflateInit2_","inflateCopy","inflate","deflateInit2_","deflateSetHeader","gzwrite","gzputc","gzwrite","gzoffset","inflateBackInit_","inflateBackEnd","deflateEnd","inflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<9) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 1024) {
	return 0;
	}
	
    
    // Step 1: Get zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Use the value to avoid unused variable warning
    
    // Step 2: Create a gzFile for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
	assert_file_closed(&out_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd, fuzz_str_1);
    if (!gz_out) {
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 3: Set up z_stream for inflation (decompression)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflation with automatic header detection
    if (inflateInit2(&strm, 15 + 32) != Z_OK) {
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set input data for inflation
    strm.next_in = (Bytef*)data;
    strm.avail_in = size;
    
    // Create output buffer for decompressed data
    size_t out_buf_size = size * 2;
    Bytef* out_buf = (Bytef*)malloc(out_buf_size);
    if (!out_buf) {
        inflateEnd(&strm);
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    strm.next_out = out_buf;
    strm.avail_out = out_buf_size;
    
    // Step 4: Create a copy of the inflate stream
    z_stream strm_copy;
    memset(&strm_copy, 0, sizeof(strm_copy));
    
    if (inflateCopy(&strm_copy, &strm) != Z_OK) {
        free(out_buf);
        inflateEnd(&strm);
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 5: Perform inflation with the original stream
    int inflate_result = inflate(&strm, fuzz_int32_t_2);
    
    // Step 6: Set up for deflation with header
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    if (deflateInit2(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                    15 + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        inflateEnd(&strm_copy);
        inflateEnd(&strm);
        free(out_buf);
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Create and set gz_header
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.time = 1234567890;
    header.os = 255; // Unknown OS
    
    if (deflateSetHeader(&def_strm, &header) != Z_OK) {
        deflateEnd(&def_strm);
        inflateEnd(&strm_copy);
        inflateEnd(&strm);
        free(out_buf);
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 7: Write to gzFile using multiple methods
    size_t decompressed_size = strm.total_out;
    
    // Write first part using gzwrite
    size_t write_size = decompressed_size / 2;
    if (write_size > 0) {
        gzwrite(gz_out, out_buf, write_size);
    }
    
    // Write a character using gzputc
    gzputc(gz_out, 'X');
    
    // Write remaining data if any
    if (decompressed_size > write_size) {
        gzwrite(gz_out, out_buf + write_size, decompressed_size - write_size);
    }
    
    // Step 8: Get current offset
    off_t offset = gzoffset(gz_out);
    (void)offset; // Use the value to avoid unused variable warning
    
    // Step 9: Clean up inflateBackEnd (even though we didn't use inflateBack)
    // We'll create a minimal inflateBack setup just to demonstrate the API
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    
    // Initialize for inflateBack (windowBits must be <= 15 for inflateBack)
    unsigned char* window = (unsigned char*)malloc(32768);
    if (window && inflateBackInit(&back_strm, 15, window) == Z_OK) {
        // We won't actually call inflateBack since we don't have proper callbacks
        // But we'll call inflateBackEnd to clean up
        inflateBackEnd(&back_strm);
        free(window);
    } else {
        if (window) free(window);
    }
    
    // Step 10: Clean up all resources
    deflateEnd(&def_strm);
    inflateEnd(&strm_copy);
    inflateEnd(&strm);
    free(out_buf);
    gzclose(gz_out);
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
}
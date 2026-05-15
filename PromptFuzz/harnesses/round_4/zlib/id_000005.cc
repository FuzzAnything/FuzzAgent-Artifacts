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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 102, nr_unique_branch: 6
//<Quality> {"density":17,"unique_branches":{"deflateInit2_":[[435,59,435,68,0,0,4,0]],"gz_error":[[573,9,573,27,0,0,4,0]],"gzclose_w":[[685,9,685,39,0,0,4,0],[687,9,687,20,0,0,4,1]],"gz_init":[[38,13,38,24,0,0,4,0]],"gz_comp":[[71,29,71,49,0,0,4,0]]},"library_calls":["zlibVersion","gzdopen","gzclose","gzdopen","gzclose","gzsetparams","deflateInit2_","gzclose","gzclose","deflateSetHeader","gzeof","gzfread","gzwrite","gzflush","gzflush","deflateEnd","gzclose","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzdopen","gzsetparams","deflateInit2_","deflateSetHeader","gzeof","gzfread","gzwrite","gzflush","gzflush","deflateEnd","gzclose","gzclose"],"visited":0}
/*Here's a fuzz driver that uses all the specified APIs in a coherent way:

*/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful operations
    
    // Get zlib version (API 1)
    const char *version = zlibVersion();
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor and create gzFile for reading
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Create output file for writing compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Get file descriptor and create gzFile for writing
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd_out, fuzz_str_2);
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Set compression parameters on output file (API 2)
    // Use fuzzer data to determine level and strategy
    int level = (data[0] % 10) + 1; // 1-10
    int strategy = data[1] % 4; // 0-3
    if (gzsetparams(gz_out, level, strategy) != Z_OK) {
        // Continue anyway - some parameters might be invalid
    }
    
    // Create z_stream for deflate operations
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    // Initialize deflate with header support
    if (deflateInit2(&strm, level, Z_DEFLATED, 15 + 16, 8, strategy) != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Create and set gz_header (API 3)
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    header.text = (data[2] & 1) ? 1 : 0;
    header.time = (uLong)data[3] << 24 | (uLong)data[4] << 16 | 
                  (uLong)data[5] << 8 | (uLong)data[6];
    header.os = data[7] % 256;
    
    // Set extra field if we have enough data
    if (size > 150) {
        header.extra = (Bytef*)(data + 50);
        header.extra_len = (uInt)(size - 50 > 100 ? 100 : size - 50);
    }
    
    // Set header on deflate stream (API 4)
    deflateSetHeader(&strm, &header);
    
    // Buffer for reading/writing
    const size_t BUF_SIZE = 4096;
    char buffer[BUF_SIZE];
    int total_read = 0;
    
    // Read from input gzFile using gzfread (API 5)
    while (total_read < size && !gzeof(gz_in)) {
        size_t items_to_read = 1;
        size_t bytes_to_read = BUF_SIZE;
        
        // Use fuzzer data to determine read size
        if (total_read + 10 < size) {
            bytes_to_read = (data[total_read + 8] % BUF_SIZE) + 1;
            items_to_read = (data[total_read + 9] % 10) + 1;
        }
        
        // Read using gzfread
        size_t items_read = gzfread(buffer, fuzz_uint64_t_3, items_to_read, gz_in);
        total_read += items_read * bytes_to_read;
        
        if (items_read == 0) break;
        
        // Write to output gzFile
        gzwrite(gz_out, buffer, (unsigned int)(items_read * bytes_to_read));
        
        // Occasionally flush the output (API 6)
        if (total_read % 1000 < 100) {
            int flush_type = (data[total_read % size] % 3);
            gzflush(gz_out, flush_type);
        }
    }
    
    // Final flush (API 6)
    gzflush(gz_out, fuzz_int32_t_4);
    
    // Clean up deflate stream
    deflateEnd(&strm);
    
    // Close all files
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
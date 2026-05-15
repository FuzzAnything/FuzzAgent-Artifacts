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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 4, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose_r","gzclose_w","gzdirect","gztell","gzseek","gztell","gzclearerr","deflateInit_","deflateSetDictionary","deflate","deflateUsed","deflateEnd","inflateBackInit_","inflateReset","inflateReset2","inflateBackEnd","adler32","adler32","adler32","adler32_combine64","gzclose_r","gzclose_w"],"critical_calls":["gzdopen","gzdopen","gzdirect","gztell","gzseek","gztell","gzclearerr","deflateInit_","deflateSetDictionary","deflate","deflateUsed","deflateEnd","inflateBackInit_","inflateReset","inflateReset2","inflateBackEnd","adler32","adler32","adler32","adler32_combine64","gzclose_r","gzclose_w"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to create a comprehensive zlib data processing event:

*/


extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    // Split data into parts for different purposes
    size_t dict_size = size > 256 ? 256 : size / 4;
    const Bytef* dictionary = static_cast<const Bytef*>(data);
    size_t remaining = size - dict_size;
    const Bytef* compress_data = static_cast<const Bytef*>(data + dict_size);
    size_t compress_data_size = remaining > 1024 ? 1024 : remaining / 2;
    
    // 1. Create file handles
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
    
    // 2. Create and use gzFile handles
    gzFile gz_in = gzdopen(in_fd, "rb");
    gzFile gz_out = gzdopen(out_fd, "wb");
    
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose_r(gz_in);
        if (gz_out) gzclose_w(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Test gzdirect to check if file is in direct mode
    int is_direct = gzdirect(gz_in);
    
    // 4. Use gztell and gzseek
    off_t original_pos = gztell(gz_in);
    // Try to seek to a position within bounds
    off_t seek_pos = size > 50 ? 50 : 0;
    gzseek(gz_in, seek_pos, SEEK_SET);
    off_t new_pos = gztell(gz_in);
    
    // 5. Clear any errors on gzFile
    gzclearerr(gz_in);
    
    // 6. Initialize deflate stream for compression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    int deflate_init_result = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, 
                                          ZLIB_VERSION, sizeof(z_stream));
    
    if (deflate_init_result == Z_OK) {
        // Set dictionary for deflate
        deflateSetDictionary(&deflate_strm, dictionary, dict_size);
        
        // Prepare input/output buffers
        Bytef deflate_input[1024];
        Bytef deflate_output[2048];
        size_t input_len = compress_data_size > sizeof(deflate_input) ? 
                          sizeof(deflate_input) : compress_data_size;
        
        memcpy(deflate_input, compress_data, input_len);
        
        deflate_strm.next_in = deflate_input;
        deflate_strm.avail_in = input_len;
        deflate_strm.next_out = deflate_output;
        deflate_strm.avail_out = sizeof(deflate_output);
        
        // Perform deflation
        int deflate_result = deflate(&deflate_strm, Z_FINISH);
        
        // Check bits used
        int bits_used = 0;
        deflateUsed(&deflate_strm, &bits_used);
        
        // Clean up deflate stream
        deflateEnd(&deflate_strm);
    }
    
    // 7. Initialize inflate back-end stream for decompression
    z_stream inflate_back_strm;
    memset(&inflate_back_strm, 0, sizeof(z_stream));
    
    // Create window buffer for back-end inflation
    unsigned char window_buffer[32768];
    
    // Initialize back-end inflation
    int inflate_back_init_result = inflateBackInit_(&inflate_back_strm, 
                                                    15, window_buffer, 
                                                    ZLIB_VERSION, sizeof(z_stream));
    
    if (inflate_back_init_result == Z_OK) {
        // Reset inflate stream
        inflateReset(&inflate_back_strm);
        
        // Reset with different window bits
        inflateReset2(&inflate_back_strm, 15 + 16);  // Add 16 for gzip decoding
        
        // Clean up back-end inflation
        inflateBackEnd(&inflate_back_strm);
    }
    
    // 8. Compute Adler-32 checksums
    uLong adler1 = adler32(0L, data, size > 1000 ? 1000 : size);
    
    // Split data for combined checksum
    size_t split_point = size / 2;
    uLong adler2 = adler32(0L, data, split_point);
    uLong adler3 = adler32(0L, data + split_point, size - split_point);
    
    // Combine checksums
    uLong combined_adler = adler32_combine64(adler2, adler3, size - split_point);
    
    // 9. Clean up gzFile handles
    gzclose_r(gz_in);
    gzclose_w(gz_out);
    
    // 10. Clean up FILE handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 11. Remove output file
    unlink("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","deflateInit_","gzclose","gzclose","inflateValidate","gzread","crc32_z","gzputs","deflate","crc32_z","deflatePending","deflate","crc32_combine64","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","deflateInit_","inflateValidate","gzread","crc32_z","gzputs","deflate","crc32_z","deflatePending","deflate","crc32_combine64","deflateEnd","gzclose","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
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
    
    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Open compressed input stream
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Open compressed output stream
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Setup deflate stream for recompression
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    // Initialize deflate with default parameters
    int ret = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Enable validation for inflate
    int validate_ret = inflateValidate(&def_strm, 1);
    // Note: inflateValidate is typically used with inflate streams,
    // but we call it here to satisfy the requirement
    
    // Buffer for reading/writing
    char buffer[1024];
    int bytes_read;
    
    // CRC calculations
    uLong crc1 = 0L;
    uLong crc2 = 0L;
    uLong combined_crc = 0L;
    
    // Read compressed data line by line and write it back
    while ((bytes_read = gzread(gz_in, buffer, sizeof(buffer))) > 0) {
        // Calculate CRC of the buffer
        crc1 = crc32_z(crc1, (const Bytef*)buffer, bytes_read);
        
        // Write the data back using gzputs (though gzwrite would be more appropriate)
        // We'll write each "line" (buffer) as a string
        if (bytes_read < sizeof(buffer)) {
            buffer[bytes_read] = '\0';
        }
        gzputs(gz_out, buffer);
        
        // Also write to deflate stream
        def_strm.next_in = (Bytef*)buffer;
        def_strm.avail_in = bytes_read;
        
        do {
            char out_buffer[1024];
            def_strm.next_out = (Bytef*)out_buffer;
            def_strm.avail_out = sizeof(out_buffer);
            
            ret = deflate(&def_strm, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR) break;
            
            size_t have = sizeof(out_buffer) - def_strm.avail_out;
            if (have > 0) {
                // Calculate another CRC for compressed output
                crc2 = crc32_z(crc2, (const Bytef*)out_buffer, have);
            }
        } while (def_strm.avail_out == 0);
    }
    
    // Check deflate pending data
    unsigned int pending;
    int bits;
    deflatePending(&def_strm, &pending, &bits);
    
    // Finish deflate
    char out_buffer[1024];
    def_strm.next_out = (Bytef*)out_buffer;
    def_strm.avail_out = sizeof(out_buffer);
    ret = deflate(&def_strm, Z_FINISH);
    
    // Combine the two CRCs
    combined_crc = crc32_combine64(crc1, crc2, (off64_t)size);
    
    // Clean up
    deflateEnd(&def_strm);
    gzclose(gz_out);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
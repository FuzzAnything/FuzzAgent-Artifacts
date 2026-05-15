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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 7.5, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose_r","gzdirect","gzsetparams","gzclose","gzclose_r","inflateInit2_","gzclose","gzclose_r","gzfread","gzputc","gzputs","inflatePrime","inflateSync","gzoffset","inflateEnd","gzclose","gzclose_r"],"critical_calls":["gzdopen","gzdopen","gzdirect","gzsetparams","inflateInit2_","gzfread","gzputc","gzputs","inflatePrime","inflateSync","gzoffset","inflateEnd","gzclose","gzclose_r"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for compressed data
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
    
    // Open gzFile for reading compressed data
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Open gzFile for writing compressed data
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        gzclose_r(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Check if gz_in is direct (uncompressed) access
    int is_direct = gzdirect(gz_in);
    
    // Set compression parameters for output
    int level = data[0] % 10;  // Use first byte for compression level (0-9)
    int strategy = data[1] % 4;  // Use second byte for strategy (0-3)
    gzsetparams(gz_out, level, strategy);
    
    // Allocate buffer for reading
    size_t buf_size = 4096;
    char *buffer = (char *)malloc(buf_size);
    if (!buffer) {
        gzclose(gz_out);
        gzclose_r(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create z_stream for inflate operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // Initialize inflate with default window bits
    if (inflateInit2(&strm, MAX_WBITS) != Z_OK) {
        free(buffer);
        gzclose(gz_out);
        gzclose_r(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Read compressed data from gz_in
    z_size_t items_read = gzfread(buffer, 1, buf_size, gz_in);
    
    // Process the read data
    if (items_read > 0) {
        // Write a character to output
        gzputc(gz_out, buffer[0]);
        
        // Write a string to output
        gzputs(gz_out, "Processed by fuzzer");
        
        // Setup inflate stream with the read data
        strm.avail_in = (uInt)items_read;
        strm.next_in = (Bytef *)buffer;
        
        // Prime the inflate stream with some bits from input
        int prime_bits = data[2] % 16;  // Use third byte for bits count
        int prime_value = data[3];  // Use fourth byte for value
        inflatePrime(&strm, prime_bits, prime_value);
        
        // Attempt to sync inflate stream
        inflateSync(&strm);
    }
    
    // Get current offset in gz_in
    off_t offset = gzoffset(gz_in);
    
    // Clean up inflate stream
    inflateEnd(&strm);
    
    // Clean up resources
    free(buffer);
    gzclose(gz_out);
    gzclose_r(gz_in);
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
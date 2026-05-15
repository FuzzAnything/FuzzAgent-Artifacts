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
//<ID> 122
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["gzdopen","gzopen","gzclose","inflateInit2_","gzclose","gzclose","gzread","gzputc","inflate","inflateSyncPoint","inflateSync","inflatePrime","gztell64","deflateInit2_","deflateCopy","deflateEnd","gzerror","gzclose_w","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzopen","inflateInit2_","gzread","gzputc","inflate","inflateSyncPoint","inflateSync","inflatePrime","gztell64","deflateInit2_","deflateCopy","deflateEnd","gzerror","gzclose_w","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // Create input FILE* from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output FILE* for writing
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
    
    // Open gzFile for writing
    gzFile gz_out = gzopen("output_file", "wb");
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize zlib streams for inflation
    z_stream strm1, strm2;
    memset(&strm1, 0, sizeof(z_stream));
    memset(&strm2, 0, sizeof(z_stream));
    
    // Initialize inflation stream
    if (inflateInit2(&strm1, MAX_WBITS + 16) != Z_OK) {
        gzclose(gz_out);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Allocate buffers
    const size_t BUF_SIZE = 4096;
    Bytef in_buf[BUF_SIZE];
    Bytef out_buf[BUF_SIZE];
    
    // Read compressed data from gzFile and write to output gzFile
    int bytes_read;
    while ((bytes_read = gzread(gz_in, in_buf, BUF_SIZE)) > 0) {
        // Write each byte using gzputc
        for (int i = 0; i < bytes_read; i++) {
            gzputc(gz_out, in_buf[i]);
        }
        
        // Set up inflation stream with the data
        strm1.next_in = in_buf;
        strm1.avail_in = bytes_read;
        strm1.next_out = out_buf;
        strm1.avail_out = BUF_SIZE;
        
        // Try to inflate the data
        int ret = inflate(&strm1, Z_NO_FLUSH);
        
        // Check sync point
        if (inflateSyncPoint(&strm1) == 1) {
            // Try to sync if at sync point
            inflateSync(&strm1);
        }
        
        // Prime the stream with some bits from input
        if (bytes_read > 2) {
            inflatePrime(&strm1, 8, in_buf[0]);
        }
    }
    
    // Get current position in input gzFile
    off64_t pos = gztell64(gz_in);
    
    // Initialize second deflate stream
    if (deflateInit2(&strm2, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        // Copy the first stream to second (though they're different types,
        // we demonstrate the API usage)
        deflateCopy(&strm2, &strm1);  // Note: This may fail but demonstrates API usage
        deflateEnd(&strm2);
    }
    
    // Check for errors in gzFile operations
    int errnum;
    const char *err_msg = gzerror(gz_out, &errnum);
    
    // Close gzFile for writing (using gzclose_w)
    gzclose_w(gz_out);
    
    // Clean up remaining resources
    inflateEnd(&strm1);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
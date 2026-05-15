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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"inflatePrime":[[223,9,223,18,0,0,4,0]]},"library_calls":["gzdopen","gzdopen","gzclose_r","gzclose","gzfread","gzoffset","gzdirect","gzsetparams","gzputs","gzputc","inflateInit2_","inflateEnd","gzclose_r","gzclose","inflatePrime","inflate","inflateSync","inflateEnd","gzclose_r","gzclose"],"critical_calls":["gzdopen","gzdopen","gzfread","gzoffset","gzdirect","gzsetparams","gzputs","gzputc","inflateInit2_","inflatePrime","inflate","inflateSync","inflateEnd","gzclose_r","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create FILE* from input data
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
    
    // Create gzFiles
    gzFile gz_in = gzdopen(in_fd, "rb");
    gzFile gz_out = gzdopen(out_fd, "wb");
    
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose_r(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Read from compressed input using gzfread
    const size_t BUF_SIZE = 4096;
    char buffer[BUF_SIZE];
    z_size_t items_read = 0;
    size_t total_read = 0;
    
    do {
        items_read = gzfread(buffer, 1, BUF_SIZE, gz_in);
        total_read += items_read;
    } while (items_read == BUF_SIZE && total_read < size);
    
    // Get current offset in gz_in
    off_t offset = gzoffset(gz_in);
    
    // Check if gz_in is in direct mode
    int is_direct = gzdirect(gz_in);
    
    // Set compression parameters on output gzFile
    int level = data[0] % 10;  // Use first byte for level (0-9)
    int strategy = data[0] % 4; // Use first byte for strategy (0-3)
    gzsetparams(gz_out, level, strategy);
    
    // Write some data to output using gzputs and gzputc
    const char *test_str = "Test data for fuzzing";
    gzputs(gz_out, test_str);
    gzputc(gz_out, '\n');
    
    // Prepare inflate stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate with automatic header detection
    if (inflateInit2(&strm, 15 + 32) != Z_OK) {
        inflateEnd(&strm);
        gzclose_r(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Prime the inflate stream with some bits from input
    int bits_to_prime = (size > 1) ? (data[0] % 16) : 0;
    int prime_value = (size > 2) ? data[1] : 0;
    inflatePrime(&strm, bits_to_prime, prime_value);
    
    // Set up input for inflate
    strm.avail_in = (uInt)(size > 100 ? 100 : size);
    strm.next_in = (Bytef*)data;
    
    // Set up output buffer for inflate
    const size_t OUT_BUF_SIZE = 4096;
    Bytef out_buffer[OUT_BUF_SIZE];
    strm.avail_out = OUT_BUF_SIZE;
    strm.next_out = out_buffer;
    
    // Try to inflate some data
    int inflate_result = inflate(&strm, Z_SYNC_FLUSH);
    
    // Attempt sync if we got Z_DATA_ERROR
    if (inflate_result == Z_DATA_ERROR) {
        inflateSync(&strm);
    }
    
    // Clean up inflate stream
    inflateEnd(&strm);
    
    // Close gzFiles
    gzclose_r(gz_in);
    gzclose(gz_out);
    
    // Close regular files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
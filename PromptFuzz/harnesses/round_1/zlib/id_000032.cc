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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["gzdopen","inflateInit2_","gzclose","inflateSync","inflateSyncPoint","gzread","gzeof","gzdopen","gzsetparams","gzwrite","gzclose","deflateInit2_","inflateEnd","gzclose","deflateSetHeader","deflate","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","inflateInit2_","inflateSync","inflateSyncPoint","gzread","gzeof","gzdopen","gzsetparams","gzwrite","gzclose","deflateInit2_","deflateSetHeader","deflate","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create input FILE* from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output FILE* for writing compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 1. Open gzFile for reading compressed input
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
    
    // Create z_stream for inflation (decompression)
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    
    // Initialize inflation stream
    if (inflateInit2(&strm_inflate, MAX_WBITS + 32) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Allocate buffer for decompressed data
    std::vector<Bytef> decompressed_buffer(4096);
    strm_inflate.next_out = decompressed_buffer.data();
    strm_inflate.avail_out = decompressed_buffer.size();
    
    // 2. Try to sync to next deflate block
    inflateSync(&strm_inflate);
    
    // 3. Check if we're at sync point
    inflateSyncPoint(&strm_inflate);
    
    // Read some data from gzFile to check EOF
    Bytef read_buffer[1024];
    gzread(gz_in, read_buffer, sizeof(read_buffer));
    
    // 4. Check if we're at end of gzFile
    gzeof(gz_in);
    
    // Open another gzFile for writing with parameters
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        // 5. Set compression parameters
        gzsetparams(gz_out, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
        
        // Write some test data
        const char *test_data = "Test output";
        gzwrite(gz_out, test_data, strlen(test_data));
        
        // Close output gzFile
        gzclose(gz_out);
    }
    
    // Create another z_stream for deflation (compression)
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    
    // Initialize deflation stream
    if (deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Create and set gzip header
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.time = time(NULL);
    header.os = 255; // Unknown OS
    
    deflateSetHeader(&strm_deflate, &header);
    
    // Set input for deflation (using decompressed buffer)
    strm_deflate.next_in = decompressed_buffer.data();
    strm_deflate.avail_in = decompressed_buffer.size() - strm_inflate.avail_out;
    
    // Allocate output buffer for deflation
    std::vector<Bytef> compressed_buffer(4096);
    strm_deflate.next_out = compressed_buffer.data();
    strm_deflate.avail_out = compressed_buffer.size();
    
    // Perform deflation
    deflate(&strm_deflate, Z_FINISH);
    
    // Clean up
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    gzclose(gz_in);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
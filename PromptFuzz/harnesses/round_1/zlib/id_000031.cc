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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","inflateInit2_","gzclose","gzclose","gzeof","gzread","inflate","inflateSync","inflateSyncPoint","deflateInit2_","inflateEnd","gzclose","gzclose","deflateSetHeader","gzsetparams","gzwrite","deflate","inflateEnd","deflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","inflateInit2_","gzeof","gzread","inflate","inflateSync","inflateSyncPoint","deflateInit2_","deflateSetHeader","gzsetparams","gzwrite","deflate","inflateEnd","deflateEnd","gzclose","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified zlib APIs with the given constraints:

*/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // Step 1: Create input and output FILE pointers
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
    
    // Step 2: Create gzFile handles from file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    gzFile gz_in = gzdopen(fd_in, "rb");
    gzFile gz_out = gzdopen(fd_out, "wb");
    
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Step 3: Prepare gzip header for deflateSetHeader
    gz_header header;
    memset(&header, 0, sizeof(header));
    header.text = 1;
    header.os = 255;  // Unknown OS
    header.time = 0;
    
    // Step 4: Set up z_stream for inflation
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    
    // Initialize inflation with maximum window bits (32 for gzip detection)
    if (inflateInit2(&strm_inflate, 32) != Z_OK) {
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Step 5: Read compressed data through gzFile
    unsigned char read_buffer[4096];
    unsigned char inflate_buffer[8192];
    unsigned char deflate_buffer[8192];
    
    // Use gzeof API
    while (!gzeof(gz_in)) {
        int bytes_read = gzread(gz_in, read_buffer, sizeof(read_buffer));
        if (bytes_read <= 0) break;
        
        // Feed data to inflate stream
        strm_inflate.next_in = read_buffer;
        strm_inflate.avail_in = bytes_read;
        strm_inflate.next_out = inflate_buffer;
        strm_inflate.avail_out = sizeof(inflate_buffer);
        
        // Try to inflate
        int ret = inflate(&strm_inflate, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) break;
        
        // Use inflateSync when we have compressed data
        if (ret == Z_BUF_ERROR && strm_inflate.avail_in > 0) {
            // Attempt to find sync point
            inflateSync(&strm_inflate);
        }
        
        // Check sync point status
        inflateSyncPoint(&strm_inflate);
        
        // Write inflated data if we have any
        if (sizeof(inflate_buffer) - strm_inflate.avail_out > 0) {
            fwrite(inflate_buffer, 1, sizeof(inflate_buffer) - strm_inflate.avail_out, out_file);
        }
    }
    
    // Step 6: Set up z_stream for deflation with header
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    
    if (deflateInit2(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     31, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Use deflateSetHeader API
    deflateSetHeader(&strm_deflate, &header);
    
    // Step 7: Set parameters on gzFile
    // Reset file position to beginning
    fseek(out_file, 0, SEEK_SET);
    
    // Apply compression parameters
    gzsetparams(gz_out, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    
    // Step 8: Read from output file, compress through gzFile
    rewind(out_file);
    while (!feof(out_file)) {
        size_t bytes = fread(read_buffer, 1, sizeof(read_buffer), out_file);
        if (bytes == 0) break;
        
        // Write through gzFile
        gzwrite(gz_out, read_buffer, bytes);
        
        // Also compress through deflate stream
        strm_deflate.next_in = read_buffer;
        strm_deflate.avail_in = bytes;
        strm_deflate.next_out = deflate_buffer;
        strm_deflate.avail_out = sizeof(deflate_buffer);
        
        deflate(&strm_deflate, Z_FINISH);
    }
    
    // Step 9: Clean up all resources
    inflateEnd(&strm_inflate);
    deflateEnd(&strm_deflate);
    gzclose(gz_in);
    gzclose(gz_out);
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
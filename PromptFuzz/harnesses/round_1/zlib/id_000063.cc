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
//<ID> 154
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 2
//<Quality> {"density":8,"unique_branches":{"gzungetc":[[516,9,516,27,0,0,4,0],[516,31,516,49,0,0,4,0]]},"library_calls":["gzdopen","inflateInit2_","gzclose","inflateInit_","inflateEnd","gzclose","gzungetc","gzgets","inflate","inflateSync","inflateSyncPoint","deflateInit2_","inflateEnd","inflateEnd","gzclose","deflateSetHeader","gzdopen","deflateEnd","inflateEnd","inflateEnd","gzclose","gzsetparams","gzwrite","deflate","inflateEnd","deflateEnd","gzclose","gzclose","inflateEnd"],"critical_calls":["gzdopen","inflateInit2_","inflateInit_","gzungetc","gzgets","inflate","inflateSync","inflateSyncPoint","deflateInit2_","deflateSetHeader","gzdopen","gzsetparams","gzwrite","deflate","inflateEnd","deflateEnd","gzclose","gzclose","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from the fuzzing data
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
    
    // Variables for zlib operations
    z_stream strm_inflate1, strm_inflate2, strm_deflate;
    gz_header head;
    gzFile gz_input = nullptr, gz_output = nullptr;
    int ret;
    
    // Initialize gz_header
    memset(&head, 0, sizeof(head));
    head.text = 1;
    head.os = 255;
    
    // 1. Open compressed input file with gzdopen
    gz_input = gzdopen(in_fd, "rb");
    if (!gz_input) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Initialize first inflate stream with inflateInit2_
    memset(&strm_inflate1, 0, sizeof(strm_inflate1));
    ret = inflateInit2_(&strm_inflate1, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_input);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Initialize second inflate stream with inflateInit_
    memset(&strm_inflate2, 0, sizeof(strm_inflate2));
    ret = inflateInit_(&strm_inflate2, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate1);
        gzclose(gz_input);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 4. Try to ungetc if there's data
    if (size > 0) {
        gzungetc(data[0], gz_input);
    }
    
    // 5. Read from gzFile using gzgets
    char buffer[1024];
    if (size > 0) {
        gzgets(gz_input, buffer, sizeof(buffer));
    }
    
    // 6. Try inflateSync on first stream
    if (size > 0) {
        // Set up input for inflate
        strm_inflate1.next_in = (Bytef*)data;
        strm_inflate1.avail_in = size > 100 ? 100 : (uInt)size;
        
        // Try to inflate some data
        unsigned char out_buf[1024];
        strm_inflate1.next_out = out_buf;
        strm_inflate1.avail_out = sizeof(out_buf);
        
        inflate(&strm_inflate1, Z_NO_FLUSH);
        
        // Try sync
        inflateSync(&strm_inflate1);
    }
    
    // 7. Check sync point
    inflateSyncPoint(&strm_inflate1);
    
    // 8. Initialize deflate stream with deflateInit2_
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15 + 16, 8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate1);
        inflateEnd(&strm_inflate2);
        gzclose(gz_input);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 9. Set header for deflate stream
    deflateSetHeader(&strm_deflate, &head);
    
    // 10. Open output gzFile
    gz_output = gzdopen(out_fd, "wb");
    if (!gz_output) {
        deflateEnd(&strm_deflate);
        inflateEnd(&strm_inflate1);
        inflateEnd(&strm_inflate2);
        gzclose(gz_input);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 11. Set parameters for gz output
    gzsetparams(gz_output, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
    
    // 12. Write some data to gz output
    if (size > 0) {
        const char *test_data = "Test output data";
        gzwrite(gz_output, test_data, strlen(test_data));
    }
    
    // 13. Use deflate to compress some data
    if (size > 0) {
        unsigned char deflate_in[1024];
        unsigned char deflate_out[2048];
        
        // Copy some input data for deflation
        size_t copy_size = size > sizeof(deflate_in) ? sizeof(deflate_in) : size;
        memcpy(deflate_in, data, copy_size);
        
        strm_deflate.next_in = deflate_in;
        strm_deflate.avail_in = copy_size;
        strm_deflate.next_out = deflate_out;
        strm_deflate.avail_out = sizeof(deflate_out);
        
        deflate(&strm_deflate, Z_FINISH);
    }
    
    // Clean up all resources
    // 14. End inflate streams
    inflateEnd(&strm_inflate1);
    
    // 15. End deflate stream
    deflateEnd(&strm_deflate);
    
    // 16. Close gz files
    gzclose(gz_input);
    gzclose(gz_output);
    
    // End second inflate stream
    inflateEnd(&strm_inflate2);
    
    // Close regular files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
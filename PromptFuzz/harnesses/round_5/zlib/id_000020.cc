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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["zlibVersion","gzdopen","gzseek","gzread","gzseek","inflateInit2_","gzclose","inflateBackInit_","inflateEnd","gzclose","deflateInit_","inflateBackEnd","inflateEnd","gzclose","inflateValidate","inflate","inflateResetKeep","inflate","deflateParams","deflatePrime","deflate","deflateEnd","inflateBackEnd","inflateEnd","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzseek","gzread","gzseek","inflateInit2_","inflateBackInit_","deflateInit_","inflateValidate","inflate","inflateResetKeep","inflate","deflateParams","deflatePrime","deflate","deflateEnd","inflateBackEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get zlib version
    const char *version = zlibVersion();
    
    // Create input FILE* from the fuzzer data
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
    
    // 2. Set up gzFile for seeking
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
    
    // 3. Use gzseek to navigate through compressed data
    // Seek to beginning
    gzseek(gz_in, 0, SEEK_SET);
    
    // Read some data to advance position
    char buffer[1024];
    gzread(gz_in, buffer, sizeof(buffer));
    
    // Seek back to beginning
    gzseek(gz_in, 0, SEEK_SET);
    
    // 4. Set up z_stream for inflation (decompression)
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(strm_inflate));
    
    // Initialize inflation with windowBits = 15 + 32 for automatic header detection
    if (inflateInit2_(&strm_inflate, 15 + 32, version, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 5. Set up inflateBack for callback-based inflation
    z_stream strm_inflate_back;
    memset(&strm_inflate_back, 0, sizeof(strm_inflate_back));
    
    // Window for inflateBack
    unsigned char window[32768];
    
    // Initialize inflateBack
    if (inflateBackInit_(&strm_inflate_back, 15, window, version, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Set up z_stream for deflation (compression)
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(strm_deflate));
    
    // Initialize deflation
    if (deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION) != Z_OK) {
        inflateBackEnd(&strm_inflate_back);
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 7. Use inflateValidate to enable validation during inflation
    inflateValidate(&strm_inflate, 1);  // Enable validation
    
    // 8. Process the input data through inflation
    strm_inflate.next_in = (Bytef*)data;
    strm_inflate.avail_in = size;
    
    unsigned char inflate_out[4096];
    strm_inflate.next_out = inflate_out;
    strm_inflate.avail_out = sizeof(inflate_out);
    
    int inflate_result = inflate(&strm_inflate, Z_NO_FLUSH);
    
    // 9. Use inflateResetKeep to reset inflation stream while keeping dictionary
    inflateResetKeep(&strm_inflate);
    
    // Reset and try again
    strm_inflate.next_in = (Bytef*)data;
    strm_inflate.avail_in = size;
    strm_inflate.next_out = inflate_out;
    strm_inflate.avail_out = sizeof(inflate_out);
    inflate(&strm_inflate, Z_FINISH);
    
    // 10. Set up deflation with specific parameters
    // First use deflateParams to change compression level and strategy
    deflateParams(&strm_deflate, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    
    // 11. Use deflatePrime to insert bits into the bitstream
    // Insert 5 bits with value 0x1F (binary 11111)
    deflatePrime(&strm_deflate, 5, 0x1F);
    
    // 12. Process the inflated data through deflation
    strm_deflate.next_in = inflate_out;
    strm_deflate.avail_in = sizeof(inflate_out) - strm_inflate.avail_out;
    
    unsigned char deflate_out[8192];
    strm_deflate.next_out = deflate_out;
    strm_deflate.avail_out = sizeof(deflate_out);
    
    int deflate_result = deflate(&strm_deflate, Z_FINISH);
    
    // 13. Write the deflated output to file
    size_t deflated_size = sizeof(deflate_out) - strm_deflate.avail_out;
    if (deflated_size > 0) {
        fwrite(deflate_out, 1, deflated_size, out_file);
    }
    
    // 14. Clean up all resources
    deflateEnd(&strm_deflate);
    inflateBackEnd(&strm_inflate_back);
    inflateEnd(&strm_inflate);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Note: The gzFile already closed the underlying fd, so we don't need to close it again
    // The in_file and out_file FILE* pointers are already closed above
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
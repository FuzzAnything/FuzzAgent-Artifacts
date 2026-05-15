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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["zlibCompileFlags","gzdopen","gzbuffer","gzread","inflateInit_","zError","gzclose","inflateValidate","inflate","inflateCopy","inflateEnd","deflateInit_","deflateTune","deflate","deflateUsed","deflateEnd","inflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","gzbuffer","gzread","inflateInit_","inflateValidate","inflate","inflateCopy","inflateEnd","deflateInit_","deflateTune","deflate","deflateUsed","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 10) {
	return 0;
	}
	
    
    // 1. Get zlib compile flags
    uLong flags = zlibCompileFlags();
    (void)flags; // Use flags to avoid unused variable warning
    
    // 2. Create input FILE* using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Get file descriptor and create gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Set buffer size for gzFile
    int buffer_result = gzbuffer(gz_in, 8192);
    (void)buffer_result; // Use result to avoid unused warning
    
    // 5. Read some data from gzFile
    char read_buffer[1024];
    int bytes_read = gzread(gz_in, read_buffer, sizeof(read_buffer));
    
    // 6. Initialize z_stream for inflation
    z_stream strm_in;
    strm_in.zalloc = Z_NULL;
    strm_in.zfree = Z_NULL;
    strm_in.opaque = Z_NULL;
    strm_in.avail_in = 0;
    strm_in.next_in = Z_NULL;
    
    int init_result = inflateInit(&strm_in);
    if (init_result != Z_OK) {
        // Use zError to get error string
        const char *err_msg = zError(init_result);
        (void)err_msg; // Use error message
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Set up input for inflation from our data
    strm_in.avail_in = (uInt)(size > 1024 ? 1024 : size);
    strm_in.next_in = (Bytef *)data;
    
    // 8. Enable validation during inflation
    int validate_result = inflateValidate(&strm_in, 1);
    (void)validate_result; // Use result
    
    // 9. Create output buffer for inflation
    Bytef out_buffer[2048];
    strm_in.avail_out = sizeof(out_buffer);
    strm_in.next_out = out_buffer;
    
    // 10. Perform inflation
    int inflate_result = inflate(&strm_in, Z_NO_FLUSH);
    
    // 11. Copy the inflation stream state
    z_stream strm_copy;
    strm_copy.zalloc = Z_NULL;
    strm_copy.zfree = Z_NULL;
    strm_copy.opaque = Z_NULL;
    
    int copy_result = inflateCopy(&strm_copy, &strm_in);
    if (copy_result == Z_OK) {
        // Clean up the copied stream
        inflateEnd(&strm_copy);
    }
    
    // 12. Initialize deflate stream for compression
    z_stream strm_deflate;
    strm_deflate.zalloc = Z_NULL;
    strm_deflate.zfree = Z_NULL;
    strm_deflate.opaque = Z_NULL;
    
    int deflate_init = deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION);
    if (deflate_init == Z_OK) {
        // 13. Tune the deflate parameters
        int tune_result = deflateTune(&strm_deflate, 8, 9, 4, 3);
        (void)tune_result; // Use result
        
        // 14. Set up input for deflation (using inflated data if available)
        if (inflate_result == Z_OK || inflate_result == Z_STREAM_END) {
            strm_deflate.avail_in = (uInt)(sizeof(out_buffer) - strm_in.avail_out);
            strm_deflate.next_in = out_buffer;
            
            Bytef deflate_out[4096];
            strm_deflate.avail_out = sizeof(deflate_out);
            strm_deflate.next_out = deflate_out;
            
            // 15. Perform deflation
            int deflate_result = deflate(&strm_deflate, Z_FINISH);
            
            // 16. Check how many bits were used
            int bits_used = 0;
            int used_result = deflateUsed(&strm_deflate, &bits_used);
            (void)used_result; // Use result
            (void)bits_used; // Use bits_used
        }
        
        // Clean up deflate stream
        deflateEnd(&strm_deflate);
    }
    
    // 17. Clean up all resources
    inflateEnd(&strm_in);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
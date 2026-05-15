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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.666667, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"compress2_z":[[55,13,55,33,0,0,4,1]]},"library_calls":["zlibVersion","inflateInit2_","inflateEnd","deflateInit_","inflateEnd","deflateEnd","inflateResetKeep","deflateParams","inflateBackInit_","inflateBackEnd","inflateValidate","compress_z","compress2_z","gzdopen","gzread","gzclose","deflatePrime","inflate","deflate","deflateEnd","inflateEnd"],"critical_calls":["zlibVersion","inflateInit2_","deflateInit_","inflateResetKeep","deflateParams","inflateBackInit_","inflateBackEnd","inflateValidate","compress_z","compress2_z","gzdopen","gzread","gzclose","deflatePrime","inflate","deflate","deflateEnd","inflateEnd"],"visited":2}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs with the given constraints:

*/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    // 1. Get zlib version
    const char* version = zlibVersion();
    
    // Split input data for different operations
    size_t quarter = size / 4;
    const uint8_t* comp_data = data;
    size_t comp_size = quarter * 2;
    const uint8_t* decomp_data = data + quarter * 2;
    size_t decomp_size = quarter * 2;
    
    // 2. Setup streams
    z_stream strm_inflate, strm_deflate, strm_back;
    memset(&strm_inflate, 0, sizeof(z_stream));
    memset(&strm_deflate, 0, sizeof(z_stream));
    memset(&strm_back, 0, sizeof(z_stream));
    
    // 3. Initialize inflation with provided data
    int ret = inflateInit2_(&strm_inflate, 15 + 16, version, sizeof(z_stream));
    if (ret != Z_OK) {
        // Cleanup and return
        inflateEnd(&strm_inflate);
        return 0;
    }
    
    // 4. Initialize deflation
    ret = deflateInit_(&strm_deflate, Z_DEFAULT_COMPRESSION, version, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        deflateEnd(&strm_deflate);
        return 0;
    }
    
    // 5. Reset inflate stream keeping dictionary
    inflateResetKeep(&strm_inflate);
    
    // 6. Set deflate parameters
    deflateParams(&strm_deflate, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    
    // 7. Setup inflate back stream with window buffer
    unsigned char window[32768];
    ret = inflateBackInit_(&strm_back, 15, window, version, sizeof(z_stream));
    
    // 8. End inflate back stream (cleanup)
    if (ret == Z_OK) {
        inflateBackEnd(&strm_back);
    }
    
    // 9. Set validation on inflate stream
    inflateValidate(&strm_inflate, 1);
    
    // 10-11. Use compression APIs
    Bytef comp_output[1024];
    z_size_t comp_out_len = 1024;
    
    // Use compress_z
    if (comp_size > 0) {
        compress_z(comp_output, &comp_out_len, comp_data, comp_size);
    }
    
    // Use compress2_z with different level
    Bytef comp2_output[1024];
    z_size_t comp2_out_len = 1024;
    if (comp_size > 0) {
        compress2_z(comp2_output, &comp2_out_len, comp_data, comp_size, Z_BEST_SPEED);
    }
    
    // 12-13. Work with gzFile using file descriptors
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        gzFile gz = gzdopen(fd, "rb");
        
        if (gz) {
            // Read a small amount from gz file
            char buffer[100];
            gzread(gz, buffer, sizeof(buffer));
            
            // 14. Close gzFile
            gzclose(gz);
        }
        assert_file_closed(&in_file);;
    }
    
    // 15. Use deflatePrime with bits from input
    if (size > 0) {
        int bits = data[0] & 0x07;
        int value = data[1] & 0xFF;
        deflatePrime(&strm_deflate, bits, value);
    }
    
    // 16. Perform inflation with provided compressed data
    Bytef inflate_out[4096];
    strm_inflate.next_in = (Bytef*)decomp_data;
    strm_inflate.avail_in = decomp_size < 100 ? decomp_size : 100;
    strm_inflate.next_out = inflate_out;
    strm_inflate.avail_out = sizeof(inflate_out);
    
    inflate(&strm_inflate, Z_NO_FLUSH);
    
    // 17. Perform deflation with inflation output
    Bytef deflate_out[4096];
    strm_deflate.next_in = inflate_out;
    strm_deflate.avail_in = strm_inflate.total_out < sizeof(inflate_out) ? 
                           strm_inflate.total_out : sizeof(inflate_out);
    strm_deflate.next_out = deflate_out;
    strm_deflate.avail_out = sizeof(deflate_out);
    
    deflate(&strm_deflate, Z_FINISH);
    
    // 18-19. Clean up streams
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    
    assert_file_closed(&in_file);
	return 0;
}
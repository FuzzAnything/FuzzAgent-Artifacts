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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"fill_window":[[330,21,330,57,0,0,4,0]],"deflate":[[1045,13,1045,29,0,0,4,0],[1051,13,1051,29,0,0,4,0]]},"library_calls":["zlibVersion","gzdopen","gzread","gzclose","inflateInit2_","inflateValidate","inflate","inflateResetKeep","deflateInit_","inflateEnd","deflateSetDictionary","deflateParams","deflatePrime","deflate","compressBound_z","compress_z","compressBound_z","compress2_z","inflateBackInit_","inflateBack","inflateBackEnd","deflateEnd","inflateEnd"],"critical_calls":["zlibVersion","gzdopen","gzread","gzclose","inflateInit2_","inflateValidate","inflate","inflateResetKeep","deflateInit_","deflateSetDictionary","deflateParams","deflatePrime","deflate","compressBound_z","compress_z","compressBound_z","compress2_z","inflateBackInit_","inflateBack","inflateBackEnd","deflateEnd","inflateEnd"],"visited":1}
/*Here's a comprehensive fuzz driver that utilizes all the specified zlib APIs to achieve a complete round-trip compression/decompression cycle with various zlib features:

*/


// Callback functions for inflateBack
static unsigned int  id_000035_back_in_func(void* opaque, unsigned char** buf) {
    *buf = (unsigned char*)opaque;
    return 0; // No more input
}

static int  id_000035_back_out_func(void* opaque, unsigned char* buf, unsigned int len) {
    return 0; // Discard output
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need enough data for meaningful fuzzing
    
    // 1. Get zlib version
    const char* version = zlibVersion();
    
    // 2. Prepare input and output files
    FILE* in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 3. Use gzFile APIs
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
    
    // Read some data using gzread
    char gz_buffer[1024];
    int gz_read = gzread(gz_in, gz_buffer, sizeof(gz_buffer));
    
    // Close gz file
    gzclose(gz_in);
    
    // 4. Initialize decompression stream
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    if (inflateInit2_(&strm_inflate, 15 + 32, version, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Set up input for decompression
    strm_inflate.next_in = (Bytef*)data;
    strm_inflate.avail_in = size;
    
    // 6. Allocate output buffer for decompression
    Bytef* decompressed = new Bytef[size * 4];
    strm_inflate.next_out = decompressed;
    strm_inflate.avail_out = size * 4;
    
    // 7. Perform decompression with validation
    inflateValidate(&strm_inflate, 1); // Enable validation
    
    int inflate_result = inflate(&strm_inflate, Z_FINISH);
    
    size_t decompressed_size = size * 4 - strm_inflate.avail_out;
    
    // 8. Reset decompression stream (keep dictionary)
    inflateResetKeep(&strm_inflate);
    
    // 9. Initialize compression stream
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    if (deflateInit_(&strm_deflate, Z_DEFAULT_COMPRESSION, version, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&strm_inflate);
        delete[] decompressed;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 10. Set dictionary for compression (use first 256 bytes of data as dictionary)
    size_t dict_size = size > 256 ? 256 : size;
    deflateSetDictionary(&strm_deflate, data, dict_size);
    
    // 11. Change compression parameters during operation
    deflateParams(&strm_deflate, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    
    // 12. Insert bits into deflate stream
    deflatePrime(&strm_deflate, 5, 31); // Add some bits
    
    // 13. Prepare for compression
    strm_deflate.next_in = decompressed;
    strm_deflate.avail_in = decompressed_size;
    
    // 14. Allocate output buffer for compression
    Bytef* recompressed = new Bytef[size * 4];
    strm_deflate.next_out = recompressed;
    strm_deflate.avail_out = size * 4;
    
    // 15. Perform compression
    int deflate_result = deflate(&strm_deflate, Z_FINISH);
    
    size_t recompressed_size = size * 4 - strm_deflate.avail_out;
    
    // 16. Use compress_z API on original data
    z_size_t comp_size_z = compressBound_z(size);
    Bytef* comp_buffer_z = new Bytef[comp_size_z];
    if (compress_z(comp_buffer_z, &comp_size_z, data, size) == Z_OK) {
        // Successfully compressed
    }
    
    // 17. Use compress2_z API on original data with different level
    z_size_t comp2_size_z = compressBound_z(size);
    Bytef* comp2_buffer_z = new Bytef[comp2_size_z];
    if (compress2_z(comp2_buffer_z, &comp2_size_z, data, size, Z_BEST_SPEED) == Z_OK) {
        // Successfully compressed with different level
    }
    
    // 18. Initialize inflateBack for callback-based decompression
    z_stream strm_back;
    memset(&strm_back, 0, sizeof(z_stream));
    
    unsigned char back_window[32768];
    if (inflateBackInit_(&strm_back, 15, back_window, version, sizeof(z_stream)) == Z_OK) {
        // Set up callbacks
        void* back_in_data = (void*)data;
        
        // Perform inflateBack
        int back_result = inflateBack(&strm_back,  id_000035_back_in_func, back_in_data, 
                                      id_000035_back_out_func, NULL);
        
        // End inflateBack
        inflateBackEnd(&strm_back);
    }
    
    // 19. Write compressed data to output file
    if (out_file && recompressed_size > 0) {
        fwrite(recompressed, 1, recompressed_size, out_file);
    }
    
    // 20. Clean up all resources
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    
    delete[] decompressed;
    delete[] recompressed;
    delete[] comp_buffer_z;
    delete[] comp2_buffer_z;
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
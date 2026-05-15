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
//<ID> 338
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 1
//<Quality> {"density":11,"unique_branches":{"deflateStateCheck":[[541,9,541,38,0,0,4,0]]},"library_calls":["zlibVersion","compressBound_z","deflateInit_","deflateSetDictionary","deflate","deflateParams","deflateEnd","inflateInit_","inflatePrime","inflateMark","inflate","inflateReset2","inflateEnd","deflateInit_","deflateBound","inflateCopy","deflateEnd","deflateEnd","gzdopen","gzbuffer","gzwrite","gzclose","uncompress2_z","gzopen64","gzclose"],"critical_calls":["zlibVersion","compressBound_z","deflateInit_","deflateSetDictionary","deflate","deflateParams","deflateEnd","inflateInit_","inflatePrime","inflateMark","inflate","inflateReset2","inflateEnd","deflateInit_","deflateBound","inflateCopy","deflateEnd","deflateEnd","gzdopen","gzbuffer","gzwrite","gzclose","uncompress2_z","gzopen64","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to implement a comprehensive compression/decompression workflow:

*/


extern "C" int LLVMFuzzerTestOneInput_101(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful fuzzing
    
    // 1. Get zlib version
    const char* version = zlibVersion();
    (void)version;  // Avoid unused variable warning
    
    // 2. Create input/output files
    FILE* in_file = fmemopen((void*)data, size, "rb");
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
    
    // 3. Initialize z_stream for compression
    z_stream comp_stream;
    memset(&comp_stream, 0, sizeof(z_stream));
    
    // 4. Allocate buffers
    size_t bound_size = compressBound_z(size);
    Bytef* comp_buffer = (Bytef*)malloc(bound_size);
    Bytef* decomp_buffer = (Bytef*)malloc(size * 2);  // Extra space for safety
    Bytef* dict_buffer = (Bytef*)malloc(size / 4);
    
    if (!comp_buffer || !decomp_buffer || !dict_buffer) {
        free(comp_buffer);
        free(decomp_buffer);
        free(dict_buffer);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Prepare dictionary from first part of input
    uInt dict_len = size / 4;
    if (dict_len > 0) {
        memcpy(dict_buffer, data, dict_len);
    }
    
    // 6. Test compression with dictionary
    if (deflateInit_(&comp_stream, Z_DEFAULT_COMPRESSION, 
                    ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        deflateSetDictionary(&comp_stream, dict_buffer, dict_len);
        
        comp_stream.next_in = (Bytef*)data;
        comp_stream.avail_in = size;
        comp_stream.next_out = comp_buffer;
        comp_stream.avail_out = bound_size;
        
        deflate(&comp_stream, Z_FINISH);
        deflateParams(&comp_stream, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
        deflateEnd(&comp_stream);
    }
    
    // 7. Test decompression
    z_stream decomp_stream;
    memset(&decomp_stream, 0, sizeof(z_stream));
    
    if (inflateInit_(&decomp_stream, ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        inflatePrime(&decomp_stream, 2, 0x03);  // Add some bits
        inflateMark(&decomp_stream);
        
        decomp_stream.next_in = comp_buffer;
        decomp_stream.avail_in = comp_stream.total_out;
        decomp_stream.next_out = decomp_buffer;
        decomp_stream.avail_out = size * 2;
        
        inflate(&decomp_stream, Z_FINISH);
        inflateReset2(&decomp_stream, MAX_WBITS);
        inflateEnd(&decomp_stream);
    }
    
    // 8. Test compression with different parameters
    z_stream comp_stream2;
    memset(&comp_stream2, 0, sizeof(z_stream));
    
    if (deflateInit_(&comp_stream2, Z_BEST_SPEED, 
                    ZLIB_VERSION, sizeof(z_stream)) == Z_OK) {
        // Test deflateBound
        uLong bound = deflateBound(&comp_stream2, size);
        (void)bound;  // Avoid unused warning
        
        // Test stream copy
        z_stream comp_stream_copy;
        memset(&comp_stream_copy, 0, sizeof(z_stream));
        inflateCopy(&comp_stream_copy, &comp_stream2);
        deflateEnd(&comp_stream_copy);
        
        deflateEnd(&comp_stream2);
    }
    
    // 9. Test gzip file operations
    int fd = fuzz_fileno(out_file);
    gzFile gz = gzdopen(fd, "wb9");
    if (gz) {
        gzbuffer(gz, 8192);  // Set buffer size
        gzwrite(gz, decomp_buffer, decomp_stream.total_out);
        gzclose(gz);  // This also closes the underlying fd
    } else {
        assert_file_closed(&out_file);;
    }
    
    // 10. Test uncompress2_z with buffer API
    z_size_t source_len = size;
    z_size_t dest_len = size * 2;
    if (uncompress2_z(decomp_buffer, &dest_len, data, &source_len) != Z_OK) {
        // This is expected to fail sometimes with random input
    }
    
    // 11. Test file-based gzip operations
    gzFile gzfile = gzopen64("input_file", "rb");
    if (gzfile) {
        // Note: "input_file" doesn't exist, but we still test the API
        gzclose(gzfile);
    }
    
    // 12. Clean up all resources
    free(comp_buffer);
    free(decomp_buffer);
    free(dict_buffer);
    
    if (in_file) assert_file_closed(&in_file);;
    
    // Remove output file if created
    remove("output_file");
    remove("input_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}
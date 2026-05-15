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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["inflateInit_","inflateEnd","inflate","inflateMark","inflateEnd","gzdopen","gzsetparams","gzwrite","gztell64","compressBound","compress2","compressBound_z","compress_z","deflateInit_","gzclose","deflateBound_z","deflateEnd","gzclose","deflate","deflateParams","deflateResetKeep","deflateEnd","gzclose"],"critical_calls":["inflateInit_","inflate","inflateMark","inflateEnd","gzdopen","gzsetparams","gzwrite","gztell64","compressBound","compress2","compressBound_z","compress_z","deflateInit_","deflateBound_z","deflate","deflateParams","deflateResetKeep","deflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to process zlib-compressed input data:

*/


extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Step 1: First decompress the input data (which is zlib output)
    z_stream stream_inflate;
    memset(&stream_inflate, 0, sizeof(stream_inflate));
    
    // Initialize decompression stream
    if (inflateInit(&stream_inflate) != Z_OK) {
        return 0;
    }
    
    // Set input data for decompression
    stream_inflate.next_in = (Bytef *)data;
    stream_inflate.avail_in = size;
    
    // Allocate output buffer for decompressed data
    size_t decompressed_size = size * 10; // Conservative estimate
    Bytef *decompressed_data = (Bytef *)malloc(decompressed_size);
    if (!decompressed_data) {
        inflateEnd(&stream_inflate);
        return 0;
    }
    
    stream_inflate.next_out = decompressed_data;
    stream_inflate.avail_out = decompressed_size;
    
    // Decompress the data
    int inflate_result = inflate(&stream_inflate, Z_FINISH);
    
    // Get mark position during decompression
    long mark_position = inflateMark(&stream_inflate);
    
    // Clean up decompression stream
    inflateEnd(&stream_inflate);
    
    if (inflate_result != Z_STREAM_END && inflate_result != Z_OK) {
        free(decompressed_data);
        return 0;
    }
    
    // Get actual decompressed size
    size_t actual_decompressed_size = decompressed_size - stream_inflate.avail_out;
    
    // Step 2: Create a gzFile for compression
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        free(decompressed_data);
        assert_file_closed(&out_file);
	return 0;
    }
	int fuzz_fd_0 = fuzz_fileno(out_file);

    
    gzFile gz_out = gzdopen(fuzz_fd_0, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        free(decompressed_data);
        assert_file_closed(&out_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // Set gzFile parameters
    gzsetparams(gz_out, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
    
    // Write some data to gzFile
    gzwrite(gz_out, decompressed_data, actual_decompressed_size);
    
    // Get current position in gzFile
    off64_t gz_position = gztell64(gz_out);
    
    // Step 3: Compress using compress2
    uLongf compressed_size1 = compressBound(actual_decompressed_size);
    Bytef *compressed_data1 = (Bytef *)malloc(compressed_size1);
    if (compressed_data1) {
        compress2(compressed_data1, &compressed_size1, decompressed_data, 
                 actual_decompressed_size, Z_BEST_COMPRESSION);
        free(compressed_data1);
    }
    
    // Step 4: Compress using compress_z
    z_size_t compressed_size2 = compressBound_z(actual_decompressed_size);
    Bytef *compressed_data2 = (Bytef *)malloc(compressed_size2);
    if (compressed_data2) {
        compress_z(compressed_data2, &compressed_size2, decompressed_data, 
                  actual_decompressed_size);
        free(compressed_data2);
    }
    
    // Step 5: Use deflate stream operations
    z_stream stream_deflate;
    memset(&stream_deflate, 0, sizeof(stream_deflate));
    
    // Initialize deflation stream
    if (deflateInit(&stream_deflate, Z_DEFAULT_COMPRESSION) != Z_OK) {
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        free(decompressed_data);
        assert_file_closed(&out_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    // Set input for deflation
    stream_deflate.next_in = decompressed_data;
    stream_deflate.avail_in = actual_decompressed_size;
    
    // Allocate output buffer for deflation
    z_size_t deflate_bound = deflateBound_z(&stream_deflate, actual_decompressed_size);
    Bytef *deflated_data = (Bytef *)malloc(deflate_bound);
    if (!deflated_data) {
        deflateEnd(&stream_deflate);
        gzclose(gz_out);
        assert_file_closed(&out_file);;
        free(decompressed_data);
        assert_file_closed(&out_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
    }
    
    stream_deflate.next_out = deflated_data;
    stream_deflate.avail_out = deflate_bound;
    
    // Perform deflation
    deflate(&stream_deflate, Z_FINISH);
    
    // Change compression parameters
    deflateParams(&stream_deflate, Z_BEST_SPEED, Z_FILTERED);
    
    // Reset stream while keeping internal state
    deflateResetKeep(&stream_deflate);
    
    // Clean up deflate stream
    deflateEnd(&stream_deflate);
    
    // Clean up gzFile
    gzclose(gz_out);
    assert_file_closed(&out_file);;
    
    // Clean up allocated memory
    free(deflated_data);
    free(decompressed_data);
    
    // Use the computed values to prevent optimization
    (void)mark_position;
    (void)gz_position;
    
    assert_file_closed(&out_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
}
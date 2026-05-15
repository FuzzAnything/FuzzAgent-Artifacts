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
//<ID> 131
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["uncompress2","gzdopen","gzread","gzerror","gzclose","deflateInit_","deflateTune","inflateInit_","deflateEnd","inflateCopy","inflateResetKeep","deflateEnd","inflateEnd","inflateInit2_","inflate","inflateResetKeep","inflateEnd","deflateInit2_","deflateTune","deflateEnd"],"critical_calls":["uncompress2","gzdopen","gzread","gzerror","gzclose","deflateInit_","deflateTune","inflateInit_","inflateCopy","inflateResetKeep","deflateEnd","inflateEnd","inflateInit2_","inflate","inflateResetKeep","inflateEnd","deflateInit2_","deflateTune","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check minimum size requirement for zlib operations
    if (size < 32) {
        return 0;
    }

    // Allocate output buffers
    size_t max_output_size = size * 4 + 1024; // Conservative estimate
    Bytef *dest = (Bytef*)malloc(max_output_size);
    Bytef *dest2 = (Bytef*)malloc(max_output_size);
    if (!dest || !dest2) {
        free(dest);
        free(dest2);
        return 0;
    }

    // Variables for uncompress2
    uLongf destLen = max_output_size;
    uLong sourceLen = size;
    const Bytef* source = (const Bytef*)data;

    // First, try to decompress using uncompress2
    int uncompress_result = uncompress2(dest, &destLen, source, &sourceLen);
    
    // If uncompress2 partially succeeded, we can use the decompressed data
    if (uncompress_result == Z_OK || uncompress_result == Z_BUF_ERROR) {
        // Create a temporary file with compressed data
        FILE* tmp_file = fmemopen((void*)data, size, "rb");
        if (!tmp_file) {
            free(dest);
            free(dest2);
            assert_file_closed(&tmp_file);
	return 0;
        }
	int fuzz_fd_0 = fuzz_fileno(tmp_file);

        
        // Open as gzFile for error checking
        gzFile gzfile = gzdopen(fuzz_fd_0, "rb");
        if (!gzfile) {
            assert_file_closed(&tmp_file);;
            free(dest);
            free(dest2);
            assert_file_closed(&tmp_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
        }
        
        // Read some data to trigger potential errors
        Bytef buffer[1024];
        gzread(gzfile, buffer, sizeof(buffer));
        
        // Check for gz errors using gzerror
        int gz_errnum = 0;
        const char* gz_error_msg = gzerror(gzfile, &gz_errnum);
        (void)gz_error_msg; // Suppress unused variable warning
        
        // Clean up gz file
        gzclose(gzfile);
        assert_file_closed(&tmp_file);;
        
        // Now setup z_stream for deflate/inflate operations
        z_stream strm1, strm2;
        memset(&strm1, 0, sizeof(z_stream));
        memset(&strm2, 0, sizeof(z_stream));
        
        // Initialize deflate stream
        if (deflateInit(&strm1, Z_DEFAULT_COMPRESSION) != Z_OK) {
            free(dest);
            free(dest2);
            assert_file_closed(&tmp_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
        }
        
        // Tune the deflate parameters using deflateTune
        // Extract tuning parameters from input data
        int good_length = data[0] % 32 + 8;
        int max_lazy = data[1] % 32 + 8;
        int nice_length = data[2] % 32 + 16;
        int max_chain = data[3] % 128 + 8;
        
        deflateTune(&strm1, good_length, max_lazy, nice_length, max_chain);
        
        // Initialize inflate stream
        if (inflateInit(&strm2) != Z_OK) {
            deflateEnd(&strm1);
            free(dest);
            free(dest2);
            assert_file_closed(&tmp_file);
	assert_fd_closed(fuzz_fd_0);
	return 0;
        }
        
        // Copy the inflate stream state
        if (inflateCopy(&strm2, &strm1) == Z_OK) {
            // Reset the inflate stream while keeping the dictionary
            inflateResetKeep(&strm2);
        }
        
        // Clean up streams
        deflateEnd(&strm1);
        inflateEnd(&strm2);
    }
    
    // Alternative approach: Process the data using different window bits
    // Reinitialize for inflate with different window bits
    z_stream strm3;
    memset(&strm3, 0, sizeof(z_stream));
    
    // Use first few bytes to determine window bits
    int windowBits = 15 + (data[0] % 8); // Random between 15 and 22
    if (inflateInit2(&strm3, windowBits) == Z_OK) {
        // Set up input
        strm3.next_in = (Bytef*)data;
        strm3.avail_in = size > 1024 ? 1024 : size;
        
        // Set up output
        strm3.next_out = dest2;
        strm3.avail_out = max_output_size;
        
        // Perform inflation
        int inflate_result = inflate(&strm3, Z_NO_FLUSH);
        (void)inflate_result; // Suppress unused variable warning
        
        // Reset while keeping dictionary
        inflateResetKeep(&strm3);
        
        // Clean up
        inflateEnd(&strm3);
    }
    
    // Another test: Try with raw deflate/inflate
    z_stream strm4;
    memset(&strm4, 0, sizeof(z_stream));
    
    if (deflateInit2(&strm4, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     -15, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        // Tune this stream too
        deflateTune(&strm4, 
                   data[4] % 32 + 8,
                   data[5] % 32 + 8,
                   data[6] % 32 + 16,
                   data[7] % 128 + 8);
        
        deflateEnd(&strm4);
    }
    
    // Clean up
    free(dest);
    free(dest2);
    
    return 0;
}
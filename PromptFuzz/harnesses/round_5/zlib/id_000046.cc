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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 2
//<Quality> {"density":15,"unique_branches":{"inflateStateCheck":[[94,9,94,24,0,0,4,0]],"inflate":[[540,17,540,34,0,0,4,0]]},"library_calls":["zlibVersion","inflateInit2_","inflateEnd","inflateGetHeader","inflate","inflateReset","inflate","inflateEnd","inflateResetKeep","inflateEnd","compressBound_z","compress_z","compress2_z","deflateInit_","deflateParams","compressBound","deflatePrime","deflate","deflateEnd","gzdopen","gzread","gzclose"],"critical_calls":["zlibVersion","inflateInit2_","inflateGetHeader","inflate","inflateReset","inflate","inflateEnd","inflateResetKeep","inflateEnd","compressBound_z","compress_z","compress2_z","deflateInit_","deflateParams","compressBound","deflatePrime","deflate","deflateEnd","gzdopen","gzread","gzclose"],"visited":0}
/*To create a fuzz driver that tests zlib's compression/decompression cycle with various APIs, I'll implement the following event: **Decompressing input data, modifying compression parameters, and recompressing it**.

*/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 0. Get zlib version
    const char* version = zlibVersion();
    if (!version) {
	return 0;
	}
	
    
    // 1. First, try to decompress the input (assuming it's zlib-compressed data)
    z_stream decomp_stream;
    memset(&decomp_stream, 0, sizeof(decomp_stream));
    
    // Initialize decompression with maximum window bits (32 for gzip detection)
    if (inflateInit2_(&decomp_stream, 32, version, sizeof(z_stream)) != Z_OK) {
        return 0;
    }
    
    // Set up input buffer
    decomp_stream.next_in = (Bytef*)data;
    decomp_stream.avail_in = size;
    
    // Allocate output buffer for decompressed data
    size_t decomp_buf_size = size * 4;  // Reasonable guess
    if (decomp_buf_size < 1024) decomp_buf_size = 1024;
    Bytef* decomp_buf = (Bytef*)malloc(decomp_buf_size);
    if (!decomp_buf) {
        inflateEnd(&decomp_stream);
        return 0;
    }
    
    decomp_stream.next_out = decomp_buf;
    decomp_stream.avail_out = decomp_buf_size;
    
    // Try to get gzip header if present
    gz_header gzip_header;
    memset(&gzip_header, 0, sizeof(gzip_header));
    inflateGetHeader(&decomp_stream, &gzip_header);
    
    // Decompress the data
    int decomp_result = inflate(&decomp_stream, Z_FINISH);
    
    // If decompression failed, try resetting the stream
    if (decomp_result < 0 && decomp_result != Z_BUF_ERROR) {
        inflateReset(&decomp_stream);
        decomp_stream.next_in = (Bytef*)data;
        decomp_stream.avail_in = size;
        decomp_stream.next_out = decomp_buf;
        decomp_stream.avail_out = decomp_buf_size;
        decomp_result = inflate(&decomp_stream, Z_SYNC_FLUSH);
    }
    
    // Get decompressed data size
    size_t decompressed_size = decomp_buf_size - decomp_stream.avail_out;
    
    // Clean up decompression stream
    inflateEnd(&decomp_stream);
    
    // 2. If we got any decompressed data, try various compression operations
    if (decompressed_size > 0 && decomp_result != Z_STREAM_ERROR) {
        // Try resetting the stream without freeing state
        inflateResetKeep(&decomp_stream);
        inflateEnd(&decomp_stream);
        
        // 3. Test compress_z API
        z_size_t comp_bound = compressBound_z(decompressed_size);
        Bytef* comp_buf1 = (Bytef*)malloc(comp_bound);
        if (comp_buf1) {
            z_size_t dest_len1 = comp_bound;
            int comp_result1 = compress_z(comp_buf1, &dest_len1, decomp_buf, decompressed_size);
            free(comp_buf1);
        }
        
        // 4. Test compress2_z API with different levels
        Bytef* comp_buf2 = (Bytef*)malloc(comp_bound);
        if (comp_buf2) {
            z_size_t dest_len2 = comp_bound;
            // Try multiple compression levels
            for (int level = 0; level <= 9; level += 4) {
                compress2_z(comp_buf2, &dest_len2, decomp_buf, decompressed_size, level);
            }
            free(comp_buf2);
        }
        
        // 5. Test streaming compression with deflate APIs
        z_stream comp_stream;
        memset(&comp_stream, 0, sizeof(comp_stream));
        
        // Initialize compression with default level
        if (deflateInit_(&comp_stream, Z_DEFAULT_COMPRESSION, version, sizeof(z_stream)) == Z_OK) {
            // Try changing parameters
            deflateParams(&comp_stream, 6, Z_DEFAULT_STRATEGY);
            
            // Set up buffers
            size_t comp_buf_size = compressBound(decompressed_size);
            Bytef* comp_buf3 = (Bytef*)malloc(comp_buf_size);
            if (comp_buf3) {
                comp_stream.next_in = decomp_buf;
                comp_stream.avail_in = decompressed_size;
                comp_stream.next_out = comp_buf3;
                comp_stream.avail_out = comp_buf_size;
                
                // Add some prime bits (simulate partial flush)
                deflatePrime(&comp_stream, 3, 5);
                
                // Compress the data
                deflate(&comp_stream, Z_FINISH);
                
                free(comp_buf3);
            }
            
            // Clean up compression stream
            deflateEnd(&comp_stream);
        }
    }
    
    // 6. Test gzFile APIs using memory file
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        gzFile gz = gzdopen(fd, "rb");
        if (gz) {
            // Try to read some data
            unsigned char read_buf[1024];
            while (gzread(gz, read_buf, sizeof(read_buf)) > 0) {
                // Just consume the data
            }
            gzclose(gz);
        }
        assert_file_closed(&in_file);;
    }
    
    // 7. Clean up allocated memory
    free(decomp_buf);
    
    assert_file_closed(&in_file);
	return 0;
}
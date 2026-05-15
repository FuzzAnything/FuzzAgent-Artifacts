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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["zlibVersion","gzdopen","gzseek","inflateBackInit_","gzclose","inflateValidate","deflateInit_","inflateBackEnd","gzclose","deflateParams","deflatePrime","gzread","inflateResetKeep","inflate","deflate","deflateEnd","inflateBackEnd","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzseek","inflateBackInit_","inflateValidate","deflateInit_","deflateParams","deflatePrime","gzread","inflateResetKeep","inflate","deflate","deflateEnd","inflateBackEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get zlib version
    const char *version = zlibVersion();
    (void)version; // Prevent unused variable warning
    
    // 2. Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create gzFile from FILE descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, "rb");
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Use gzseek to seek to beginning
    off_t seek_result = gzseek(gz_file, 0, SEEK_SET);
    (void)seek_result; // Prevent unused variable warning
    
    // 6. Initialize inflate stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    // Use inflateBackInit_ with custom window
    unsigned char window[32768]; // 32KB window
    int windowBits = 15; // Default window size
    int stream_size = sizeof(z_stream);
    int init_result = inflateBackInit_(&inflate_strm, windowBits, window, 
                                      version, stream_size);
    
    if (init_result != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Enable validation
    int validate_result = inflateValidate(&inflate_strm, 1);
    (void)validate_result; // Prevent unused variable warning
    
    // 8. Initialize deflate stream for recompression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    // Initialize deflate with default parameters
    int deflate_init = deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    if (deflate_init != Z_OK) {
        inflateBackEnd(&inflate_strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 9. Change compression parameters
    int level = Z_DEFAULT_COMPRESSION;
    int strategy = Z_DEFAULT_STRATEGY;
    int params_result = deflateParams(&deflate_strm, level, strategy);
    (void)params_result; // Prevent unused variable warning
    
    // 10. Insert bits into deflate stream (no-op in this context)
    int bits = 0;
    int value = 0;
    int prime_result = deflatePrime(&deflate_strm, bits, value);
    (void)prime_result; // Prevent unused variable warning
    
    // 11. Read from gzFile and process
    unsigned char buffer[4096];
    unsigned char out_buffer[8192];
    
    while (true) {
        int bytes_read = gzread(gz_file, buffer, sizeof(buffer));
        if (bytes_read <= 0) {
            break;
        }
        
        // Setup inflate stream
        inflate_strm.next_in = buffer;
        inflate_strm.avail_in = bytes_read;
        inflate_strm.next_out = out_buffer;
        inflate_strm.avail_out = sizeof(out_buffer);
        
        // Reset inflate stream without freeing state
        int reset_result = inflateResetKeep(&inflate_strm);
        (void)reset_result; // Prevent unused variable warning
        
        // Inflate (decompress) the data
        int inflate_result = inflate(&inflate_strm, Z_NO_FLUSH);
        if (inflate_result != Z_OK && inflate_result != Z_STREAM_END) {
            break;
        }
        
        size_t decompressed_size = sizeof(out_buffer) - inflate_strm.avail_out;
        
        // Setup deflate stream with decompressed data
        deflate_strm.next_in = out_buffer;
        deflate_strm.avail_in = decompressed_size;
        deflate_strm.next_out = buffer; // Reuse buffer for compressed output
        deflate_strm.avail_out = sizeof(buffer);
        
        // Compress the decompressed data
        int deflate_result = deflate(&deflate_strm, Z_NO_FLUSH);
        if (deflate_result != Z_OK && deflate_result != Z_STREAM_END) {
            break;
        }
        
        size_t compressed_size = sizeof(buffer) - deflate_strm.avail_out;
        
        // Write to output file
        if (compressed_size > 0) {
            fwrite(buffer, 1, compressed_size, out_file);
        }
    }
    
    // 12. Clean up
    deflateEnd(&deflate_strm);
    inflateBackEnd(&inflate_strm);
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
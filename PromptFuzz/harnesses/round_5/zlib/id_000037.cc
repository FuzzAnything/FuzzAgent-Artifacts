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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["zlibVersion","deflateInit_","inflateInit2_","deflateEnd","inflateBackInit_","inflateBackEnd","gzdopen","inflateEnd","deflateEnd","inflateValidate","inflate","inflateResetKeep","compressBound_z","compress_z","compressBound_z","compress2_z","deflateParams","deflatePrime","deflate","deflateResetKeep","gzread","gzclose","inflateEnd","deflateEnd"],"critical_calls":["zlibVersion","deflateInit_","inflateInit2_","inflateBackInit_","inflateBackEnd","gzdopen","inflateValidate","inflate","inflateResetKeep","compressBound_z","compress_z","compressBound_z","compress2_z","deflateParams","deflatePrime","deflate","deflateResetKeep","gzread","gzclose","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get zlib version
    const char *version = zlibVersion();
    if (!version) {
	return 0;
	}
	
    
    // 2. Create input/output file descriptors
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 3. Setup compression stream (deflate)
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    // Initialize deflate with default level
    if (deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, version, sizeof(z_stream)) != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Setup decompression stream (inflate)
    z_stream inf_strm;
    memset(&inf_strm, 0, sizeof(inf_strm));
    
    // Initialize inflate with automatic header detection
    if (inflateInit2_(&inf_strm, 15 + 32, version, sizeof(z_stream)) != Z_OK) {
        deflateEnd(&def_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Setup inflateBack for alternative decompression
    unsigned char back_window[32768];
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    
    if (inflateBackInit_(&back_strm, 15, back_window, version, sizeof(z_stream)) == Z_OK) {
        // We'll just initialize and end it since we don't have proper callbacks
        inflateBackEnd(&back_strm);
    }
    
    // 6. Create gzFile from file descriptor
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        inflateEnd(&inf_strm);
        deflateEnd(&def_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 7. Read and decompress using inflate
    Bytef in_buffer[4096];
    Bytef out_buffer[8192];
    
    // First, try to decompress the input data
    inf_strm.next_in = (Bytef*)data;
    inf_strm.avail_in = size > 4096 ? 4096 : size;
    memcpy(in_buffer, data, inf_strm.avail_in);
    inf_strm.next_in = in_buffer;
    
    inf_strm.next_out = out_buffer;
    inf_strm.avail_out = sizeof(out_buffer);
    
    // Enable validation during inflation
    inflateValidate(&inf_strm, 1);
    
    int inflate_result = inflate(&inf_strm, Z_NO_FLUSH);
    
    // Reset inflate stream while keeping internal state
    inflateResetKeep(&inf_strm);
    
    // 8. Compress the decompressed data (if any)
    if (inflate_result == Z_OK || inflate_result == Z_STREAM_END) {
        size_t decompressed_size = sizeof(out_buffer) - inf_strm.avail_out;
        
        // Use compress_z for simple compression
        z_size_t comp_len_z = compressBound_z(decompressed_size);
        Bytef *compressed_z = (Bytef*)malloc(comp_len_z);
        if (compressed_z) {
            compress_z(compressed_z, &comp_len_z, out_buffer, decompressed_size);
            free(compressed_z);
        }
        
        // Use compress2_z with specific level
        z_size_t comp_len2_z = compressBound_z(decompressed_size);
        Bytef *compressed2_z = (Bytef*)malloc(comp_len2_z);
        if (compressed2_z) {
            compress2_z(compressed2_z, &comp_len2_z, out_buffer, decompressed_size, 9);
            free(compressed2_z);
        }
        
        // Now use stream compression with deflate
        def_strm.next_in = out_buffer;
        def_strm.avail_in = decompressed_size;
        def_strm.next_out = out_buffer;  // Reuse buffer
        def_strm.avail_out = sizeof(out_buffer);
        
        // Change compression parameters
        deflateParams(&def_strm, 6, Z_DEFAULT_STRATEGY);
        
        // Insert some bits into the stream
        deflatePrime(&def_strm, 3, 5);
        
        // Compress the data
        int deflate_result = deflate(&def_strm, Z_FINISH);
        
        // Reset deflate if needed
        if (deflate_result != Z_STREAM_END) {
            deflateResetKeep(&def_strm);
        }
    }
    
    // 9. Read from gzFile
    char gz_buffer[1024];
    int bytes_read = gzread(gz_in, gz_buffer, sizeof(gz_buffer));
    
    // 10. Clean up
    gzclose(gz_in);
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
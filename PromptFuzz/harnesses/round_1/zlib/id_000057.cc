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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.6666665, nr_unique_branch: 1
//<Quality> {"density":7,"unique_branches":{"gzsetparams":[[641,9,641,32,0,0,4,0]]},"library_calls":["inflateInit2_","zError","inflate","zError","inflateSetDictionary","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateBound_z","compressBound","deflate","deflatePending","deflateResetKeep","deflateEnd","gzdopen","gzsetparams","gzclose","inflateEnd"],"critical_calls":["inflateInit2_","inflate","zError","inflateSetDictionary","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateBound_z","compressBound","deflate","deflatePending","deflateResetKeep","deflateEnd","gzdopen","gzsetparams","gzclose","inflateEnd"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 50) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    // Create temporary files for I/O operations
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
    
    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    z_stream inf_stream, def_stream;
    gzFile gz_file = nullptr;
    Bytef *dictionary = nullptr;
    uInt dict_len = 0;
    Bytef *compressed = nullptr;
    uLong compressed_bound = 0;
    int ret = Z_OK;
    
    // Initialize inflation stream for decompression
    inf_stream.zalloc = Z_NULL;
    inf_stream.zfree = Z_NULL;
    inf_stream.opaque = Z_NULL;
    inf_stream.next_in = Z_NULL;
    inf_stream.avail_in = 0;
    
    // Use inflateInit2_ with windowBits parameter
    ret = inflateInit2_(&inf_stream, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Allocate input buffer for inflation
    uLong inf_buf_size = size > 1024 ? 1024 : size;
    Bytef *inf_buf = new Bytef[inf_buf_size];
    inf_stream.avail_in = fread(inf_buf, 1, inf_buf_size, in_file);
    inf_stream.next_in = inf_buf;
    
    // Allocate output buffer for inflation
    uLong out_buf_size = 2048;
    Bytef *out_buf = new Bytef[out_buf_size];
    inf_stream.avail_out = out_buf_size;
    inf_stream.next_out = out_buf;
    
    // Perform inflation (decompression)
    ret = inflate(&inf_stream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
        const char *err_msg = zError(ret);
        // Continue anyway
    }
    
    // Try to set dictionary for inflation if we have enough decompressed data
    if (inf_stream.total_out > 32) {
        // Use first 32 bytes of decompressed data as dictionary
        uInt dict_size = inf_stream.total_out > 256 ? 256 : (uInt)inf_stream.total_out;
        ret = inflateSetDictionary(&inf_stream, out_buf, dict_size);
    }
    
    // Reset file pointer for deflation test
    rewind(in_file);
    
    // Initialize deflation stream for compression
    def_stream.zalloc = Z_NULL;
    def_stream.zfree = Z_NULL;
    def_stream.opaque = Z_NULL;
    
    // Use deflateInit2_ with various parameters
    ret = deflateInit2_(&def_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15 + 16, 8, Z_DEFAULT_STRATEGY,
                        ZLIB_VERSION, sizeof(z_stream));
    
    if (ret == Z_OK) {
        // Set dictionary for deflation if we have enough data
        if (size > 32) {
            dict_len = size > 256 ? 256 : (uInt)size;
            dictionary = new Bytef[dict_len];
            memcpy(dictionary, data, dict_len);
            ret = deflateSetDictionary(&def_stream, dictionary, dict_len);
            
            // Get the dictionary back
            uInt get_dict_len = dict_len;
            Bytef *get_dict = new Bytef[get_dict_len];
            ret = deflateGetDictionary(&def_stream, get_dict, &get_dict_len);
            delete[] get_dict;
        }
        
        // Calculate bounds for compression
        z_size_t source_len_z = size > 1024 ? 1024 : (z_size_t)size;
        z_size_t bound_z = deflateBound_z(&def_stream, source_len_z);
        
        uLong source_len = size > 1024 ? 1024 : size;
        compressed_bound = compressBound(source_len);
        
        // Allocate compression buffer
        compressed = new Bytef[compressed_bound];
        
        // Read data for compression
        Bytef *source_buf = new Bytef[source_len];
        fread(source_buf, 1, source_len, in_file);
        
        def_stream.avail_in = (uInt)source_len;
        def_stream.next_in = source_buf;
        def_stream.avail_out = (uInt)compressed_bound;
        def_stream.next_out = compressed;
        
        // Perform deflation (compression)
        ret = deflate(&def_stream, Z_FINISH);
        
        // Check for pending bits
        unsigned int pending;
        int bits;
        ret = deflatePending(&def_stream, &pending, &bits);
        
        // Reset deflate stream keeping dictionary
        ret = deflateResetKeep(&def_stream);
        
        delete[] source_buf;
    }
    
    // Clean up deflate stream
    if (def_stream.state) {
        deflateEnd(&def_stream);
    }
    
    // Test gzFile operations
    gz_file = gzdopen(fd_in, "rb");
    if (gz_file) {
        // Set parameters for gz file
        ret = gzsetparams(gz_file, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);
        
        // Close gz file
        gzclose(gz_file);
    }
    
    // Clean up inflation stream
    inflateEnd(&inf_stream);
    
    // Clean up allocated memory
    delete[] inf_buf;
    delete[] out_buf;
    if (compressed) delete[] compressed;
    if (dictionary) delete[] dictionary;
    
    // Close file handles
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
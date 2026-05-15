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
//<ID> 70
//<Prompt> []
/*<Combination>: [
*/
//<score> 11.5, nr_unique_branch: 0
//<Quality> {"density":23,"unique_branches":{},"library_calls":["deflateInit2_","deflateSetDictionary","deflateEnd","deflateBound_z","deflateEnd","deflate","deflateEnd","deflateResetKeep","compressBound","deflateEnd","inflateInit2_","inflateUndermine","inflateEnd","inflateSetDictionary","inflateEnd","inflateGetDictionary","inflate","inflateEnd","crc32","crc32","inflateResetKeep","inflateEnd","gzdopen","gzclose","uncompress_z","deflateInit_","deflateEnd","inflateInit_","inflateEnd"],"critical_calls":["deflateInit2_","deflateSetDictionary","deflateBound_z","deflate","deflateResetKeep","compressBound","deflateEnd","inflateInit2_","inflateUndermine","inflateSetDictionary","inflateGetDictionary","inflate","crc32","crc32","inflateResetKeep","inflateEnd","gzdopen","gzclose","uncompress_z","deflateInit_","deflateEnd","inflateInit_","inflateEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for meaningful operations
    if (size < 50) {
	return 0;
	}
	
    
    // Split input into different parts for various purposes
    size_t dict_size = size / 4;
    size_t compress_data_size = size / 2;
    size_t remaining_size = size - dict_size - compress_data_size;
    
    if (dict_size == 0 || compress_data_size == 0 || remaining_size == 0) {
	return 0;
	}
	
    
    const Bytef* dictionary = data;
    const Bytef* compress_data = data + dict_size;
    const Bytef* extra_data = data + dict_size + compress_data_size;
    
    // Part 1: Compression with dictionary
    z_stream c_stream;
    c_stream.zalloc = Z_NULL;
    c_stream.zfree = Z_NULL;
    c_stream.opaque = Z_NULL;
    
    // Initialize compression with deflateInit2_
    int ret = deflateInit2_(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                            MAX_WBITS, 8, Z_DEFAULT_STRATEGY,
                            ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }
    
    // Set dictionary for compression
    ret = deflateSetDictionary(&c_stream, dictionary, dict_size);
    if (ret != Z_OK) {
        deflateEnd(&c_stream);
        return 0;
    }
    
    // Allocate compression buffer using deflateBound_z
    z_size_t compressed_bound = deflateBound_z(&c_stream, compress_data_size);
    Bytef* compressed = (Bytef*)malloc(compressed_bound);
    if (!compressed) {
        deflateEnd(&c_stream);
        return 0;
    }
    
    c_stream.next_in = (Bytef*)compress_data;
    c_stream.avail_in = compress_data_size;
    c_stream.next_out = compressed;
    c_stream.avail_out = compressed_bound;
    
    // Perform compression
    ret = deflate(&c_stream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        free(compressed);
        deflateEnd(&c_stream);
        return 0;
    }
    
    size_t compressed_size = compressed_bound - c_stream.avail_out;
    
    // Test deflateResetKeep
    deflateResetKeep(&c_stream);
    
    // Also test compressBound (different API)
    uLong regular_bound = compressBound(compress_data_size);
    
    // End compression stream
    deflateEnd(&c_stream);
    
    // Part 2: Decompression with dictionary
    z_stream d_stream;
    d_stream.zalloc = Z_NULL;
    d_stream.zfree = Z_NULL;
    d_stream.opaque = Z_NULL;
    
    // Initialize decompression with inflateInit2_
    ret = inflateInit2_(&d_stream, MAX_WBITS, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(compressed);
        return 0;
    }
    
    // Test inflateUndermine (should be called before first inflate)
    inflateUndermine(&d_stream, 0);
    
    // Allocate decompression buffer
    Bytef* decompressed = (Bytef*)malloc(compress_data_size);
    if (!decompressed) {
        free(compressed);
        inflateEnd(&d_stream);
        return 0;
    }
    
    d_stream.next_in = compressed;
    d_stream.avail_in = compressed_size;
    d_stream.next_out = decompressed;
    d_stream.avail_out = compress_data_size;
    
    // Set dictionary for decompression (same as compression dictionary)
    ret = inflateSetDictionary(&d_stream, dictionary, dict_size);
    if (ret != Z_OK) {
        free(compressed);
        free(decompressed);
        inflateEnd(&d_stream);
        return 0;
    }
    
    // Get the dictionary back using inflateGetDictionary
    uInt get_dict_len = dict_size;
    Bytef* dict_copy = (Bytef*)malloc(dict_size);
    if (dict_copy) {
        inflateGetDictionary(&d_stream, dict_copy, &get_dict_len);
        free(dict_copy);
    }
    
    // Perform decompression
    ret = inflate(&d_stream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        free(compressed);
        free(decompressed);
        inflateEnd(&d_stream);
        return 0;
    }
    
    // Compute CRC32 of decompressed data
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, decompressed, compress_data_size - d_stream.avail_out);
    
    // Test inflateResetKeep
    inflateResetKeep(&d_stream);
    
    // End decompression stream
    inflateEnd(&d_stream);
    
    // Part 3: Test gzFile APIs
    // Create a temporary file with compressed data
    FILE* tmp_file = fopen("output_file", "wb");
    if (tmp_file) {
        fwrite(compressed, 1, compressed_size, tmp_file);
        assert_file_closed(&tmp_file);;
        
        // Reopen with gzdopen
        tmp_file = fopen("output_file", "rb");
        if (tmp_file) {
            int fd = fuzz_fileno(tmp_file);
            gzFile gz = gzdopen(fd, "rb");
            if (gz) {
                // Read one byte using gzgetc
                int ch = gzgetc(gz);
                // Close gz file
                gzclose(gz);
            }
            assert_file_closed(&tmp_file);;
        }
    }
    
    // Part 4: Test uncompress_z
    // This should work since we're uncompressing valid zlib data
    Bytef* uncompressed = (Bytef*)malloc(compress_data_size);
    if (uncompressed) {
        z_size_t uncompressed_len = compress_data_size;
        ret = uncompress_z(uncompressed, &uncompressed_len, 
                          compressed, compressed_size);
        free(uncompressed);
    }
    
    // Part 5: Test simpler initialization functions
    z_stream simple_stream;
    simple_stream.zalloc = Z_NULL;
    simple_stream.zfree = Z_NULL;
    simple_stream.opaque = Z_NULL;
    
    // Test deflateInit_
    deflateInit_(&simple_stream, Z_DEFAULT_COMPRESSION, 
                 ZLIB_VERSION, sizeof(z_stream));
    deflateEnd(&simple_stream);
    
    // Test inflateInit_
    inflateInit_(&simple_stream, ZLIB_VERSION, sizeof(z_stream));
    inflateEnd(&simple_stream);
    
    // Cleanup
    free(compressed);
    free(decompressed);
    
    assert_file_closed(&tmp_file);
	return 0;
}
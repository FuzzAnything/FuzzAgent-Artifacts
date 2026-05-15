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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","inflateInit2_","gzclose","gzclose","gzungetc","compressBound","inflateEnd","gzclose","gzclose","deflateInit_","deflateSetDictionary","deflateEnd","compress_z","compress2_z","gzprintf","inflateCodesUsed","inflateEnd","gzclose","gzclose"],"critical_calls":["gzdopen","gzdopen","inflateInit2_","gzungetc","compressBound","deflateInit_","deflateSetDictionary","deflateEnd","compress_z","compress2_z","gzprintf","inflateCodesUsed","inflateEnd","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 4. Create gzFile handles
    gzFile gz_in = gzdopen(fd_in, "rb");
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Initialize z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use inflateInit2_ with negative windowBits for raw deflate data
    int windowBits = -15;  // Raw deflate format
    if (inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Read compressed data from gzFile
    // First, get one character
    int first_char = gzgetc(gz_in);
    if (first_char != -1) {
        // Push it back using gzungetc
        gzungetc(first_char, gz_in);
    }
    
    // 7. Prepare buffers for compression/decompression
    size_t max_compressed_size = compressBound(size);
    Bytef *compressed_buf = (Bytef*)malloc(max_compressed_size);
    Bytef *decompressed_buf = (Bytef*)malloc(size * 2);  // Extra space
    
    if (!compressed_buf || !decompressed_buf) {
        free(compressed_buf);
        free(decompressed_buf);
        inflateEnd(&strm);
        gzclose(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 8. Set dictionary for deflation if we have enough data
    if (size > 32) {
        // Use first 32 bytes as dictionary
        uInt dict_length = 32;
        Bytef dictionary[32];
        memcpy(dictionary, data, dict_length);
        
        // Note: deflateSetDictionary is for compression, but we'll demonstrate its usage
        // We need to initialize a deflate stream first
        z_stream deflate_strm;
        memset(&deflate_strm, 0, sizeof(deflate_strm));
        deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
        
        // Set dictionary
        deflateSetDictionary(&deflate_strm, dictionary, dict_length);
        deflateEnd(&deflate_strm);
    }
    
    // 9. Use compress_z (single function compression)
    z_size_t dest_len = max_compressed_size;
    if (compress_z(compressed_buf, &dest_len, data, size) == Z_OK) {
        // 10. Use compress2_z with different compression level
        z_size_t dest_len2 = max_compressed_size;
        compress2_z(compressed_buf, &dest_len2, data, size, Z_BEST_COMPRESSION);
    }
    
    // 11. Write formatted output to gzFile using gzprintf
    gzprintf(gz_out, "Processed %zu bytes of input data\n", size);
    
    // 12. Get inflate codes usage (after some inflation operations)
    unsigned long codes_used = inflateCodesUsed(&strm);
    
    // 13. Clean up z_stream
    inflateEnd(&strm);
    
    // 14. Clean up gzFile handles
    gzclose(gz_in);
    gzclose(gz_out);
    
    // 15. Clean up FILE handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 16. Free allocated buffers
    free(compressed_buf);
    free(decompressed_buf);
    
    // 17. Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["gzdopen","gzrewind","inflateInit2_","gzclose","inflateSetDictionary","inflateCodesUsed","inflateEnd","deflateInit_","deflateSetDictionary","gzdopen","gzprintf","gzprintf","gzputc","gzungetc","gzputc","gzclose","compressBound_z","compress_z","compressBound_z","compress2_z","deflateEnd","gzclose"],"critical_calls":["gzdopen","gzrewind","inflateInit2_","inflateSetDictionary","inflateCodesUsed","inflateEnd","deflateInit_","deflateSetDictionary","gzdopen","gzprintf","gzprintf","gzputc","gzungetc","gzputc","gzclose","compressBound_z","compress_z","compressBound_z","compress2_z","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Consume input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create gzFile for reading compressed data
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Read and uncompress data using gzgetc and gzungetc
    size_t uncompressed_size = 0;
    int ch;
    while ((ch = gzgetc(gz_in)) != -1) {
        uncompressed_size++;
    }
    
    // Rewind and read actual data
    gzrewind(gz_in);
    Bytef *uncompressed = (Bytef *)malloc(uncompressed_size);
    for (size_t i = 0; i < uncompressed_size; i++) {
        uncompressed[i] = gzgetc(gz_in);
    }
    
    // 4. Initialize inflation stream for dictionary operations
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    // Use inflateInit2_ with negative windowBits for raw decompression
    int windowBits = -15;  // Raw deflate format
    int ret = inflateInit2_(&inflate_strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(uncompressed);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Set dictionary for inflation (use first part of uncompressed data as dictionary)
    uInt dict_len = (uncompressed_size > 1024) ? 1024 : (uInt)uncompressed_size;
    if (dict_len > 0) {
        inflateSetDictionary(&inflate_strm, uncompressed, dict_len);
    }
    
    // 6. Get inflation codes usage
    unsigned long codes_used = inflateCodesUsed(&inflate_strm);
    
    // 7. Clean up inflation stream
    inflateEnd(&inflate_strm);
    
    // 8. Initialize deflation stream for compression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION);
    
    // 9. Set dictionary for deflation
    if (dict_len > 0) {
        deflateSetDictionary(&deflate_strm, uncompressed, dict_len);
    }
    
    // 10. Use gzprintf to write formatted output to a gzFile
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        gzFile gz_out = gzdopen(out_fd, "wb");
        if (gz_out) {
            // Write some formatted data
            gzprintf(gz_out, "Processed %zu bytes, dictionary size: %u\n", 
                    uncompressed_size, dict_len);
            gzprintf(gz_out, "Inflation codes used: %lu\n", codes_used);
            
            // Test gzungetc
            gzputc(gz_out, 'A');
            gzungetc('B', gz_out);
            gzputc(gz_out, gzgetc(gz_out));
            
            gzclose(gz_out);
        }
        assert_file_closed(&out_file);;
    }
    
    // 11. Use compress_z with the uncompressed data
    z_size_t compressed_len = compressBound_z(uncompressed_size);
    Bytef *compressed1 = (Bytef *)malloc(compressed_len);
    if (compressed1 && uncompressed_size > 0) {
        compress_z(compressed1, &compressed_len, uncompressed, uncompressed_size);
    }
    
    // 12. Use compress2_z with different compression level
    z_size_t compressed2_len = compressBound_z(uncompressed_size);
    Bytef *compressed2 = (Bytef *)malloc(compressed2_len);
    if (compressed2 && uncompressed_size > 0) {
        int level = Z_BEST_COMPRESSION;
        compress2_z(compressed2, &compressed2_len, uncompressed, uncompressed_size, level);
    }
    
    // 13. Clean up deflation stream
    deflateEnd(&deflate_strm);
    
    // 14. Free allocated memory
    free(uncompressed);
    if (compressed1) free(compressed1);
    if (compressed2) free(compressed2);
    
    // 15. Close files
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
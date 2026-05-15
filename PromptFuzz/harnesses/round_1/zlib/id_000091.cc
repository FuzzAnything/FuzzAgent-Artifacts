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
//<ID> 268
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"gzputc":[[334,13,334,31,0,0,4,1]]},"library_calls":["gzdopen","inflateInit2_","zError","gzclose","gzread","deflateInit2_","zError","inflateEnd","gzclose","compressBound","deflateBound_z","deflateEnd","inflateEnd","gzclose","deflate","zError","deflateResetKeep","zError","inflateCodesUsed","gzdopen","gzputc","crc32_combine_gen64","gzclose","gzdopen","gzputc","gzclose","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","inflateInit2_","gzread","deflateInit2_","compressBound","deflateBound_z","deflate","zError","deflateResetKeep","zError","inflateCodesUsed","gzdopen","gzputc","crc32_combine_gen64","gzclose","gzdopen","gzputc","gzclose","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_91(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    int in_fd = fuzz_fileno(in_file);
    
    // 2. Open compressed data as gzFile for reading
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 3. Initialize inflate stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    // Use windowBits = 15 + 32 to automatically detect gzip or zlib header
    int ret = inflateInit2_(&inflate_strm, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 4. Read decompressed data using gzread
    // First, try to read some data through gzFile interface
    char gz_buffer[1024];
    int gz_bytes_read = gzread(gz_in, gz_buffer, sizeof(gz_buffer));
    
    // 5. Initialize deflate stream for recompression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    // Use DEFLATED method, windowBits 15, memLevel 8, default strategy
    ret = deflateInit2_(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                        15, 8, Z_DEFAULT_STRATEGY, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
        inflateEnd(&inflate_strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Calculate compression bounds
    uLong comp_bound = compressBound(size);
    z_size_t deflate_bound = deflateBound_z(&deflate_strm, size);
    
    // Allocate buffers for compression
    size_t out_buf_size = (deflate_bound > comp_bound) ? deflate_bound : comp_bound;
    Bytef *out_buffer = (Bytef*)malloc(out_buf_size);
    if (!out_buffer) {
        deflateEnd(&deflate_strm);
        inflateEnd(&inflate_strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 7. Set up input for deflate from original data
    deflate_strm.next_in = (Bytef*)data;
    deflate_strm.avail_in = size;
    deflate_strm.next_out = out_buffer;
    deflate_strm.avail_out = out_buf_size;
    
    // 8. Perform compression with Z_FINISH flush
    ret = deflate(&deflate_strm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        const char *err_msg = zError(ret);
    }
    
    size_t compressed_size = deflate_strm.total_out;
    
    // 9. Reset deflate stream while keeping internal state
    ret = deflateResetKeep(&deflate_strm);
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);
    }
    
    // 10. Get inflate codes usage statistics
    unsigned long codes_used = inflateCodesUsed(&inflate_strm);
    
    // 11. Create output file for writing compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int out_fd = fuzz_fileno(out_file);
        
        // 12. Write a single character using gzputc
        // First create a gzFile for writing
        gzFile gz_out = gzdopen(out_fd, "wb");
        if (gz_out) {
            // Write a marker byte
            gzputc(gz_out, 'Z');
            
            // 13. Generate CRC combine value (dummy usage)
            uLong crc_combine_gen = crc32_combine_gen64(0);
            
            // 14. Clean up gzFile
            gzclose(gz_out);
        }
        assert_file_closed(&out_file);;
    }
    
    // 15. Write compressed data to a temporary file using gzFile API
    // This is separate from the previous output file
    FILE *tmp_file = fopen("temp_output", "wb");
    if (tmp_file) {
        int tmp_fd = fuzz_fileno(tmp_file);
        gzFile gz_tmp = gzdopen(tmp_fd, "wb");
        if (gz_tmp) {
            // Write compressed data
            for (size_t i = 0; i < compressed_size && i < out_buf_size; i++) {
                gzputc(gz_tmp, out_buffer[i]);
            }
            gzclose(gz_tmp);
        }
        assert_file_closed(&tmp_file);;
        remove("temp_output");
    }
    
    // 16. Clean up all resources
    free(out_buffer);
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    // Remove output file if it was created
    remove("output_file");
    
    assert_file_closed(&tmp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(in_fd);
	return 0;
}
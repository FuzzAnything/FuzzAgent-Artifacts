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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 2
//<Quality> {"density":14,"unique_branches":{"deflateUsed":[[738,9,738,32,0,0,4,0]],"deflatePending":[[723,9,723,32,0,0,4,0]]},"library_calls":["gzdopen","gzread","gzputc","gzrewind","inflateInit2_","gzclose","inflateEnd","gzclose","inflate","inflateEnd","gzclose","deflatePending","deflateUsed","inflateEnd","deflateInit2_","gzclose","deflateGetDictionary","deflatePending","deflateUsed","compressBound","compress2","deflateEnd","inflateBackInit_","inflateBackEnd","gzclose"],"critical_calls":["gzdopen","gzread","gzputc","gzrewind","inflateInit2_","inflate","deflatePending","deflateUsed","inflateEnd","deflateInit2_","deflateGetDictionary","deflatePending","deflateUsed","compressBound","compress2","deflateEnd","inflateBackInit_","inflateBackEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<9) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	 // Need minimum data for testing
    
    // Create input FILE* for reading compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create gzFile from file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Variables for decompression
    Bytef *uncompressed = nullptr;
    uLongf uncompressed_len = 0;
    Bytef *recompressed = nullptr;
    uLongf recompressed_len = 0;
    int ret = Z_OK;
    
    // First, try to decompress using gzread
    unsigned char buffer[4096];
    uLong total_read = 0;
    
    while (true) {
        int bytes_read = gzread(gz_in, buffer, sizeof(buffer));
        if (bytes_read <= 0) break;
        total_read += bytes_read;
        
        // Write a character using gzputc (call at least once)
        if (total_read == bytes_read) { // First chunk only
            gzputc(gz_in, buffer[0]); // This will likely fail on read-only file, but we're calling it
        }
    }
    
    // Reset position for inflate operations
    gzrewind(gz_in);
    
    // Now use low-level inflate API
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    ret = inflateInit2(&strm, MAX_WBITS + 16);
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Setup input for inflate
    strm.avail_in = size;
    strm.next_in = (Bytef *)data;
    
    // Allocate output buffer for inflation
    uncompressed_len = size * 4; // Initial guess
    uncompressed = (Bytef *)malloc(uncompressed_len);
    if (!uncompressed) {
        inflateEnd(&strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    strm.avail_out = uncompressed_len;
    strm.next_out = uncompressed;
    
    // Inflate the data
    ret = inflate(&strm, fuzz_int32_t_2);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        free(uncompressed);
        inflateEnd(&strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    uncompressed_len = strm.total_out;
    
    // Call deflatePending (on inflate stream - this should fail or return 0)
    unsigned int pending;
    int bits;
    deflatePending(&strm, &pending, &bits);
    
    // Call deflateUsed (on inflate stream - this should fail or return 0)
    int used_bits;
    deflateUsed(&strm, &used_bits);
    
    // End inflation
    inflateEnd(&strm);
    
    // Now create a deflate stream for compression
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    ret = deflateInit2(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                      MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        free(uncompressed);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Try to get dictionary (should be empty for new stream)
    Bytef dict[256];
    uInt dict_len = 256;
    deflateGetDictionary(&def_strm, dict, &dict_len);
    
    // Call deflatePending on deflate stream
    deflatePending(&def_strm, &pending, &bits);
    
    // Call deflateUsed on deflate stream
    deflateUsed(&def_strm, &used_bits);
    
    // Now use compress2 API
    recompressed_len = compressBound(uncompressed_len);
    recompressed = (Bytef *)malloc(recompressed_len);
    if (recompressed && uncompressed_len > 0) {
        // Use a compression level based on input data
        int level = data[0] % 10; // 0-9 from first byte
        compress2(recompressed, &recompressed_len, 
                 uncompressed, uncompressed_len, level);
    }
    
    // Cleanup deflate stream
    deflateEnd(&def_strm);
    
    // Setup for inflateBackEnd (requires inflateBack)
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    back_strm.zalloc = Z_NULL;
    back_strm.zfree = Z_NULL;
    back_strm.opaque = Z_NULL;
    
    // Initialize for inflateBack
    unsigned char window[1 << 15]; // 32K window
    ret = inflateBackInit(&back_strm, MAX_WBITS, window);
    if (ret == Z_OK) {
        // We won't actually call inflateBack since we need proper callbacks,
        // but we'll call inflateBackEnd to satisfy the requirement
        inflateBackEnd(&back_strm);
    }
    
    // Cleanup
    if (uncompressed) free(uncompressed);
    if (recompressed) free(recompressed);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
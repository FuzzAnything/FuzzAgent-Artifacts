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
//<score> 12, nr_unique_branch: 2
//<Quality> {"density":6,"unique_branches":{"deflateCopy":[[1355,14,1355,22,0,0,4,0],[1355,26,1355,64,0,0,4,0]]},"library_calls":["zlibVersion","compressBound_z","compress_z","crc32_z","crc32_z","gzdopen","gzclose","gzdopen","gzclose","gzsetparams","gzgetc_","gzputs","deflateInit_","deflate","deflateUsed","deflateCopy","deflateEnd","deflateEnd","inflateInit_","inflate","inflateCodesUsed","inflateEnd","gzclose","gzclose"],"critical_calls":["zlibVersion","compressBound_z","compress_z","crc32_z","crc32_z","gzdopen","gzdopen","gzsetparams","gzgetc_","gzputs","deflateInit_","deflate","deflateUsed","deflateCopy","deflateEnd","deflateEnd","inflateInit_","inflate","inflateCodesUsed","inflateEnd","gzclose","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to achieve a complete event sequence:

*/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Early return if we don't have enough data
    if (size < 32) {
	return 0;
	}
	

    // 1. Get zlib version
    const char *version = zlibVersion();
    
    // 2. Create source data for compression
    // Use part of input as source data
    size_t sourceLen = size / 2;
    const Bytef *source = data;
    
    // 3. Compress data using compress_z
    z_size_t destLen = compressBound_z(sourceLen);
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) {
	return 0;
	}
	
    
    int compress_result = compress_z(dest, &destLen, source, sourceLen);
    
    // 4. Calculate CRC32 of compressed data
    uLong crc = crc32_z(fuzz_uint64_t_1, Z_NULL, 0);
    crc = crc32_z(crc, dest, destLen);
    
    // 5. Create a gzFile for reading compressed data
    FILE *in_file = fmemopen((void *)dest, destLen, "rb");
    if (!in_file) {
        free(dest);
        assert_file_closed(&in_file);
	return 0;
    }
    
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, fuzz_str_2);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        free(dest);
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Create output gzFile
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        free(dest);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd_out, fuzz_str_3);
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        free(dest);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 7. Set parameters on output gzFile
    gzsetparams(gz_out, fuzz_int32_t_4, fuzz_int32_t_5);
    
    // 8. Read one character from input gzFile
    int read_char = gzgetc_(gz_in);
    
    // 9. Write version string to output gzFile
    gzputs(gz_out, version);
    
    // 10. Create z_stream structures for deflate operations
    z_stream strm1, strm2;
    memset(&strm1, 0, sizeof(z_stream));
    memset(&strm2, 0, sizeof(z_stream));
    
    // 11. Initialize deflate stream
    int init_result = deflateInit(&strm1, Z_DEFAULT_COMPRESSION);
    if (init_result == Z_OK) {
        // 12. Set up input for strm1 (use remaining part of input data)
        strm1.avail_in = size - sourceLen;
        strm1.next_in = (Bytef *)(data + sourceLen);
        
        // 13. Create output buffer
        Bytef out_buffer[1024];
        strm1.avail_out = sizeof(out_buffer);
        strm1.next_out = out_buffer;
        
        // 14. Perform deflate operation
        deflate(&strm1, fuzz_int32_t_6);
        
        // 15. Get bits used
        int bits_used = 0;
        deflateUsed(&strm1, &bits_used);
        
        // 16. Copy deflate stream
        deflateCopy(&strm2, &strm1);
        
        // 17. Clean up deflate streams
        deflateEnd(&strm1);
        deflateEnd(&strm2);
    }
    
    // 18. Create z_stream for inflation to use inflateCodesUsed
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    int inflate_init_result = inflateInit(&strm_inflate);
    if (inflate_init_result == Z_OK) {
        // 19. Set up some dummy data for inflation
        Bytef dummy_in[64];
        Bytef dummy_out[256];
        
        // Use part of input data as dummy compressed data
        size_t dummy_size = (size > 64) ? 64 : size;
        memcpy(dummy_in, data, dummy_size);
        
        strm_inflate.avail_in = dummy_size;
        strm_inflate.next_in = dummy_in;
        strm_inflate.avail_out = sizeof(dummy_out);
        strm_inflate.next_out = dummy_out;
        
        // Try to inflate
        inflate(&strm_inflate, fuzz_int32_t_7);
        
        // 20. Get inflate codes used
        unsigned long codes_used = inflateCodesUsed(&strm_inflate);
        
        inflateEnd(&strm_inflate);
    }
    
    // 21. Clean up all resources
    gzclose(gz_out);
    gzclose(gz_in);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    free(dest);
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 2
//<Quality> {"density":24,"unique_branches":{"deflateStateCheck":[[540,9,540,23,0,0,4,0]],"inflateGetDictionary":[[1172,9,1172,32,0,0,4,0]]},"library_calls":["deflateBound","inflateInit2_","inflate","inflateEnd","inflateGetDictionary","deflateInit2_","deflateSetHeader","deflateBound","deflate","deflateEnd","compress","compress2","compress_z","compress2_z","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine64","crc32_combine_gen64","gzdopen","gzdopen","gzread","gzwrite","gzclose_r","gzclose_w","gzclose_r","gzclose_w","inflateInit2_","inflateSetDictionary","inflateEnd"],"critical_calls":["deflateBound","inflateInit2_","inflate","inflateEnd","inflateGetDictionary","deflateInit2_","deflateSetHeader","deflateBound","deflate","deflateEnd","compress","compress2","compress_z","compress2_z","crc32","crc32","crc32","crc32","crc32_combine","crc32_combine64","crc32_combine_gen64","gzdopen","gzdopen","gzread","gzwrite","gzclose_r","gzclose_w","inflateInit2_","inflateSetDictionary","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* f_data, size_t f_size) {
	if(f_size<99) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for output data
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
    
    // Allocate buffers
    size_t max_compressed = deflateBound(nullptr, size);
    Bytef *compressed = (Bytef*)malloc(max_compressed);
    Bytef *decompressed = (Bytef*)malloc(size * 2);
    Bytef *dict_buffer = (Bytef*)malloc(32768);
    uInt dict_len = 0;
    
    if (!compressed || !decompressed || !dict_buffer) {
        free(compressed);
        free(decompressed);
        free(dict_buffer);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Initialize streams
    z_stream strm_inflate = {0};
    z_stream strm_deflate = {0};
    
    // 1. Inflate the input (decompress it)
    inflateInit2_(&strm_inflate, MAX_WBITS + 16, fuzz_str_1, sizeof(z_stream));
    strm_inflate.next_in = (Bytef*)data;
    strm_inflate.avail_in = size;
    strm_inflate.next_out = decompressed;
    strm_inflate.avail_out = size * 2;
    
    inflate(&strm_inflate, fuzz_int32_t_2);
    inflateEnd(&strm_inflate);
    
    size_t decompressed_size = strm_inflate.total_out;
    
    // 2. Try to get dictionary from inflate stream
    inflateGetDictionary(&strm_inflate, dict_buffer, &dict_len);
    
    // 3. Set up deflate stream with gzip header
    deflateInit2_(&strm_deflate, fuzz_int32_t_4, fuzz_int32_t_5, 
                  MAX_WBITS + 16, fuzz_int32_t_6, fuzz_int32_t_7, 
                  fuzz_str_3, sizeof(z_stream));
    
    // Set gzip header
    gz_header header = {0};
    header.time = 0;
    header.os = 255;
    deflateSetHeader(&strm_deflate, &header);
    
    // 4. Use deflateBound to get maximum compressed size
    uLong bound = deflateBound(&strm_deflate, decompressed_size);
    
    // 5. Re-compress the decompressed data using deflate
    strm_deflate.next_in = decompressed;
    strm_deflate.avail_in = decompressed_size;
    strm_deflate.next_out = compressed;
    strm_deflate.avail_out = bound;
    
    deflate(&strm_deflate, fuzz_int32_t_8);
    size_t compressed_size = strm_deflate.total_out;
    deflateEnd(&strm_deflate);
    
    // 6. Use various compression APIs
    uLongf destLen = max_compressed;
    compress(compressed, &destLen, decompressed, decompressed_size);
    
    uLongf destLen2 = max_compressed;
    compress2(compressed, &destLen2, decompressed, decompressed_size, fuzz_int32_t_9);
    
    z_size_t destLen_z = max_compressed;
    compress_z(compressed, &destLen_z, decompressed, decompressed_size);
    
    z_size_t destLen2_z = max_compressed;
    compress2_z(compressed, &destLen2_z, decompressed, decompressed_size, fuzz_int32_t_10);
    
    // 7. CRC32 calculations
    uLong crc1 = crc32(fuzz_uint64_t_11, Z_NULL, 0);
    crc1 = crc32(crc1, decompressed, decompressed_size / 2);
    
    uLong crc2 = crc32(fuzz_uint64_t_12, Z_NULL, 0);
    crc2 = crc32(crc2, decompressed + decompressed_size / 2, 
                decompressed_size - decompressed_size / 2);
    
    // 8. CRC32 combine operations
    uLong combined_crc = crc32_combine(crc1, crc2, decompressed_size / 2);
    
    off64_t len64 = decompressed_size;
    uLong combined_crc64 = crc32_combine64(crc1, crc2, len64);
    
    uLong combine_gen = crc32_combine_gen64(len64);
    
    // 9. Gzip file operations
    gzFile gz_in = gzdopen(fd_in, "rb");
    gzFile gz_out = gzdopen(fd_out, "wb");
    
    if (gz_in && gz_out) {
        // Read from gz input
        Bytef gz_buffer[4096];
        int bytes_read = gzread(gz_in, gz_buffer, sizeof(gz_buffer));
        
        // Write to gz output
        if (bytes_read > 0) {
            gzwrite(gz_out, gz_buffer, bytes_read);
        }
        
        gzclose_r(gz_in);
        gzclose_w(gz_out);
    } else {
        if (gz_in) gzclose_r(gz_in);
        if (gz_out) gzclose_w(gz_out);
    }
    
    // 10. Set and get dictionary for inflate (round-trip test)
    if (dict_len > 0) {
        z_stream strm_test = {0};
        inflateInit2_(&strm_test, MAX_WBITS + 16, fuzz_str_13, sizeof(z_stream));
        inflateSetDictionary(&strm_test, dict_buffer, dict_len);
        inflateEnd(&strm_test);
    }
    
    // Cleanup
    free(compressed);
    free(decompressed);
    free(dict_buffer);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
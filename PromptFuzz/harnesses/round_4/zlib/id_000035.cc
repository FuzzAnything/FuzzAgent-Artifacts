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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["gzdopen","gzseek","gzclose","inflateInit2_","gzclose","inflateGetHeader","inflateEnd","gzclose","gzread","inflateGetDictionary","inflatePrime","inflate","deflateInit_","deflateBound_z","deflatePrime","deflate","deflateEnd","uncompress2","crc32","crc32","crc32","crc32","crc32_combine","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzseek","inflateInit2_","inflateGetHeader","gzread","inflateGetDictionary","inflatePrime","inflate","deflateInit_","deflateBound_z","deflatePrime","deflate","deflateEnd","uncompress2","crc32","crc32","crc32","crc32","crc32_combine","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<57) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_11, fdp);
	//fuzzer shim end}




    // Early exit if insufficient data
    if (size < 100) {
	return 0;
	}
	
    
    // Step 1: Create a FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Create gzFile from FILE descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 3: Use gzseek to move within the compressed stream
    // Move to a position within the data (ensuring we don't seek past end)
    off_t seek_pos = size / 4;
    if (gzseek(gz_in, seek_pos, fuzz_int32_t_2) == -1) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 4: Prepare decompression structures
    z_stream strm_inflate;
    gz_header header;
    Bytef dictionary[32768];
    uInt dictLength = sizeof(dictionary);
    
    memset(&strm_inflate, 0, sizeof(z_stream));
    memset(&header, 0, sizeof(gz_header));
    
    // Initialize inflation stream
    if (inflateInit2(&strm_inflate, MAX_WBITS + 16) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Step 5: Setup inflation header and try to get it
    strm_inflate.avail_in = 0;
    strm_inflate.next_in = Z_NULL;
    if (inflateGetHeader(&strm_inflate, &header) != Z_OK) {
        // Continue anyway - not all compressed data has gzip headers
    }
    
    // Step 6: Prepare buffers for decompression
    const size_t out_buf_size = 65536;
    Bytef *out_buf = (Bytef *)malloc(out_buf_size);
    Bytef *compressed_buf = (Bytef *)malloc(size);
    if (!out_buf || !compressed_buf) {
        free(out_buf);
        free(compressed_buf);
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Read some compressed data for inflation
    size_t read_size = gzread(gz_in, compressed_buf, size < 8192 ? size : 8192);
    if (read_size > 0) {
        strm_inflate.avail_in = read_size;
        strm_inflate.next_in = compressed_buf;
        strm_inflate.avail_out = out_buf_size;
        strm_inflate.next_out = out_buf;
        
        // Step 7: Try to get dictionary from inflation stream
        if (inflateGetDictionary(&strm_inflate, dictionary, &dictLength) != Z_OK) {
            // Not an error - stream might not have dictionary
        }
        
        // Step 8: Try to inflate with prime bits
        // Add some bits to the stream before inflation
        if (inflatePrime(&strm_inflate, fuzz_int32_t_3, fuzz_int32_t_4) == Z_OK) {
            // Inflate a bit
            int ret = inflate(&strm_inflate, fuzz_int32_t_5);
            if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
                // Inflation error
            }
        }
    }
    
    // Step 9: Prepare for compression to use deflateBound_z
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    if (deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Step 10: Use deflateBound_z to estimate compressed size
        z_size_t est_size = deflateBound_z(&strm_deflate, fuzz_uint64_t_6);
        
        // Step 11: Try deflatePrime with some bits
        if (deflatePrime(&strm_deflate, fuzz_int32_t_7, fuzz_int32_t_8) == Z_OK) {
            // Do a small compression
            strm_deflate.avail_in = 100;
            strm_deflate.next_in = (Bytef *)data;
            strm_deflate.avail_out = out_buf_size;
            strm_deflate.next_out = out_buf;
            
            deflate(&strm_deflate, fuzz_int32_t_9);
        }
        deflateEnd(&strm_deflate);
    }
    
    // Step 12: Try uncompress2 on part of the data
    uLongf destLen = out_buf_size;
    uLong sourceLen = size > 1024 ? 1024 : size;
    Bytef *dest_buf = (Bytef *)malloc(out_buf_size);
    
    if (dest_buf) {
        // uncompress2 expects mutable source, so copy
        Bytef *source_copy = (Bytef *)malloc(sourceLen);
        if (source_copy) {
            memcpy(source_copy, data, sourceLen);
            uLong actualSourceLen = sourceLen;
            int uncompress_ret = uncompress2(dest_buf, &destLen, 
                                           source_copy, &actualSourceLen);
            free(source_copy);
        }
        free(dest_buf);
    }
    
    // Step 13: Compute and combine CRC32 values
    uLong crc1 = crc32(fuzz_uint64_t_10, Z_NULL, 0);
    uLong crc2 = crc32(fuzz_uint64_t_11, Z_NULL, 0);
    
    if (size > 200) {
        crc1 = crc32(crc1, data, 100);
        crc2 = crc32(crc2, data + 100, 100);
        
        // Combine the two CRC32 values
        uLong combined_crc = crc32_combine(crc1, crc2, 100);
        (void)combined_crc; // Prevent unused variable warning
    }
    
    // Step 14: Cleanup
    inflateEnd(&strm_inflate);
    free(out_buf);
    free(compressed_buf);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
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
//<ID> 85
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"uncompress2_z":[[36,31,36,45,0,0,4,1]]},"library_calls":["gzdopen","gztell","gzrewind","gzclose","gzread","gzclose","inflateInit_","gzclose","inflateReset","inflateEnd","gzclose","uncompress2_z","uncompress2_z","crc32_z","crc32_z","compressBound","compress","inflateEnd","gzclose"],"critical_calls":["gzdopen","gztell","gzrewind","gzread","inflateInit_","inflateReset","uncompress2_z","uncompress2_z","crc32_z","crc32_z","compressBound","compress","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
	//fuzzer shim end}




    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Get file descriptor and create gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Use gztell to get current position (should be 0 initially)
    off_t current_pos = gztell(gz_in);
    
    // 4. Read compressed data from gzFile
    // First, get the compressed size by seeking to end
    gzrewind(gz_in);
    size_t compressed_size = 0;
    uint8_t* compressed_buf = nullptr;
    
    // Read the entire gzFile to get compressed data
    // We'll use a buffer that grows as needed
    size_t buf_size = size * 2;  // Start with double the input size
    compressed_buf = (uint8_t*)malloc(buf_size);
    if (!compressed_buf) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int bytes_read;
    size_t total_read = 0;
    while ((bytes_read = gzread(gz_in, compressed_buf + total_read, 4096)) > 0) {
        total_read += bytes_read;
        if (total_read + 4096 > buf_size) {
            buf_size *= 2;
            uint8_t* new_buf = (uint8_t*)realloc(compressed_buf, buf_size);
            if (!new_buf) {
                free(compressed_buf);
                gzclose(gz_in);
                assert_file_closed(&in_file);;
                assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
            }
            compressed_buf = new_buf;
        }
    }
    
    // 5. Create inflate stream for inflateReset
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int ret = inflateInit(&strm);
    if (ret != Z_OK) {
        free(compressed_buf);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Use inflateReset
    ret = inflateReset(&strm);
    if (ret != Z_OK) {
        inflateEnd(&strm);
        free(compressed_buf);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Use uncompress2_z to decompress the data
    // First pass: get decompressed size
    z_size_t destLen = 0;
    z_size_t sourceLen = total_read;
    ret = uncompress2_z(nullptr, &destLen, compressed_buf, &sourceLen);
    
    if (ret == Z_OK || ret == Z_BUF_ERROR) {
        // Allocate buffer for decompressed data
        uint8_t* decompressed_buf = (uint8_t*)malloc(destLen);
        if (decompressed_buf) {
            // Second pass: actually decompress
            z_size_t sourceLen2 = total_read;
            ret = uncompress2_z(decompressed_buf, &destLen, compressed_buf, &sourceLen2);
            
            if (ret == Z_OK) {
                // 7. Use crc32_z to compute CRC of decompressed data
                uLong crc = crc32_z(fuzz_uint64_t_2, Z_NULL, 0);
                crc = crc32_z(crc, decompressed_buf, destLen);
                
                // 8. Use compress to recompress the decompressed data
                uLongf comprLen = compressBound(destLen);
                uint8_t* recompr_buf = (uint8_t*)malloc(comprLen);
                
                if (recompr_buf) {
                    ret = compress(recompr_buf, &comprLen, decompressed_buf, destLen);
                    
                    // Clean up recompression buffer
                    free(recompr_buf);
                }
            }
            
            free(decompressed_buf);
        }
    }
    
    // 9. Clean up
    inflateEnd(&strm);
    free(compressed_buf);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
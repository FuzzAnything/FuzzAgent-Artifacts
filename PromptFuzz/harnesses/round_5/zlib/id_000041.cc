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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["zlibCompileFlags","compressBound","compress2","gzdopen","gzfwrite","crc32","crc32","crc32","crc32","crc32_combine64","crc32_combine_gen64","crc32_combine_op","gzclose","gzdopen","gzseek64","gzrewind","inflateInit2_","inflateReset2","inflateEnd","gzclose_r"],"critical_calls":["zlibCompileFlags","compressBound","compress2","gzdopen","gzfwrite","crc32","crc32","crc32","crc32","crc32_combine64","crc32_combine_gen64","crc32_combine_op","gzclose","gzdopen","gzseek64","gzrewind","inflateInit2_","inflateReset2","inflateEnd","gzclose_r"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    // 1. Get zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Suppress unused warning
    
    // 2. Create compressed data using compress2
    uLongf compressed_len = compressBound(size);
    Bytef* compressed = new Bytef[compressed_len];
    int level = data[0] % 10; // Use first byte for level (0-9)
    
    int compress_result = compress2(compressed, &compressed_len, 
                                    data, size, level);
    
    if (compress_result != Z_OK) {
        delete[] compressed;
        return 0;
    }
    
    // 3. Create gzip file and write compressed data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        delete[] compressed;
        assert_file_closed(&out_file);
	return 0;
    }
    
    int fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        delete[] compressed;
        assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Write compressed data in chunks
    size_t chunks = (compressed_len > 0) ? 1 : 0;
    if (chunks > 0) {
        gzfwrite(compressed, compressed_len, 1, gz_out);
    }
    
    // 4. CRC32 operations
    // Split data into two parts for CRC combination
    size_t half1_len = size / 2;
    size_t half2_len = size - half1_len;
    
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    if (half1_len > 0) {
        crc1 = crc32(crc1, data, half1_len);
    }
    if (half2_len > 0) {
        crc2 = crc32(crc2, data + half1_len, half2_len);
    }
    
    // Combine CRCs using crc32_combine64
    off64_t len2 = half2_len;
    uLong combined_crc = crc32_combine64(crc1, crc2, len2);
    
    // Also use crc32_combine_op
    uLong op = crc32_combine_gen64(len2);
    uLong combined_crc_op = crc32_combine_op(crc1, crc2, op);
    
    (void)combined_crc; // Suppress unused warnings
    (void)combined_crc_op;
    
    // 5. Close and reopen for reading
    gzclose(gz_out); // Close for writing
    assert_file_closed(&out_file);;
    
    // Reopen for reading
    FILE* in_file = fopen("output_file", "rb");
    if (!in_file) {
        delete[] compressed;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        delete[] compressed;
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Use gzseek64
    off64_t seek_pos = (size > 0) ? (data[1] % compressed_len) : 0;
    gzseek64(gz_in, seek_pos, SEEK_SET);
    
    // 7. Use gzrewind
    gzrewind(gz_in);
    
    // 8. Setup inflate stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int windowBits = 15 + 16; // gzip decoding
    int init_result = inflateInit2(&strm, windowBits);
    
    if (init_result == Z_OK) {
        // 9. Use inflateReset2
        int reset_result = inflateReset2(&strm, windowBits);
        (void)reset_result; // Suppress unused warning
        
        // Clean up inflate stream
        inflateEnd(&strm);
    }
    
    // 10. Close reading file
    gzclose_r(gz_in);
    assert_file_closed(&in_file);;
    
    // Cleanup
    delete[] compressed;
    
    // Remove temporary file
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	assert_fd_closed(fd);
	return 0;
}
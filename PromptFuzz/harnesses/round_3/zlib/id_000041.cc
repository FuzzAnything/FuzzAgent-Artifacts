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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 0
//<Quality> {"density":29,"unique_branches":{},"library_calls":["zlibVersion","gzdopen","gzdopen","gzclose","gzsetparams","gzrewind","gzread","gzputs","gzwrite","gzgetc_","gzungetc","gzclose_w","gzclose","compressBound","compress2","compressBound_z","compress_z","crc32_z","crc32_z","deflateInit_","deflateCopy","deflateEnd","inflateInit_","deflateEnd","deflateEnd","inflateCodesUsed","deflateEnd","deflateEnd","inflateEnd"],"critical_calls":["zlibVersion","gzdopen","gzdopen","gzsetparams","gzrewind","gzread","gzputs","gzwrite","gzgetc_","gzungetc","gzclose_w","gzclose","compressBound","compress2","compressBound_z","compress_z","crc32_z","crc32_z","deflateInit_","deflateCopy","inflateInit_","inflateCodesUsed","deflateEnd","deflateEnd","inflateEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve the event of compressing, decompressing, and manipulating compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<56) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // 1. Get zlib version
    const char *version = zlibVersion();
    if (!version) {
	return 0;
	}
	
    
    // 2. Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Create output file for compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 5. Open gzFile for reading
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Open gzFile for writing
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        gzclose(gz_in);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 7. Set compression parameters for output
    int level = Z_DEFAULT_COMPRESSION;
    int strategy = Z_DEFAULT_STRATEGY;
    if (size > 0) {
        level = data[0] % 10;  // 0-9 compression levels
        strategy = data[size % size] % 4;  // 0-3 strategies
    }
    gzsetparams(gz_out, fuzz_int32_t_2, fuzz_int32_t_3);
    
    // 8. Read from input gzFile and write to output gzFile
    std::vector<char> buffer(4096);
    int bytes_read = 0;
    
    // First, rewind to start
    gzrewind(gz_in);
    
    // Read and write data
    while ((bytes_read = gzread(gz_in, buffer.data(), buffer.size())) > 0) {
        // Write a test string first
        gzputs(gz_out, fuzz_str_4);
        
        // Then write the actual data
        gzwrite(gz_out, buffer.data(), bytes_read);
        
        // Read one character
        int c = gzgetc_(gz_in);
        if (c != -1) {
            // Put it back
            gzungetc(c, gz_in);
        }
    }
    
    // 9. Close gz files
    gzclose_w(gz_out);  // Close writing file specifically
    gzclose(gz_in);     // Close reading file
    
    // 10. Now work with raw compression APIs
    
    // Prepare source data (use original input)
    Bytef *source = (Bytef *)data;
    uLong sourceLen = (uLong)(size > 1000000 ? 1000000 : size);  // Limit size for safety
    
    // 11. Compress using compress2
    uLongf destLen1 = compressBound(sourceLen);
    std::vector<Bytef> dest1(destLen1);
    
    int ret1 = compress2(dest1.data(), &destLen1, source, sourceLen, fuzz_int32_t_5);
    if (ret1 != Z_OK) {
        // Clean up and return
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
    
    // 12. Compress using compress_z
    z_size_t destLen2 = compressBound_z(sourceLen);
    z_size_t sourceLen_z = sourceLen;
    std::vector<Bytef> dest2(destLen2);
    
    int ret2 = compress_z(dest2.data(), &destLen2, source, sourceLen_z);
    if (ret2 != Z_OK) {
        // Clean up and return
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
    
    // 13. Calculate CRC32 of compressed data
    uLong crc = crc32_z(fuzz_uint64_t_6, Z_NULL, 0);
    crc = crc32_z(crc, dest1.data(), destLen1);
    
    // 14. Set up z_stream for deflate
    z_stream strm1;
    memset(&strm1, 0, sizeof(strm1));
    
    ret1 = deflateInit_(&strm1, fuzz_int32_t_7, version, sizeof(z_stream));
    if (ret1 != Z_OK) {
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
    
    // 15. Set up another z_stream and copy
    z_stream strm2;
    memset(&strm2, 0, sizeof(strm2));
    
    int ret2_stream = deflateCopy(&strm2, &strm1);
    if (ret2_stream != Z_OK) {
        deflateEnd(&strm1);
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
    
    // 16. Set up z_stream for inflate
    z_stream strm3;
    memset(&strm3, 0, sizeof(strm3));
    
    ret1 = inflateInit_(&strm3, version, sizeof(z_stream));
    if (ret1 != Z_OK) {
        deflateEnd(&strm1);
        deflateEnd(&strm2);
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
    
    // 17. Get inflate codes used
    unsigned long codes_used = inflateCodesUsed(&strm3);
    (void)codes_used;  // Use variable to avoid unused warning
    
    // 18. Clean up all z_streams
    deflateEnd(&strm1);
    deflateEnd(&strm2);
    inflateEnd(&strm3);
    
    // 19. Clean up file resources
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 20. Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
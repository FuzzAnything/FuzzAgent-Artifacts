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
//<ID> 220
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"deflateSetHeader":[[715,36,715,58,0,0,4,0]]},"library_calls":["gzdopen","gzclose","gzclose","uncompress2","inflateInit_","gzclose","inflateReset","deflateInit_","inflateEnd","gzclose","deflateSetHeader","deflateUsed","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","uncompress2","inflateInit_","inflateReset","deflateInit_","deflateSetHeader","deflateUsed","deflateEnd","inflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs together to achieve decompression and header manipulation:

*/


extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t* f_data, size_t f_size) {
	if(f_size<5) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 20) {
	return 0;
	}
	 // Need minimum data for operations

    // Create FILE* variables
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Use gzgetc to read first character from compressed stream
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzdopen(fd, fuzz_str_1);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int first_char = gzgetc(gz);
    (void)first_char; // Use result to avoid compiler warning

    // 2. Use uncompress2 to decompress data
    // Allocate destination buffer (assume 2x compression ratio)
    uLongf destLen = size * 2;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Create copy of source data for uncompress2
    Bytef *source_copy = (Bytef *)malloc(size);
    if (!source_copy) {
        free(dest);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    memcpy(source_copy, data, size);
    
    uLong sourceLen = size;
    int uncompress_result = uncompress2(dest, &destLen, source_copy, &sourceLen);
    (void)uncompress_result; // Use result to avoid compiler warning

    // 3. Setup z_stream for inflation
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // Initialize inflation stream
    if (inflateInit(&strm_inflate) != Z_OK) {
        free(source_copy);
        free(dest);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Use inflateReset
    int reset_result = inflateReset(&strm_inflate);
    (void)reset_result; // Use result to avoid compiler warning

    // 4. Setup z_stream for deflation with header
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    // Initialize deflation stream
    if (deflateInit(&strm_deflate, Z_DEFAULT_COMPRESSION) != Z_OK) {
        inflateEnd(&strm_inflate);
        free(source_copy);
        free(dest);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Use deflateSetHeader
    gz_header header;
    memset(&header, 0, sizeof(gz_header));
    header.text = 1;
    header.os = 3; // Unix
    
    int set_header_result = deflateSetHeader(&strm_deflate, &header);
    (void)set_header_result; // Use result to avoid compiler warning

    // 6. Use deflateUsed to check bit usage
    int bits_used = 0;
    int used_result = deflateUsed(&strm_deflate, &bits_used);
    (void)used_result; // Use result to avoid compiler warning

    // Cleanup all resources
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    gzclose(gz);
    free(source_copy);
    free(dest);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
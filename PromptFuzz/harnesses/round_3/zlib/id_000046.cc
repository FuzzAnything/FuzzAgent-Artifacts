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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 3
//<Quality> {"density":6,"unique_branches":{"gz_read":[[357,40,357,62,0,0,4,1],[368,18,368,36,0,0,4,0],[368,18,368,36,0,0,4,1]]},"library_calls":["gzopen64","gzclose","gzeof","gzread","gzopen64","gzflush","gzclose","deflateInit_","deflateReset","deflateEnd","inflateInit2_","inflateReset2","uncompress2","inflateEnd","gzclose"],"critical_calls":["gzopen64","gzeof","gzread","gzopen64","gzflush","gzclose","deflateInit_","deflateReset","deflateEnd","inflateInit2_","inflateReset2","uncompress2","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    // Create a temporary file with compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open the compressed file with gzopen64
    gzFile gz_in = gzopen64("input_file", fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Read and decompress the data using gzread
    Bytef *decompressed = (Bytef*)malloc(size * 2);
    if (!decompressed) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    unsigned int decompressed_len = 0;
    int bytes_read;
    while (!gzeof(gz_in) && decompressed_len < size * 2) {
        bytes_read = gzread(gz_in, decompressed + decompressed_len, 
                            size * 2 - decompressed_len);
        if (bytes_read <= 0) break;
        decompressed_len += bytes_read;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        fwrite(decompressed, 1, decompressed_len, out_file);
        
        // Open output file for writing compressed data
        gzFile gz_out = gzopen64("output_file", fuzz_str_2);
        if (gz_out) {
            gzflush(gz_out, fuzz_int32_t_3);
            gzclose(gz_out);
        }
        assert_file_closed(&out_file);;
    }
    
    // Initialize z_stream for compression
    z_stream strm_comp;
    strm_comp.zalloc = Z_NULL;
    strm_comp.zfree = Z_NULL;
    strm_comp.opaque = Z_NULL;
    
    // Initialize deflate
    if (deflateInit(&strm_comp, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Reset deflate stream
        deflateReset(&strm_comp);
        deflateEnd(&strm_comp);
    }
    
    // Initialize z_stream for decompression
    z_stream strm_decomp;
    strm_decomp.zalloc = Z_NULL;
    strm_decomp.zfree = Z_NULL;
    strm_decomp.opaque = Z_NULL;
    
    // Initialize inflate
    if (inflateInit2(&strm_decomp, 15 + 16) == Z_OK) { // 15 window bits + gzip header
        // Reset inflate stream with different window bits
        inflateReset2(&strm_decomp, fuzz_int32_t_4); // Reset to zlib format
        
        // Prepare for uncompress2
        Bytef *dest = (Bytef*)malloc(size * 4);
        if (dest) {
            uLongf destLen = size * 4;
            uLong sourceLen = size;
            
            // Use uncompress2 to decompress the original data
            int ret = uncompress2(dest, &destLen, data, &sourceLen);
            
            free(dest);
        }
        
        inflateEnd(&strm_decomp);
    }
    
    // Cleanup
    free(decompressed);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}
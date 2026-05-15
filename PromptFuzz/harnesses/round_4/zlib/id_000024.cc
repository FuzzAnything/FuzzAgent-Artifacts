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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 2
//<Quality> {"density":10,"unique_branches":{"gz_read":[[357,40,357,62,0,0,4,1],[368,18,368,36,0,0,4,0]]},"library_calls":["zlibCompileFlags","gzopen","gzclose","gzread","gzclose","uncompress2","inflateInit2_","inflateReset2","inflateSetDictionary","compressBound_z","compress_z","deflateInit_","deflatePrime","deflateEnd","inflateEnd"],"critical_calls":["zlibCompileFlags","gzopen","gzread","gzclose","uncompress2","inflateInit2_","inflateReset2","inflateSetDictionary","compressBound_z","compress_z","deflateInit_","deflatePrime","deflateEnd","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	
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





    // Early return if input is too small
    if (size < 10) {
	return 0;
	}
	
    
    // 1. Get zlib compilation flags (API: zlibCompileFlags)
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Prevent unused variable warning
    
    // 2. Write input data to a temporary file for gzopen
    FILE *tmp_file = fopen("input_file", "wb");
    if (!tmp_file) {
	assert_file_closed(&tmp_file);
	return 0;
	}
	
    fwrite(data, 1, size, tmp_file);
    assert_file_closed(&tmp_file);;
    
    // 3. Open the compressed file with gzopen (API: gzopen)
    gzFile gz = gzopen(fuzz_str_1, fuzz_str_2);
    if (!gz) {
        remove("input_file");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 4. Read decompressed data from gz file
    size_t decompressed_size = size * 10; // Estimate
    Bytef *decompressed = (Bytef*)malloc(decompressed_size);
    if (!decompressed) {
        gzclose(gz);
        remove("input_file");
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    int read_bytes = gzread(gz, decompressed, decompressed_size);
    gzclose(gz);
    remove("input_file");
    
    if (read_bytes <= 0) {
        free(decompressed);
        assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 5. Prepare for uncompress2 (API: uncompress2)
    uLongf dest_len = size * 20; // Estimate
    Bytef *dest = (Bytef*)malloc(dest_len);
    uLong source_len = size;
    
    if (dest) {
        // Use uncompress2 on the original input data
        int uncompress_result = uncompress2(dest, &dest_len, data, &source_len);
        (void)uncompress_result; // Prevent unused variable warning
    }
    
    // 6. Set up z_stream for inflation
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    // 7. Initialize inflation with windowBits (API: inflateReset2)
    inflateInit2(&strm, 15 + 32); // 15 window bits + 32 for gzip decoding
    inflateReset2(&strm, fuzz_int32_t_3); // Reset to zlib format
    
    // 8. Set dictionary for inflation if we have enough data (API: inflateSetDictionary)
    if (decompressed && read_bytes >= 256) {
        inflateSetDictionary(&strm, decompressed, 256);
    }
    
    // 9. Prepare for compression with compress_z (API: compress_z)
    if (decompressed && read_bytes > 0) {
        z_size_t comp_len = compressBound_z(read_bytes);
        Bytef *compressed = (Bytef*)malloc(comp_len);
        if (compressed) {
            z_size_t actual_comp_len = comp_len;
            compress_z(compressed, &actual_comp_len, decompressed, read_bytes);
            free(compressed);
        }
    }
    
    // 10. Set up deflation stream for deflatePrime
    z_stream def_strm;
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    deflateInit(&def_strm, Z_DEFAULT_COMPRESSION);
    
    // 11. Use deflatePrime with bits from input data (API: deflatePrime)
    if (size >= 2) {
        // Use first 8 bits from input data
        deflatePrime(&def_strm, fuzz_int32_t_4, data[0]);
    }
    
    // 12. Clean up all resources
    deflateEnd(&def_strm);
    inflateEnd(&strm);
    free(decompressed);
    free(dest);
    
    assert_file_closed(&tmp_file);
	assert_file_name_closed("input_file");
	return 0;
}
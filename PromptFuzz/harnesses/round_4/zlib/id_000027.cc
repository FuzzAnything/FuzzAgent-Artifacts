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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"gz_read":[[388,16,388,26,0,0,4,1]]},"library_calls":["zlibCompileFlags","gzopen","gzread","gzclose","uncompress2","compressBound_z","compress_z","inflateInit2_","inflateSetDictionary","inflateReset2","deflateInit_","deflatePrime","inflateEnd","deflateEnd"],"critical_calls":["zlibCompileFlags","gzopen","gzread","gzclose","uncompress2","compressBound_z","compress_z","inflateInit2_","inflateSetDictionary","inflateReset2","deflateInit_","deflatePrime","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
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





    // Early return if input is too small for meaningful operations
    if (size < 10) {
	return 0;
	}
	

    // 1. Get zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Use variable to avoid unused warning

    // 2. Write input data to a file for gzopen to read
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;

    // 3. Open the file with gzopen (read mode)
    gzFile gz_file = gzopen(fuzz_str_1, fuzz_str_2);
    if (!gz_file) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 4. Read some data from gz file
    char buffer[1024];
    int bytes_read = gzread(gz_file, buffer, sizeof(buffer));
    gzclose(gz_file);
    remove("input_file");

    // 5. Prepare for uncompress2 operation
    // Use the original input data as compressed source
    uLongf dest_len = size * 4; // Reasonable expansion factor
    Bytef *dest = (Bytef *)malloc(dest_len);
    if (!dest) {
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
	}
	
    
    uLong source_len = size;
    int uncompress_result = uncompress2(dest, &dest_len, data, &source_len);
    (void)uncompress_result; // Result may be Z_OK, Z_DATA_ERROR, etc.

    // 6. Prepare for compress_z operation (using decompressed data if available)
    z_size_t comp_dest_len = compressBound_z(dest_len);
    Bytef *comp_dest = (Bytef *)malloc(comp_dest_len);
    if (comp_dest && dest_len > 0) {
        z_size_t actual_comp_len = comp_dest_len;
        int compress_result = compress_z(comp_dest, &actual_comp_len, dest, dest_len);
        (void)compress_result;
    }

    // 7. Set up z_stream for inflation with dictionary
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize for inflation with automatic header detection
    int init_result = inflateInit2(&strm, 15 + 32); // 32 enables automatic header detection
    if (init_result != Z_OK) {
        free(dest);
        free(comp_dest);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 8. Set a dictionary for inflation (use first part of decompressed data as dictionary)
    if (dest_len > 256) {
        inflateSetDictionary(&strm, dest, 256);
    }

    // 9. Reset inflation stream with different window bits
    inflateReset2(&strm, fuzz_int32_t_3); // Reset with 15 window bits (32K window)

    // 10. Set up another z_stream for deflation
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    deflateInit(&def_strm, Z_DEFAULT_COMPRESSION);

    // 11. Use deflatePrime to insert bits into the deflation stream
    if (size > 0) {
        // Use first byte of input as bits to prime
        deflatePrime(&def_strm, fuzz_int32_t_4, data[0]);
    }

    // 12. Clean up all resources
    inflateEnd(&strm);
    deflateEnd(&def_strm);
    free(dest);
    free(comp_dest);

    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}
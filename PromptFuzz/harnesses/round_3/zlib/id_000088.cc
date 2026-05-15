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
//<ID> 264
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 3
//<Quality> {"density":9,"unique_branches":{"inflate":[[1078,21,1078,38,0,0,4,1],[1082,21,1082,38,0,0,4,1]],"inflateValidate":[[1390,9,1390,14,0,0,4,1]]},"library_calls":["zlibCompileFlags","gzopen64","gzopen64","compressBound","gzclose","gzclose","compress","compress2_z","deflateInit_","deflateSetDictionary","deflatePrime","compressBound","deflate","deflatePending","deflateEnd","inflateInit2_","inflateValidate","inflate","inflateCodesUsed","inflateEnd","gzrewind","gzread","gzclose"],"critical_calls":["zlibCompileFlags","gzopen64","gzopen64","compressBound","compress","compress2_z","deflateInit_","deflateSetDictionary","deflatePrime","compressBound","deflate","deflatePending","deflateEnd","inflateInit2_","inflateValidate","inflate","inflateCodesUsed","inflateEnd","gzrewind","gzread","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to test zlib functionality with fuzzing input:

*/


extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Return early if input is too small for meaningful operations
    if (size < 100) {
	return 0;
	}
	
    
    // 1. Get zlib compile flags
    uLong flags = zlibCompileFlags();
    (void)flags; // Suppress unused variable warning
    
    // 2. Create a temporary file with the input data for gz operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Open gz file for reading
    gzFile gz = gzopen64("input_file", fuzz_str_1);
    if (!gz) {
        // Create a gz file from the input data
        FILE *tmp = fopen("input_file", "wb");
        if (tmp) {
            fwrite(data, 1, size, tmp);
            assert_file_closed(&tmp);;
        }
        gz = gzopen64("input_file", fuzz_str_2);
        if (!gz) {
            assert_file_closed(&in_file);;
            assert_file_closed(&tmp);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
        }
    }
    
    // 4. Calculate compression bound and compress data
    uLong bound = compressBound(size);
    if (bound == 0 || bound > 1000000) { // Limit memory usage
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    Bytef *compressed = (Bytef *)malloc(bound);
    Bytef *compressed2 = (Bytef *)malloc(bound);
    Bytef *decompressed = (Bytef *)malloc(size * 2);
    
    if (!compressed || !compressed2 || !decompressed) {
        free(compressed);
        free(compressed2);
        free(decompressed);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Perform various compression operations
    uLongf comprLen = bound;
    int compress_ret = compress(compressed, &comprLen, data, size);
    
    // Use compress2_z with compression level derived from input
    z_size_t comprLen2 = bound;
    int level = (data[0] % 10) + 1; // Level 1-10
    int compress2_ret = compress2_z(compressed2, &comprLen2, data, size, level);
    
    // 6. Setup deflate stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    int deflate_init_ret = deflateInit_(&deflate_strm, level, fuzz_str_3, sizeof(z_stream));
    if (deflate_init_ret == Z_OK) {
        // Set dictionary from part of input data
        uInt dictSize = size > 256 ? 256 : (uInt)size;
        int dict_ret = deflateSetDictionary(&deflate_strm, data, dictSize);
        
        // Prime deflate with some bits from input
        if (size > 10) {
            int prime_bits = data[1] % 8 + 1;
            int prime_value = data[2] % 256;
            deflatePrime(&deflate_strm, prime_bits, prime_value);
        }
        
        // Process some data with deflate
        uLong sourceLen = size > 1000 ? 1000 : (uLong)size;
        uLong destLen = compressBound(sourceLen);
        Bytef *dest = (Bytef *)malloc(destLen);
        
        if (dest) {
            deflate_strm.next_in = (Bytef *)data;
            deflate_strm.avail_in = (uInt)sourceLen;
            deflate_strm.next_out = dest;
            deflate_strm.avail_out = (uInt)destLen;
            
            int deflate_ret = deflate(&deflate_strm, fuzz_int32_t_4);
            
            // Check pending bits
            unsigned int pending;
            int bits;
            deflatePending(&deflate_strm, &pending, &bits);
            
            free(dest);
        }
        
        // End deflate
        deflateEnd(&deflate_strm);
    }
    
    // 7. Setup inflate stream
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    int windowBits = 15 + 32; // gzip mode
    int inflate_init_ret = inflateInit2_(&inflate_strm, windowBits, fuzz_str_5, sizeof(z_stream));
    
    if (inflate_init_ret == Z_OK && comprLen > 0) {
        // Validate inflate stream
        int validate = (data[3] % 2); // 0 or 1
        inflateValidate(&inflate_strm, validate);
        
        // Setup inflate
        inflate_strm.next_in = compressed;
        inflate_strm.avail_in = (uInt)comprLen;
        inflate_strm.next_out = decompressed;
        inflate_strm.avail_out = (uInt)(size * 2);
        
        // Inflate the compressed data
        int inflate_ret = inflate(&inflate_strm, fuzz_int32_t_6);
        
        // Get codes used
        unsigned long codes_used = inflateCodesUsed(&inflate_strm);
        (void)codes_used;
        
        // End inflate
        inflateEnd(&inflate_strm);
    }
    
    // 8. Perform gz file operations
    if (gz) {
        // Rewind gz file
        gzrewind(gz);
        
        // Read from gz file
        char buf[1024];
        int read_ret = gzread(gz, buf, sizeof(buf));
        
        // Close gz file
        gzclose(gz);
    }
    
    // 9. Clean up
    free(compressed);
    free(compressed2);
    free(decompressed);
    
    // Note: inflateBackEnd is not used because inflateBack wasn't called
    // This API requires a full inflateBack setup which is complex
    
    // Close the memory file
    assert_file_closed(&in_file);;
    
    // Clean up temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}
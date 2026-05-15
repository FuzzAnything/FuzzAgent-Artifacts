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
//<ID> 16
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.666667, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"compress2_z":[[31,10,31,23,0,0,4,1]]},"library_calls":["zlibCompileFlags","gzdopen","gzungetc","gzclose","inflateBackInit_","gzclose","inflateBackEnd","gzrewind","gzread","gzclose","deflateInit_","gzclose","deflateSetDictionary","inflateInit2_","inflateValidate","inflateEnd","deflatePending","deflatePrime","compressBound","compress2_z","inflateInit2_","inflate","inflateCodesUsed","inflateEnd","deflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","gzungetc","inflateBackInit_","inflateBackEnd","gzrewind","gzread","deflateInit_","deflateSetDictionary","inflateInit2_","inflateValidate","inflateEnd","deflatePending","deflatePrime","compressBound","compress2_z","inflateInit2_","inflate","inflateCodesUsed","inflateEnd","deflateEnd","gzclose"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<38) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Get zlib compile flags
    uLong flags = zlibCompileFlags();
    
    // Create a file from the input data for gzip operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open a gzip file for reading
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzdopen(fd, fuzz_str_1);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Try to unget a character
    int first_byte = gzgetc(gz);
    if (first_byte != -1) {
        gzungetc(first_byte, gz);
    }
    
    // Read decompressed data
    size_t decomp_size = size * 4;  // Initial guess
    Bytef *decompressed = (Bytef *)malloc(decomp_size);
    if (!decompressed) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Initialize inflation stream for back-end inflation
    z_stream strm_back;
    unsigned char window[1 << 15];  // 32KB window
    if (inflateBackInit_(&strm_back, 15, window, 
                         fuzz_str_2, sizeof(z_stream)) != Z_OK) {
        free(decompressed);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Clean up back-end inflation stream
    inflateBackEnd(&strm_back);
    
    // Re-open the gzip file for reading again
    gzrewind(gz);
    
    // Read using normal gzread
    int read_bytes = gzread(gz, decompressed, decomp_size);
    if (read_bytes < 0) {
        free(decompressed);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Get actual decompressed size
    size_t actual_decomp_size = read_bytes;
    
    // Initialize a deflate stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(decompressed);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set dictionary for validation
    if (actual_decomp_size > 0) {
        deflateSetDictionary(&strm, decompressed, 
                           (uInt)(actual_decomp_size < 32768 ? actual_decomp_size : 32768));
    }
    
    // Validate inflation would accept this data
    z_stream strm_inflate;
    strm_inflate.zalloc = Z_NULL;
    strm_inflate.zfree = Z_NULL;
    strm_inflate.opaque = Z_NULL;
    
    if (inflateInit2(&strm_inflate, 15) == Z_OK) {
        inflateValidate(&strm_inflate, fuzz_int32_t_3);  // Enable validation
        inflateEnd(&strm_inflate);
    }
    
    // Check pending bits before compression
    unsigned int pending;
    int bits;
    deflatePending(&strm, &pending, &bits);
    
    // Prime the deflate stream with some bits
    deflatePrime(&strm, fuzz_int32_t_4, fuzz_int32_t_5);  // Add 3 bits with value 5
    
    // Get compression bound
    uLong bound = compressBound((uLong)actual_decomp_size);
    
    // Compress using compress2_z
    z_size_t comp_size = bound;
    Bytef *compressed = (Bytef *)malloc(comp_size);
    if (compressed) {
        // Recompress the decompressed data
        int level = (data[0] % 10) + 1;  // Random level 1-10
        if (compress2_z(compressed, &comp_size, 
                       decompressed, (z_size_t)actual_decomp_size, 
                       level) == Z_OK) {
            // Check codes that would be used for inflation
            z_stream strm_inflate2;
            strm_inflate2.zalloc = Z_NULL;
            strm_inflate2.zfree = Z_NULL;
            strm_inflate2.opaque = Z_NULL;
            Bytef *dummy_out = (Bytef *)malloc(1024);
            
            if (dummy_out && inflateInit2(&strm_inflate2, 15) == Z_OK) {
                strm_inflate2.next_in = compressed;
                strm_inflate2.avail_in = (uInt)(comp_size < 1024 ? comp_size : 1024);
                strm_inflate2.next_out = dummy_out;
                strm_inflate2.avail_out = 1024;
                
                inflate(&strm_inflate2, fuzz_int32_t_6);
                unsigned long codes_used = inflateCodesUsed(&strm_inflate2);
                (void)codes_used;  // Use the value
                
                inflateEnd(&strm_inflate2);
                free(dummy_out);
            }
        }
        free(compressed);
    }
    
    // Clean up
    deflateEnd(&strm);
    free(decompressed);
    gzclose(gz);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
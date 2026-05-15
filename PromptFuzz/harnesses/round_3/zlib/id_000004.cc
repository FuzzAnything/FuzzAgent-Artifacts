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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":16,"unique_branches":{"inflateValidate":[[1388,9,1388,32,0,0,4,0]],"inflateStateCheck":[[94,28,94,47,0,0,4,0]]},"library_calls":["compressBound_z","compress2_z","zlibCompileFlags","inflateBackInit_","deflateInit2_","inflateBackEnd","deflatePrime","deflatePending","inflateValidate","deflateEnd","inflateBackEnd","gzdopen","deflateEnd","inflateBackEnd","gzungetc","inflateInit2_","inflate","inflateCodesUsed","inflateEnd","gzclose","deflateEnd","inflateBackEnd"],"critical_calls":["compressBound_z","compress2_z","zlibCompileFlags","inflateBackInit_","deflateInit2_","deflatePrime","deflatePending","inflateValidate","gzdopen","gzungetc","inflateInit2_","inflate","inflateCodesUsed","inflateEnd","gzclose","deflateEnd","inflateBackEnd"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<92) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	//fuzzer shim end}




    // 1. First, we need to create a compressed version of the input data
    // to simulate zlib output data for the APIs to work with
    z_size_t compressed_len = compressBound_z(size);
    if (compressed_len == 0) {
	return 0;
	}
	
    
    Bytef *compressed_data = (Bytef *)malloc(compressed_len);
    if (!compressed_data) {
	return 0;
	}
	
    
    // Use compress2_z to create compressed data
    int compress_result = compress2_z(compressed_data, &compressed_len, 
                                      (const Bytef *)data, size, fuzz_int32_t_1);
    if (compress_result != Z_OK) {
        free(compressed_data);
        return 0;
    }
    
    // 2. Check zlib compile flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags;  // Use the value to avoid unused variable warning
    
    // 3. Set up inflation stream for back-inflation
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    
    // Initialize window for inflateBack
    unsigned char window[1 << 15];  // 32KB window
    int windowBits = 15;
    
    // Initialize inflateBack stream
    int init_result = inflateBackInit_(&inflate_strm, windowBits, window,
                                       fuzz_str_2, sizeof(z_stream));
    if (init_result != Z_OK) {
        free(compressed_data);
        return 0;
    }
    
    // 4. Set up a deflate stream to test deflate-related APIs
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    
    // Initialize deflate stream
    if (deflateInit2_(&deflate_strm, fuzz_int32_t_4, fuzz_int32_t_5,
                      fuzz_int32_t_6, fuzz_int32_t_7, fuzz_int32_t_8,
                      fuzz_str_3, sizeof(z_stream)) != Z_OK) {
        inflateBackEnd(&inflate_strm);
        free(compressed_data);
        return 0;
    }
    
    // 5. Test deflatePrime with some bits from the input
    if (size > 0) {
        int bits = data[0] % 16;  // Use 0-15 bits
        int value = (size > 1) ? data[1] : 0;
        deflatePrime(&deflate_strm, bits, value);
    }
    
    // 6. Test deflatePending
    unsigned int pending;
    int bits;
    deflatePending(&deflate_strm, &pending, &bits);
    
    // 7. Test inflateValidate
    int validate_result = inflateValidate(&inflate_strm, fuzz_int32_t_9);
    (void)validate_result;  // Use the value
    
    // 8. Create a FILE* from the compressed data for gzungetc
    FILE *in_file = fmemopen((void *)compressed_data, compressed_len, "rb");
    if (!in_file) {
        deflateEnd(&deflate_strm);
        inflateBackEnd(&inflate_strm);
        free(compressed_data);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 9. Create gzFile for gzungetc testing
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, fuzz_str_10);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        deflateEnd(&deflate_strm);
        inflateBackEnd(&inflate_strm);
        free(compressed_data);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 10. Test gzungetc
    if (compressed_len > 0) {
        int c = gzgetc(gz_file);
        if (c != -1) {
            gzungetc(c, gz_file);
        }
    }
    
    // 11. Get inflate codes used (this requires an active inflation)
    // We'll set up a simple inflation to test this
    z_stream test_inflate_strm;
    memset(&test_inflate_strm, 0, sizeof(z_stream));
    
    if (inflateInit2_(&test_inflate_strm, fuzz_int32_t_12, fuzz_str_11, sizeof(z_stream)) == Z_OK) {
        // Set input
        test_inflate_strm.next_in = compressed_data;
        test_inflate_strm.avail_in = (uInt)compressed_len;
        
        // Create output buffer
        Bytef *decompressed = (Bytef *)malloc(size);
        if (decompressed) {
            test_inflate_strm.next_out = decompressed;
            test_inflate_strm.avail_out = (uInt)size;
            
            // Inflate a bit
            inflate(&test_inflate_strm, fuzz_int32_t_13);
            
            // Get codes used
            unsigned long codes_used = inflateCodesUsed(&test_inflate_strm);
            (void)codes_used;  // Use the value
            
            free(decompressed);
        }
        inflateEnd(&test_inflate_strm);
    }
    
    // 12. Clean up resources
    gzclose(gz_file);  // This will also close the underlying file descriptor
    // Note: gzclose closes the file, so we don't need to assert_file_closed(&in_file); separately
    
    deflateEnd(&deflate_strm);
    inflateBackEnd(&inflate_strm);
    free(compressed_data);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
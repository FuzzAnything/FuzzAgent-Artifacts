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
//<ID> 262
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":5,"unique_branches":{"inflateBackEnd":[[573,27,573,48,0,0,4,0]]},"library_calls":["zlibCompileFlags","gzopen64","gzread","gzrewind","compressBound","compress","compress2_z","deflateInit_","deflateSetDictionary","deflatePending","deflatePrime","deflateEnd","inflateInit2_","inflateValidate","inflate","inflateCodesUsed","inflateEnd","inflateBackEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzopen64","gzread","gzrewind","compressBound","compress","compress2_z","deflateInit_","deflateSetDictionary","deflatePending","deflatePrime","deflateEnd","inflateInit2_","inflateValidate","inflate","inflateCodesUsed","inflateEnd","inflateBackEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t* f_data, size_t f_size) {
	if(f_size<51) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Get zlib compile flags
    uLong flags = zlibCompileFlags();
    (void)flags; // Prevent unused variable warning

    // 2. Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Open compressed file using gzopen64
    int fd = fuzz_fileno(in_file);
    gzFile gzfile = gzopen64("input_file", fuzz_str_1);
    if (!gzfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Read some data from gzfile
    unsigned char read_buf[1024];
    int bytes_read = gzread(gzfile, read_buf, sizeof(read_buf));
    
    // 5. Rewind gzfile
    gzrewind(gzfile);
    
    // 6. Get compressed data size bound
    uLong bound = compressBound(size);
    
    // 7. Prepare buffers for compression
    Bytef *compressed = (Bytef *)malloc(bound);
    uLongf compressed_len = bound;
    
    // 8. Compress using fuzzer data
    int compress_result = compress(compressed, &compressed_len, data, size);
    
    // 9. Prepare for compress2_z
    z_size_t compressed_z_len = bound;
    Bytef *compressed_z = (Bytef *)malloc(bound);
    
    // 10. Use compress2_z
    if (size > 0) {
        int level = data[0] % 10; // Use first byte for level
        compress2_z(compressed_z, &compressed_z_len, data, size, level);
    }
    
    // 11. Initialize deflate stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    int deflate_init_result = deflateInit_(&deflate_strm, fuzz_int32_t_3,
                                          fuzz_str_2, sizeof(z_stream));
    
    if (deflate_init_result == Z_OK && size > 0) {
        // 12. Set dictionary for deflate
        Bytef dict_buf[256];
        uInt dict_len = size > 256 ? 256 : (uInt)size;
        memcpy(dict_buf, data, dict_len);
        deflateSetDictionary(&deflate_strm, dict_buf, dict_len);
        
        // 13. Check pending bits
        unsigned int pending;
        int bits;
        deflatePending(&deflate_strm, &pending, &bits);
        
        // 14. Prime the deflate stream
        if (size > 1) {
            int prime_bits = data[1] % 8;
            int prime_value = data[1] & 0xFF;
            deflatePrime(&deflate_strm, prime_bits, prime_value);
        }
        
        // 15. End deflate stream
        deflateEnd(&deflate_strm);
    }
    
    // 16. Initialize inflate stream
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    
    // 17. Use inflateInit2_ with windowBits
    int windowBits = 15 + (size > 0 ? (data[0] % 16) : 0);
    int inflate_init_result = inflateInit2_(&inflate_strm, windowBits,
                                           fuzz_str_4, sizeof(z_stream));
    
    if (inflate_init_result == Z_OK && compressed_len > 0) {
        // 18. Set up inflate input
        inflate_strm.avail_in = compressed_len;
        inflate_strm.next_in = compressed;
        
        // 19. Set up inflate output
        Bytef inflate_out[4096];
        inflate_strm.avail_out = sizeof(inflate_out);
        inflate_strm.next_out = inflate_out;
        
        // 20. Enable validation
        inflateValidate(&inflate_strm, fuzz_int32_t_5);
        
        // 21. Perform inflation
        int inflate_result = inflate(&inflate_strm, fuzz_int32_t_6);
        
        // 22. Get codes used
        unsigned long codes_used = inflateCodesUsed(&inflate_strm);
        (void)codes_used; // Prevent unused variable warning
        
        // 23. End inflate stream
        inflateEnd(&inflate_strm);
    }
    
    // 24. Initialize inflateBack stream for completeness
    z_stream inflate_back_strm;
    memset(&inflate_back_strm, 0, sizeof(z_stream));
    inflate_back_strm.zalloc = Z_NULL;
    inflate_back_strm.zfree = Z_NULL;
    inflate_back_strm.opaque = Z_NULL;
    
    // 25. We would normally use inflateBack here, but we can't without
    // proper callbacks. We'll just end it to use the API.
    inflateBackEnd(&inflate_back_strm);
    
    // 26. Close gzfile
    gzclose(gzfile);
    
    // 27. Clean up
    free(compressed);
    free(compressed_z);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}
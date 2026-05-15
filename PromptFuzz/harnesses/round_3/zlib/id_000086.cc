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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["zlibCompileFlags","gzdopen","gzread","gzrewind","gzclose","compressBound","compress","compressBound_z","compress2_z","deflateInit_","deflateSetDictionary","deflate","deflatePending","deflatePrime","deflateEnd","inflateInit2_","inflateValidate","inflate","inflateCodesUsed","inflateEnd"],"critical_calls":["zlibCompileFlags","gzdopen","gzread","gzrewind","gzclose","compressBound","compress","compressBound_z","compress2_z","deflateInit_","deflateSetDictionary","deflate","deflatePending","deflatePrime","deflateEnd","inflateInit2_","inflateValidate","inflate","inflateCodesUsed","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t* f_data, size_t f_size) {
	if(f_size<59) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
	//fuzzer shim end}




    // 1. Check zlib compile flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags;  // Prevent unused variable warning
    
    // 2. Create temporary files for gzip operations
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
    
    // 3. Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 4. Open gzip file for reading
    gzFile gz_in = gzdopen(in_fd, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 5. Try to read from gzip file
    unsigned char read_buf[4096];
    int bytes_read = gzread(gz_in, read_buf, sizeof(read_buf));
    
    // 6. Try to rewind gzip file
    gzrewind(gz_in);
    
    // 7. Close gzip file
    gzclose(gz_in);
    
    // 8. Prepare for compression operations
    uLong sourceLen = size > 1000 ? 1000 : size;
    uLongf destLen = compressBound(sourceLen);
    Bytef* compressed = (Bytef*)malloc(destLen);
    Bytef* uncompressed = (Bytef*)malloc(sourceLen);
    
    if (!compressed || !uncompressed) {
        free(compressed);
        free(uncompressed);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 9. Simple compress operation
    if (size > 0) {
        compress(compressed, &destLen, data, sourceLen);
    }
    
    // 10. compress2_z operation
    z_size_t destLen_z = compressBound_z(sourceLen);
    z_size_t sourceLen_z = sourceLen;
    Bytef* compressed_z = (Bytef*)malloc(destLen_z);
    
    if (compressed_z && size > 0) {
        compress2_z(compressed_z, &destLen_z, data, sourceLen_z, fuzz_int32_t_2);
    }
    
    // 11. Set up deflate stream
    z_stream def_stream;
    memset(&def_stream, 0, sizeof(def_stream));
    deflateInit_(&def_stream, fuzz_int32_t_4, fuzz_str_3, sizeof(z_stream));
    
    // 12. Set dictionary if we have enough data
    if (size >= 32) {
        deflateSetDictionary(&def_stream, data, 32);
    }
    
    // 13. Configure deflate stream
    def_stream.next_in = (Bytef*)data;
    def_stream.avail_in = size > 1024 ? 1024 : size;
    def_stream.next_out = compressed;
    def_stream.avail_out = destLen;
    
    // 14. Perform deflate operation
    int flush = size < 1024 ? Z_FINISH : Z_NO_FLUSH;
    deflate(&def_stream, flush);
    
    // 15. Check deflate pending data
    unsigned int pending;
    int bits;
    deflatePending(&def_stream, &pending, &bits);
    
    // 16. Prime deflate with some bits
    if (size > 0) {
        deflatePrime(&def_stream, fuzz_int32_t_5, data[0] & 0x07);
    }
    
    // 17. End deflate
    deflateEnd(&def_stream);
    
    // 18. Set up inflate stream
    z_stream inf_stream;
    memset(&inf_stream, 0, sizeof(inf_stream));
    inflateInit2_(&inf_stream, 15 + 16, fuzz_str_6, sizeof(z_stream)); // 16 for gzip
    
    // 19. Enable validation
    inflateValidate(&inf_stream, fuzz_int32_t_7);
    
    // 20. Configure inflate stream with compressed data
    inf_stream.next_in = compressed;
    inf_stream.avail_in = def_stream.total_out;
    inf_stream.next_out = uncompressed;
    inf_stream.avail_out = sourceLen;
    
    // 21. Perform inflate operation
    inflate(&inf_stream, fuzz_int32_t_8);
    
    // 22. Get codes used
    unsigned long codes_used = inflateCodesUsed(&inf_stream);
    (void)codes_used;  // Prevent unused variable warning
    
    // 23. End inflate
    inflateEnd(&inf_stream);
    
    // 24. Clean up allocated memory
    free(compressed);
    free(uncompressed);
    free(compressed_z);
    
    // 25. Close file handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // 26. Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
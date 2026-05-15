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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"deflatePrime":[[762,13,762,23,0,0,4,1]]},"library_calls":["zlibCompileFlags","gzdopen","inflateInit2_","gzclose","inflateSetDictionary","inflateReset2","deflateInit_","inflateEnd","gzclose","deflatePrime","compressBound","compressBound","uncompress2","compressBound_z","compress_z","deflateEnd","inflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzdopen","inflateInit2_","inflateSetDictionary","inflateReset2","deflateInit_","deflatePrime","compressBound","compressBound","uncompress2","compressBound_z","compress_z","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<5) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // 1. First, get zlib compile flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags; // Use it to avoid unused variable warning
    
    // 2. Create a temporary file with the input data for gzopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Use gzopen to open the file (simulating reading zlib compressed data)
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Create z_stream for inflation
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Initialize inflation with default windowBits
    if (inflateInit2(&inflate_strm, MAX_WBITS) != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Try to set dictionary for inflation (use first part of data as dictionary)
    uInt dict_length = size > 256 ? 256 : (uInt)size;
    if (dict_length > 0) {
        // Use first dict_length bytes as dictionary
        inflateSetDictionary(&inflate_strm, data, dict_length);
    }
    
    // 6. Reset inflation stream with different windowBits
    inflateReset2(&inflate_strm, MAX_WBITS + 16); // Add 16 for gzip header
    
    // 7. Create z_stream for deflation
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Initialize deflation
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        inflateEnd(&inflate_strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 8. Try to prime the deflate stream with some bits from input
    if (size >= 2) {
        int bits = data[0] % 16 + 1; // 1-16 bits
        int value = data[1];
        deflatePrime(&deflate_strm, bits, value);
    }
    
    // 9. Try to uncompress data using uncompress2
    // We'll use a portion of the input as compressed data
    size_t compress_test_size = size > 1024 ? 1024 : size;
    if (compress_test_size > 0) {
        Bytef *dest = (Bytef *)malloc(compressBound((uLong)compress_test_size));
        uLongf destLen = compressBound((uLong)compress_test_size);
        uLong sourceLen = (uLong)compress_test_size;
        
        if (dest) {
            // Note: This will likely fail since input isn't valid compressed data
            // But we're calling the API as requested
            uncompress2(dest, &destLen, data, &sourceLen);
            free(dest);
        }
    }
    
    // 10. Try to compress data using compress_z
    if (size > 0) {
        size_t compress_z_size = size > 512 ? 512 : size;
        z_size_t destLen = compressBound_z(compress_z_size);
        Bytef *dest_z = (Bytef *)malloc(destLen);
        
        if (dest_z) {
            z_size_t sourceLen = compress_z_size;
            // This might succeed since we're compressing arbitrary data
            compress_z(dest_z, &destLen, data, sourceLen);
            free(dest_z);
        }
    }
    
    // 11. Clean up - release all resources
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
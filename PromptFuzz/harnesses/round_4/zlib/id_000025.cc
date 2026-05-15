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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 5, nr_unique_branch: 0
//<Quality> {"density":5,"unique_branches":{},"library_calls":["zlibCompileFlags","gzopen","gzclose","gzclose","uncompress2","deflateInit_","gzclose","deflatePrime","inflateInit_","inflateSetDictionary","inflateReset2","inflateEnd","compressBound_z","compress_z","deflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","gzopen","uncompress2","deflateInit_","deflatePrime","inflateInit_","inflateSetDictionary","inflateReset2","inflateEnd","compressBound_z","compress_z","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<18) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // 1. Get zlib compile flags first
    uLong compile_flags = zlibCompileFlags();
    
    // 2. Create a memory file from input data for gzopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Open the memory file as a gzip stream
    gzFile gz = gzopen(fuzz_str_1, fuzz_str_2);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create output buffer for uncompressed data
    uLongf destLen = size * 4;  // Reasonable expansion factor
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create source buffer from input data
    Bytef *source = (Bytef *)malloc(size);
    if (!source) {
        free(dest);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    memcpy(source, data, size);
    
    // 6. Use uncompress2 with input data
    uLong sourceLen = size;
    int uncompress_result = uncompress2(dest, &destLen, source, &sourceLen);
    
    // 7. Create compression stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    // 8. Initialize deflate with default parameters
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(source);
        free(dest);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 9. Use deflatePrime with bits from the input data
    if (size > 2) {
        int bits = data[0] & 0x07;  // Use first 3 bits
        int value = data[1] & 0xFF; // Use second byte as value
        deflatePrime(&strm, bits, value);
    }
    
    // 10. Create dictionary from input data (truncated to reasonable size)
    uInt dictLength = size > 32768 ? 32768 : (uInt)size;
    Bytef *dictionary = (Bytef *)malloc(dictLength);
    if (dictionary && size > 0) {
        memcpy(dictionary, data, dictLength < size ? dictLength : size);
        
        // 11. Use inflateSetDictionary with the created dictionary
        // First create an inflate stream
        z_stream inflate_strm;
        inflate_strm.zalloc = Z_NULL;
        inflate_strm.zfree = Z_NULL;
        inflate_strm.opaque = Z_NULL;
        
        if (inflateInit(&inflate_strm) == Z_OK) {
            // Set dictionary for inflation
            inflateSetDictionary(&inflate_strm, dictionary, dictLength);
            
            // 12. Use inflateReset2 with windowBits from input data
            int windowBits = size > 3 ? (data[2] % 16) + 8 : 15;
            inflateReset2(&inflate_strm, windowBits);
            
            inflateEnd(&inflate_strm);
        }
    }
    
    // 13. Compress data using compress_z
    z_size_t compDestLen = compressBound_z(size);
    Bytef *compDest = (Bytef *)malloc(compDestLen);
    if (compDest && size > 0) {
        z_size_t compSrcLen = size;
        compress_z(compDest, &compDestLen, source, compSrcLen);
    }
    
    // 14. Clean up resources
    if (dictionary) free(dictionary);
    if (compDest) free(compDest);
    deflateEnd(&strm);
    free(source);
    free(dest);
    gzclose(gz);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}
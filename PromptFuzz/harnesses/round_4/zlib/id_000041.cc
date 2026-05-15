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
//<ID> 83
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["uncompress","deflateInit2_","deflateSetDictionary","deflateEnd","deflateGetDictionary","compressBound","deflateEnd","compress2","deflatePending","deflateUsed","gzdopen","gzputc","gzwrite","gzclose","inflateBackInit_","inflateBackEnd","deflateEnd"],"critical_calls":["uncompress","deflateInit2_","deflateSetDictionary","deflateGetDictionary","compressBound","compress2","deflatePending","deflateUsed","gzdopen","gzputc","gzwrite","gzclose","inflateBackInit_","inflateBackEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // 1. First, decompress the input data which is zlib-compressed
    uLongf decompressed_size = size * 10; // Initial estimate
    Bytef* decompressed_data = (Bytef*)malloc(decompressed_size);
    if (!decompressed_data) {
	return 0;
	}
	
    
    int uncompress_result = uncompress(decompressed_data, &decompressed_size, 
                                       data, (uLong)size);
    if (uncompress_result != Z_OK) {
        free(decompressed_data);
        return 0;
    }
    
    // 2. Create and initialize deflate stream for recompression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(z_stream));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    int deflate_init_result = deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION,
                                          Z_DEFLATED, MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
    if (deflate_init_result != Z_OK) {
        free(decompressed_data);
        return 0;
    }
    
    // 3. Set dictionary from decompressed data (if available)
    uInt dictLength = 0;
    if (decompressed_size > 0) {
        // Use first 32KB or less as dictionary
        dictLength = (uInt)(decompressed_size > 32768 ? 32768 : decompressed_size);
        int dict_result = deflateSetDictionary(&deflate_strm, decompressed_data, dictLength);
        if (dict_result != Z_OK) {
            deflateEnd(&deflate_strm);
            free(decompressed_data);
            return 0;
        }
    }
    
    // 4. Get dictionary back using deflateGetDictionary
    Bytef* retrieved_dict = (Bytef*)malloc(dictLength);
    uInt retrieved_dict_len = dictLength;
    if (retrieved_dict) {
        deflateGetDictionary(&deflate_strm, retrieved_dict, &retrieved_dict_len);
        free(retrieved_dict);
    }
    
    // 5. Create output buffer for compressed data
    uLongf compressed_size = compressBound(decompressed_size);
    Bytef* compressed_data = (Bytef*)malloc(compressed_size);
    if (!compressed_data) {
        deflateEnd(&deflate_strm);
        free(decompressed_data);
        return 0;
    }
    
    // 6. Use compress2 to compress the decompressed data
    uLongf actual_compressed_size = compressed_size;
    int compress2_result = compress2(compressed_data, &actual_compressed_size,
                                    decompressed_data, decompressed_size, 
                                    fuzz_int32_t_1);
    
    // 7. Check pending bits in deflate stream
    unsigned int pending;
    int bits;
    deflatePending(&deflate_strm, &pending, &bits);
    
    // 8. Get used bits in deflate stream
    int used_bits;
    deflateUsed(&deflate_strm, &used_bits);
    
    // 9. Create a gzFile and write a character to it
    FILE* tmp_file = fopen("output_file", "wb");
    if (tmp_file) {
	int fuzz_fd_0 = fuzz_fileno(tmp_file);

        gzFile gz = gzdopen(fuzz_fd_0, fuzz_str_2);
        if (gz) {
            gzputc(gz, 'A');  // Write a character
            
            // Also write some of the compressed data
            if (actual_compressed_size > 0) {
                gzwrite(gz, compressed_data, 
                       (unsigned int)(actual_compressed_size > 100 ? 100 : actual_compressed_size));
            }
            
            gzclose(gz);
        }
        assert_file_closed(&tmp_file);;
        remove("output_file");
    }
    
    // 10. Setup and initialize inflateBack stream
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(z_stream));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    
    unsigned char window[1 << 15]; // 32KB window
    int inflate_back_init_result = inflateBackInit_(&inflate_strm, MAX_WBITS, 
                                                   window, fuzz_str_3, 
                                                   (int)sizeof(z_stream));
    
    if (inflate_back_init_result == Z_OK) {
        // In a real scenario, we'd set up callbacks here, but for fuzzing we'll just end it
        inflateBackEnd(&inflate_strm);
    }
    
    // 11. Cleanup all resources
    deflateEnd(&deflate_strm);
    free(decompressed_data);
    free(compressed_data);
    
    assert_file_closed(&tmp_file);
	return 0;
}
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
//<ID> 84
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 23
//<Quality> {"density":11,"unique_branches":{"deflate":[[1036,46,1036,58,0,0,4,0],[1038,18,1038,30,0,0,4,0]],"deflate_stored":[[1694,13,1694,48,0,0,4,1],[1704,13,1704,28,0,0,4,1],[1704,34,1704,42,0,0,4,1],[1712,37,1712,68,0,0,4,1],[1745,13,1745,16,0,0,4,0],[1751,14,1751,23,0,0,4,0],[1760,9,1760,13,0,0,4,0],[1764,13,1764,30,0,0,4,0],[1764,13,1764,30,0,0,4,1],[1771,17,1771,53,0,0,4,1],[1786,9,1786,36,0,0,4,0],[1651,20,1651,29,10,0,4,1]],"gzungetc":[[509,9,509,21,0,0,4,1],[512,9,512,31,0,0,4,1],[516,9,516,27,0,0,4,1],[520,9,520,27,0,0,4,1],[525,9,525,20,0,0,4,1],[529,9,529,14,0,0,4,1],[533,9,533,27,0,0,4,1],[543,9,543,44,0,0,4,1],[549,9,549,36,0,0,4,1]]},"library_calls":["compressBound","compress2","deflateInit_","deflateSetDictionary","deflateGetDictionary","deflatePending","deflateUsed","deflateEnd","inflateBackInit_","inflateBackEnd","gzopen","gzungetc","gzopen","gzputc","gzclose","gzclose"],"critical_calls":["compressBound","compress2","deflateInit_","deflateSetDictionary","deflateGetDictionary","deflatePending","deflateUsed","deflateEnd","inflateBackInit_","inflateBackEnd","gzopen","gzungetc","gzopen","gzputc","gzclose","gzclose"],"visited":0}
/*Here's a comprehensive fuzz driver that uses all the specified zlib APIs to process compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<39) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Variables for compression
    uLongf compressed_len = compressBound(size);
    Bytef *compressed = (Bytef *)malloc(compressed_len);
    Bytef *decompressed = (Bytef *)malloc(size);
    uLongf decompressed_len = size;
    
    if (!compressed || !decompressed) {
        free(compressed);
        free(decompressed);
        return 0;
    }
    
    // 1. Use compress2 to compress the input data
    int level = data[0] % 10;  // Random level from 0-9
    if (compress2(compressed, &compressed_len, data, size, level) != Z_OK) {
        free(compressed);
        free(decompressed);
        return 0;
    }
    
    // Create a temporary file with compressed data for gz operations
    FILE *tmp_file = fopen("compressed.tmp", "wb");
    if (tmp_file) {
        fwrite(compressed, 1, compressed_len, tmp_file);
        assert_file_closed(&tmp_file);;
    }
    
    // 2. Setup deflate stream to use deflateGetDictionary
    z_stream deflate_strm;
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    if (deflateInit(&deflate_strm, level) != Z_OK) {
        free(compressed);
        free(decompressed);
        assert_file_closed(&tmp_file);
	return 0;
    }
    
    // Set a dictionary from input data
    uInt dict_len = size > 256 ? 256 : (uInt)size;
    if (deflateSetDictionary(&deflate_strm, data, dict_len) == Z_OK) {
        // Use deflateGetDictionary
        uInt get_dict_len = 0;
        Bytef *dictionary = (Bytef *)malloc(256);
        if (dictionary) {
            deflateGetDictionary(&deflate_strm, dictionary, &get_dict_len);
            free(dictionary);
        }
    }
    
    // 3. Use deflatePending
    unsigned int pending = 0;
    int bits = 0;
    deflatePending(&deflate_strm, &pending, &bits);
    
    // 4. Use deflateUsed
    int used_bits = 0;
    deflateUsed(&deflate_strm, &used_bits);
    
    // Clean up deflate stream
    deflateEnd(&deflate_strm);
    
    // 5. Setup inflateBack stream (for inflateBackEnd)
    z_stream inflate_strm;
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    inflate_strm.next_in = compressed;
    inflate_strm.avail_in = compressed_len;
    
    // Initialize inflateBack
    unsigned char window[32768];
    if (inflateBackInit(&inflate_strm, 15, window) == Z_OK) {
        // We need callback functions for inflateBack
        // For simplicity, we'll just end it since we need to call inflateBackEnd
        inflateBackEnd(&inflate_strm);
    }
    
    // 6. Use gzputc with a gzFile
    gzFile gz_file = gzopen(fuzz_str_1, fuzz_str_2);
    if (gz_file) {
        // Read one character and put it back
        int c = gzgetc(gz_file);
        if (c >= 0) {
            gzungetc(c, gz_file);
            // Use gzputc (though typically used for writing)
            // We'll create a new gzFile for writing
            gzFile gz_out = gzopen(fuzz_str_3, fuzz_str_4);
            if (gz_out) {
                // Write a single character
                gzputc(gz_out, data[0] & 0xFF);
                gzclose(gz_out);
            }
        }
        gzclose(gz_file);
    }
    
    // Clean up
    free(compressed);
    free(decompressed);
    
    // Remove temporary files
    remove("compressed.tmp");
    remove("output.gz");
    
    assert_file_closed(&tmp_file);
	return 0;
}
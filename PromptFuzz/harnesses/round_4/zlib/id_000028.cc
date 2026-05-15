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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 9
//<Quality> {"density":7,"unique_branches":{"deflateCopy":[[1327,9,1327,34,0,0,4,1],[1327,38,1327,52,0,0,4,1],[1336,9,1336,21,0,0,4,1],[1347,9,1347,29,0,0,4,1],[1347,33,1347,51,0,0,4,1],[1347,55,1347,73,0,0,4,1],[1348,9,1348,34,0,0,4,1],[1355,14,1355,22,0,0,4,1],[1355,26,1355,64,0,0,4,1]]},"library_calls":["gzopen","gzoffset","gzopen","gzputs","gzclose","uncompress","deflateInit_","deflateBound","deflate","deflateCopy","inflateInit_","inflateSyncPoint","inflateEnd","deflateEnd","deflateEnd","gzclose"],"critical_calls":["gzopen","gzoffset","gzopen","gzputs","gzclose","uncompress","deflateInit_","deflateBound","deflate","deflateCopy","inflateInit_","inflateSyncPoint","inflateEnd","deflateEnd","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<67) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a temporary file with the compressed data for gzFile operations
    FILE *temp_file = fopen("input_file", "wb");
    if (!temp_file) {
        assert_file_closed(&temp_file);
	return 0;
    }
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    
    // Open the file as gzFile for reading
    gzFile gz_in = gzopen(fuzz_str_1, fuzz_str_2);
    if (!gz_in) {
        remove("input_file");
        assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Use gzoffset to get current file position (should be 0 initially)
    off_t offset = gzoffset(gz_in);
    
    // Use gzgetc to read first byte
    int first_byte = gzgetc(gz_in);
    
    // Create a gzFile for writing
    gzFile gz_out = gzopen(fuzz_str_3, fuzz_str_4);
    if (gz_out) {
        // Use gzputs to write a test string
        gzputs(gz_out, fuzz_str_5);
        gzclose(gz_out);
    }
    
    // Try to uncompress the input data if it appears to be zlib-compressed
    // We'll attempt decompression into a buffer
    uLongf destLen = size * 2 + 100;  // Reasonable guess for decompressed size
    Bytef *dest = (Bytef *)malloc(destLen);
    if (dest) {
        // Attempt to uncompress
        int uncompress_result = uncompress(dest, &destLen, data, size);
        
        // If decompression succeeded, we can use the decompressed data
        if (uncompress_result == Z_OK) {
            // Create z_stream structures for deflate operations
            z_stream strm1, strm2;
            memset(&strm1, 0, sizeof(z_stream));
            memset(&strm2, 0, sizeof(z_stream));
            
            // Initialize deflate stream
            if (deflateInit(&strm1, Z_DEFAULT_COMPRESSION) == Z_OK) {
                // Setup the source data for compression
                strm1.next_in = dest;
                strm1.avail_in = destLen;
                
                // Create output buffer for compressed data
                uLong comprLen = deflateBound(&strm1, destLen);
                Bytef *compr = (Bytef *)malloc(comprLen);
                
                if (compr) {
                    strm1.next_out = compr;
                    strm1.avail_out = comprLen;
                    
                    // Perform deflate operation
                    deflate(&strm1, fuzz_int32_t_6);
                    
                    // Use deflateCopy to copy the stream state
                    int copy_result = deflateCopy(&strm2, &strm1);
                    
                    if (copy_result == Z_OK) {
                        // Create another inflate stream to use inflateSyncPoint
                        z_stream inflate_strm;
                        memset(&inflate_strm, 0, sizeof(z_stream));
                        
                        if (inflateInit(&inflate_strm) == Z_OK) {
                            // Setup inflate stream with compressed data
                            inflate_strm.next_in = compr;
                            inflate_strm.avail_in = strm1.total_out;
                            
                            // Use inflateSyncPoint to check synchronization
                            int sync_point = inflateSyncPoint(&inflate_strm);
                            
                            // Clean up inflate stream
                            inflateEnd(&inflate_strm);
                        }
                        
                        // Clean up copied deflate stream
                        deflateEnd(&strm2);
                    }
                    
                    free(compr);
                }
                
                // Clean up original deflate stream
                deflateEnd(&strm1);
            }
        }
        
        free(dest);
    }
    
    // Clean up gzFile resources
    gzclose(gz_in);
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&temp_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}
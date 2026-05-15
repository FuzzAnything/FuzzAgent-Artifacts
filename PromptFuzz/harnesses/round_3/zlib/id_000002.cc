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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 78, nr_unique_branch: 24
//<Quality> {"density":13,"unique_branches":{"deflateBound_z":[[876,9,876,32,0,0,4,0],[877,17,877,36,0,0,4,0],[877,17,877,36,0,0,4,1],[878,16,878,34,0,0,4,1]],"deflate":[[1218,18,1218,47,0,0,4,0],[293,64,293,65,36,0,4,0]],"read_buf":[[232,14,232,36,0,0,4,0]],"deflate_huff":[[2160,13,2160,30,0,0,4,0],[2160,13,2160,30,0,0,4,1],[2162,17,2162,34,0,0,4,0],[2162,17,2162,34,0,0,4,1],[2163,21,2163,40,0,0,4,1],[2175,13,2175,19,0,0,4,0],[2175,13,2175,19,0,0,4,1],[2178,9,2178,26,0,0,4,0],[1644,8,1644,31,3,0,4,1],[1644,8,1644,31,5,0,4,1],[1631,24,1631,44,8,0,4,0],[1631,24,1631,44,9,0,4,0]],"inflateSync":[[1274,9,1274,28,0,0,4,0],[1274,32,1274,47,0,0,4,0],[1274,32,1274,47,0,0,4,1],[1282,16,1282,32,0,0,4,0],[1299,9,1299,27,0,0,4,1]]},"library_calls":["uncompress","gzopen64","gzwrite","gzclose","inflateInit2_","inflateUndermine","inflate","inflateSync","inflateReset","inflateEnd","deflateBound_z","deflateInit2_","deflateParams","deflate","deflateEnd","crc32_combine_gen64","crc32","crc32","crc32_combine_op"],"critical_calls":["uncompress","gzopen64","gzwrite","gzclose","inflateInit2_","inflateUndermine","inflate","inflateSync","inflateReset","inflateEnd","deflateBound_z","deflateInit2_","deflateParams","deflate","deflateEnd","crc32_combine_gen64","crc32","crc32","crc32_combine_op"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<57) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_8, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_9, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_10, fdp);
	//fuzzer shim end}




    // 1. First, try to uncompress the input data (assuming it's zlib-compressed)
    // We'll need to allocate output buffer for uncompress
    uLongf uncompressed_size = size * 4; // Initial guess - compressed data expands
    if (uncompressed_size < 1024) uncompressed_size = 1024; // Minimum buffer
    
    Bytef* uncompressed_data = (Bytef*)malloc(uncompressed_size);
    if (!uncompressed_data) {
	return 0;
	}
	
    
    int uncompress_result = uncompress(uncompressed_data, &uncompressed_size, 
                                       data, (uLong)size);
    
    // 2. Create a gzip file and write the uncompressed data to it
    gzFile gz_file = gzopen64("output_file", fuzz_str_1);
    if (gz_file) {
        if (uncompress_result == Z_OK && uncompressed_size > 0) {
            gzwrite(gz_file, uncompressed_data, (unsigned int)uncompressed_size);
        }
        gzclose(gz_file);
    }
    
    // 3. Initialize inflation stream for decompression
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    
    // Allocate buffers for inflation
    Bytef* inflate_in = (Bytef*)malloc(size);
    Bytef* inflate_out = (Bytef*)malloc(size * 2);
    if (!inflate_in || !inflate_out) {
        free(inflate_in);
        free(inflate_out);
        free(uncompressed_data);
        assert_file_name_closed("output_file");
	return 0;
    }
    
    memcpy(inflate_in, data, size);
    
    inflate_stream.next_in = inflate_in;
    inflate_stream.avail_in = (uInt)size;
    inflate_stream.next_out = inflate_out;
    inflate_stream.avail_out = (uInt)(size * 2);
    
    // Initialize inflation with automatic header detection
    if (inflateInit2(&inflate_stream, 15 + 32) == Z_OK) {
        // 4. Try to undermine the inflation (for testing error conditions)
        inflateUndermine(&inflate_stream, fuzz_int32_t_2);
        
        // 5. Attempt inflation
        int inflate_result = inflate(&inflate_stream, fuzz_int32_t_3);
        
        // 6. Try to sync to next compressed block
        inflateSync(&inflate_stream);
        
        // 7. Reset the inflation stream
        inflateReset(&inflate_stream);
        
        // Clean up inflation
        inflateEnd(&inflate_stream);
    }
    
    // 8. Initialize deflation stream for compression
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    // Allocate output buffer for deflation
    z_size_t source_len = uncompressed_size;
    z_size_t deflate_bound = deflateBound_z(&deflate_stream, source_len);
    Bytef* deflate_out = (Bytef*)malloc(deflate_bound);
    
    if (deflate_out && deflateInit2(&deflate_stream, Z_DEFAULT_COMPRESSION, 
                                    Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        deflate_stream.next_in = uncompressed_data;
        deflate_stream.avail_in = (uInt)uncompressed_size;
        deflate_stream.next_out = deflate_out;
        deflate_stream.avail_out = (uInt)deflate_bound;
        
        // 9. Set deflation parameters
        deflateParams(&deflate_stream, fuzz_int32_t_4, fuzz_int32_t_5);
        
        // Perform deflation
        deflate(&deflate_stream, fuzz_int32_t_6);
        
        deflateEnd(&deflate_stream);
    }
    
    // 10. Combine CRC32 values using combine operations
    // Generate combine operation for offset 100
    uLong combine_op = crc32_combine_gen64(100);
    
    // Compute two CRC32 values from different parts of the data
    uLong crc1 = 0;
    uLong crc2 = 0;
    
    if (size > 0) {
        size_t split = size / 2;
        if (split > 0) {
            crc1 = crc32(fuzz_uint64_t_7, data, (uInt)split);
        }
        if (size - split > 0) {
            crc2 = crc32(fuzz_uint64_t_8, data + split, (uInt)(size - split));
        }
        
        // 11. Combine the two CRC32 values
        uLong combined_crc = crc32_combine_op(fuzz_uint64_t_9, fuzz_uint64_t_10, combine_op);
        (void)combined_crc; // Avoid unused variable warning
    }
    
    // 12. Clean up all allocated resources
    free(uncompressed_data);
    free(inflate_in);
    free(inflate_out);
    free(deflate_out);
    
    // Remove the output file we created
    remove("output_file");
    
    assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}
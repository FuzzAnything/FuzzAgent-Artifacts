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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 5.5, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["uncompress","inflateInit_","inflateSync","inflateUndermine","inflateReset","inflateEnd","deflateInit_","deflateBound_z","deflateParams","deflateEnd","gzopen64","gzread","gzclose","crc32","crc32","crc32","crc32","crc32_combine_gen64","crc32_combine_op"],"critical_calls":["uncompress","inflateInit_","inflateSync","inflateUndermine","inflateReset","inflateEnd","deflateInit_","deflateBound_z","deflateParams","deflateEnd","gzopen64","gzread","gzclose","crc32","crc32","crc32","crc32","crc32_combine_gen64","crc32_combine_op"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<33) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. First, try to decompress the input data using uncompress
    // We'll use a reasonable maximum decompressed size
    uLongf destLen = size * 10;  // Estimate decompressed size
    if (destLen < 100) destLen = 100;
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) {
	return 0;
	}
	
    
    // Call uncompress with the input data
    int uncompress_result = uncompress(dest, &destLen, data, (uLong)size);
    
    // 2. Now let's create a z_stream for inflation operations
    z_stream inflate_strm;
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    inflate_strm.avail_in = (uInt)size;
    inflate_strm.next_in = (Bytef *)data;
    
    // Initialize inflation
    if (inflateInit(&inflate_strm) == Z_OK) {
        // 3. Call inflateSync to try to synchronize the stream
        inflateSync(&inflate_strm);
        
        // 4. Call inflateUndermine to test error conditions
        inflateUndermine(&inflate_strm, fuzz_int32_t_1);
        
        // 5. Call inflateReset to reset the inflation stream
        inflateReset(&inflate_strm);
        
        // Clean up inflation stream
        inflateEnd(&inflate_strm);
    }
    
    // 6. Create another z_stream for deflation operations
    z_stream deflate_strm;
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // 7. Call deflateBound_z to get maximum compressed size
        z_size_t max_bound = deflateBound_z(&deflate_strm, (z_size_t)destLen);
        
        // 8. Call deflateParams to change compression parameters
        // We'll use some random but valid parameters
        deflateParams(&deflate_strm, fuzz_int32_t_2, fuzz_int32_t_3);
        
        // Clean up deflation stream
        deflateEnd(&deflate_strm);
    }
    
    // 9. Use gzopen64 to open a gzip file
    // First, write the input data to a file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Create a temporary file for gzip operations
        FILE *tmp_file = fopen("input_file", "wb");
        if (tmp_file) {
            // Copy data to temporary file
            uint8_t buffer[4096];
            size_t bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
                fwrite(buffer, 1, bytes_read, tmp_file);
            }
            assert_file_closed(&tmp_file);;
        }
        assert_file_closed(&in_file);;
        
        // Now open with gzopen64
        gzFile gz = gzopen64("input_file", fuzz_str_4);
        if (gz) {
            // Read a small amount to test
            char read_buf[100];
            gzread(gz, read_buf, sizeof(read_buf));
            gzclose(gz);
        }
        
        // Clean up temporary file
        remove("input_file");
    }
    
    // 10. CRC32 operations
    if (destLen > 0) {
        // Calculate CRC32 of the decompressed data
        uLong crc1 = crc32(fuzz_uint64_t_5, Z_NULL, 0);
        crc1 = crc32(crc1, dest, (uInt)destLen);
        
        // Calculate another CRC32 (simulate second data chunk)
        uLong crc2 = crc32(fuzz_uint64_t_6, Z_NULL, 0);
        if (destLen > 10) {
            crc2 = crc32(crc2, dest + 5, (uInt)(destLen - 5));
        }
        
        // 11. Generate combine operator using crc32_combine_gen64
        uLong op = crc32_combine_gen64((off64_t)destLen);
        
        // 12. Combine the two CRC32 values using crc32_combine_op
        uLong combined_crc = crc32_combine_op(crc1, crc2, op);
        
        // Use the combined CRC (prevent optimization)
        (void)combined_crc;
    }
    
    // Clean up allocated memory
    free(dest);
    
    assert_file_closed(&in_file);
	return 0;
}
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
//<ID> 230
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 8
//<Quality> {"density":7,"unique_branches":{"gzungetc":[[516,9,516,27,0,0,4,1],[520,9,520,27,0,0,4,0],[520,31,520,56,0,0,4,0],[520,31,520,56,0,0,4,1],[520,60,520,73,0,0,4,0],[533,9,533,27,0,0,4,1],[543,9,543,44,0,0,4,1],[549,9,549,36,0,0,4,1]]},"library_calls":["gzopen","gzerror","gzclearerr","gzungetc","deflateInit_","gzclose","deflatePrime","deflateCopy","deflateResetKeep","deflateEnd","deflateEnd","gzopen","gzwrite","gzflush","gzclose","gzclose","uncompress2_z","gzclose"],"critical_calls":["gzopen","gzerror","gzclearerr","gzungetc","deflateInit_","deflatePrime","deflateCopy","deflateResetKeep","deflateEnd","deflateEnd","gzopen","gzwrite","gzflush","gzclose","uncompress2_z","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t* f_data, size_t f_size) {
	if(f_size<14) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Minimum size check for meaningful operations
    if (size < 10) {
	return 0;
	}
	
    
    // Create a temporary file for reading compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 1. Open the compressed data file with gzopen
    gzFile gz_file = gzopen("input_file", fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // Write the compressed data to the file
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
    }
    
    // 2. Check for errors with gzerror
    int errnum = 0;
    const char *error_msg = gzerror(gz_file, &errnum);
    (void)error_msg; // Use the error message to satisfy compiler
    
    // 3. Clear any errors with gzclearerr
    gzclearerr(gz_file);
    
    // 4. Try to read and unget a character
    int c = gzgetc(gz_file);
    if (c != -1) {
        // 5. Use gzungetc to push back the character
        gzungetc(c, gz_file);
    }
    
    // 6. Create a deflate stream for testing deflate operations
    z_stream strm1, strm2;
    memset(&strm1, 0, sizeof(strm1));
    memset(&strm2, 0, sizeof(strm2));
    
    // Initialize deflate stream
    if (deflateInit(&strm1, Z_DEFAULT_COMPRESSION) != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Use deflatePrime to insert bits
    // Use a few bits from the input data
    int bits = data[0] & 0x07; // Use first 3 bits (0-7)
    int value = data[1] & 0xFF; // Use next byte as value
    deflatePrime(&strm1, bits, value);
    
    // 8. Use deflateCopy to copy the stream state
    if (deflateCopy(&strm2, &strm1) == Z_OK) {
        // 9. Use deflateResetKeep on the original stream
        deflateResetKeep(&strm1);
        
        // Clean up the copied stream
        deflateEnd(&strm2);
    }
    
    // Clean up the original deflate stream
    deflateEnd(&strm1);
    
    // 10. Use gzflush (on a different file opened for writing)
    gzFile gz_out = gzopen("output_file", fuzz_str_2);
    if (gz_out) {
        // Write some data
        gzwrite(gz_out, data, size > 100 ? 100 : (unsigned)size);
        // 11. Use gzflush with Z_SYNC_FLUSH
        gzflush(gz_out, fuzz_int32_t_3);
        gzclose(gz_out);
    }
    
    // 12. Use uncompress2_z to decompress the input data
    // We need to allocate buffers for decompression
    const size_t max_uncompressed_size = size * 10; // Estimate
    Bytef* dest = (Bytef*)malloc(max_uncompressed_size);
    if (!dest) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    z_size_t destLen = max_uncompressed_size;
    z_size_t sourceLen = size;
    
    // Note: uncompress2_z expects compressed data, but our input might not be
    // properly compressed. We'll try anyway - it may fail gracefully.
    int uncompress_result = uncompress2_z(dest, &destLen, 
                                         (const Bytef*)data, &sourceLen);
    
    // Use the result to avoid compiler warnings
    (void)uncompress_result;
    
    // Clean up
    free(dest);
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}
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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["gzdopen","gzgetc_","gzclose","gzeof","gzclose","gzoffset64","gzclose","uncompress_z","uncompress","deflateInit_","deflateResetKeep","deflateReset","deflateEnd","adler32","adler32","adler32","adler32","adler32_combine64"],"critical_calls":["gzdopen","gzgetc_","gzeof","gzoffset64","gzclose","uncompress_z","uncompress","deflateInit_","deflateResetKeep","deflateReset","deflateEnd","adler32","adler32","adler32","adler32","adler32_combine64"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
	//fuzzer shim end}




    // 1. Create input file from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Open compressed file using gzopen64
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Duplicate file descriptor since gzdopen will close it
    int fd2 = dup(fd);
    if (fd2 < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    gzFile gz = gzdopen(fd2, fuzz_str_1);
    if (!gz) {
        close(fd2);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Read compressed data until EOF using gzgetc_
    Bytef *compressed_buffer = nullptr;
    size_t compressed_size = 0;
    int c;
    
    while ((c = gzgetc_(gz)) != EOF) {
        Bytef *new_buffer = (Bytef *)realloc(compressed_buffer, compressed_size + 1);
        if (!new_buffer) {
            free(compressed_buffer);
            gzclose(gz);
            assert_file_closed(&in_file);;
            assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
        }
        compressed_buffer = new_buffer;
        compressed_buffer[compressed_size++] = (Bytef)c;
    }
    
    // 4. Check if we reached EOF
    if (gzeof(gz) != 1) {
        // There was an error reading
        free(compressed_buffer);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Get file offset using gzoffset64
    off64_t offset = gzoffset64(gz);
    (void)offset; // Use offset to avoid unused variable warning
    
    // 6. Close the gz file
    gzclose(gz);
    assert_file_closed(&in_file);;
    
    if (!compressed_buffer || compressed_size == 0) {
        free(compressed_buffer);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 7. Try to uncompress using uncompress_z (z_size_t version)
    z_size_t uncompressed_size_z = compressed_size * 10; // Initial guess
    Bytef *uncompressed_buffer_z = (Bytef *)malloc(uncompressed_size_z);
    if (uncompressed_buffer_z) {
        int ret_z = uncompress_z(uncompressed_buffer_z, &uncompressed_size_z, 
                               compressed_buffer, compressed_size);
        free(uncompressed_buffer_z);
    }
    
    // 8. Try to uncompress using uncompress (uLong version)
    uLongf uncompressed_size = compressed_size * 10;
    Bytef *uncompressed_buffer = (Bytef *)malloc(uncompressed_size);
    if (uncompressed_buffer) {
        int ret = uncompress(uncompressed_buffer, &uncompressed_size,
                           compressed_buffer, compressed_size);
        free(uncompressed_buffer);
    }
    
    // 9. Create z_stream for deflation operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize deflate with default parameters
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // 10. Use deflateResetKeep
        deflateResetKeep(&strm);
        
        // 11. Use deflateReset
        deflateReset(&strm);
        
        // Clean up
        deflateEnd(&strm);
    }
    
    // 12. Compute Adler-32 checksum for two buffers and combine them
    if (compressed_size > 10) {
        size_t split = compressed_size / 2;
        
        // Compute checksum for first half
        uLong adler1 = adler32(fuzz_uint64_t_2, Z_NULL, 0);
        adler1 = adler32(adler1, compressed_buffer, split);
        
        // Compute checksum for second half
        uLong adler2 = adler32(fuzz_uint64_t_3, Z_NULL, 0);
        adler2 = adler32(adler2, compressed_buffer + split, compressed_size - split);
        
        // Combine checksums using adler32_combine64
        uLong combined = adler32_combine64(adler1, adler2, compressed_size - split);
        (void)combined; // Use to avoid unused variable warning
    }
    
    // 13. Clean up all allocated resources
    free(compressed_buffer);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
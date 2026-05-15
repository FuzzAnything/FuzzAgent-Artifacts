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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["gzdopen","gzeof","gzgetc_","gzoffset64","uncompress_z","uncompress","deflateInit_","deflateReset","deflateResetKeep","deflateEnd","adler32","adler32","adler32","adler32","adler32_combine64","gzclose"],"critical_calls":["gzdopen","gzeof","gzgetc_","gzoffset64","uncompress_z","uncompress","deflateInit_","deflateReset","deflateResetKeep","deflateEnd","adler32","adler32","adler32","adler32","adler32_combine64","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<37) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
	//fuzzer shim end}




    // Early return if no data
    if (size == 0) {
	return 0;
	}
	
    
    // 1. Create a memory file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzdopen(fd, fuzz_str_1);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Read compressed data byte by byte using gzgetc_
    int bytes_read = 0;
    while (!gzeof(gz) && bytes_read < 10000) {  // Limit to prevent infinite loops
        int ch = gzgetc_(gz);
        if (ch == -1) break;
        bytes_read++;
    }
    
    // 4. Get current offset in gz file
    off64_t offset = gzoffset64(gz);
    
    // 5. Prepare for decompression using uncompress APIs
    // First, try to uncompress with uncompress_z
    Bytef *dest_z = nullptr;
    z_size_t destLen_z = 0;
    int result_z = Z_BUF_ERROR;
    
    // Estimate decompressed size (use compressed size * 4 as initial guess)
    destLen_z = size * 4;
    if (destLen_z > 0 && destLen_z < 10*1024*1024) {  // Limit to 10MB
        dest_z = (Bytef *)malloc(destLen_z);
        if (dest_z) {
            result_z = uncompress_z(dest_z, &destLen_z, data, (z_size_t)size);
        }
    }
    
    // 6. Also try with regular uncompress
    Bytef *dest = nullptr;
    uLongf destLen = 0;
    int result = Z_BUF_ERROR;
    
    destLen = size * 4;
    if (destLen > 0 && destLen < 10*1024*1024) {  // Limit to 10MB
        dest = (Bytef *)malloc(destLen);
        if (dest) {
            result = uncompress(dest, &destLen, data, (uLong)size);
        }
    }
    
    // 7. Set up z_stream for compression (to test deflateReset/deflateResetKeep)
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    int init_result = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (init_result == Z_OK) {
        // 8. Test deflateReset
        int reset_result = deflateReset(&strm);
        
        // 9. Test deflateResetKeep
        int reset_keep_result = deflateResetKeep(&strm);
        
        // Clean up deflate
        deflateEnd(&strm);
    }
    
    // 10. Compute Adler-32 checksums and combine them
    uLong adler1 = adler32(fuzz_uint64_t_2, Z_NULL, 0);
    uLong adler2 = adler32(fuzz_uint64_t_3, Z_NULL, 0);
    
    if (dest && result == Z_OK && destLen > 0) {
        // Compute checksum for first half
        size_t half = destLen / 2;
        if (half > 0) {
            adler1 = adler32(fuzz_uint64_t_4, dest, (uInt)half);
        }
        
        // Compute checksum for second half
        if (destLen - half > 0) {
            adler2 = adler32(fuzz_uint64_t_5, dest + half, (uInt)(destLen - half));
        }
        
        // Combine the checksums
        uLong combined = adler32_combine64(adler1, adler2, (off64_t)destLen);
        (void)combined;  // Use to avoid unused variable warning
    }
    
    // 11. Clean up
    if (dest_z) free(dest_z);
    if (dest) free(dest);
    
    // Close gz file (this also closes the underlying file descriptor)
    gzclose(gz);
    // Note: assert_file_closed(&in_file); is not needed here because gzclose closes the fd
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
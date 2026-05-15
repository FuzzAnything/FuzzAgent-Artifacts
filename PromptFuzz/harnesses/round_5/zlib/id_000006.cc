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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["gzdopen","gzread","inflateInit_","gzclose","inflateMark","uncompress","uncompress","adler32","adler32","adler32","adler32","adler32_combine64","deflateInit_","deflateCopy","deflateEnd","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzread","inflateInit_","inflateMark","uncompress","uncompress","adler32","adler32","adler32","adler32","adler32_combine64","deflateInit_","deflateCopy","deflateEnd","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 1. Use gzdopen to create a gzFile from file descriptor
    gzFile gz = gzdopen(fd, "rb");
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Read some data through gzFile to consume input
    char buffer[1024];
    int bytes_read = gzread(gz, buffer, sizeof(buffer));
    
    // 2. Use inflateMark on a z_stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    // Initialize inflate stream
    if (inflateInit(&strm) != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set up input data for inflate
    strm.avail_in = (uInt)(size > 1024 ? 1024 : size);
    strm.next_in = (Bytef *)data;
    
    // Call inflateMark to get current position
    long mark = inflateMark(&strm);
    (void)mark; // Mark may be -1 if not at boundary, but we called the API
    
    // 3. Try to uncompress the input data (may fail if not valid zlib data)
    Bytef *uncompressed = nullptr;
    uLongf uncompressed_len = 0;
    
    // First call to get required buffer size
    int uncompress_result = uncompress(nullptr, &uncompressed_len, data, 
                                      (uLong)(size > 10000 ? 10000 : size));
    
    if (uncompress_result == Z_BUF_ERROR || uncompress_result == Z_OK) {
        // Allocate buffer and try again
        uncompressed = (Bytef *)malloc(uncompressed_len + 1024); // Extra padding
        if (uncompressed) {
            uLongf dest_len = uncompressed_len;
            // This may fail if input isn't valid compressed data
            uncompress(uncompressed, &dest_len, data, 
                      (uLong)(size > 10000 ? 10000 : size));
            
            // 4. Use adler32_combine64 with computed checksums
            if (dest_len > 0) {
                // Compute Adler-32 for two halves of uncompressed data
                uLong adler1 = adler32(0L, Z_NULL, 0);
                uLong adler2 = adler32(0L, Z_NULL, 0);
                
                size_t half1_len = dest_len / 2;
                size_t half2_len = dest_len - half1_len;
                
                if (half1_len > 0) {
                    adler1 = adler32(adler1, uncompressed, (uInt)half1_len);
                }
                if (half2_len > 0) {
                    adler2 = adler32(adler2, uncompressed + half1_len, (uInt)half2_len);
                }
                
                // Combine the two checksums
                uLong combined = adler32_combine64(adler1, adler2, (off64_t)dest_len);
                (void)combined; // Use the result
            }
            
            free(uncompressed);
        }
    }

    // 5. Use deflateCopy between two z_streams
    z_stream deflate_strm1, deflate_strm2;
    deflate_strm1.zalloc = Z_NULL;
    deflate_strm1.zfree = Z_NULL;
    deflate_strm1.opaque = Z_NULL;
    
    deflate_strm2.zalloc = Z_NULL;
    deflate_strm2.zfree = Z_NULL;
    deflate_strm2.opaque = Z_NULL;
    
    // Initialize first deflate stream
    if (deflateInit(&deflate_strm1, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Copy the stream state
        int copy_result = deflateCopy(&deflate_strm2, &deflate_strm1);
        (void)copy_result; // Result may be Z_OK or error
        
        // Clean up both streams
        deflateEnd(&deflate_strm1);
        if (copy_result == Z_OK) {
            deflateEnd(&deflate_strm2);
        }
    }

    // Clean up inflate stream
    inflateEnd(&strm);
    
    // Clean up gzFile and FILE*
    gzclose(gz);
    // Note: gzclose already closes the underlying fd, so we don't need to fclose separately
    // But we should still close the FILE* if gzdopen failed earlier
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
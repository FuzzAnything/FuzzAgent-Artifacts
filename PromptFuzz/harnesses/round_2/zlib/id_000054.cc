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
//<ID> 135
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["uncompress_z","inflateInit2_","inflateValidate","inflate","inflateSyncPoint","inflateResetKeep","inflateEnd","deflateInit_","deflate","deflatePending","deflateEnd","gzdopen","gzoffset64","gzungetc","gzclose","gzdopen","gzclose","gzputc","adler32","adler32","adler32","adler32","adler32_combine","gzclose","gzclose"],"critical_calls":["uncompress_z","inflateInit2_","inflateValidate","inflate","inflateSyncPoint","inflateResetKeep","inflateEnd","deflateInit_","deflate","deflatePending","deflateEnd","gzdopen","gzoffset64","gzungetc","gzdopen","gzputc","adler32","adler32","adler32","adler32","adler32_combine","gzclose","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Prepare input data as zlib compressed stream
    if (size < 2) {
	return 0;
	}
	 // Need minimal data
    
    // 2. First attempt: uncompress with uncompress_z
    Bytef* uncompressed = nullptr;
    z_size_t uncompressed_len = size * 10; // Initial guess
    Bytef* compressed_data = nullptr;
    z_size_t compressed_len = 0;
    
    // We'll treat the input as compressed data
    compressed_data = (Bytef*)malloc(size);
    if (!compressed_data) {
	return 0;
	}
	
    memcpy(compressed_data, data, size);
    compressed_len = size;
    
    // Try to uncompress
    uncompressed = (Bytef*)malloc(uncompressed_len);
    if (!uncompressed) {
        free(compressed_data);
        return 0;
    }
    
    int uncompress_result = uncompress_z(uncompressed, &uncompressed_len, 
                                         compressed_data, compressed_len);
    // Don't fail on decompression errors - input might not be valid zlib data
    
    // 3. Setup inflation stream for stream validation
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    
    // Initialize inflation
    if (inflateInit2(&inflate_stream, MAX_WBITS) != Z_OK) {
        free(uncompressed);
        free(compressed_data);
        return 0;
    }
    
    // Set input for inflation
    inflate_stream.next_in = compressed_data;
    inflate_stream.avail_in = (uInt)compressed_len;
    
    // Setup output buffer
    Bytef inflate_out[4096];
    inflate_stream.next_out = inflate_out;
    inflate_stream.avail_out = sizeof(inflate_out);
    
    // Use inflateValidate - turn validation on (1) or off (0) based on input
    int validate_param = (data[0] & 1) ? 1 : 0;
    inflateValidate(&inflate_stream, validate_param);
    
    // Perform partial inflation
    int inflate_result = inflate(&inflate_stream, Z_NO_FLUSH);
    
    // Check synchronization point
    int sync_point = inflateSyncPoint(&inflate_stream);
    (void)sync_point; // Use variable
    
    // Reset while keeping dictionary if any
    inflateResetKeep(&inflate_stream);
    
    // Clean up inflation
    inflateEnd(&inflate_stream);
    
    // 4. Setup deflation stream for pending bits check
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    if (deflateInit(&deflate_stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(uncompressed);
        free(compressed_data);
        return 0;
    }
    
    // Compress some test data
    const char* test_str = "Test data for compression";
    deflate_stream.next_in = (Bytef*)test_str;
    deflate_stream.avail_in = (uInt)strlen(test_str);
    
    Bytef deflate_out[4096];
    deflate_stream.next_out = deflate_out;
    deflate_stream.avail_out = sizeof(deflate_out);
    
    deflate(&deflate_stream, Z_FINISH);
    
    // Check pending bits
    unsigned int pending = 0;
    int bits = 0;
    deflatePending(&deflate_stream, &pending, &bits);
    
    deflateEnd(&deflate_stream);
    
    // 5. Process with gzFile APIs
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        free(uncompressed);
        free(compressed_data);
        assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        free(uncompressed);
        free(compressed_data);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Use gzoffset64
    off64_t offset = gzoffset64(gz_in);
    (void)offset; // Use variable
    
    // Try to read a character and push it back
    int c = gzgetc(gz_in);
    if (c != -1) {
        gzungetc(c, gz_in);
    }
    
    // 6. Create output gzFile for writing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in); // This closes the underlying fd, so we don't double-close
        free(uncompressed);
        free(compressed_data);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int out_fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        assert_file_closed(&out_file);;
        gzclose(gz_in);
        free(uncompressed);
        free(compressed_data);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Write a character
    gzputc(gz_out, 'A');
    
    // 7. Calculate Adler-32 checksums and combine them
    if (uncompressed && uncompressed_len > 0) {
        // Calculate Adler-32 for first half
        uLong adler1 = adler32(0L, Z_NULL, 0);
        size_t half1_len = uncompressed_len / 2;
        if (half1_len > 0) {
            adler1 = adler32(adler1, uncompressed, (uInt)half1_len);
        }
        
        // Calculate Adler-32 for second half
        uLong adler2 = adler32(0L, Z_NULL, 0);
        size_t half2_len = uncompressed_len - half1_len;
        if (half2_len > 0) {
            adler2 = adler32(adler2, uncompressed + half1_len, (uInt)half2_len);
        }
        
        // Combine the checksums
        uLong combined = adler32_combine(adler1, adler2, (off_t)half2_len);
        (void)combined; // Use variable
    }
    
    // 8. Cleanup
    gzclose(gz_out); // This closes the underlying file descriptor
    // Don't assert_file_closed(&out_file); because gzclose already closed it
    
    gzclose(gz_in); // This closes the underlying file descriptor
    // Don't assert_file_closed(&in_file); because gzclose already closed it
    
    free(uncompressed);
    free(compressed_data);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(fd);
	return 0;
}
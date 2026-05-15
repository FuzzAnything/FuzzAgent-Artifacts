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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["zlibCompileFlags","gzdopen","gzbuffer","gzclose","gzread","gzerror","gzclose","deflateInit_","deflateTune","inflateInit_","inflateCopy","inflateEnd","inflateEnd","inflateInit_","inflateValidate","inflateEnd","deflateUsed","deflate","zError","deflateUsed","deflate","zError","deflateEnd"],"critical_calls":["zlibCompileFlags","gzdopen","gzbuffer","gzread","gzerror","gzclose","deflateInit_","deflateTune","inflateInit_","inflateCopy","inflateEnd","inflateEnd","inflateInit_","inflateValidate","inflateEnd","deflateUsed","deflate","zError","deflateUsed","deflate","zError","deflateEnd"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve the event of decompressing, analyzing, and recompressing zlib data with validation and tuning:

*/


// Define the maximum buffer sizes for safety
#define MAX_BUFFER_SIZE 1024 * 1024  // 1MB
#define CHUNK_SIZE 16384

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have enough data for meaningful processing
    if (size < 10) {
        return 0;
    }
    
    // Limit input size to prevent excessive memory usage
    if (size > MAX_BUFFER_SIZE) {
        size = MAX_BUFFER_SIZE;
    }
    
    // 1. Get zlib compile flags (API #1: uLong zlibCompileFlags())
    uLong compile_flags = zlibCompileFlags();
    // Use the flags to avoid unused variable warning
    (void)compile_flags;
    
    // 2. Create FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get file descriptor and create gzFile for reading
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 4. Set buffer size for gzFile (API #2: int gzbuffer(gzFile file, unsigned int size))
    // Use a random buffer size based on input data
    unsigned int buffer_size = (data[0] % 256 + 1) * 1024;
    if (buffer_size > 256 * 1024) buffer_size = 256 * 1024;
    gzbuffer(gz_in, buffer_size);
    
    // 5. Create output FILE for decompressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        gzclose(gz_in);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Decompress the input data
    char decompress_buffer[CHUNK_SIZE];
    int decompressed_bytes = 0;
    
    while (true) {
        int bytes_read = gzread(gz_in, decompress_buffer, CHUNK_SIZE);
        if (bytes_read < 0) {
            // Get error message (API #3: const char *zError(int))
            int errnum;
            const char *error_msg = gzerror(gz_in, &errnum);
            (void)error_msg;  // Use to avoid unused warning
            break;
        }
        if (bytes_read == 0) {
            break;
        }
        
        // Write decompressed data to output file
        fwrite(decompress_buffer, 1, bytes_read, out_file);
        decompressed_bytes += bytes_read;
    }
    
    // 7. Close files before re-opening for recompression
    fflush(out_file);
    assert_file_closed(&out_file);;
    gzclose(gz_in);
    
    // Re-open output file for reading
    FILE *recompress_in = fopen("output_file", "rb");
    if (!recompress_in) {
        assert_file_closed(&recompress_in);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 8. Create z_stream for recompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize deflate stream
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        assert_file_closed(&recompress_in);;
        assert_file_closed(&recompress_in);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 9. Apply tuning parameters (API #4: int deflateTune(z_streamp strm, int good_length, int max_lazy, int nice_length, int max_chain))
    // Use values derived from input data
    int good_length = (data[1] % 8) + 8;
    int max_lazy = (data[2] % 32) + 8;
    int nice_length = (data[3] % 32) + 8;
    int max_chain = (data[4] % 4096) + 4;
    
    deflateTune(&strm, good_length, max_lazy, nice_length, max_chain);
    
    // 10. Create a copy of the stream state (API #5: int inflateCopy(z_streamp dest, z_streamp source))
    // We'll create an inflate stream first to demonstrate inflateCopy
    z_stream inflate_strm, inflate_strm_copy;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    memset(&inflate_strm_copy, 0, sizeof(inflate_strm_copy));
    
    // Initialize inflate stream
    if (inflateInit(&inflate_strm) == Z_OK) {
        // Copy the inflate stream state
        inflateCopy(&inflate_strm_copy, &inflate_strm);
        
        // Clean up the copied stream
        inflateEnd(&inflate_strm_copy);
        inflateEnd(&inflate_strm);
    }
    
    // 11. Enable validation during inflation (API #6: int inflateValidate(z_streamp, int))
    // Create another inflate stream for validation
    z_stream validate_strm;
    memset(&validate_strm, 0, sizeof(validate_strm));
    
    if (inflateInit(&validate_strm) == Z_OK) {
        // Enable validation (1 = enable, 0 = disable)
        int validate_flag = (data[5] % 2);
        inflateValidate(&validate_strm, validate_flag);
        
        // Clean up
        inflateEnd(&validate_strm);
    }
    
    // 12. Check bits used in deflate stream (API #7: int deflateUsed(z_streamp strm, int *bits))
    int bits_used = 0;
    deflateUsed(&strm, &bits_used);
    
    // 13. Recompress the decompressed data
    unsigned char compress_buffer[CHUNK_SIZE];
    unsigned char out_buffer[CHUNK_SIZE];
    
    strm.avail_out = CHUNK_SIZE;
    strm.next_out = out_buffer;
    
    while (!feof(recompress_in) && !ferror(recompress_in)) {
        size_t bytes_read = fread(compress_buffer, 1, CHUNK_SIZE, recompress_in);
        if (bytes_read == 0) {
            break;
        }
        
        strm.avail_in = (uInt)bytes_read;
        strm.next_in = compress_buffer;
        
        // Compress the data
        int ret = deflate(&strm, Z_NO_FLUSH);
        if (ret != Z_OK) {
            // Use zError to get error message
            const char *err_msg = zError(ret);
            (void)err_msg;  // Use to avoid unused warning
            break;
        }
        
        // Check bits used again after compression
        deflateUsed(&strm, &bits_used);
    }
    
    // Finish compression
    strm.avail_in = 0;
    int ret = deflate(&strm, Z_FINISH);
    
    // Get final error status if any
    if (ret != Z_STREAM_END && ret != Z_OK) {
        const char *err_msg = zError(ret);
        (void)err_msg;
    }
    
    // 14. Clean up all resources
    deflateEnd(&strm);
    assert_file_closed(&recompress_in);;
    
    // Remove the temporary output file
    remove("output_file");
    
    assert_file_closed(&recompress_in);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_in);
	return 0;
}
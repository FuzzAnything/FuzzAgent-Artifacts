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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 4
//<Quality> {"density":7,"unique_branches":{"inflate":[[542,29,542,47,0,0,4,0]],"inflateReset2":[[160,9,160,19,0,0,4,1],[160,24,160,38,0,0,4,0]],"inflateInit2_":[[207,9,207,20,0,0,4,0]]},"library_calls":["zlibCompileFlags","get_crc_table","gzdopen","gztell","compressBound","gzclose","uncompress_z","inflateInit2_","gzclose","inflateValidate","inflate","inflateResetKeep","inflateCopy","adler32","adler32","adler32","adler32","adler32_combine","inflateEnd","inflateEnd","deflateInit_","gzclose","deflateSetHeader","deflate","deflatePending","deflateEnd","gzclose"],"critical_calls":["zlibCompileFlags","get_crc_table","gzdopen","gztell","compressBound","uncompress_z","inflateInit2_","inflateValidate","inflate","inflateResetKeep","inflateCopy","adler32","adler32","adler32","adler32","adler32_combine","inflateEnd","inflateEnd","deflateInit_","deflateSetHeader","deflate","deflatePending","deflateEnd","gzclose"],"visited":1}
/*Looking at these APIs, I can see they cover both compression and decompression operations in zlib, along with utility functions. The event that could be achieved is: **"Decompressing zlib-compressed data, validating it, optionally re-compressing it, and performing checksum validation with gzip file operations"**.

Here's a fuzz driver that uses all the specified APIs:

*/


extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data to work with
    
    // 1. Get zlib compilation flags
    uLong compile_flags = zlibCompileFlags();
    (void)compile_flags;  // Use the value to avoid unused variable warning
    
    // 2. Get CRC table (just for demonstration, doesn't consume input)
    const z_crc_t *crc_table = get_crc_table();
    (void)crc_table;
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create gzFile from file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz_input = gzdopen(fd, "rb");
    if (!gz_input) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Get current position in gzFile
    off_t pos = gztell(gz_input);
    (void)pos;
    
    // Allocate buffers for compression/decompression
    const size_t max_compressed_size = compressBound(size);
    Bytef *compressed_buf = (Bytef *)malloc(max_compressed_size);
    Bytef *decompressed_buf = (Bytef *)malloc(size * 2);  // Extra space
    
    if (!compressed_buf || !decompressed_buf) {
        free(compressed_buf);
        free(decompressed_buf);
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 5. Try to decompress using uncompress_z (non-streaming API)
    z_size_t dest_len = size * 2;
    int uncompress_result = uncompress_z(decompressed_buf, &dest_len, 
                                        (const Bytef *)data, size);
    (void)uncompress_result;  // Result might be error, but that's OK for fuzzing
    
    // 6. Now use streaming API for more controlled decompression
    z_stream decomp_stream;
    memset(&decomp_stream, 0, sizeof(z_stream));
    
    // 7. Initialize decompression with specific window bits
    int window_bits = 15;  // Default window size
    if (size > 20) {
        window_bits = data[0] % 10 + 8;  // Vary window bits based on input
    }
    
    int init_result = inflateInit2_(&decomp_stream, window_bits, 
                                   ZLIB_VERSION, sizeof(z_stream));
    if (init_result != Z_OK) {
        free(compressed_buf);
        free(decompressed_buf);
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 8. Set validation mode based on input
    int validate_mode = 0;
    if (size > 5) {
        validate_mode = data[1] % 2;
    }
    inflateValidate(&decomp_stream, validate_mode);
    
    // 9. Prepare input for decompression
    decomp_stream.next_in = (Bytef *)data;
    decomp_stream.avail_in = (uInt)size;
    decomp_stream.next_out = decompressed_buf;
    decomp_stream.avail_out = (uInt)(size * 2);
    
    // 10. Perform decompression
    int inflate_result = inflate(&decomp_stream, Z_FINISH);
    (void)inflate_result;
    
    // 11. Reset decompression stream while keeping internal state
    inflateResetKeep(&decomp_stream);
    
    // 12. Create a copy of the decompression stream
    z_stream decomp_stream_copy;
    memset(&decomp_stream_copy, 0, sizeof(z_stream));
    int copy_result = inflateCopy(&decomp_stream_copy, &decomp_stream);
    (void)copy_result;
    
    // 13. Calculate Adler-32 checksum of decompressed data
    uLong adler1 = adler32(0L, Z_NULL, 0);
    if (decomp_stream.total_out > 0) {
        // Use first half of decompressed data for checksum
        uInt len = decomp_stream.total_out > 100 ? 100 : (uInt)decomp_stream.total_out;
        adler1 = adler32(adler1, decompressed_buf, len);
    }
    
    // Calculate another Adler-32 for second half (if available)
    uLong adler2 = 0;
    if (decomp_stream.total_out > 100) {
        uInt start = 100;
        uInt len = decomp_stream.total_out - start > 100 ? 100 : 
                   (uInt)decomp_stream.total_out - start;
        adler2 = adler32(0L, Z_NULL, 0);
        adler2 = adler32(adler2, decompressed_buf + start, len);
    }
    
    // 14. Combine Adler-32 checksums
    uLong combined_adler = adler32_combine(adler1, adler2, 50);
    (void)combined_adler;
    
    // 15. End decompression streams
    inflateEnd(&decomp_stream);
    inflateEnd(&decomp_stream_copy);
    
    // 16. Now try compression using streaming API
    z_stream comp_stream;
    memset(&comp_stream, 0, sizeof(z_stream));
    
    // 17. Initialize compression
    int level = Z_DEFAULT_COMPRESSION;
    if (size > 10) {
        level = data[2] % 10;  // Vary compression level
    }
    
    int deflate_init_result = deflateInit_(&comp_stream, level, 
                                          ZLIB_VERSION, sizeof(z_stream));
    if (deflate_init_result != Z_OK) {
        free(compressed_buf);
        free(decompressed_buf);
        gzclose(gz_input);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 18. Set gzip header if we have enough data
    gz_header header;
    memset(&header, 0, sizeof(header));
    if (size > 50) {
        header.text = data[3] % 2;
        header.time = (uLong)(data[4] << 24 | data[5] << 16 | data[6] << 8 | data[7]);
        header.os = data[8] % 256;
        deflateSetHeader(&comp_stream, &header);
    }
    
    // 19. Prepare compression
    comp_stream.next_in = decompressed_buf;
    comp_stream.avail_in = (uInt)(decomp_stream.total_out > size ? size : decomp_stream.total_out);
    comp_stream.next_out = compressed_buf;
    comp_stream.avail_out = (uInt)max_compressed_size;
    
    // 20. Perform compression
    int deflate_result = deflate(&comp_stream, Z_FINISH);
    (void)deflate_result;
    
    // 21. Check for pending data in compression stream
    unsigned int pending;
    int bits;
    deflatePending(&comp_stream, &pending, &bits);
    (void)pending;
    (void)bits;
    
    // 22. End compression
    deflateEnd(&comp_stream);
    
    // Write some output to file
    if (out_file && comp_stream.total_out > 0) {
        fwrite(compressed_buf, 1, comp_stream.total_out, out_file);
        fflush(out_file);
    }
    
    // 23. Close gzFile
    gzclose(gz_input);
    
    // Clean up
    free(compressed_buf);
    free(decompressed_buf);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
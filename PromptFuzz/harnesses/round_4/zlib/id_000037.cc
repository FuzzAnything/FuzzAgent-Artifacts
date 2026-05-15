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
//<ID> 77
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["inflateInit_","inflateEnd","inflate","inflateEnd","inflateEnd","gzdopen","gzbuffer","gzeof","gzoffset","gzclose","adler32_z","compressBound","compress2","deflateInit_","deflateTune","deflateEnd","inflateInit_","inflateCopy","inflateEnd","inflateEnd","crc32_combine_gen","inflateBackInit_","inflateBackEnd"],"critical_calls":["inflateInit_","inflate","inflateEnd","gzdopen","gzbuffer","gzeof","gzoffset","gzclose","adler32_z","compressBound","compress2","deflateInit_","deflateTune","deflateEnd","inflateInit_","inflateCopy","inflateEnd","inflateEnd","crc32_combine_gen","inflateBackInit_","inflateBackEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<49) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	//fuzzer shim end}




    // 1. First, we need to decompress the input (which is zlib output data)
    // Since the input is compressed data, we'll use inflate functions
    
    z_stream strm;
    Bytef *decompressed = nullptr;
    uLongf decompressed_size = 0;
    int ret = Z_OK;
    
    // Initialize zlib stream for decompression
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    ret = inflateInit(&strm);
    if (ret != Z_OK) {
        return 0;
    }
    
    // Decompress the input data
    strm.avail_in = static_cast<uInt>(size);
    strm.next_in = const_cast<Bytef*>(data);
    
    // Estimate decompressed size (worst case: 10x compression)
    decompressed_size = size * 10;
    decompressed = static_cast<Bytef*>(malloc(decompressed_size));
    if (!decompressed) {
        inflateEnd(&strm);
        return 0;
    }
    
    strm.avail_out = decompressed_size;
    strm.next_out = decompressed;
    
    ret = inflate(&strm, fuzz_int32_t_1);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        free(decompressed);
        inflateEnd(&strm);
        return 0;
    }
    
    decompressed_size = strm.total_out;
    inflateEnd(&strm);
    
    // 2. Now we have decompressed data, let's use the required APIs
    
    // Create a FILE* from the decompressed data for gz functions
    FILE *in_file = fmemopen(decompressed, decompressed_size, "rb");
    if (!in_file) {
        free(decompressed);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create gzFile from file descriptor
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzdopen(fd, fuzz_str_2);
    if (!gz) {
        assert_file_closed(&in_file);;
        free(decompressed);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Use gzbuffer API
    gzbuffer(gz, fuzz_uint32_t_3);  // Set buffer size to 8KB
    
    // Check if we're at EOF (gzeof)
    int at_eof = gzeof(gz);
    
    // Get current offset (gzoffset)
    off_t offset = gzoffset(gz);
    
    // Close gz file
    gzclose(gz);
    assert_file_closed(&in_file);;
    
    // 3. Now work with compression APIs
    
    // Calculate Adler-32 checksum of decompressed data
    uLong adler = adler32_z(fuzz_uint64_t_4, decompressed, fuzz_uint64_t_5);
    
    // Compress the decompressed data using compress2
    uLongf compressed_size = compressBound(decompressed_size);
    Bytef *compressed = static_cast<Bytef*>(malloc(compressed_size));
    if (!compressed) {
        free(decompressed);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int level = 6;  // Default compression level
    ret = compress2(compressed, &compressed_size, decompressed, decompressed_size, fuzz_int32_t_6);
    
    // 4. Create streams for deflateTune and inflateCopy
    
    z_stream deflate_strm;
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    ret = deflateInit(&deflate_strm, level);
    if (ret == Z_OK) {
        // Use deflateTune API
        deflateTune(&deflate_strm, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9, fuzz_int32_t_10);
        deflateEnd(&deflate_strm);
    }
    
    // Create inflate stream for inflateCopy
    z_stream inflate_strm1, inflate_strm2;
    inflate_strm1.zalloc = Z_NULL;
    inflate_strm1.zfree = Z_NULL;
    inflate_strm1.opaque = Z_NULL;
    inflate_strm1.avail_in = 0;
    inflate_strm1.next_in = Z_NULL;
    
    ret = inflateInit(&inflate_strm1);
    if (ret == Z_OK) {
        // Use inflateCopy API
        inflate_strm2.zalloc = Z_NULL;
        inflate_strm2.zfree = Z_NULL;
        inflate_strm2.opaque = Z_NULL;
        ret = inflateCopy(&inflate_strm2, &inflate_strm1);
        if (ret == Z_OK) {
            inflateEnd(&inflate_strm2);
        }
        inflateEnd(&inflate_strm1);
    }
    
    // 5. Use crc32_combine_gen
    // Generate combination operator for CRC32
    off_t len2 = 1024;  // Arbitrary length
    uLong op = crc32_combine_gen(len2);
    
    // 6. Use inflateBackEnd (though we haven't used inflateBack)
    // Create a dummy stream for inflateBackEnd
    z_stream back_strm;
    back_strm.zalloc = Z_NULL;
    back_strm.zfree = Z_NULL;
    back_strm.opaque = Z_NULL;
    
    // Initialize for back inflate
    unsigned char window[32768];  // 32K window
    ret = inflateBackInit(&back_strm, 15, window);
    if (ret == Z_OK) {
        // Use inflateBackEnd API
        inflateBackEnd(&back_strm);
    }
    
    // 7. Clean up
    free(decompressed);
    free(compressed);
    
    // Force usage of all variables to avoid compiler warnings
    (void)at_eof;
    (void)offset;
    (void)adler;
    (void)op;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
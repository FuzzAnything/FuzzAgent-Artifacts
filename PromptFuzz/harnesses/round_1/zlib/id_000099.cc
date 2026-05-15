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
//<ID> 325
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"gzerror":[[520,35,520,58,0,0,4,0]]},"library_calls":["zlibVersion","zlibCompileFlags","inflateInit_","inflateEnd","inflateMark","inflate","inflateCodesUsed","inflateCopy","inflateReset2","inflateResetKeep","inflateEnd","inflateEnd","inflateEnd","deflateInit_","inflateEnd","deflateBound","deflateBound","deflate","deflateUsed","deflateEnd","gzerror","inflateEnd"],"critical_calls":["zlibVersion","zlibCompileFlags","inflateInit_","inflateMark","inflate","inflateCodesUsed","inflateCopy","inflateReset2","inflateResetKeep","inflateEnd","deflateInit_","deflateBound","deflateBound","deflate","deflateUsed","deflateEnd","gzerror","inflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	

    // Get zlib version
    const char *version = zlibVersion();
    (void)version; // Use version to avoid unused variable warning
    
    // Get compile flags
    uLong flags = zlibCompileFlags();
    (void)flags; // Use flags to avoid unused variable warning

    // Initialize decompression stream
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    
    // Use inflateInit_ with proper parameters
    int ret = inflateInit_(&inflate_stream, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    // Set up input buffer for decompression
    inflate_stream.next_in = (Bytef*)data;
    inflate_stream.avail_in = (uInt)size;
    
    // Create output buffer for decompression
    size_t out_buf_size = size * 4; // Heuristic for decompressed size
    Bytef *out_buf = (Bytef*)malloc(out_buf_size);
    if (!out_buf) {
        inflateEnd(&inflate_stream);
        return 0;
    }
    
    inflate_stream.next_out = out_buf;
    inflate_stream.avail_out = (uInt)out_buf_size;

    // Get a mark during decompression
    long mark = inflateMark(&inflate_stream);
    (void)mark; // Use mark to avoid unused variable warning

    // Perform decompression
    ret = inflate(&inflate_stream, Z_NO_FLUSH);
    
    // Get codes used after decompression
    unsigned long codes_used = inflateCodesUsed(&inflate_stream);
    (void)codes_used; // Use codes_used to avoid unused variable warning

    // Create a copy of the inflation stream
    z_stream inflate_stream_copy;
    memset(&inflate_stream_copy, 0, sizeof(inflate_stream_copy));
    
    ret = inflateCopy(&inflate_stream_copy, &inflate_stream);
    if (ret == Z_OK) {
        // Reset the copied stream with different window bits
        ret = inflateReset2(&inflate_stream_copy, 15+16); // 15+16 for gzip decoding
        
        // Use inflateResetKeep on original stream
        ret = inflateResetKeep(&inflate_stream);
        
        // End the copied stream
        inflateEnd(&inflate_stream_copy);
    }

    // Create FILE* from input data for gz operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        free(out_buf);
        inflateEnd(&inflate_stream);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor
    int fd = fuzz_fileno(in_file);
    (void)fd; // Use fd to avoid unused variable warning

    // Create output file for gz operations
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        free(out_buf);
        inflateEnd(&inflate_stream);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Initialize compression stream
    z_stream deflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    
    // Use deflateInit_ with proper parameters
    ret = deflateInit_(&deflate_stream, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        free(out_buf);
        inflateEnd(&inflate_stream);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Set up input for compression (use decompressed output if available)
    size_t compressed_size = 0;
    if (inflate_stream.total_out > 0) {
        deflate_stream.next_in = out_buf;
        deflate_stream.avail_in = (uInt)inflate_stream.total_out;
        
        // Create output buffer for compression
        Bytef *compressed_buf = (Bytef*)malloc(deflateBound(&deflate_stream, inflate_stream.total_out));
        if (compressed_buf) {
            deflate_stream.next_out = compressed_buf;
            deflate_stream.avail_out = (uInt)deflateBound(&deflate_stream, inflate_stream.total_out);
            
            // Perform compression
            ret = deflate(&deflate_stream, Z_FINISH);
            
            // Get bits used during deflation
            int bits_used = 0;
            ret = deflateUsed(&deflate_stream, &bits_used);
            (void)bits_used; // Use bits_used to avoid unused variable warning
            
            compressed_size = deflate_stream.total_out;
            free(compressed_buf);
        }
    }

    // End compression stream
    deflateEnd(&deflate_stream);

    // Check for gz errors using gzerror
    int errnum = 0;
    const char *error_msg = gzerror((gzFile)in_file, &errnum);
    (void)error_msg; // Use error_msg to avoid unused variable warning

    // End decompression stream
    inflateEnd(&inflate_stream);

    // Cleanup
    free(out_buf);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
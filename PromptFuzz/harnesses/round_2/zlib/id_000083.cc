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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"gzdirect":[[637,9,637,31,0,0,4,1]]},"library_calls":["gzdopen","inflateInit2_","gzclose","inflateBackInit_","gzread","inflate","inflateSync","inflateCopy","inflateReset2","inflatePrime","deflateInit2_","inflateEnd","inflateEnd","inflateBackEnd","gzclose","deflatePrime","deflate","deflateUsed","deflateCopy","gzdopen","gzprintf","gzputs","gzwrite","gzdirect","gzoffset64","gzflush","gzclose","deflateEnd","deflateEnd","inflateEnd","inflateEnd","inflateBackEnd","gzclose"],"critical_calls":["gzdopen","inflateInit2_","inflateBackInit_","gzread","inflate","inflateSync","inflateCopy","inflateReset2","inflatePrime","deflateInit2_","deflatePrime","deflate","deflateUsed","deflateCopy","gzdopen","gzprintf","gzputs","gzwrite","gzdirect","gzoffset64","gzflush","gzclose","deflateEnd","deflateEnd","inflateEnd","inflateEnd","inflateBackEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Validate input
    if (!data || size == 0) {
	return 0;
	}
	

    // Create input FILE* from memory buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);

    // Step 1: Open compressed input with gzdopen
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 2: Initialize decompression stream
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // Use inflateInit2_ with windowBits for automatic header detection
    int ret = inflateInit2_(&strm_inflate, MAX_WBITS + 32, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 3: Initialize back inflate for another stream (though we may not use it fully)
    z_stream strm_inflate_back;
    memset(&strm_inflate_back, 0, sizeof(z_stream));
    unsigned char window[32768]; // 32KB window
    
    // Initialize inflateBack stream
    ret = inflateBackInit_(&strm_inflate_back, MAX_WBITS, window, 
                          ZLIB_VERSION, sizeof(z_stream));
    
    // Step 4: Read compressed data with gzread and decompress with inflate
    unsigned char decompressed[65536];
    unsigned char recompressed[65536];
    int decompressed_len = 0;
    
    while (1) {
        // Read from gzFile
        int bytes_read = gzread(gz_in, decompressed + decompressed_len, 
                               sizeof(decompressed) - decompressed_len);
        if (bytes_read <= 0) break;
        decompressed_len += bytes_read;
    }

    // Setup inflate stream with the data
    strm_inflate.next_in = (Bytef*)data;
    strm_inflate.avail_in = size;
    strm_inflate.next_out = decompressed;
    strm_inflate.avail_out = sizeof(decompressed);

    // Perform inflation
    ret = inflate(&strm_inflate, Z_NO_FLUSH);
    
    // Try inflateSync if we hit a sync point
    if (ret == Z_BUF_ERROR) {
        inflateSync(&strm_inflate);
    }

    // Step 5: Create another inflate stream using copy
    z_stream strm_inflate_copy;
    memset(&strm_inflate_copy, 0, sizeof(z_stream));
    inflateCopy(&strm_inflate_copy, &strm_inflate);
    
    // Reset the copied stream with different windowBits
    inflateReset2(&strm_inflate_copy, MAX_WBITS + 16);
    
    // Add some bits to the stream with inflatePrime (even if not strictly needed)
    inflatePrime(&strm_inflate_copy, 2, 1);

    // Step 6: Initialize compression stream
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    // Use deflateInit2_ with specific parameters
    ret = deflateInit2_(&strm_deflate, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY,
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&strm_inflate);
        inflateEnd(&strm_inflate_copy);
        inflateBackEnd(&strm_inflate_back);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 7: Compress the decompressed data
    strm_deflate.next_in = decompressed;
    strm_deflate.avail_in = strm_inflate.total_out;
    strm_deflate.next_out = recompressed;
    strm_deflate.avail_out = sizeof(recompressed);

    // Add some bits with deflatePrime
    deflatePrime(&strm_deflate, 3, 2);
    
    // Perform deflation
    ret = deflate(&strm_deflate, Z_FINISH);
    
    // Check how many bits were used
    int bits_used;
    deflateUsed(&strm_deflate, &bits_used);

    // Step 8: Create another deflate stream using copy
    z_stream strm_deflate_copy;
    memset(&strm_deflate_copy, 0, sizeof(z_stream));
    deflateCopy(&strm_deflate_copy, &strm_deflate);

    // Step 9: Open output gzFile
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        // Write to gzFile using various methods
        gzprintf(gz_out, "Recompressed data: ");
        gzputs(gz_out, "Processed by fuzzer\n");
        
        // Write the recompressed data
        gzwrite(gz_out, recompressed, strm_deflate.total_out);
        
        // Check if direct
        int is_direct = gzdirect(gz_out);
        
        // Get offset
        off64_t offset = gzoffset64(gz_out);
        
        // Flush
        gzflush(gz_out, Z_FULL_FLUSH);
    }

    // Cleanup
    if (gz_out) gzclose(gz_out);
    deflateEnd(&strm_deflate);
    deflateEnd(&strm_deflate_copy);
    inflateEnd(&strm_inflate);
    inflateEnd(&strm_inflate_copy);
    inflateBackEnd(&strm_inflate_back);
    gzclose(gz_in);
    
    // Close files (file descriptors were used by gzFile, so don't double-close)
    // Just close the FILE* handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    unlink("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
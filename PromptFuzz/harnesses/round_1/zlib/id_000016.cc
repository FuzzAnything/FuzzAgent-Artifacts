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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["zlibVersion","inflateBackInit_","gzdopen","inflateBackEnd","gzdirect","deflateInit_","gzclose","inflateBackEnd","deflateCopy","deflateEnd","gzclose","inflateBackEnd","gzprintf","gzflush","deflateEnd","deflateEnd","gzclose","inflateBackEnd"],"critical_calls":["zlibVersion","inflateBackInit_","gzdopen","gzdirect","deflateInit_","deflateCopy","gzprintf","gzflush","deflateEnd","deflateEnd","gzclose","inflateBackEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need enough data for meaningful operations

    // Create a temporary input file from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Create output files
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Initialize inflateBack stream for decompression
    z_stream strm;
    unsigned char window[32768];  // 32KB window as required by inflateBack
    memset(&strm, 0, sizeof(strm));
    
    int windowBits = 15;
    const char* version = zlibVersion();
    int stream_size = sizeof(z_stream);
    
    // Initialize inflateBack for decompression
    int ret = inflateBackInit_(&strm, windowBits, window, version, stream_size);
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 2: Create a gzFile from the output file descriptor for writing
    int out_fd = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (!gz_out) {
        inflateBackEnd(&strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
    }

    // Step 3: Check if gzFile is direct (should return false for compressed files)
    int is_direct = gzdirect(gz_out);
    (void)is_direct;  // Use result to avoid unused variable warning

    // Step 4: Set up source and destination streams for deflateCopy
    z_stream source_strm, dest_strm;
    memset(&source_strm, 0, sizeof(source_strm));
    memset(&dest_strm, 0, sizeof(dest_strm));
    
    // Initialize source stream for compression
    source_strm.zalloc = Z_NULL;
    source_strm.zfree = Z_NULL;
    source_strm.opaque = Z_NULL;
    ret = deflateInit(&source_strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        gzclose(gz_out);
        inflateBackEnd(&strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
    }

    // Step 5: Copy the compression state from source to dest
    ret = deflateCopy(&dest_strm, &source_strm);
    if (ret != Z_OK) {
        deflateEnd(&source_strm);
        gzclose(gz_out);
        inflateBackEnd(&strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
    }

    // Step 6: Write formatted output to gzFile using gzprintf
    // We'll write some information about the input data
    ret = gzprintf(gz_out, "Input size: %zu, First byte: 0x%02x\n", 
                   size, data[0]);
    if (ret < 0) {
        // Error in writing
    }

    // Step 7: Flush the gzFile
    ret = gzflush(gz_out, Z_SYNC_FLUSH);
    if (ret != Z_OK) {
        // Handle flush error
    }

    // Step 8: Clean up all resources
    deflateEnd(&dest_strm);
    deflateEnd(&source_strm);
    gzclose(gz_out);  // This also closes the underlying file descriptor
    inflateBackEnd(&strm);
    
    // Note: in_file was closed by fmemopen when we fclose it
    // out_file was closed by gzclose, so we don't need to fclose it again
    assert_file_closed(&in_file);;
    
    // Remove the temporary output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	return 0;
}
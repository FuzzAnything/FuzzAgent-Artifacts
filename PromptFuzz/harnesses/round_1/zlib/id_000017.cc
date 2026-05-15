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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"gzvprintf":[[428,9,428,25,0,0,4,1],[450,9,450,28,0,0,4,1]]},"library_calls":["inflateBackInit_","gzdopen","inflateBackEnd","gzdirect","deflateInit_","gzclose","inflateBackEnd","deflateCopy","gzprintf","gzprintf","gzprintf","gzprintf","gzflush","deflateEnd","deflateEnd","inflateBackEnd","gzclose"],"critical_calls":["inflateBackInit_","gzdopen","gzdirect","deflateInit_","deflateCopy","gzprintf","gzprintf","gzprintf","gzprintf","gzflush","deflateEnd","deflateEnd","inflateBackEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to achieve a compression-decompression cycle with formatted output:

*/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create memory file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Variables to track status
    int ret = Z_OK;
    
    // 1. Initialize inflation stream for back-to-front decompression
    z_stream strm_in;
    memset(&strm_in, 0, sizeof(z_stream));
    
    // Allocate window buffer for inflateBack
    unsigned char *window = (unsigned char*)malloc(1 << 15); // 32K window
    if (!window) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize inflateBack with window
    ret = inflateBackInit_(&strm_in, 15, window, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        free(window);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create gzFile from file descriptor for writing
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_out) {
        inflateBackEnd(&strm_in);
        free(window);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }
    
    // 3. Check if gzFile is direct (no compression)
    int is_direct = gzdirect(gz_out);
    
    // 4. Initialize deflation stream for compression
    z_stream strm_def;
    memset(&strm_def, 0, sizeof(z_stream));
    ret = deflateInit(&strm_def, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        gzclose(gz_out);
        inflateBackEnd(&strm_in);
        free(window);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
    }
    
    // 5. Create another deflation stream and copy from first
    z_stream strm_copy;
    memset(&strm_copy, 0, sizeof(z_stream));
    ret = deflateCopy(&strm_copy, &strm_def);
    
    // 6. Write formatted data to gzFile using gzprintf
    if (ret == Z_OK) {
        // Write some formatted information about the input
        gzprintf(gz_out, "Input size: %zu, Direct mode: %d\n", size, is_direct);
        
        // Write a portion of input data as formatted output
        size_t sample_len = size > 100 ? 100 : size;
        gzprintf(gz_out, "First %zu bytes (hex): ", sample_len);
        for (size_t i = 0; i < sample_len && i < size; i++) {
            gzprintf(gz_out, "%02x", data[i]);
        }
        gzprintf(gz_out, "\n");
    }
    
    // 7. Flush the gzFile output
    gzflush(gz_out, Z_SYNC_FLUSH);
    
    // 8. Clean up all allocated resources
    if (ret == Z_OK) {
        deflateEnd(&strm_copy);
    }
    deflateEnd(&strm_def);
    inflateBackEnd(&strm_in);
    
    // Close gzFile - this also closes the underlying file descriptor
    gzclose(gz_out);
    
    // Note: out_file is already closed by gzclose, so don't fclose it again
    // in_file needs to be closed explicitly
    assert_file_closed(&in_file);;
    
    free(window);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	return 0;
}
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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 4.5, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["inflateInit2_","zError","inflateEnd","inflateEnd","inflate","zError","inflateEnd","deflateInit2_","zError","deflateBound_z","compressBound","gzdopen","gzwrite","gzclose","deflateResetKeep","deflateEnd"],"critical_calls":["inflateInit2_","inflate","zError","inflateEnd","deflateInit2_","deflateBound_z","compressBound","gzdopen","gzwrite","gzclose","deflateResetKeep","deflateEnd"],"visited":1}
/*Here's a fuzz driver that uses the specified zlib APIs to achieve a specific event: processing compressed data and estimating compression bounds. The event involves decompressing input data, then estimating compression bounds for the decompressed data.

*/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a FILE* for writing output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Initialize decompression stream
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Initialize inflation with default window bits
    int ret = inflateInit2(&inflate_strm, 15 + 16); // 15+16 for gzip header
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);  // Use zError API
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Prepare input buffer
    Bytef *in_buffer = (Bytef *)malloc(size);
    if (!in_buffer) {
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    memcpy(in_buffer, data, size);
    
    inflate_strm.next_in = in_buffer;
    inflate_strm.avail_in = (uInt)size;

    // Prepare output buffer (using worst-case expansion)
    size_t out_buf_size = size * 2;
    Bytef *out_buffer = (Bytef *)malloc(out_buf_size);
    if (!out_buffer) {
        free(in_buffer);
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    inflate_strm.next_out = out_buffer;
    inflate_strm.avail_out = (uInt)out_buf_size;

    // Decompress the data
    ret = inflate(&inflate_strm, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        const char *err_msg = zError(ret);  // Use zError API
        // Continue anyway to use other APIs
    }

    size_t decompressed_size = out_buf_size - inflate_strm.avail_out;

    // End inflation
    inflateEnd(&inflate_strm);  // Use inflateEnd API

    // Initialize compression stream for bounds calculation
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    ret = deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        const char *err_msg = zError(ret);  // Use zError API
        free(in_buffer);
        free(out_buffer);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Calculate compression bound using deflateBound_z API
    z_size_t bound_z = deflateBound_z(&deflate_strm, decompressed_size);
    
    // Also calculate using compressBound API for comparison
    uLong bound_ul = compressBound((uLong)decompressed_size);

    // Use gzdopen API to create a gzFile from file descriptor
    gzFile gz_out = gzdopen(out_fd, "wb");
    if (gz_out) {
        // Write decompressed data to gz file
        gzwrite(gz_out, out_buffer, (unsigned)decompressed_size);
        gzclose(gz_out);  // This will close the file
    } else {
        // If gzdopen failed, we need to close the file descriptor manually
        assert_file_closed(&out_file);;
    }

    // Reset deflate stream keeping the compression level and strategy
    deflateResetKeep(&deflate_strm);  // Use deflateResetKeep API
    
    // End deflation
    deflateEnd(&deflate_strm);  // Use deflateEnd API

    // Clean up all allocated resources
    free(in_buffer);
    free(out_buffer);
    assert_file_closed(&in_file);;
    
    // out_file was closed by gzclose if gzdopen succeeded
    // If gzdopen failed, it's already closed above
    if (gz_out) {
        // File is already closed by gzclose
    }

    // Use the calculated bounds (prevent optimization)
    static volatile z_size_t keep_bound_z = 0;
    static volatile uLong keep_bound_ul = 0;
    keep_bound_z = bound_z;
    keep_bound_ul = bound_ul;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
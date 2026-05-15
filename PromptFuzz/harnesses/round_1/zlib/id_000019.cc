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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["gzdopen","inflateInit2_","gzclose","inflateUndermine","inflateSetDictionary","deflateInit2_","inflateEnd","gzclose","compressBound","deflateBound_z","deflateSetDictionary","deflateEnd","inflateEnd","gzclose","gzfread","inflate","zError","crc32","deflate","deflatePending","deflateResetKeep","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","inflateInit2_","inflateUndermine","inflateSetDictionary","deflateInit2_","compressBound","deflateBound_z","deflateSetDictionary","gzfread","inflate","zError","crc32","deflate","deflatePending","deflateResetKeep","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Basic sanity check
    if (size < 10) {
	return 0;
	}
	
    
    // Create memory file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for compressed data
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
    
    // Initialize zlib structures
    z_stream deflate_stream;
    z_stream inflate_stream;
    memset(&deflate_stream, 0, sizeof(z_stream));
    memset(&inflate_stream, 0, sizeof(z_stream));
    
    // Variables for various operations
    Bytef *dict = nullptr;
    uInt dict_len = 0;
    unsigned int pending;
    int bits;
    uLong crc_val = 0;
    const char *error_str = nullptr;
    
    // 1. Open compressed file with gzdopen
    gzFile gz_file = gzdopen(fd_in, "rb");
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 2. Initialize decompression stream with inflateInit2_
    int window_bits = 15 + 32; // Enable gzip decoding
    if (inflateInit2_(&inflate_stream, window_bits, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. Try to undermine inflation (test error handling)
    inflateUndermine(&inflate_stream, 1);
    
    // 4. Set dictionary for inflation if we have enough data
    if (size > 100) {
        dict_len = (uInt)(size % 100) + 1;
        dict = (Bytef *)malloc(dict_len);
        if (dict) {
            memcpy(dict, data, dict_len);
            inflateSetDictionary(&inflate_stream, dict, dict_len);
            free(dict);
            dict = nullptr;
        }
    }
    
    // 5. Initialize compression stream with deflateInit2_
    if (deflateInit2_(&deflate_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                      window_bits, 8, Z_DEFAULT_STRATEGY,
                      ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&inflate_stream);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 6. Calculate compression bounds
    uLong bound1 = compressBound((uLong)size);
    z_size_t bound2 = deflateBound_z(&deflate_stream, (z_size_t)size);
    
    // 7. Set dictionary for deflation
    if (size > 50) {
        dict_len = (uInt)(size % 50) + 1;
        dict = (Bytef *)malloc(dict_len);
        if (dict) {
            memcpy(dict, data + 10, dict_len);
            deflateSetDictionary(&deflate_stream, dict, dict_len);
            free(dict);
            dict = nullptr;
        }
    }
    
    // 8. Process data through inflation (decompression)
    Bytef *in_buffer = (Bytef *)malloc(size);
    Bytef *out_buffer = (Bytef *)malloc(bound1 > size * 2 ? bound1 : size * 2);
    
    if (!in_buffer || !out_buffer) {
        free(in_buffer);
        free(out_buffer);
        deflateEnd(&deflate_stream);
        inflateEnd(&inflate_stream);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Read from gz file
    size_t read_size = gzfread(in_buffer, 1, size, gz_file);
    
    // Setup inflation stream
    inflate_stream.next_in = in_buffer;
    inflate_stream.avail_in = (uInt)read_size;
    inflate_stream.next_out = out_buffer;
    inflate_stream.avail_out = (uInt)(bound1 > size * 2 ? bound1 : size * 2);
    
    // Perform inflation
    int inflate_result = inflate(&inflate_stream, Z_FINISH);
    
    // Get error string if inflation failed
    if (inflate_result != Z_STREAM_END && inflate_result != Z_OK) {
        error_str = zError(inflate_result);
    }
    
    // 9. Calculate CRC32 of (partial) input data
    if (size > 20) {
        uInt crc_len = (uInt)(size % 100);
        if (crc_len > size) crc_len = (uInt)size;
        crc_val = crc32(0L, data, crc_len);
    }
    
    // 10. Setup deflation stream with inflated data
    deflate_stream.next_in = out_buffer;
    deflate_stream.avail_in = (uInt)inflate_stream.total_out;
    deflate_stream.next_out = in_buffer;  // Reuse buffer
    deflate_stream.avail_out = (uInt)size;
    
    // 11. Perform deflation
    int deflate_result = deflate(&deflate_stream, Z_FINISH);
    
    // 12. Check for pending data
    deflatePending(&deflate_stream, &pending, &bits);
    
    // 13. Reset deflate stream keeping internal state
    deflateResetKeep(&deflate_stream);
    
    // Write compressed data to output file
    if (deflate_stream.total_out > 0) {
        fwrite(in_buffer, 1, deflate_stream.total_out, out_file);
    }
    
    // 14. Cleanup
    deflateEnd(&deflate_stream);
    inflateEnd(&inflate_stream);
    gzclose(gz_file);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Free buffers
    free(in_buffer);
    free(out_buffer);
    
    // Prevent unused variable warnings
    (void)bound2;
    (void)crc_val;
    (void)error_str;
    (void)deflate_result;
    (void)inflate_result;
    (void)fd_out;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
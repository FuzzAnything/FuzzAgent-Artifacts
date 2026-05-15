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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["zlibVersion","gzdopen","inflateInit2_","gzclose","deflateInit_","inflateEnd","gzclose","gzeof","gzread","gzrewind","inflateValidate","inflateBackInit_","inflateBackEnd","deflateEnd","inflateEnd","gzclose","compress_z","compress2_z","inflate","inflateMark","inflateSyncPoint","inflateResetKeep","deflateParams","deflate","deflatePrime","deflateEnd","inflateEnd","gzclose"],"critical_calls":["zlibVersion","gzdopen","inflateInit2_","deflateInit_","gzeof","gzread","gzrewind","inflateValidate","inflateBackInit_","inflateBackEnd","compress_z","compress2_z","inflate","inflateMark","inflateSyncPoint","inflateResetKeep","deflateParams","deflate","deflatePrime","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get zlib version
    const char *version = zlibVersion();
    (void)version; // Prevent unused variable warning
    
    // 2. Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create a FILE* for writing output (not actually used for fuzzing)
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)out_fd; // Prevent unused variable warning
    
    // 5. Open gzFile for reading
    gzFile gz_in = gzdopen(in_fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 6. Initialize inflate stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Use inflateInit2_ with windowBits = 15+32 for automatic header detection
    int ret = inflateInit2_(&inflate_strm, 15+32, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 7. Initialize deflate stream for compression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Use deflateInit_ with default compression level
    ret = deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 8. Read from gzFile
    unsigned char gz_buffer[4096];
    unsigned int bytes_read = 0;
    
    while (!gzeof(gz_in)) {
        int read_ret = gzread(gz_in, gz_buffer, sizeof(gz_buffer));
        if (read_ret <= 0) {
            break;
        }
        bytes_read += read_ret;
    }
    
    // 9. Reset gzFile pointer
    gzrewind(gz_in);
    
    // 10. Set inflate validation
    inflateValidate(&inflate_strm, 1);
    
    // 11. Initialize inflateBack stream
    z_stream inflate_back_strm;
    memset(&inflate_back_strm, 0, sizeof(inflate_back_strm));
    unsigned char window[32768];
    
    // Initialize inflateBack for backward inflation
    ret = inflateBackInit_(&inflate_back_strm, 15, window, ZLIB_VERSION, sizeof(z_stream));
    if (ret == Z_OK) {
        // We would need proper callbacks for inflateBack, but we'll just initialize and end it
        inflateBackEnd(&inflate_back_strm);
    }
    
    // 12. Create buffers for compression/decompression
    unsigned char *compressed_buf = (unsigned char *)malloc(size * 2);
    unsigned char *decompressed_buf = (unsigned char *)malloc(size * 2);
    if (!compressed_buf || !decompressed_buf) {
        free(compressed_buf);
        free(decompressed_buf);
        deflateEnd(&deflate_strm);
        inflateEnd(&inflate_strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 13. Use compress_z and compress2_z APIs
    z_size_t dest_len = size * 2;
    int compress_ret = compress_z(compressed_buf, &dest_len, data, size);
    (void)compress_ret; // Prevent unused variable warning
    
    dest_len = size * 2;
    compress2_z(compressed_buf, &dest_len, data, size, Z_BEST_COMPRESSION);
    
    // 14. Set up inflate stream with input data
    inflate_strm.next_in = (Bytef *)data;
    inflate_strm.avail_in = size;
    inflate_strm.next_out = decompressed_buf;
    inflate_strm.avail_out = size * 2;
    
    // 15. Perform inflation
    ret = inflate(&inflate_strm, Z_FINISH);
    
    // 16. Get inflate mark
    long mark = inflateMark(&inflate_strm);
    (void)mark; // Prevent unused variable warning
    
    // 17. Check sync point
    int sync_point = inflateSyncPoint(&inflate_strm);
    (void)sync_point; // Prevent unused variable warning
    
    // 18. Reset inflate stream keeping dictionary
    inflateResetKeep(&inflate_strm);
    
    // 19. Set up deflate stream
    deflate_strm.next_in = decompressed_buf;
    deflate_strm.avail_in = inflate_strm.total_out;
    deflate_strm.next_out = compressed_buf;
    deflate_strm.avail_out = size * 2;
    
    // 20. Change deflate parameters
    deflateParams(&deflate_strm, Z_BEST_SPEED, Z_DEFAULT_STRATEGY);
    
    // 21. Perform deflation
    ret = deflate(&deflate_strm, Z_FINISH);
    
    // 22. Insert bits into deflate stream (prime)
    deflatePrime(&deflate_strm, 5, 31);
    
    // 23. Clean up
    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    free(compressed_buf);
    free(decompressed_buf);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
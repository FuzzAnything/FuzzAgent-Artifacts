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
//<ID> 334
//<Prompt> []
/*<Combination>: [
*/
//<score> 68, nr_unique_branch: 7
//<Quality> {"density":17,"unique_branches":{"gzsetparams":[[655,9,655,20,0,0,4,0],[657,13,657,27,0,0,4,0],[657,31,657,60,0,0,4,1]],"deflate":[[1241,24,1241,40,0,0,4,1],[133,31,133,38,10,0,4,0],[133,31,133,38,11,0,4,0]],"deflateParams":[[797,31,797,76,0,0,4,1]]},"library_calls":["gzdopen","gzdopen","gzclose","gzclose","gzbuffer","deflateInit_","gzclose_r","gzclose","deflateBound_z","deflateEnd","gzclose_r","gzclose","deflateEnd","gzclose_r","gzclose","gzread","deflateParams","gzputc","crc32_z","crc32_z","compressBound","compress","gzputc","gzsetparams","inflateBackInit_","inflateBackEnd","gzclose_r","gzclose","deflateEnd"],"critical_calls":["gzdopen","gzdopen","gzbuffer","deflateInit_","deflateBound_z","gzread","deflateParams","gzputc","crc32_z","crc32_z","compressBound","compress","gzputc","gzsetparams","inflateBackInit_","inflateBackEnd","gzclose_r","gzclose","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for meaningful processing
    if (size < 10) {
        return 0;
    }

    // 1. Create a FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Create an output file for writing compressed data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 3. Get file descriptors for gzdopen
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 4. Open gzFile handles for reading and writing
    gzFile gz_in = gzdopen(fd_in, "rb");
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (!gz_in || !gz_out) {
        if (gz_in) gzclose(gz_in);
        if (gz_out) gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 5. Set buffer size for gzFile
    gzbuffer(gz_out, 8192);

    // 6. Initialize z_stream for deflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        gzclose_r(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 7. Use deflateBound_z to estimate maximum compressed size
    z_size_t estimated_size = deflateBound_z(&strm, size);
    
    // 8. Allocate buffers for compression
    Bytef *dest_buf = (Bytef *)malloc(estimated_size);
    uLongf dest_len = estimated_size;
    
    if (!dest_buf) {
        deflateEnd(&strm);
        gzclose_r(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 9. Read data from gzFile input
    Bytef *read_buf = (Bytef *)malloc(size);
    if (!read_buf) {
        free(dest_buf);
        deflateEnd(&strm);
        gzclose_r(gz_in);
        gzclose(gz_out);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Read from gzFile - simulate reading compressed data
    // Note: This is artificial since data is already uncompressed in memory
    // but we need to use gzread API
    int bytes_read = gzread(gz_in, read_buf, size > 1000 ? 1000 : size);

    // 10. Change compression parameters using deflateParams
    if (bytes_read > 0) {
        // Update strm with the read data
        strm.next_in = read_buf;
        strm.avail_in = bytes_read;
        
        // Change compression level and strategy
        deflateParams(&strm, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    }

    // 11. Write a character to gzFile output using gzputc
    gzputc(gz_out, 'X');

    // 12. Calculate CRC32 of the input data using crc32_z
    uLong crc = crc32_z(0L, Z_NULL, 0);
    crc = crc32_z(crc, data, size > 1024 ? 1024 : size);

    // 13. Compress a portion of data using compress API
    // Use first 500 bytes or less if input is smaller
    size_t compress_size = size > 500 ? 500 : size;
    uLongf comp_len = compressBound(compress_size);
    Bytef *comp_buf = (Bytef *)malloc(comp_len);
    
    if (comp_buf) {
        compress(comp_buf, &comp_len, data, compress_size);
        free(comp_buf);
    }

    // 14. Write the CRC value to gzFile as a marker
    char crc_str[20];
    snprintf(crc_str, sizeof(crc_str), "CRC:%lu", crc);
    for (size_t i = 0; i < strlen(crc_str); i++) {
        gzputc(gz_out, crc_str[i]);
    }

    // 15. Read from gzFile input until EOF
    int ch;
    while ((ch = gzgetc(gz_in)) != -1) {
        // Process each character if needed
    }

    // 16. Set compression parameters for gzFile
    gzsetparams(gz_out, Z_BEST_SPEED, Z_DEFAULT_STRATEGY);

    // 17. Initialize inflateBack structures (for demonstration)
    // Note: We need to allocate window buffer for inflateBack
    z_stream strm2;
    memset(&strm2, 0, sizeof(strm2));
    strm2.zalloc = Z_NULL;
    strm2.zfree = Z_NULL;
    strm2.opaque = Z_NULL;
    
    unsigned char window[32768];
    if (inflateBackInit(&strm2, 15, window) == Z_OK) {
        // In a real scenario, we would set up callbacks here
        // For fuzzing, we just demonstrate the API usage
        
        // 18. Clean up inflateBack with inflateBackEnd
        inflateBackEnd(&strm2);
    }

    // 19. Close gzFile handles
    gzclose_r(gz_in);  // Specifically using gzclose_r
    gzclose(gz_out);   // Using regular gzclose for writing

    // 20. Clean up deflate stream
    deflateEnd(&strm);

    // 21. Clean up allocated memory
    free(dest_buf);
    free(read_buf);

    // 22. Close FILE handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
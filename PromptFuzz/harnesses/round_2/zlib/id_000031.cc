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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["gzdopen","gzread","gztell64","gzsetparams","compressBound_z","gzclose","compress_z","compressBound","gzclose","compress2","deflateInit2_","deflateBound_z","deflateParams","deflateResetKeep","inflateInit2_","inflate","inflateMark","inflateEnd","deflateEnd","gzclose"],"critical_calls":["gzdopen","gzread","gztell64","gzsetparams","compressBound_z","compress_z","compressBound","compress2","deflateInit2_","deflateBound_z","deflateParams","deflateResetKeep","inflateInit2_","inflate","inflateMark","inflateEnd","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Step 1: Open input as FILE* and gzFile
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Step 2: Read some data from gzFile and use gztell64
    unsigned char buffer[4096];
    int bytes_read = gzread(gz_in, buffer, sizeof(buffer));
    off64_t position = gztell64(gz_in);

    // Step 3: Set parameters on gzFile
    gzsetparams(gz_in, Z_DEFAULT_COMPRESSION, Z_DEFAULT_STRATEGY);

    // Step 4: Prepare for compression using compress2 and compress_z
    // First compress using compress_z
    z_size_t compressed_len_z = compressBound_z(size);
    Bytef *compressed_z = (Bytef *)malloc(compressed_len_z);
    if (!compressed_z) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int result_z = compress_z(compressed_z, &compressed_len_z, data, size);

    // Then compress using compress2
    uLongf compressed_len2 = compressBound(size);
    Bytef *compressed2 = (Bytef *)malloc(compressed_len2);
    if (!compressed2) {
        free(compressed_z);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    int result2 = compress2(compressed2, &compressed_len2, data, size, Z_BEST_COMPRESSION);

    // Step 5: Set up deflate stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                  15 + 16, 8, Z_DEFAULT_STRATEGY,
                  ZLIB_VERSION, sizeof(z_stream));

    // Use deflateBound_z
    z_size_t bound = deflateBound_z(&strm, size);

    // Use deflateParams
    deflateParams(&strm, Z_BEST_COMPRESSION, Z_HUFFMAN_ONLY);

    // Use deflateResetKeep
    deflateResetKeep(&strm);

    // Step 6: Set up inflate stream for marking
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    inflateInit2_(&inflate_strm, 15 + 32, ZLIB_VERSION, sizeof(z_stream));
    
    inflate_strm.avail_in = (uInt)size;
    inflate_strm.next_in = (Bytef *)data;

    unsigned char out_buffer[4096];
    inflate_strm.avail_out = sizeof(out_buffer);
    inflate_strm.next_out = out_buffer;

    // Inflate a bit and use inflateMark
    inflate(&inflate_strm, Z_NO_FLUSH);
    long mark = inflateMark(&inflate_strm);

    // Clean up inflate stream
    inflateEnd(&inflate_strm);

    // Clean up deflate stream
    deflateEnd(&strm);

    // Clean up gzFile
    gzclose(gz_in);
    assert_file_closed(&in_file);;

    // Clean up allocated memory
    free(compressed_z);
    free(compressed2);

    // Use the variables to avoid unused variable warnings
    (void)bytes_read;
    (void)position;
    (void)result_z;
    (void)result2;
    (void)bound;
    (void)mark;

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
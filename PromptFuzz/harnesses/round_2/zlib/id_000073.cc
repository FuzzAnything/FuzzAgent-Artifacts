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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 58, nr_unique_branch: 3
//<Quality> {"density":29,"unique_branches":{"gz_read":[[357,40,357,62,0,0,4,1],[368,18,368,36,0,0,4,0],[368,18,368,36,0,0,4,1]]},"library_calls":["gzopen","gzoffset","gzoffset64","gzclose","gzread","gzclearerr","gzclose","gzseek","gzrewind","gzclose","gzdopen","gzclose","gzread","inflateInit_","gzclose","gzclose","inflateGetHeader","inflateGetDictionary","inflateSyncPoint","deflateInit_","inflateEnd","gzclose","gzclose","deflateInit2_","deflateEnd","inflateEnd","gzclose","gzclose","deflateTune","deflateGetDictionary","deflateBound","compressBound_z","compress2_z","deflateEnd","deflateEnd","inflateEnd","gzclose","gzclose"],"critical_calls":["gzopen","gzoffset","gzoffset64","gzread","gzseek","gzrewind","gzdopen","gzread","inflateInit_","inflateGetHeader","inflateGetDictionary","inflateSyncPoint","deflateInit_","deflateInit2_","deflateTune","deflateGetDictionary","deflateBound","compressBound_z","compress2_z","deflateEnd","deflateEnd","inflateEnd","gzclose","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to test decompression and recompression of zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Return early if input is too small for meaningful testing
    if (size < 100) {
	return 0;
	}
	
    
    // Create a temporary input file with the compressed data
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Open the compressed file with gzopen
    gzFile gz_in = gzopen("input_file", "rb");
    if (!gz_in) {
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Get current offset in file
    off_t offset1 = gzoffset(gz_in);
    (void)offset1; // Use result to avoid warnings
    
    // Try to get offset using 64-bit version
    off64_t offset64 = gzoffset64(gz_in);
    (void)offset64; // Use result to avoid warnings
    
    // Allocate buffer for decompressed data
    size_t decomp_size = size * 10; // Reasonable estimate
    if (decomp_size > 10 * 1024 * 1024) decomp_size = 10 * 1024 * 1024; // Cap at 10MB
    Bytef *decomp_buf = (Bytef*)malloc(decomp_size);
    if (!decomp_buf) {
        gzclose(gz_in);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Read decompressed data using gzread
    int read_len = gzread(gz_in, decomp_buf, (unsigned int)decomp_size);
    if (read_len <= 0) {
        // Clear any errors
        gzclearerr(gz_in);
        free(decomp_buf);
        gzclose(gz_in);
        remove("input_file");
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Try seeking and rewinding
    gzseek(gz_in, 0, SEEK_SET);
    gzrewind(gz_in);
    
    // Create FILE* from memory for deflate operations
    FILE *mem_file = fmemopen((void *)data, size, "rb");
    if (!mem_file) {
        free(decomp_buf);
        gzclose(gz_in);
        remove("input_file");
        assert_file_closed(&mem_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    int fd = fuzz_fileno(mem_file);
    
    // Open compressed data with gzdopen
    gzFile gz_fd = gzdopen(fd, "rb");
    if (!gz_fd) {
        assert_file_closed(&mem_file);;
        free(decomp_buf);
        gzclose(gz_in);
        remove("input_file");
        assert_file_closed(&mem_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // Try reading from this file too
    Bytef *temp_buf = (Bytef*)malloc(1024);
    if (temp_buf) {
        gzread(gz_fd, temp_buf, 1024);
        free(temp_buf);
    }
    
    // Initialize inflate stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Try different initialization methods
    int ret = inflateInit_(&inflate_strm, ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_fd);
        assert_file_closed(&mem_file);;
        free(decomp_buf);
        gzclose(gz_in);
        remove("input_file");
        assert_file_closed(&mem_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // Try to get inflate header and dictionary
    gz_header head;
    memset(&head, 0, sizeof(head));
    inflateGetHeader(&inflate_strm, &head);
    
    Bytef dict_buf[256];
    uInt dict_len = 256;
    inflateGetDictionary(&inflate_strm, dict_buf, &dict_len);
    
    // Check sync point
    inflateSyncPoint(&inflate_strm);
    
    // Initialize deflate streams for recompression
    z_stream deflate_strm1, deflate_strm2;
    memset(&deflate_strm1, 0, sizeof(deflate_strm1));
    memset(&deflate_strm2, 0, sizeof(deflate_strm2));
    
    // Initialize first deflate stream
    ret = deflateInit_(&deflate_strm1, Z_DEFAULT_COMPRESSION, 
                      ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        gzclose(gz_fd);
        assert_file_closed(&mem_file);;
        free(decomp_buf);
        gzclose(gz_in);
        remove("input_file");
        assert_file_closed(&mem_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // Initialize second deflate stream with more parameters
    ret = deflateInit2_(&deflate_strm2, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       MAX_WBITS, 8, Z_DEFAULT_STRATEGY,
                       ZLIB_VERSION, sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&deflate_strm1);
        inflateEnd(&inflate_strm);
        gzclose(gz_fd);
        assert_file_closed(&mem_file);;
        free(decomp_buf);
        gzclose(gz_in);
        remove("input_file");
        assert_file_closed(&mem_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // Tune deflate parameters
    deflateTune(&deflate_strm2, 8, 9, 10, 11);
    
    // Get dictionary from deflate stream
    dict_len = 256;
    deflateGetDictionary(&deflate_strm2, dict_buf, &dict_len);
    
    // Calculate bound for compressed size
    uLong bound = deflateBound(&deflate_strm2, (uLong)read_len);
    
    // Calculate bound using _z version
    z_size_t bound_z = compressBound_z((z_size_t)read_len);
    
    // Prepare buffer for recompressed data
    z_size_t recomp_size = (z_size_t)bound_z;
    if (recomp_size > 10 * 1024 * 1024) recomp_size = 10 * 1024 * 1024;
    Bytef *recomp_buf = (Bytef*)malloc(recomp_size);
    
    if (recomp_buf) {
        // Recompress the decompressed data
        z_size_t recomp_len = recomp_size;
        compress2_z(recomp_buf, &recomp_len, decomp_buf, (z_size_t)read_len, 
                   Z_DEFAULT_COMPRESSION);
        free(recomp_buf);
    }
    
    // Clean up all resources
    deflateEnd(&deflate_strm2);
    deflateEnd(&deflate_strm1);
    inflateEnd(&inflate_strm);
    gzclose(gz_fd);
    assert_file_closed(&mem_file);;
    free(decomp_buf);
    gzclose(gz_in);
    
    // Clean up file resources
    remove("input_file");
    
    assert_file_closed(&mem_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}
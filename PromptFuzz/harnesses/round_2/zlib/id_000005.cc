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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":8,"unique_branches":{},"library_calls":["gzdopen","compressBound_z","gzclose","deflateInit_","gzclose","deflateTune","deflateBound","deflateGetDictionary","compress2_z","gzclearerr","gzrewind","gzoffset","gzread","deflateEnd","gzclose"],"critical_calls":["gzdopen","compressBound_z","deflateInit_","deflateTune","deflateBound","deflateGetDictionary","compress2_z","gzclearerr","gzrewind","gzoffset","gzread","deflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a FILE* from the input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor from in_file
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Open gzFile using gzdopen with the file descriptor
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Allocate buffers for compression
    Bytef *source = (Bytef *)data;
    z_size_t sourceLen = size;
    z_size_t destLen = compressBound_z(sourceLen);
    Bytef *dest = (Bytef *)malloc(destLen);
    if (!dest) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Initialize z_stream for deflate operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize deflate with default parameters
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        free(dest);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Call deflateTune with some example parameters
    deflateTune(&strm, 8, 9, 8, 4096);

    // Calculate bound using deflateBound
    uLong bound = deflateBound(&strm, sourceLen);
    
    // Try to get dictionary (may fail if none set)
    Bytef dictionary[256];
    uInt dictLength = 256;
    deflateGetDictionary(&strm, dictionary, &dictLength);

    // Perform compression using compress2_z
    int level = Z_DEFAULT_COMPRESSION;
    int compress_result = compress2_z(dest, &destLen, source, sourceLen, level);
    
    // If compression succeeded, try to read from gzFile
    if (compress_result == Z_OK) {
        // Clear any error state on gzFile
        gzclearerr(gz_in);
        
        // Rewind the gzFile
        gzrewind(gz_in);
        
        // Get current offset (should be 0 after rewind)
        off_t offset = gzoffset(gz_in);
        
        // Try to read some data from gzFile
        Bytef read_buffer[1024];
        int bytes_read = gzread(gz_in, read_buffer, sizeof(read_buffer));
        
        // Write compressed data to output file
        if (destLen > 0) {
            fwrite(dest, 1, destLen, out_file);
        }
    }

    // Clean up: release all allocated resources
    deflateEnd(&strm);
    free(dest);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file we created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
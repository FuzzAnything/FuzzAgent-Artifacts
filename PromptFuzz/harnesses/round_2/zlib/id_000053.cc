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
//<ID> 133
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"inflateSetDictionary":[[1194,9,1194,32,0,0,4,0]]},"library_calls":["compressBound_z","compress_z","deflateInit2_","inflateSetDictionary","deflateEnd","inflateCopy","deflateEnd","inflateGetDictionary","deflatePrime","deflateEnd","deflateEnd","gzdopen","deflateEnd","deflateEnd","gzclearerr","gzclose_r","deflateEnd","deflateEnd"],"critical_calls":["compressBound_z","compress_z","deflateInit2_","inflateSetDictionary","inflateCopy","inflateGetDictionary","deflatePrime","gzdopen","gzclearerr","gzclose_r","deflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;  // Need minimum data for meaningful operations
    }

    // Split input data for different purposes
    size_t dict_size = size / 4;
    size_t compress_size = size - dict_size;
    const Bytef* dict_data = data + compress_size;
    
    // Step 1: Create compressed data using compress_z
    z_size_t destLen = compressBound_z(compress_size);
    Bytef* compressed_data = (Bytef*)malloc(destLen);
    if (!compressed_data) {
	return 0;
	}
	
    
    int compress_result = compress_z(compressed_data, &destLen, data, compress_size);
    if (compress_result != Z_OK) {
        free(compressed_data);
        return 0;
    }

    // Step 2: Initialize inflation stream with deflateInit2_
    z_stream inflate_stream;
    memset(&inflate_stream, 0, sizeof(z_stream));
    
    int init_result = deflateInit2_(&inflate_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                                    15 + 16, 8, Z_DEFAULT_STRATEGY,
                                    ZLIB_VERSION, sizeof(z_stream));
    if (init_result != Z_OK) {
        free(compressed_data);
        return 0;
    }

    // Step 3: Set dictionary for inflation
    int dict_result = inflateSetDictionary(&inflate_stream, dict_data, dict_size);
    if (dict_result != Z_OK) {
        deflateEnd(&inflate_stream);
        free(compressed_data);
        return 0;
    }

    // Step 4: Create copy of inflation stream
    z_stream inflate_copy;
    memset(&inflate_copy, 0, sizeof(z_stream));
    
    int copy_result = inflateCopy(&inflate_copy, &inflate_stream);
    if (copy_result != Z_OK) {
        deflateEnd(&inflate_stream);
        free(compressed_data);
        return 0;
    }

    // Step 5: Get dictionary back
    Bytef* retrieved_dict = (Bytef*)malloc(dict_size);
    if (retrieved_dict) {
        uInt retrieved_size = dict_size;
        inflateGetDictionary(&inflate_stream, retrieved_dict, &retrieved_size);
        free(retrieved_dict);
    }

    // Step 6: Use deflatePrime to insert bits
    // Use first byte of data for bits and value
    int prime_result = deflatePrime(&inflate_stream, data[0] & 7, data[0] >> 3);
    // We don't check result as this is optional operation

    // Step 7: Create gzFile for compressed data
    FILE* in_file = fmemopen((void*)compressed_data, destLen, "rb");
    if (!in_file) {
        deflateEnd(&inflate_stream);
        deflateEnd(&inflate_copy);
        free(compressed_data);
        assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, "rb");
    if (!gz_file) {
        assert_file_closed(&in_file);;
        deflateEnd(&inflate_stream);
        deflateEnd(&inflate_copy);
        free(compressed_data);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Step 8: Clear any errors on gzFile
    gzclearerr(gz_file);

    // Step 9: Close gzFile for reading
    gzclose_r(gz_file);
    // Note: gzclose_r closes the underlying file descriptor, so we don't need fclose

    // Clean up
    deflateEnd(&inflate_stream);
    deflateEnd(&inflate_copy);
    free(compressed_data);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
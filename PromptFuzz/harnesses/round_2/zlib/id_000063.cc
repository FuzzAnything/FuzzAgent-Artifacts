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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"deflateSetDictionary":[[580,9,580,32,0,0,4,0],[581,13,581,22,0,0,4,1]]},"library_calls":["gzdopen","gzbuffer","gzclose","gzgets","gztell","gzoffset","gzoffset64","deflateInit_","gzclose","deflateSetDictionary","deflateEnd","gzclose","deflateGetDictionary","deflateEnd","gzclose","deflatePending","deflateEnd","gzclose","gzdopen","deflateEnd","gzclose","gzwrite","gzclose_w","deflateEnd","gzclose"],"critical_calls":["gzdopen","gzbuffer","gzgets","gztell","gzoffset","gzoffset64","deflateInit_","deflateSetDictionary","deflateGetDictionary","deflatePending","gzdopen","gzwrite","gzclose_w","deflateEnd","gzclose"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs to achieve the event of reading and manipulating compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Create a FILE* from input data
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

    // 1. gzdopen - open file descriptor as gzFile for reading
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_read = gzdopen(fd_in, "rb");
    if (!gz_read) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 2. gzbuffer - set buffer size for reading
    if (gzbuffer(gz_read, 8192) != 0) {
        gzclose(gz_read);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 3. gzgets - read a line from the compressed file
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    gzgets(gz_read, buffer, sizeof(buffer) - 1);

    // 4. gztell - get current position
    off_t tell_pos = gztell(gz_read);

    // 5. gzoffset - get offset (same as gztell for uncompressed data)
    off_t offset_pos = gzoffset(gz_read);

    // 6. gzoffset64 - get offset with 64-bit precision
    off64_t offset64_pos = gzoffset64(gz_read);

    // 7. Set up deflate stream for dictionary operations
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    // Initialize deflate stream
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        gzclose(gz_read);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 8. deflateSetDictionary - set dictionary for compression
    // Use part of the input data as dictionary (max 32768 bytes as per zlib spec)
    uInt dict_size = size > 32768 ? 32768 : static_cast<uInt>(size);
    if (deflateSetDictionary(&strm, data, dict_size) != Z_OK) {
        deflateEnd(&strm);
        gzclose(gz_read);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 9. deflateGetDictionary - retrieve the dictionary
    Bytef retrieved_dict[32768];
    uInt retrieved_dict_len = sizeof(retrieved_dict);
    if (deflateGetDictionary(&strm, retrieved_dict, &retrieved_dict_len) != Z_OK) {
        deflateEnd(&strm);
        gzclose(gz_read);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 10. deflatePending - check pending output
    unsigned int pending;
    int bits;
    if (deflatePending(&strm, &pending, &bits) != Z_OK) {
        deflateEnd(&strm);
        gzclose(gz_read);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 11. Open a gzFile for writing
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_write = gzdopen(fd_out, "wb");
    if (!gz_write) {
        deflateEnd(&strm);
        gzclose(gz_read);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Write some data to the output gzFile
    const char *test_data = "Test compressed data\n";
    gzwrite(gz_write, test_data, strlen(test_data));

    // 12. gzclose_w - close writing gzFile
    gzclose_w(gz_write);

    // Clean up remaining resources
    deflateEnd(&strm);
    gzclose(gz_read);
    assert_file_closed(&in_file);;
    
    // Close the output FILE* (gzclose_w already closed the fd)
    // But we need to close the FILE* too
    assert_file_closed(&out_file);;

    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
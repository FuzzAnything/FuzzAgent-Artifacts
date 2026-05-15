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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 1
//<Quality> {"density":6,"unique_branches":{"crc32_combine_gen64":[[955,9,955,17,0,0,4,0]]},"library_calls":["gzdopen","gzbuffer","inflateInit_","gzclose_r","inflateSetDictionary","deflateInit_","inflateEnd","gzclose_r","deflateTune","gzread","inflate","crc32_combine_gen64","inflateEnd","deflateEnd","gzclose_r"],"critical_calls":["gzdopen","gzbuffer","inflateInit_","inflateSetDictionary","deflateInit_","deflateTune","gzread","inflate","crc32_combine_gen64","inflateEnd","deflateEnd","gzclose_r"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified zlib APIs in a logical sequence:

*/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

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

    // 1. Create gzFile for reading compressed data
    int fd = fuzz_fileno(in_file);
    gzFile gz_input = gzdopen(fd, "rb");
    if (!gz_input) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 2. Set buffer size for gzFile
    gzbuffer(gz_input, 8192);

    // 3. Prepare decompression stream
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    
    // Initialize inflate with default parameters
    if (inflateInit(&strm) != Z_OK) {
        gzclose_r(gz_input);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Set dictionary for inflation
    // Use first 32 bytes as dictionary (if available)
    uInt dict_size = size > 32 ? 32 : (uInt)size;
    Bytef dictionary[32];
    memcpy(dictionary, data, dict_size);
    
    // Try to set dictionary (might fail if data doesn't expect one)
    inflateSetDictionary(&strm, dictionary, dict_size);

    // 5. Create separate compression stream for deflateTune
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    
    // Initialize deflate stream
    if (deflateInit(&def_strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        inflateEnd(&strm);
        gzclose_r(gz_input);
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 6. Tune deflation parameters using random values from input
    int good_length = data[0] % 64 + 8;
    int max_lazy = data[1] % 128 + 8;
    int nice_length = data[2] % 258 + 32;
    int max_chain = data[3] % 4096 + 4;
    
    deflateTune(&def_strm, good_length, max_lazy, nice_length, max_chain);

    // 7. Read from gzFile and decompress/inflate
    Bytef in_buffer[4096];
    Bytef out_buffer[8192];
    int bytes_read;
    
    while ((bytes_read = gzread(gz_input, in_buffer, sizeof(in_buffer))) > 0) {
        strm.next_in = in_buffer;
        strm.avail_in = bytes_read;
        
        do {
            strm.next_out = out_buffer;
            strm.avail_out = sizeof(out_buffer);
            
            int ret = inflate(&strm, Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
                break;
            }
            
            size_t have = sizeof(out_buffer) - strm.avail_out;
            if (have > 0) {
                fwrite(out_buffer, 1, have, out_file);
            }
            
        } while (strm.avail_out == 0);
    }

    // 8. Generate CRC32 combine operation using offset from input
    off64_t offset = 0;
    if (size >= sizeof(off64_t) + 8) {
        memcpy(&offset, data + size - sizeof(off64_t), sizeof(off64_t));
    }
    
    // Generate CRC combine operation
    uLong crc_combine_op = crc32_combine_gen64(offset);
    
    // 9. Clean up
    inflateEnd(&strm);
    deflateEnd(&def_strm);
    gzclose_r(gz_input);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");

    // Use the CRC combine operation to avoid compiler optimization
    volatile uLong dummy = crc_combine_op;
    (void)dummy;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
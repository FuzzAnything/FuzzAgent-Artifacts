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
//<ID> 277
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"crc32_combine_gen64":[[955,9,955,17,0,0,4,0]]},"library_calls":["gzdopen","inflateInit_","gzclose","inflateValidate","gzread","inflateGetDictionary","crc32_z","deflateInit_","inflateEnd","gzclose","deflate","inflateMark","deflatePending","deflateBound","gzdopen","gzputs","crc32_combine64","gzclose","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","inflateInit_","inflateValidate","gzread","inflateGetDictionary","crc32_z","deflateInit_","deflate","inflateMark","deflatePending","deflateBound","gzdopen","gzputs","crc32_combine64","gzclose","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small for meaningful operations
    if (size < 10) {
	return 0;
	}
	

    // Create temporary files for reading/writing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Step 1: Create gzFile for reading compressed data
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 2: Initialize inflate stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    
    if (inflateInit_(&inflate_strm, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 3: Set validation option for inflate
    inflateValidate(&inflate_strm, 1);

    // Step 4: Read from gzFile
    Bytef read_buffer[1024];
    unsigned int bytes_read = gzread(gz_in, read_buffer, sizeof(read_buffer));
    
    // Step 5: Get dictionary from inflate stream
    Bytef dict_buffer[256];
    uInt dict_len = sizeof(dict_buffer);
    inflateGetDictionary(&inflate_strm, dict_buffer, &dict_len);

    // Step 6: Calculate CRC32 of read data
    uLong read_crc = crc32_z(0L, read_buffer, bytes_read);

    // Step 7: Initialize deflate stream for compression
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    if (deflateInit_(&deflate_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&inflate_strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Step 8: Set up output buffer for compression
    Bytef out_buffer[2048];
    deflate_strm.avail_in = bytes_read;
    deflate_strm.next_in = read_buffer;
    deflate_strm.avail_out = sizeof(out_buffer);
    deflate_strm.next_out = out_buffer;

    // Step 9: Perform compression
    int deflate_result = deflate(&deflate_strm, Z_FINISH);

    // Step 10: Get mark position from inflate stream
    long mark_pos = inflateMark(&inflate_strm);

    // Step 11: Get pending data from deflate stream
    unsigned int pending;
    int bits;
    deflatePending(&deflate_strm, &pending, &bits);

    // Step 12: Get bound for compressed size
    uLong bound_size = deflateBound(&deflate_strm, bytes_read);

    // Step 13: Create gzFile for writing
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd_out, "wb");
    if (gz_out) {
        // Step 14: Write to gzFile
        const char *test_string = "Fuzz test output";
        gzputs(gz_out, test_string);
        
        // Step 15: Combine CRCs (using mark position as offset)
        uLong combined_crc = crc32_combine64(read_crc, 0L, (off64_t)mark_pos);
        
        // Clean up gz output
        gzclose(gz_out);
    }

    // Step 16: Clean up deflate stream
    deflateEnd(&deflate_strm);
    
    // Step 17: Clean up inflate stream
    inflateEnd(&inflate_strm);
    
    // Step 18: Clean up gz input
    gzclose(gz_in);
    
    // Clean up files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
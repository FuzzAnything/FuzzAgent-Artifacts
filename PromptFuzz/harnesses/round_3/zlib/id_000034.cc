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
//<ID> 61
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 3
//<Quality> {"density":16,"unique_branches":{"gzungetc":[[525,9,525,20,0,0,4,0],[525,24,525,44,0,0,4,1]],"deflateBound_z":[[893,13,893,32,0,0,4,1]]},"library_calls":["gzdopen","gzdirect","gzerror","gzseek64","gzoffset","gzungetc","inflateInit2_","gzclose","inflateGetHeader","inflateEnd","gzclose","inflatePrime","inflateSetDictionary","inflate","deflateInit2_","deflateBound","deflate","deflateGetDictionary","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gzerror","gzseek64","gzoffset","gzungetc","inflateInit2_","inflateGetHeader","inflatePrime","inflateSetDictionary","inflate","deflateInit2_","deflateBound","deflate","deflateGetDictionary","deflateEnd","inflateEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<71) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for gzdopen
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Open compressed data as gzFile
    gzFile gz = gzdopen(fd, fuzz_str_1);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Check if file is in gzip format
    int is_direct = gzdirect(gz);
    
    // Get error state
    int errnum = 0;
    const char *error_msg = gzerror(gz, &errnum);
    (void)error_msg; // Prevent unused variable warning

    // Try seeking in the file (use small offset to avoid issues)
    off64_t seek_pos = gzseek64(gz, size > 10 ? 10 : 0, fuzz_int32_t_2);
    (void)seek_pos;
    
    // Get current offset
    off_t offset = gzoffset(gz);
    (void)offset;

    // Try to unget a character (use first byte of input)
    if (size > 0) {
        gzungetc(data[0], gz);
    }

    // Now use inflate APIs to decompress the data
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflation with gzip header detection
    int ret = inflateInit2_(&strm, 15 + 16, fuzz_str_3, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Setup gzip header structure
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&strm, &header);

    // Set input data for inflation
    strm.next_in = (Bytef *)data;
    strm.avail_in = (uInt)(size > UINT_MAX ? UINT_MAX : size);
    
    // Allocate output buffer
    size_t out_size = size * 2 + 1024;
    Bytef *out_buf = (Bytef *)malloc(out_size);
    if (!out_buf) {
        inflateEnd(&strm);
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    strm.next_out = out_buf;
    strm.avail_out = (uInt)(out_size > UINT_MAX ? UINT_MAX : out_size);

    // Try to prime the inflate stream with some bits
    if (size > 0) {
        inflatePrime(&strm, fuzz_int32_t_4, data[0]);
    }

    // Try to set a dictionary (use first few bytes as dictionary)
    if (size > 32) {
        inflateSetDictionary(&strm, data, 32);
    }

    // Inflate the data
    ret = inflate(&strm, fuzz_int32_t_5);
    (void)ret; // Result may be Z_OK, Z_STREAM_END, or error

    // Now try compression using deflate APIs
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    // Initialize deflation with gzip format
    ret = deflateInit2_(&def_strm, fuzz_int32_t_7, fuzz_int32_t_8,
                        15 + 16, fuzz_int32_t_9, fuzz_int32_t_10,
                        fuzz_str_6, sizeof(z_stream));
    if (ret == Z_OK) {
        // Set input for compression (use inflated data if available)
        def_strm.next_in = out_buf;
        def_strm.avail_in = (uInt)(strm.total_out > UINT_MAX ? UINT_MAX : strm.total_out);
        
        // Allocate compression output buffer
        size_t comp_size = deflateBound(&def_strm, def_strm.avail_in);
        Bytef *comp_buf = (Bytef *)malloc(comp_size);
        if (comp_buf) {
            def_strm.next_out = comp_buf;
            def_strm.avail_out = (uInt)(comp_size > UINT_MAX ? UINT_MAX : comp_size);
            
            // Compress the data
            ret = deflate(&def_strm, fuzz_int32_t_11);
            (void)ret;
            
            // Try to get dictionary (though we didn't set one)
            Bytef dict_buf[256];
            uInt dict_len = 256;
            deflateGetDictionary(&def_strm, dict_buf, &dict_len);
            
            free(comp_buf);
        }
        
        // Clean up deflate stream
        deflateEnd(&def_strm);
    }

    // Clean up inflate stream
    inflateEnd(&strm);
    free(out_buf);

    // Close gzFile (this also closes the underlying file descriptor)
    gzclose(gz);
    
    // Note: assert_file_closed(&in_file); is not needed here because gzdopen took ownership of the fd
    // and gzclose already closed it. We just need to free the FILE* structure.
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
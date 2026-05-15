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
//<ID> 44
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"inflatePrime":[[231,22,231,51,0,0,4,0]]},"library_calls":["inflateInit2_","deflateInit2_","inflateEnd","inflateUndermine","zError","inflate","inflatePrime","zError","deflateGetDictionary","inflateSetDictionary","inflateCopy","inflateEnd","inflateEnd","deflateEnd"],"critical_calls":["inflateInit2_","deflateInit2_","inflateUndermine","zError","inflate","inflatePrime","zError","deflateGetDictionary","inflateSetDictionary","inflateCopy","inflateEnd","inflateEnd","deflateEnd"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Create input file from the fuzzer data
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

    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Initialize zlib streams
    z_stream inflate_strm, deflate_strm, copy_strm;
    Bytef dictionary[1024];
    uInt dictLength = 0;
    int ret;
    const char *err_msg;

    // Setup inflate stream
    inflate_strm.zalloc = Z_NULL;
    inflate_strm.zfree = Z_NULL;
    inflate_strm.opaque = Z_NULL;
    inflate_strm.avail_in = 0;
    inflate_strm.next_in = Z_NULL;
    
    ret = inflateInit2(&inflate_strm, 15 + 16); // 15 window bits + 16 for gzip header
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Setup deflate stream for dictionary operations
    deflate_strm.zalloc = Z_NULL;
    deflate_strm.zfree = Z_NULL;
    deflate_strm.opaque = Z_NULL;
    
    ret = deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                       15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        inflateEnd(&inflate_strm);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Read input data in chunks
    Bytef in_buffer[4096];
    Bytef out_buffer[8192];
    
    while (!feof(in_file) && !ferror(in_file)) {
        size_t bytes_read = fread(in_buffer, 1, sizeof(in_buffer), in_file);
        if (bytes_read == 0) break;
        
        inflate_strm.avail_in = bytes_read;
        inflate_strm.next_in = in_buffer;
        
        // Process available input
        do {
            inflate_strm.avail_out = sizeof(out_buffer);
            inflate_strm.next_out = out_buffer;
            
            // Use inflateUndermine to test error handling
            ret = inflateUndermine(&inflate_strm, fuzz_int32_t_1);
            if (ret != Z_OK) {
                // Get error message
                err_msg = zError(ret);
            }
            
            ret = inflate(&inflate_strm, fuzz_int32_t_2);
            
            // Use inflatePrime to add bits back to stream (if needed)
            if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR) {
                inflatePrime(&inflate_strm, fuzz_int32_t_3, fuzz_int32_t_4);
            }
            
            // Use zError for error reporting
            if (ret < 0) {
                err_msg = zError(ret);
            }
            
            // Write decompressed output
            size_t have = sizeof(out_buffer) - inflate_strm.avail_out;
            if (have > 0) {
                fwrite(out_buffer, 1, have, out_file);
            }
        } while (inflate_strm.avail_out == 0);
    }

    // Try to get dictionary from deflate stream
    dictLength = sizeof(dictionary);
    ret = deflateGetDictionary(&deflate_strm, dictionary, &dictLength);
    
    // Set dictionary on inflate stream if we got one
    if (ret == Z_OK && dictLength > 0) {
        inflateSetDictionary(&inflate_strm, dictionary, dictLength);
    }

    // Create a copy of the inflate stream using inflateCopy
    copy_strm.zalloc = Z_NULL;
    copy_strm.zfree = Z_NULL;
    copy_strm.opaque = Z_NULL;
    
    ret = inflateCopy(&copy_strm, &inflate_strm);
    if (ret == Z_OK) {
        // Clean up the copy
        inflateEnd(&copy_strm);
    }

    // Clean up
    inflateEnd(&inflate_strm);
    deflateEnd(&deflate_strm);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"inflateBackInit_":[[34,27,34,42,0,0,4,0]]},"library_calls":["gzdopen","gzbuffer","gzungetc","gzrewind","inflateInit2_","gzclose","inflateEnd","gzclose","gzfread","inflateMark","inflate","inflateGetDictionary","inflateResetKeep","deflateInit_","deflateReset","deflateEnd","inflateBackInit_","inflateBackEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzbuffer","gzungetc","gzrewind","inflateInit2_","gzfread","inflateMark","inflate","inflateGetDictionary","inflateResetKeep","deflateInit_","deflateReset","deflateEnd","inflateBackInit_","inflateBackEnd","inflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to decompress zlib data and work with gzip files:

*/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
        return 0;
    }

    // Create a file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz_file = gzdopen(fd, "rb");
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 1. Set buffer size for gzFile
    int buffer_size = 4096;
    int buffer_result = gzbuffer(gz_file, buffer_size);
    (void)buffer_result; // Result not used in fuzzing

    // 2. Read and unget a character
    int read_char = gzgetc(gz_file);
    if (read_char != -1) {
        // Put character back to stream
        int unget_result = gzungetc(read_char, gz_file);
        (void)unget_result; // Result not used in fuzzing
    }

    // Reset file position for further operations
    gzrewind(gz_file);

    // Setup z_stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use inflateInit2 for gzip decompression
    int windowBits = 15 + 16; // 15 window bits + 16 for gzip header
    if (inflateInit2(&strm, windowBits) != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Allocate input and output buffers
    const size_t chunk_size = 4096;
    Bytef *in_buf = (Bytef *)malloc(chunk_size);
    Bytef *out_buf = (Bytef *)malloc(chunk_size);
    
    if (!in_buf || !out_buf) {
        free(in_buf);
        free(out_buf);
        inflateEnd(&strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Decompress the data
    int ret;
    size_t bytes_read;
    
    do {
        // Read from gzFile
        bytes_read = gzfread(in_buf, 1, chunk_size, gz_file);
        if (bytes_read == 0) break;
        
        strm.avail_in = (uInt)bytes_read;
        strm.next_in = in_buf;
        
        do {
            strm.avail_out = chunk_size;
            strm.next_out = out_buf;
            
            // 3. Get mark before inflation
            long mark = inflateMark(&strm);
            (void)mark; // Mark not used further
            
            ret = inflate(&strm, Z_NO_FLUSH);
            
            // Check for errors
            if (ret == Z_STREAM_ERROR) break;
            
            // 4. Try to get dictionary if available
            Bytef dict_buf[256];
            uInt dict_len = 256;
            int dict_result = inflateGetDictionary(&strm, dict_buf, &dict_len);
            (void)dict_result; // Result not used in fuzzing
            
            // Process decompressed data
            size_t have = chunk_size - strm.avail_out;
            
        } while (strm.avail_out == 0);
        
    } while (ret != Z_STREAM_END);

    // 5. Reset inflate stream but keep dictionary
    int reset_keep_result = inflateResetKeep(&strm);
    (void)reset_keep_result; // Result not used in fuzzing

    // Setup a deflate stream for testing deflateReset
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // 6. Reset deflate stream
        int deflate_reset_result = deflateReset(&deflate_strm);
        (void)deflate_reset_result; // Result not used in fuzzing
        
        deflateEnd(&deflate_strm);
    }

    // 7. Setup and end inflateBack stream (requires more setup)
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    
    // Use a simple window for inflateBack
    unsigned char window[32768];
    if (inflateBackInit(&back_strm, windowBits, window) == Z_OK) {
        // 8. End the inflateBack stream
        int back_end_result = inflateBackEnd(&back_strm);
        (void)back_end_result; // Result not used in fuzzing
    }

    // Cleanup
    free(in_buf);
    free(out_buf);
    inflateEnd(&strm);
    gzclose(gz_file);
    assert_file_closed(&in_file);;

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
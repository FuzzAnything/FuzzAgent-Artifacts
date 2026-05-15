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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 6
//<Quality> {"density":6,"unique_branches":{"deflateGetDictionary":[[636,33,636,36,0,0,4,1]],"gzseek64":[[384,9,384,27,0,0,4,0],[384,31,384,49,0,0,4,1],[388,9,388,27,0,0,4,1],[391,19,391,30,0,0,4,1]],"inflate":[[1134,37,1134,54,0,0,4,1]]},"library_calls":["gzdopen","gzdirect","gzerror","gzseek64","gzoffset","inflateInit2_","inflatePrime","inflate","inflateEnd","deflateInit2_","deflateGetDictionary","deflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gzerror","gzseek64","gzoffset","inflateInit2_","inflatePrime","inflate","inflateEnd","deflateInit2_","deflateGetDictionary","deflateEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create a temporary file with the compressed data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    gzFile gz = gzdopen(fd, fuzz_str_1);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Check if file is in transparent (non-compressed) mode
    int direct = gzdirect(gz);
    
    // Get error status
    int errnum = 0;
    const char *errmsg = gzerror(gz, &errnum);
    
    // Try to seek within the file using gzseek64
    // Use mod operation to ensure offset is within reasonable bounds
    off64_t seek_offset = (size > 0) ? static_cast<off64_t>(data[0] % 1024) : 0;
    off64_t seek_result = gzseek64(gz, seek_offset, fuzz_int32_t_2);
    
    // Get current offset
    off_t offset = gzoffset(gz);
    
    // Prepare inflation stream for decompression
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Initialize inflation stream
    if (inflateInit2(&inflate_strm, 15 + 32) == Z_OK) {  // 15 window bits + 32 for gzip decoding
        // Insert some bits into the stream
        // Use first byte of data for bits and value if available
        if (size > 1) {
            int bits = data[0] % 16;  // 0-15 bits
            int value = data[1];
            inflatePrime(&inflate_strm, bits, value);
        }
        
        // Set up input buffer
        Bytef *inflate_input = (Bytef *)malloc(size);
        if (inflate_input) {
            memcpy(inflate_input, data, size);
            inflate_strm.next_in = inflate_input;
            inflate_strm.avail_in = size;
            
            // Set up output buffer
            size_t out_size = size * 2;
            Bytef *inflate_output = (Bytef *)malloc(out_size);
            if (inflate_output) {
                inflate_strm.next_out = inflate_output;
                inflate_strm.avail_out = out_size;
                
                // Try to decompress
                inflate(&inflate_strm, fuzz_int32_t_3);
                
                free(inflate_output);
            }
            free(inflate_input);
        }
        inflateEnd(&inflate_strm);
    }
    
    // Prepare deflation stream for dictionary operations
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Initialize deflation stream
    if (deflateInit2(&deflate_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                     15 + 16, 8, Z_DEFAULT_STRATEGY) == Z_OK) {  // 15 window bits + 16 for gzip header
        
        // Try to get dictionary (even though we haven't set one)
        Bytef dictionary[1024];
        uInt dictLength = 0;
        deflateGetDictionary(&deflate_strm, dictionary, &dictLength);
        
        deflateEnd(&deflate_strm);
    }
    
    // Clean up
    gzclose(gz);  // This also closes the underlying file descriptor
    
    // Note: in_file was closed by gzclose, so don't fclose it again
    // The file descriptor fd is now closed
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
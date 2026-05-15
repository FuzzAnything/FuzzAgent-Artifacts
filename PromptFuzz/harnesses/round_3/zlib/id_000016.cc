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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 6
//<Quality> {"density":6,"unique_branches":{"inflatePrime":[[223,9,223,18,0,0,4,0]],"deflateGetDictionary":[[630,9,630,32,0,0,4,1],[634,9,634,24,0,0,4,1],[636,9,636,29,0,0,4,0],[636,33,636,36,0,0,4,0],[638,9,638,29,0,0,4,0]]},"library_calls":["gzdopen","gzdirect","gzoffset","gzseek64","gzerror","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateEnd","inflateInit2_","inflatePrime","inflate","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gzoffset","gzseek64","gzerror","deflateInit2_","deflateSetDictionary","deflateGetDictionary","deflateEnd","inflateInit2_","inflatePrime","inflate","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<13) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gz = gzdopen(fd, fuzz_str_1);
    if (!gz) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Check if file is in direct (uncompressed) mode
    int is_direct = gzdirect(gz);
    
    // Get current offset in file
    off_t offset = gzoffset(gz);
    
    // Try to seek to a position (handle both forward and backward seeks)
    off64_t seek_pos = size > 10 ? 10 : size / 2;
    off64_t new_offset = gzseek64(gz, seek_pos, fuzz_int32_t_2);
    
    // Check for errors after seek
    int err_num = 0;
    const char *error_msg = gzerror(gz, &err_num);
    
    // Create deflate stream for dictionary operations
    z_stream def_strm;
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    int deflate_init_result = deflateInit2(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                                           MAX_WBITS, 8, Z_DEFAULT_STRATEGY);
    
    if (deflate_init_result == Z_OK) {
        // Try to set dictionary from input data
        uInt dict_len = size > 256 ? 256 : (uInt)size;
        int dict_result = deflateSetDictionary(&def_strm, data, dict_len);
        
        if (dict_result == Z_OK) {
            // Get the dictionary back
            uInt get_dict_len = 0;
            Bytef *dictionary = (Bytef *)malloc(dict_len);
            if (dictionary) {
                int get_result = deflateGetDictionary(&def_strm, dictionary, &get_dict_len);
                free(dictionary);
            }
        }
        
        // Clean up deflate stream
        deflateEnd(&def_strm);
    }
    
    // Create inflate stream for inflatePrime
    z_stream inf_strm;
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    inf_strm.avail_in = 0;
    inf_strm.next_in = Z_NULL;
    
    int inflate_init_result = inflateInit2(&inf_strm, MAX_WBITS);
    
    if (inflate_init_result == Z_OK) {
        // Set up input buffer for inflate
        inf_strm.avail_in = (uInt)(size > 1024 ? 1024 : size);
        inf_strm.next_in = (Bytef *)data;
        
        // Create output buffer
        Bytef out_buffer[1024];
        inf_strm.avail_out = sizeof(out_buffer);
        inf_strm.next_out = out_buffer;
        
        // Try to prime the inflate stream with some bits from input
        if (size > 2) {
            int bits = data[0] & 0x07;  // Use 0-7 bits
            int value = data[1] & 0xFF; // Use a byte as value
            inflatePrime(&inf_strm, bits, value);
        }
        
        // Try to inflate a small amount
        inflate(&inf_strm, fuzz_int32_t_3);
        
        // Clean up inflate stream
        inflateEnd(&inf_strm);
    }
    
    // Clean up resources
    gzclose(gz);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
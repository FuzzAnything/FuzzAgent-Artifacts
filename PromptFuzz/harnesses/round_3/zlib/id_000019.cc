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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 5
//<Quality> {"density":7,"unique_branches":{"inflate":[[1150,22,1150,30,0,0,4,1]],"adler32_z":[[72,13,72,26,0,0,4,0]],"inflateSetDictionary":[[1194,9,1194,32,0,0,4,1],[1196,9,1196,25,0,0,4,0],[1196,29,1196,48,0,0,4,0]]},"library_calls":["gzdopen","gzdirect","gzoffset","gzseek64","gzerror","inflateInit_","gzclose","inflatePrime","deflateInit_","deflateSetDictionary","deflateGetDictionary","inflateSetDictionary","deflateEnd","inflate","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gzoffset","gzseek64","gzerror","inflateInit_","inflatePrime","deflateInit_","deflateSetDictionary","deflateGetDictionary","inflateSetDictionary","deflateEnd","inflate","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // 1. Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Get file descriptor and open as gzFile
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    gzFile gz_file = gzdopen(fd, fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Check if file is in direct mode (gzdirect)
    int is_direct = gzdirect(gz_file);
    
    // 4. Get current offset (gzoffset)
    off_t current_offset = gzoffset(gz_file);
    
    // 5. Seek to a new position (gzseek64)
    // Use a safe offset within bounds
    off64_t seek_pos = (size > 0) ? (off64_t)(size / 2) : 0;
    off64_t new_offset = gzseek64(gz_file, seek_pos, fuzz_int32_t_2);
    
    // 6. Check for errors (gzerror)
    int errnum = 0;
    const char *error_msg = gzerror(gz_file, &errnum);
    
    // 7. Set up inflate stream for decompression
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Initialize inflate stream
    if (inflateInit(&strm) != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 8. Prime the inflate stream with bits from input (inflatePrime)
    // Use first 16 bits of data if available
    if (size >= 2) {
        int bits = 16;
        int value = (data[0] << 8) | data[1];
        inflatePrime(&strm, fuzz_int32_t_3, value);
    }
    
    // 9. Get dictionary from deflate stream (deflateGetDictionary)
    // First create a deflate stream to get dictionary from
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(def_strm));
    
    if (deflateInit(&def_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
        // Set a dictionary using part of input data
        uInt dict_len = (size > 1024) ? 1024 : (uInt)size;
        if (dict_len > 0) {
            deflateSetDictionary(&def_strm, data, dict_len);
            
            // Now get the dictionary back
            Bytef dict_buffer[1024];
            uInt retrieved_len = 1024;
            int dict_result = deflateGetDictionary(&def_strm, dict_buffer, &retrieved_len);
            
            // Use the retrieved dictionary for inflation if successful
            if (dict_result == Z_OK && retrieved_len > 0) {
                inflateSetDictionary(&strm, dict_buffer, retrieved_len);
            }
        }
        
        // Clean up deflate stream
        deflateEnd(&def_strm);
    }
    
    // 10. Prepare buffers for inflation
    const size_t out_buf_size = 4096;
    Bytef out_buffer[out_buf_size];
    
    strm.avail_in = (uInt)(size > 0 ? size : 0);
    strm.next_in = (Bytef*)data;
    
    // 11. Perform inflation
    do {
        strm.avail_out = out_buf_size;
        strm.next_out = out_buffer;
        
        int ret = inflate(&strm, fuzz_int32_t_4);
        
        if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
            break;
        }
        
    } while (strm.avail_out == 0);
    
    // 12. Clean up inflate stream
    inflateEnd(&strm);
    
    // 13. Close gzFile and FILE*
    gzclose(gz_file);
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
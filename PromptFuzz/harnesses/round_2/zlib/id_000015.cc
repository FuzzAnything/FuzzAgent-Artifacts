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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 3.5, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["zlibVersion","gzdopen","gzdirect","inflateInit2_","gzclose","inflateSetDictionary","gzread","inflate","crc32","gzeof","inflateReset","deflateInit2_","deflateSetDictionary","deflateEnd","crc32","gzeof","inflateEnd","gzclose"],"critical_calls":["zlibVersion","gzdopen","gzdirect","inflateInit2_","inflateSetDictionary","gzread","inflate","crc32","inflateReset","deflateInit2_","deflateSetDictionary","deflateEnd","crc32","gzeof","inflateEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get zlib version first
    const char *version = zlibVersion();
    if (version == nullptr) {
        return 0;
    }

    // 2. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 3. Open as gzFile for reading
    gzFile gz_in = gzdopen(fd, "rb");
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Check if file is in direct mode
    int is_direct = gzdirect(gz_in);

    // 5. Set up inflation stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    // Initialize inflation with automatic header detection
    int ret = inflateInit2(&strm, 15 + 32);  // 15 window bits + 32 for automatic header detection
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 6. Create output buffer for inflated data
    Bytef out_buffer[4096];
    strm.avail_out = sizeof(out_buffer);
    strm.next_out = out_buffer;

    // 7. Prepare dictionary from input data (use first portion as dictionary)
    uInt dict_len = size > 1024 ? 1024 : (uInt)size;
    Bytef *dictionary = (Bytef *)malloc(dict_len);
    if (dictionary && size > 0) {
        memcpy(dictionary, data, dict_len);
        
        // Set dictionary for inflation (this may fail if not needed, but we call it anyway)
        int dict_ret = inflateSetDictionary(&strm, dictionary, dict_len);
        (void)dict_ret;  // Result ignored for fuzzing
    }

    // 8. Read from gzFile and inflate
    uLong crc = 0L;
    bool eof_reached = false;
    
    while (!eof_reached) {
        // Read compressed data from gzFile
        Bytef in_buffer[1024];
        int bytes_read = gzread(gz_in, in_buffer, sizeof(in_buffer));
        
        if (bytes_read > 0) {
            strm.avail_in = bytes_read;
            strm.next_in = in_buffer;
            
            // Inflate the data
            ret = inflate(&strm, Z_NO_FLUSH);
            
            if (ret == Z_STREAM_END) {
                eof_reached = true;
            } else if (ret != Z_OK && ret != Z_BUF_ERROR) {
                break;  // Error occurred
            }
            
            // Calculate CRC of output if we have any
            uLong bytes_inflated = sizeof(out_buffer) - strm.avail_out;
            if (bytes_inflated > 0) {
                crc = crc32(crc, out_buffer, (uInt)bytes_inflated);
            }
            
            // Reset output buffer
            strm.avail_out = sizeof(out_buffer);
            strm.next_out = out_buffer;
        } else if (bytes_read == 0) {
            // Check if we're at EOF
            eof_reached = (gzeof(gz_in) != 0);
            break;
        } else {
            // Error reading from gzFile
            break;
        }
    }

    // 9. Reset the inflation stream (required API call)
    inflateReset(&strm);

    // 10. Set up a deflation stream for dictionary test
    z_stream def_strm;
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    
    if (deflateInit2(&def_strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                    15, 8, Z_DEFAULT_STRATEGY) == Z_OK) {
        
        // Try to set dictionary on deflation stream (use input data as dictionary)
        if (dictionary && dict_len > 0) {
            int set_dict_ret = deflateSetDictionary(&def_strm, dictionary, dict_len);
            (void)set_dict_ret;  // Result ignored for fuzzing
        }
        
        // Clean up deflation stream
        deflateEnd(&def_strm);
    }

    // 11. Calculate final CRC on any remaining output
    uLong final_bytes = sizeof(out_buffer) - strm.avail_out;
    if (final_bytes > 0) {
        crc = crc32(crc, out_buffer, (uInt)final_bytes);
    }

    // 12. Check gzFile EOF status one more time
    int final_eof = gzeof(gz_in);
    (void)final_eof;  // Use the result
    (void)is_direct;  // Use the result
    (void)crc;        // Use the CRC result

    // 13. Clean up
    inflateEnd(&strm);
    gzclose(gz_in);
    // Note: gzclose already closes the file descriptor, 
    // so we don't need to assert_file_closed(&in_file); separately
    
    if (dictionary) {
        free(dictionary);
    }

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
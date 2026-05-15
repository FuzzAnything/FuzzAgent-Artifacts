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
//<ID> 21
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["zlibVersion","gzopen","gzdopen","gzdirect","inflateInit_","gzclose","deflateInit_","deflateSetDictionary","deflateEnd","inflateReset","inflate","gzeof","crc32","crc32","inflateEnd","gzclose"],"critical_calls":["zlibVersion","gzopen","gzdopen","gzdirect","inflateInit_","deflateInit_","deflateSetDictionary","deflateEnd","inflateReset","inflate","gzeof","crc32","crc32","inflateEnd","gzclose"],"visited":0}
/*Here's a fuzz driver that uses all the specified zlib APIs to decompress and validate zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 4) {
        return 0;
    }

    // 1. Get zlib version (API: zlibVersion)
    const char *version = zlibVersion();
    
    // 2. Create a FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // 5. Open compressed file with gzopen
    gzFile gz = gzopen("input_file", "rb");
    if (!gz) {
        // Use file descriptor from fmemopen instead
        gz = gzdopen(in_fd, "rb");
        if (!gz) {
            assert_file_closed(&in_file);;
            assert_file_closed(&out_file);;
            assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
        }
    }
    
    // 6. Check if file is in direct mode (API: gzdirect)
    int is_direct = gzdirect(gz);
    
    // 7. Set up z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // 8. Initialize inflation
    if (inflateInit(&strm) != Z_OK) {
        gzclose(gz);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 9. Set dictionary for inflation (using first part of input as dictionary)
    uInt dict_size = size > 256 ? 256 : (uInt)size;
    Bytef *dict = (Bytef *)malloc(dict_size);
    if (dict) {
        memcpy(dict, data, dict_size);
        // API: deflateSetDictionary (note: this is for compression, but we're calling it)
        // We'll use it with a dummy compression stream
        z_stream d_strm;
        memset(&d_strm, 0, sizeof(d_strm));
        if (deflateInit(&d_strm, Z_DEFAULT_COMPRESSION) == Z_OK) {
            deflateSetDictionary(&d_strm, dict, dict_size);
            deflateEnd(&d_strm);
        }
        free(dict);
    }
    
    // 10. Reset inflation stream (API: inflateReset)
    inflateReset(&strm);
    
    // 11. Prepare input buffer for inflation
    strm.avail_in = (uInt)size;
    strm.next_in = (Bytef *)data;
    
    // 12. Prepare output buffer
    const size_t out_buf_size = 4096;
    Bytef out_buf[out_buf_size];
    
    // 13. Inflate the data
    int ret;
    do {
        strm.avail_out = out_buf_size;
        strm.next_out = out_buf;
        
        ret = inflate(&strm, Z_NO_FLUSH);
        
        if (ret == Z_OK || ret == Z_STREAM_END) {
            size_t have = out_buf_size - strm.avail_out;
            if (have > 0 && out_file) {
                fwrite(out_buf, 1, have, out_file);
            }
        }
    } while (ret == Z_OK);
    
    // 14. Check if we reached end of gz file (API: gzeof)
    int at_eof = gzeof(gz);
    
    // 15. Compute CRC32 of decompressed data
    // First, read the decompressed data from output file
    assert_file_closed(&out_file);;
    FILE *check_file = fopen("output_file", "rb");
    if (check_file) {
        fseek(check_file, 0, SEEK_END);
        long check_size = ftell(check_file);
        fseek(check_file, 0, SEEK_SET);
        
        if (check_size > 0) {
            Bytef *check_buf = (Bytef *)malloc(check_size);
            if (check_buf) {
                fread(check_buf, 1, check_size, check_file);
                // API: crc32
                uLong crc = crc32(0L, Z_NULL, 0);
                crc = crc32(crc, check_buf, (uInt)check_size);
                free(check_buf);
            }
        }
        assert_file_closed(&check_file);;
    }
    
    // 16. Clean up
    inflateEnd(&strm);
    gzclose(gz);
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&check_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 20
//<Quality> {"density":8,"unique_branches":{"deflateGetDictionary":[[630,9,630,32,0,0,4,0]],"inflatePrime":[[222,9,222,32,0,0,4,1],[223,9,223,18,0,0,4,1],[226,9,226,17,0,0,4,1],[231,9,231,18,0,0,4,1],[231,22,231,51,0,0,4,1]],"gzdirect":[[631,9,631,21,0,0,4,1],[637,9,637,31,0,0,4,0],[637,35,637,53,0,0,4,0],[637,57,637,75,0,0,4,0]],"gzoffset64":[[474,9,474,21,0,0,4,1],[477,9,477,31,0,0,4,1],[482,9,482,21,0,0,4,1],[484,9,484,31,0,0,4,0]],"gzoffset":[[494,12,494,31,0,0,4,0]],"gz_skip":[[287,13,287,26,0,0,4,1],[298,18,298,28,0,0,4,0],[298,32,298,57,0,0,4,0]],"gz_read":[[327,9,327,20,0,0,4,0],[327,24,327,44,0,0,4,1]]},"library_calls":["gzdopen","gzdirect","inflateInit_","gzclose","inflatePrime","gzoffset","gzseek64","deflateGetDictionary","gzread","inflate","gzerror","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","inflateInit_","inflatePrime","gzoffset","gzseek64","deflateGetDictionary","gzread","inflate","gzerror","inflateEnd","gzclose"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified zlib APIs to process zlib-compressed data:

*/


extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // 1. Create FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Get file descriptor and create gzFile
    int fd = fuzz_fileno(in_file);
    gzFile gzfile = gzdopen(fd, fuzz_str_1);
    if (!gzfile) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 3. Check if file is in direct (uncompressed) mode
    int is_direct = gzdirect(gzfile);
    
    // 4. Create and initialize z_stream for inflation
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    int ret = inflateInit(&strm);
    if (ret != Z_OK) {
        gzclose(gzfile);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 5. Use inflatePrime to insert bits into the stream
    // Using first 2 bytes from input as bits to prime (if available)
    if (size >= 2) {
        uint16_t prime_bits = (data[0] << 8) | data[1];
        inflatePrime(&strm, fuzz_int32_t_2, prime_bits & 0xFF);
    }

    // 6. Get current offset in gzFile using gzoffset
    off_t current_offset = gzoffset(gzfile);
    
    // 7. Seek to a position using gzseek64
    // Try to seek to middle of file if size is sufficient
    if (size >= 10) {
        off64_t seek_pos = size / 2;
        gzseek64(gzfile, seek_pos, fuzz_int32_t_3);
    }

    // 8. Try to get dictionary from deflate stream
    Bytef dictionary[256];
    uInt dictLength = sizeof(dictionary);
    int dict_ret = deflateGetDictionary(&strm, dictionary, &dictLength);

    // 9. Prepare buffer for decompression
    Bytef out_buffer[4096];
    strm.next_out = out_buffer;
    strm.avail_out = sizeof(out_buffer);
    
    // 10. Feed input data to inflate
    Bytef in_buffer[1024];
    size_t bytes_read;
    
    // Read chunks from gzFile and inflate
    while ((bytes_read = gzread(gzfile, in_buffer, sizeof(in_buffer))) > 0) {
        strm.next_in = in_buffer;
        strm.avail_in = bytes_read;
        
        ret = inflate(&strm, fuzz_int32_t_4);
        
        if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR) {
            // Handle error
            break;
        }
        
        if (strm.avail_out == 0) {
            strm.next_out = out_buffer;
            strm.avail_out = sizeof(out_buffer);
        }
    }

    // 11. Check for errors using gzerror
    int errnum = 0;
    const char *error_msg = gzerror(gzfile, &errnum);
    
    // 12. Clean up resources
    inflateEnd(&strm);
    
    // 13. Create output file for writing (if needed)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data if we decompressed successfully
        if (strm.total_out > 0) {
            fwrite(out_buffer, 1, sizeof(out_buffer) - strm.avail_out, out_file);
        }
        assert_file_closed(&out_file);;
    }

    // 14. Close all resources
    gzclose(gzfile);
    assert_file_closed(&in_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
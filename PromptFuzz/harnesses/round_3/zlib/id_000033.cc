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
//<ID> 60
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 12
//<Quality> {"density":10,"unique_branches":{"inflateGetHeader":[[1223,9,1223,32,0,0,4,1],[1225,9,1225,31,0,0,4,1]],"deflateInit2_":[[435,27,435,42,0,0,4,0]],"gzungetc":[[509,9,509,21,0,0,4,1],[512,9,512,31,0,0,4,1],[516,9,516,27,0,0,4,0],[516,31,516,49,0,0,4,0],[520,9,520,27,0,0,4,1],[525,9,525,20,0,0,4,1],[529,9,529,14,0,0,4,1],[533,9,533,27,0,0,4,0]],"inflate":[[522,17,522,38,0,0,4,0]]},"library_calls":["gzdopen","gzdirect","gzerror","gzseek64","gzoffset","gzungetc","inflateInit2_","gzclose","inflateGetHeader","inflateSetDictionary","inflatePrime","inflateEnd","gzclose","gzread","inflate","inflateEnd","deflateInit2_","deflateGetDictionary","deflateEnd","gzclose"],"critical_calls":["gzdopen","gzdirect","gzerror","gzseek64","gzoffset","gzungetc","inflateInit2_","inflateGetHeader","inflateSetDictionary","inflatePrime","gzread","inflate","inflateEnd","deflateInit2_","deflateGetDictionary","deflateEnd","gzclose"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<71) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}




    // 1. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // 4. Open gzFile for reading
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 5. Check if file is in direct mode
    int is_direct = gzdirect(gz_in);
    
    // 6. Get error status
    int errnum = 0;
    const char *err_msg = gzerror(gz_in, &errnum);
    
    // 7. Seek in gzFile
    off64_t seek_pos = gzseek64(gz_in, 0, fuzz_int32_t_2);
    
    // 8. Get current offset
    off_t offset = gzoffset(gz_in);
    
    // 9. Try to unget a character
    int unget_char = gzungetc('A', gz_in);
    
    // 10. Initialize inflation stream
    z_stream inflate_strm;
    memset(&inflate_strm, 0, sizeof(inflate_strm));
    
    // Use inflateInit2_ with windowBits for automatic header detection
    int windowBits = 15 + 32; // 32 enables automatic header detection
    int ret = inflateInit2_(&inflate_strm, windowBits, fuzz_str_3, sizeof(z_stream));
    if (ret != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 11. Set up gzip header
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&inflate_strm, &header);
    
    // 12. Try to set dictionary for inflation (if we have enough data)
    if (size > 100) {
        inflateSetDictionary(&inflate_strm, data, 100);
    }
    
    // 13. Try to prime the inflation stream
    inflatePrime(&inflate_strm, fuzz_int32_t_4, fuzz_int32_t_5);
    
    // 14. Prepare buffers for inflation
    const size_t out_buf_size = 4096;
    uint8_t *out_buf = (uint8_t *)malloc(out_buf_size);
    if (!out_buf) {
        inflateEnd(&inflate_strm);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 15. Read from gzFile and inflate
    uint8_t read_buf[1024];
    int bytes_read;
    
    do {
        bytes_read = gzread(gz_in, read_buf, sizeof(read_buf));
        if (bytes_read > 0) {
            inflate_strm.next_in = read_buf;
            inflate_strm.avail_in = bytes_read;
            
            do {
                inflate_strm.next_out = out_buf;
                inflate_strm.avail_out = out_buf_size;
                
                ret = inflate(&inflate_strm, fuzz_int32_t_6);
                
                if (ret == Z_OK || ret == Z_STREAM_END) {
                    size_t have = out_buf_size - inflate_strm.avail_out;
                    if (have > 0) {
                        // Write decompressed data to output file
                        fwrite(out_buf, 1, have, out_file);
                    }
                }
            } while (inflate_strm.avail_out == 0);
        }
    } while (bytes_read > 0 && ret != Z_STREAM_END);
    
    // 16. End inflation
    inflateEnd(&inflate_strm);
    
    // 17. Initialize deflation stream
    z_stream deflate_strm;
    memset(&deflate_strm, 0, sizeof(deflate_strm));
    
    // Use deflateInit2_ with same windowBits
    ret = deflateInit2_(&deflate_strm, fuzz_int32_t_8, fuzz_int32_t_9,
                        windowBits, fuzz_int32_t_10, fuzz_int32_t_11,
                        fuzz_str_7, sizeof(z_stream));
    
    if (ret == Z_OK) {
        // 18. Try to get dictionary from deflate stream
        uint8_t dict_buf[256];
        uInt dict_len = sizeof(dict_buf);
        deflateGetDictionary(&deflate_strm, dict_buf, &dict_len);
        
        // 19. End deflation
        deflateEnd(&deflate_strm);
    }
    
    // 20. Clean up
    free(out_buf);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
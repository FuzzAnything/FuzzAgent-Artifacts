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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 16
//<Quality> {"density":16,"unique_branches":{"gzputc":[[320,9,320,32,0,0,4,0]],"inflate":[[522,17,522,38,0,0,4,0],[568,17,568,38,0,0,4,0],[577,17,577,38,0,0,4,0],[586,17,586,38,0,0,4,0],[599,21,599,42,0,0,4,0],[605,22,605,43,0,0,4,0],[614,25,614,46,0,0,4,0],[615,25,615,53,0,0,4,1],[639,25,639,46,0,0,4,0],[640,29,640,56,0,0,4,1],[650,22,650,43,0,0,4,0],[661,25,661,46,0,0,4,0],[662,29,662,59,0,0,4,1],[672,22,672,43,0,0,4,0],[686,17,686,38,0,0,4,0]]},"library_calls":["gzdopen","gzrewind","gzread","gzputc","inflateInit2_","gzclose","inflateGetHeader","inflate","zError","deflateInit2_","inflateEnd","gzclose","deflateGetDictionary","deflate","deflatePending","deflateUsed","compressBound","compress2","inflateBackInit_","inflateBackEnd","gzdopen","gzfwrite","gzclose","deflateEnd","inflateEnd","gzclose"],"critical_calls":["gzdopen","gzrewind","gzread","gzputc","inflateInit2_","inflateGetHeader","inflate","zError","deflateInit2_","deflateGetDictionary","deflate","deflatePending","deflateUsed","compressBound","compress2","inflateBackInit_","inflateBackEnd","gzdopen","gzfwrite","gzclose","deflateEnd","inflateEnd","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<89) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful operations
    
    // 1. Create FILE* handles
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. gzdopen for reading compressed data
    int fd_in = fuzz_fileno(in_file);
    gzFile gz_in = gzdopen(fd_in, fuzz_str_1);
    if (!gz_in) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 3. gzrewind to reset position
    gzrewind(gz_in);
    
    // 4. Read some data with gzread
    unsigned char read_buf[1024];
    int read_len = gzread(gz_in, read_buf, sizeof(read_buf));
    
    // 5. gzputc - add a character
    gzputc(gz_in, 'A');
    
    // 6. Prepare for inflate operations
    z_stream strm_inflate;
    memset(&strm_inflate, 0, sizeof(z_stream));
    
    // 7. inflateInit2_ for decompression
    int windowBits = 15 + 32; // Enable gzip decoding
    if (inflateInit2_(&strm_inflate, windowBits, fuzz_str_2, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 8. Setup input/output buffers for inflate
    Bytef inflate_in[1024];
    Bytef inflate_out[2048];
    
    // Copy some input data
    size_t copy_size = size < sizeof(inflate_in) ? size : sizeof(inflate_in);
    memcpy(inflate_in, data, copy_size);
    
    strm_inflate.next_in = inflate_in;
    strm_inflate.avail_in = copy_size;
    strm_inflate.next_out = inflate_out;
    strm_inflate.avail_out = sizeof(inflate_out);
    
    // 9. inflateGetHeader - try to get gzip header
    gz_header header;
    memset(&header, 0, sizeof(header));
    inflateGetHeader(&strm_inflate, &header);
    
    // 10. Perform inflation
    int inflate_result = inflate(&strm_inflate, fuzz_int32_t_3);
    
    // 11. Get error message
    const char *err_msg = zError(inflate_result);
    
    // 12. Prepare for deflate operations
    z_stream strm_deflate;
    memset(&strm_deflate, 0, sizeof(z_stream));
    
    // 13. deflateInit2_ for compression
    int level = Z_DEFAULT_COMPRESSION;
    int method = Z_DEFLATED;
    int memLevel = 8;
    int strategy = Z_DEFAULT_STRATEGY;
    
    if (deflateInit2_(&strm_deflate, fuzz_int32_t_5, fuzz_int32_t_6, windowBits, fuzz_int32_t_7, fuzz_int32_t_8, 
                     fuzz_str_4, sizeof(z_stream)) != Z_OK) {
        inflateEnd(&strm_inflate);
        gzclose(gz_in);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // 14. Setup deflate input
    Bytef deflate_in[1024];
    Bytef deflate_out[2048];
    
    // Use inflated output as deflate input
    size_t deflate_in_size = sizeof(inflate_out) - strm_inflate.avail_out;
    memcpy(deflate_in, inflate_out, deflate_in_size);
    
    strm_deflate.next_in = deflate_in;
    strm_deflate.avail_in = deflate_in_size;
    strm_deflate.next_out = deflate_out;
    strm_deflate.avail_out = sizeof(deflate_out);
    
    // 15. deflateGetDictionary
    Bytef dict[256];
    uInt dictLength = sizeof(dict);
    deflateGetDictionary(&strm_deflate, dict, &dictLength);
    
    // 16. Perform deflation
    int deflate_result = deflate(&strm_deflate, fuzz_int32_t_9);
    
    // 17. deflatePending
    unsigned int pending;
    int bits;
    deflatePending(&strm_deflate, &pending, &bits);
    
    // 18. deflateUsed
    int used_bits;
    deflateUsed(&strm_deflate, &used_bits);
    
    // 19. Prepare for compress2 operation
    uLongf destLen = compressBound(size);
    Bytef *compress_out = (Bytef*)malloc(destLen);
    if (compress_out) {
        // 20. compress2
        int compress_result = compress2(compress_out, &destLen, data, 
                                       size < 10000 ? size : 10000, fuzz_int32_t_10);
        free(compress_out);
    }
    
    // 21. inflateBackInit_ and inflateBackEnd (minimal usage)
    z_stream strm_back;
    memset(&strm_back, 0, sizeof(z_stream));
    unsigned char back_window[32768];
    
    if (inflateBackInit_(&strm_back, windowBits, back_window, 
                        fuzz_str_11, sizeof(z_stream)) == Z_OK) {
        inflateBackEnd(&strm_back);
    }
    
    // 22. gzdopen for writing
    int fd_out = fuzz_fileno(out_file);
    gzFile gz_out = gzdopen(fd_out, fuzz_str_12);
    if (gz_out) {
        // 23. gzfwrite
        size_t items_written = 0;
        if (strm_inflate.total_out > 0) {
            items_written = gzfwrite(inflate_out, 1, 
                                   sizeof(inflate_out) - strm_inflate.avail_out, 
                                   gz_out);
        }
        
        // 24. gzclose
        gzclose(gz_out);
    }
    
    // 25. Clean up all resources
    deflateEnd(&strm_deflate);
    inflateEnd(&strm_inflate);
    gzclose(gz_in);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
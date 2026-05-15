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
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["get_crc_table","inflateBackInit_","inflateBack","inflateBackEnd","inflateInit2_","inflateReset2","inflateEnd","inflateEnd","inflate","inflateResetKeep","inflateEnd","deflateInit2_","deflateSetDictionary","deflateParams","deflateBound","deflateEnd","deflate","deflateEnd","gzdopen","gzerror","gzclose"],"critical_calls":["get_crc_table","inflateBackInit_","inflateBack","inflateBackEnd","inflateInit2_","inflateReset2","inflate","inflateResetKeep","inflateEnd","deflateInit2_","deflateSetDictionary","deflateParams","deflateBound","deflate","deflateEnd","gzdopen","gzerror","gzclose"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<96) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Step 1: Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Step 2: Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 3: Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);
    
    // Step 4: Initialize zlib structures
    z_stream strm_inflate, strm_deflate, strm_inflate_back;
    Bytef *dict = nullptr;
    uInt dict_length = 0;
    uLong source_len = 0;
    int ret = Z_OK;
    
    // Step 5: Get CRC table (just to call the function)
    const z_crc_t *crc_table = get_crc_table();
    (void)crc_table;  // Prevent unused variable warning
    
    // Step 6: First try to decompress the input using inflateBack
    unsigned char window[32768];  // 32K window as required by inflateBack
    
    // Initialize inflateBack stream
    ret = inflateBackInit_(&strm_inflate_back, 15, window, fuzz_str_1, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Define callbacks for inflateBack
    unsigned int in_index = 0;
    auto in_func = [](void *desc, unsigned char **buf) -> unsigned int {
        FILE *f = (FILE *)desc;
        static unsigned char local_buf[1024];
        size_t n = fread(local_buf, 1, sizeof(local_buf), f);
        *buf = local_buf;
        return (unsigned int)n;
    };
    
    auto out_func = [](void *desc, unsigned char *buf, unsigned int len) -> int {
        FILE *f = (FILE *)desc;
        return (fwrite(buf, 1, len, f) == len) ? 0 : -1;
    };
    
    // Reset file position for inflateBack
    rewind(in_file);
    
    // Perform inflateBack decompression
    ret = inflateBack(&strm_inflate_back, in_func, in_file, out_func, out_file);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        // Handle error - try other methods
    }
    
    // Clean up inflateBack
    inflateBackEnd(&strm_inflate_back);
    
    // Step 7: Now try regular inflate
    rewind(in_file);
    rewind(out_file);
    
    // Initialize inflate stream
    ret = inflateInit2_(&strm_inflate, 15 + 32, fuzz_str_2, sizeof(z_stream));
    if (ret != Z_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Try inflateReset2
    ret = inflateReset2(&strm_inflate, fuzz_int32_t_3);
    if (ret != Z_OK) {
        // Continue anyway
    }
    
    // Prepare input buffer
    Bytef *in_buf = (Bytef *)malloc(size);
    if (!in_buf) {
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    memcpy(in_buf, data, size);
    
    // Set up inflate stream
    strm_inflate.next_in = in_buf;
    strm_inflate.avail_in = (uInt)size;
    
    // Create output buffer
    uLong out_size = size * 2;
    Bytef *out_buf = (Bytef *)malloc(out_size);
    if (!out_buf) {
        free(in_buf);
        inflateEnd(&strm_inflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    strm_inflate.next_out = out_buf;
    strm_inflate.avail_out = (uInt)out_size;
    
    // Perform inflation
    ret = inflate(&strm_inflate, fuzz_int32_t_4);
    
    // Try inflateResetKeep
    ret = inflateResetKeep(&strm_inflate);
    
    // Clean up inflate
    inflateEnd(&strm_inflate);
    
    // Step 8: Now try deflate with the inflated data (or original if inflation failed)
    Bytef *deflate_in = out_buf;
    uLong deflate_in_len = strm_inflate.total_out > 0 ? strm_inflate.total_out : size;
    
    // Initialize deflate stream
    ret = deflateInit2_(&strm_deflate, fuzz_int32_t_6, fuzz_int32_t_7,
                        fuzz_int32_t_8, fuzz_int32_t_9, fuzz_int32_t_10,
                        fuzz_str_5, sizeof(z_stream));
    if (ret != Z_OK) {
        free(in_buf);
        free(out_buf);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Try deflateSetDictionary with first part of input as dictionary
    dict_length = deflate_in_len > 1024 ? 1024 : (uInt)deflate_in_len;
    if (dict_length > 0) {
        dict = (Bytef *)malloc(dict_length);
        if (dict) {
            memcpy(dict, deflate_in, dict_length);
            deflateSetDictionary(&strm_deflate, dict, dict_length);
        }
    }
    
    // Try deflateParams
    ret = deflateParams(&strm_deflate, fuzz_int32_t_11, fuzz_int32_t_12);
    
    // Get bound for output size
    uLong bound = deflateBound(&strm_deflate, deflate_in_len);
    Bytef *deflate_out = (Bytef *)malloc(bound);
    if (!deflate_out) {
        if (dict) free(dict);
        free(in_buf);
        free(out_buf);
        deflateEnd(&strm_deflate);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }
    
    // Set up deflate stream
    strm_deflate.next_in = deflate_in;
    strm_deflate.avail_in = (uInt)deflate_in_len;
    strm_deflate.next_out = deflate_out;
    strm_deflate.avail_out = (uInt)bound;
    
    // Perform deflation
    ret = deflate(&strm_deflate, fuzz_int32_t_13);
    
    // Clean up deflate
    deflateEnd(&strm_deflate);
    
    // Step 9: Try gzerror with gzFile
    rewind(in_file);
    gzFile gz = gzdopen(fd_in, fuzz_str_14);
    if (gz) {
        int errnum = 0;
        const char *errmsg = gzerror(gz, &errnum);
        (void)errmsg;  // Prevent unused variable warning
        gzclose(gz);
    }
    
    // Step 10: Clean up all resources
    if (dict) free(dict);
    free(in_buf);
    free(out_buf);
    free(deflate_out);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
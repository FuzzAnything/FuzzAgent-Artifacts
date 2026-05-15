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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["gzdopen","gzclose","get_crc_table","gzclose","inflateInit2_","gzclose","inflateReset","inflateEnd","gzclose","inflateResetKeep","inflateEnd","gzclose","deflateBound_z","inflateEnd","gzclose","uncompress2_z","inflateBackInit_","inflateBack","inflateBackEnd","gzfwrite","inflateEnd","gzclose"],"critical_calls":["gzdopen","get_crc_table","inflateInit2_","inflateReset","inflateResetKeep","deflateBound_z","uncompress2_z","inflateBackInit_","inflateBack","inflateBackEnd","gzfwrite","inflateEnd","gzclose"],"visited":1}
/**/


// Custom input function for inflateBack
static unsigned int  id_000003_custom_in_func(void* desc, unsigned char** buf) {
    // We'll use a simple static buffer approach
    static unsigned char static_buf[1024];
    *buf = static_buf;
    
    // For simplicity, return a small amount
    return 1024;
}

// Custom output function for inflateBack
static int  id_000003_custom_out_func(void* desc, unsigned char* buf, unsigned int size) {
    // Just discard the output for fuzzing
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<64) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Early return for very small inputs
    if (size < 10) {
        return 0;
    }

    // Create memory file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int fd_in = fuzz_fileno(in_file);
    int fd_out = fuzz_fileno(out_file);

    // Open compressed file using gzdopen
    gzFile gz_file = gzdopen(fd_in, fuzz_str_1);
    if (!gz_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 1. Use gzgetc to read first few bytes
    int first_byte = gzgetc(gz_file);
    if (first_byte == -1) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 2. Get CRC table
    const z_crc_t *crc_table = get_crc_table();
    if (!crc_table) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 3. Initialize inflation stream
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    
    // Use inflateInit2_ to initialize the stream
    int windowBits = 15 + 32; // Automatic header detection
    if (inflateInit2_(&strm, windowBits, fuzz_str_2, sizeof(z_stream)) != Z_OK) {
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 4. Use inflateReset
    if (inflateReset(&strm) != Z_OK) {
        inflateEnd(&strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 5. Use inflateResetKeep
    if (inflateResetKeep(&strm) != Z_OK) {
        inflateEnd(&strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 6. Prepare buffer for uncompress2_z
    // Estimate maximum uncompressed size using deflateBound_z
    z_size_t max_compressed = deflateBound_z(&strm, size);
    z_size_t dest_len = max_compressed * 2; // Conservative estimate
    Bytef* dest = (Bytef*)malloc(dest_len);
    if (!dest) {
        inflateEnd(&strm);
        gzclose(gz_file);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
    }

    // 7. Try to uncompress using uncompress2_z
    z_size_t source_len = size;
    int uncompress_result = uncompress2_z(dest, &dest_len, data, &source_len);
    // We ignore the result - just calling the API

    // 8. Use inflateBack
    unsigned char window[32768]; // 32K window
    if (inflateBackInit_(&strm, windowBits, window, fuzz_str_3, sizeof(z_stream)) == Z_OK) {
        // Setup dummy descriptors
        void* in_desc = nullptr;
        void* out_desc = nullptr;
        
        // Call inflateBack with custom functions
        int back_result = inflateBack(&strm,  id_000003_custom_in_func, in_desc, 
                                       id_000003_custom_out_func, out_desc);
        // Clean up inflateBack
        inflateBackEnd(&strm);
    }

    // 9. Write some data using gzfwrite
    const char* test_data = "Test data for gzfwrite";
    z_size_t test_data_size = strlen(test_data);
    z_size_t items_written = gzfwrite(fuzz_str_4, static_cast<z_size_t>(fuzz_str_sz_4), 1, gz_file);
    // Note: gzfwrite on a read-only gzFile will likely fail, but we're calling the API

    // Clean up
    free(dest);
    inflateEnd(&strm);
    
    // Close gz file
    gzclose(gz_file);
    
    // Close regular files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd_out);
	assert_fd_closed(fd_in);
	return 0;
}
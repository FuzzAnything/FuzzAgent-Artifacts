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
#include <magic.h>
}
//<ID> 293
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["magic_open","magic_setflags","magic_setflags","magic_setparam","magic_error","magic_errno","magic_getparam","magic_error","magic_errno","magic_compile","magic_error","magic_errno","magic_load","magic_error","magic_errno","magic_buffer","magic_error","magic_errno","magic_file","magic_error","magic_errno","magic_descriptor","magic_error","magic_errno","magic_close"],"critical_calls":["magic_open","magic_setflags","magic_setflags","magic_setparam","magic_error","magic_errno","magic_getparam","magic_error","magic_errno","magic_compile","magic_error","magic_errno","magic_load","magic_error","magic_errno","magic_buffer","magic_error","magic_errno","magic_file","magic_error","magic_errno","magic_descriptor","magic_error","magic_errno","magic_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file for compiled magic database
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Open magic cookie with default flags
    magic_t cookie = magic_open(fuzz_int32_t_1);
    if (!cookie) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set flags - try multiple flag combinations
    int flags = MAGIC_CONTINUE | MAGIC_ERROR | MAGIC_MIME;
    if (magic_setflags(cookie, flags) != 0) {
        // Try with different flags if first fails
        flags = MAGIC_SYMLINK | MAGIC_COMPRESS;
        magic_setflags(cookie, flags);
    }
    
    // Set a parameter (buffer size limit)
    size_t param_value = size < 1024 ? size : 1024;
    if (magic_setparam(cookie, fuzz_int32_t_2, &param_value) != 0) {
        // Error handling
        const char *err = magic_error(cookie);
        int err_num = magic_errno(cookie);
    }
    
    // Get the parameter back to verify
    size_t get_param;
    if (magic_getparam(cookie, fuzz_int32_t_3, &get_param) != 0) {
        const char *err = magic_error(cookie);
        int err_num = magic_errno(cookie);
    }
    
    // Try to compile a magic file
    // First write some data to the output file to use as magic file
    if (size > 0) {
        fwrite(data, 1, size < 100 ? size : 100, out_file);
        fflush(out_file);
    }
    
    // Try to compile the output file as a magic database
    if (magic_compile(cookie, fuzz_str_4) != 0) {
        const char *err = magic_error(cookie);
        int err_num = magic_errno(cookie);
    }
    
    // Load default magic database
    if (magic_load(cookie, NULL) != 0) {
        const char *err = magic_error(cookie);
        int err_num = magic_errno(cookie);
    }
    
    // Use magic_buffer to identify the input data
    const char *buffer_result = magic_buffer(cookie, data, size);
    if (!buffer_result) {
        const char *err = magic_error(cookie);
        int err_num = magic_errno(cookie);
    }
    
    // Reset file pointer to beginning
    fseek(in_file, 0, SEEK_SET);
    
    // Use magic_file to identify the input file
    const char *file_result = magic_file(cookie, "input_file");
    if (!file_result) {
        const char *err = magic_error(cookie);
        int err_num = magic_errno(cookie);
    }
    
    // Also try with file descriptor
    int fd = fuzz_fileno(in_file);
    if (fd >= 0) {
        const char *desc_result = magic_descriptor(cookie, fd);
        if (!desc_result) {
            const char *err = magic_error(cookie);
            int err_num = magic_errno(cookie);
        }
    }
    
    // Clean up
    magic_close(cookie);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}
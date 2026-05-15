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
//<ID> 336
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 16
//<Quality> {"density":9,"unique_branches":{"getstr":[[3049,8,3049,28,0,0,4,1]],"getvalue":[[2967,21,2967,38,0,0,4,1]],"parse":[[2175,4,2175,12,0,0,4,0],[314,3,314,23,66,0,4,0]],"check_format_type":[[2669,10,2669,14,0,0,4,1],[2747,3,2747,11,0,0,4,0],[2748,3,2748,11,0,0,4,0],[2750,3,2750,11,0,0,4,0],[2751,3,2751,11,0,0,4,0]],"check_regex":[[696,7,696,15,0,0,4,0],[699,8,699,17,0,0,4,1],[706,8,706,19,0,0,4,1],[708,8,708,18,0,0,4,1]],"parse_string_modifier":[[1946,14,1946,22,0,0,4,0]],"apprentice_magic_strength_1":[[1048,2,1048,10,0,0,4,0],[1063,2,1063,10,0,0,4,0]]},"library_calls":["magic_open","magic_getpath","magic_close","magic_close","magic_buffer","magic_check","magic_compile","magic_errno","magic_close"],"critical_calls":["magic_open","magic_getpath","magic_buffer","magic_check","magic_compile","magic_errno","magic_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize libmagic cookie with default flags
    magic_t cookie = magic_open(fuzz_int32_t_1);
    if (cookie == nullptr) {
        return 0;
    }

    // Get default magic database path
    const char *default_path = magic_getpath(nullptr, fuzz_int32_t_2);
    
    // Create a temporary file with input data
    FILE *input_file = fmemopen((void *)data, size, "rb");
    if (input_file == nullptr) {
        magic_close(cookie);
        assert_file_closed(&input_file);
	return 0;
    }

    // Get file descriptor for the input file
    int fd = fuzz_fileno(input_file);
    
    // Create output file for compiled database
    FILE *output_file = fopen("output_file", "wb");
    if (output_file == nullptr) {
        assert_file_closed(&input_file);;
        magic_close(cookie);
        assert_file_closed(&output_file);
	assert_file_closed(&input_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // First, try to use the input data as a buffer to get file type
    const char *buffer_result = magic_buffer(cookie, data, size);
    
    // Check if the current magic database is valid
    int check_result = magic_check(cookie, default_path);
    
    // Try to compile the input data as a magic database
    // We need to write the data to a file first since magic_compile expects a filename
    FILE *magic_file = fopen("input_file", "wb");
    if (magic_file != nullptr) {
        fwrite(data, 1, size, magic_file);
        assert_file_closed(&magic_file);;
        
        // Try to compile the input as a magic database
        int compile_result = magic_compile(cookie, fuzz_str_3);
        
        // Remove temporary file
        remove("input_file");
    }
    
    // Check for errors after operations
    int error_num = magic_errno(cookie);
    
    // Clean up resources
    assert_file_closed(&input_file);;
    assert_file_closed(&output_file);;
    magic_close(cookie);
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&magic_file);
	assert_file_closed(&output_file);
	assert_file_closed(&input_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
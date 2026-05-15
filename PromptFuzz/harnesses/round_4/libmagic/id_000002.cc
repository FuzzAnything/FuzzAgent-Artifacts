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
//<ID> 37
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 3
//<Quality> {"density":8,"unique_branches":{"magic_descriptor":[[405,6,405,16,0,0,4,1]],"check_buffer":[[3435,7,3435,20,0,0,4,0]],"magic_getpath":[[263,9,263,28,0,0,4,1]]},"library_calls":["magic_version","magic_open","magic_close","magic_getpath","magic_load","magic_load_buffers","magic_compile","magic_check","magic_descriptor","magic_close"],"critical_calls":["magic_version","magic_open","magic_getpath","magic_load","magic_load_buffers","magic_compile","magic_check","magic_descriptor","magic_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Step 1: Call magic_version() to get library version
    int version = magic_version();
    (void)version;  // Use version to avoid unused variable warning
    
    // Step 2: Create magic handle with default flags
    magic_t magic_cookie = magic_open(fuzz_int32_t_1);
    if (magic_cookie == nullptr) {
        return 0;  // Failed to create magic handle
    }
    
    // Step 3: Create temporary file with input data
    FILE *temp_file = fmemopen((void *)data, size, "rb");
    if (temp_file == nullptr) {
        magic_close(magic_cookie);
        assert_file_closed(&temp_file);
	return 0;
    }
    int fd = fuzz_fileno(temp_file);
    
    // Step 4: Try to load magic database from default location
    const char *magic_db = magic_getpath(nullptr, fuzz_int32_t_2);
    (void)magic_db;  // Use to avoid unused variable warning
    
    // Try loading from default database
    int load_result = magic_load(magic_cookie, nullptr);
    
    // Step 5: Try loading from input buffer
    // Prepare buffer array for magic_load_buffers
    void *buffers[1] = {(void *)data};
    size_t sizes[1] = {size};
    int load_buffers_result = magic_load_buffers(magic_cookie, buffers, sizes, 1);
    
    // Step 6: Try to compile magic database
    // Create output file for compilation
    FILE *compile_file = fopen("output_file", "wb");
    if (compile_file != nullptr) {
        int compile_result = magic_compile(magic_cookie, fuzz_str_3);
        (void)compile_result;  // Use to avoid unused warning
        assert_file_closed(&compile_file);;
    }
    
    // Step 7: Check magic database
    int check_result = magic_check(magic_cookie, nullptr);
    (void)check_result;  // Use to avoid unused warning
    
    // Step 8: Use magic_descriptor to identify file type
    const char *description = magic_descriptor(magic_cookie, fd);
    (void)description;  // Use to avoid unused warning
    
    // Step 9: Clean up resources
    assert_file_closed(&temp_file);;
    magic_close(magic_cookie);
    
    // Clean up temporary files
    remove("output_file");
    
    assert_file_closed(&compile_file);
	assert_file_closed(&temp_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
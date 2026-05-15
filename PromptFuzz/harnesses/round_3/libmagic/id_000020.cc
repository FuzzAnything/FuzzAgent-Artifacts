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
//<ID> 404
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["magic_open","magic_getpath","magic_compile","magic_file","magic_error","magic_error","magic_close"],"critical_calls":["magic_open","magic_getpath","magic_compile","magic_file","magic_error","magic_error","magic_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<20) return 0;

	
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





    // Step 1: Create a temporary input file from the fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Step 2: Write the input data to a file for magic_file() to read
    const char *input_filename = "input_file";
    FILE *temp_input = fopen(input_filename, "wb");
    if (!temp_input) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_input);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Copy data from in_file to temp_input
    rewind(in_file);
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, temp_input);
    }
    assert_file_closed(&temp_input);;
    rewind(in_file);
    
    // Step 3: Open magic library
    magic_t magic_cookie = magic_open(fuzz_int32_t_1);
    if (!magic_cookie) {
        assert_file_closed(&in_file);;
        remove(input_filename);
        assert_file_closed(&temp_input);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Step 4: Get the default magic database path
    const char *magic_path = magic_getpath(NULL, fuzz_int32_t_2);
    if (magic_path) {
        // Optional: Use the path if needed
    }
    
    // Step 5: Try to compile a magic file from the input data
    // First, write input data to a file for magic_compile
    const char *magic_src_file = "magic_src";
    FILE *magic_src = fopen(magic_src_file, "wb");
    if (magic_src) {
        rewind(in_file);
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
            fwrite(buffer, 1, bytes_read, magic_src);
        }
        assert_file_closed(&magic_src);;
        
        // Try to compile the magic source file
        // This may fail for arbitrary input, but that's expected
        magic_compile(magic_cookie, fuzz_str_3);
        
        // Clean up the magic source file
        remove(magic_src_file);
    }
    
    // Step 6: Use magic_file() to identify the input file
    const char *result = magic_file(magic_cookie, input_filename);
    if (!result) {
        // Get error if any
        const char *error = magic_error(magic_cookie);
        // error can be used for debugging
    }
    
    // Step 7: Check for errors using magic_error()
    const char *error = magic_error(magic_cookie);
    if (error && strlen(error) > 0) {
        // Error occurred, but we continue with cleanup
    }
    
    // Step 8: Clean up resources
    magic_close(magic_cookie);
    assert_file_closed(&in_file);;
    remove(input_filename);
    
    assert_file_closed(&magic_src);
	assert_file_closed(&temp_input);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}
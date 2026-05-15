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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":9,"unique_branches":{"check_format_type":[[2654,4,2654,18,0,0,4,0],[2657,4,2657,20,0,0,4,0]]},"library_calls":["magic_getpath","magic_open","magic_setparam","magic_load","magic_compile","magic_getparam","magic_buffer","magic_error","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_setparam","magic_load","magic_compile","magic_getparam","magic_buffer","magic_error","magic_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a FILE* from the input data for potential use
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // 1. Get the default magic database path
    const char *magic_db_path = magic_getpath(nullptr, 0);
    if (!magic_db_path) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 2. Create a magic cookie with default flags
    magic_t cookie = magic_open(MAGIC_NONE);
    if (!cookie) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }

    // 3. Set a parameter (buffer size limit)
    size_t param_value = size; // Use input size as parameter value
    if (magic_setparam(cookie, MAGIC_PARAM_BYTES_MAX, &param_value) != 0) {
        // Continue even if parameter setting fails
    }

    // 4. Load the magic database
    if (magic_load(cookie, magic_db_path) != 0) {
        // If loading fails, try loading from buffer
        // Create a fake buffer array for magic_load_buffers alternative
        // But we need to use magic_compile, so we'll handle error below
    }

    // 5. Try to compile the magic database to a file
    // Use the input data as a potential magic file to compile
    // First, write input data to a temporary file
    FILE *magic_src = fopen("input_file", "wb");
    if (magic_src) {
        fwrite(data, 1, size, magic_src);
        assert_file_closed(&magic_src);;
        
        // Try to compile it (may fail since input isn't a valid magic file)
        magic_compile(cookie, "input_file");
    }

    // 6. Get a parameter back to verify
    size_t retrieved_param = 0;
    magic_getparam(cookie, MAGIC_PARAM_BYTES_MAX, &retrieved_param);

    // 7. Use magic_buffer to analyze the input data itself
    // This is the main event: trying to identify what the input data is
    const char *result = magic_buffer(cookie, data, size);
    
    // 8. Check for errors
    const char *error_msg = magic_error(cookie);
    if (error_msg) {
        // Error occurred, but we continue
    }

    // Cleanup
    magic_close(cookie);
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file.mgc"); // In case magic_compile created it
    
    assert_file_closed(&magic_src);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file.mgc");
	assert_file_name_closed("input_file");
	return 0;
}
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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["magic_getpath","magic_open","magic_getflags","magic_setflags","magic_close","magic_compile","magic_load","magic_close","magic_buffer","magic_file","magic_getparam","magic_getpath","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_getflags","magic_setflags","magic_compile","magic_load","magic_buffer","magic_file","magic_getparam","magic_getpath","magic_close"],"visited":0}
/**/


// Include libmagic headers

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0;
    }

    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int fd = fuzz_fileno(in_file);
    
    // 1. Get the default magic database path
    const char *db_path = magic_getpath(NULL, 0);
    
    // 2. Open a magic cookie
    magic_t magic_cookie = magic_open(MAGIC_NONE);
    if (!magic_cookie) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 3. Set some flags - use MAGIC_CONTINUE to get multiple results
    int flags = magic_getflags(magic_cookie);
    if (magic_setflags(magic_cookie, flags | MAGIC_CONTINUE) == -1) {
        magic_close(magic_cookie);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Try to compile a magic database from the input data
    // First write input data to a file
    FILE *compile_file = fopen("input_file", "wb");
    if (compile_file) {
        fwrite(data, 1, size, compile_file);
        assert_file_closed(&compile_file);;
        
        // Try to compile it as a magic database
        magic_compile(magic_cookie, "input_file");
    }
    
    // 5. Load the default magic database
    if (magic_load(magic_cookie, NULL) == -1) {
        magic_close(magic_cookie);
        assert_file_closed(&in_file);;
        remove("input_file");
        assert_file_closed(&compile_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
    }
    
    // 6. Use magic_buffer to identify the input data
    const char *buffer_result = magic_buffer(magic_cookie, data, size);
    
    // 7. Use magic_file to identify the temporary file
    const char *file_result = magic_file(magic_cookie, "input_file");
    
    // 8. Get a parameter from the magic cookie
    int param_value;
    if (magic_getparam(magic_cookie, MAGIC_PARAM_INDIR_MAX, &param_value) == -1) {
        // Parameter retrieval failed, but continue
    }
    
    // 9. Get the path again for comparison
    const char *db_path2 = magic_getpath("magic", 0);
    
    // Clean up resources
    magic_close(magic_cookie);
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    
    assert_file_closed(&compile_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}
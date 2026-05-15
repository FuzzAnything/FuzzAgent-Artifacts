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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 6
//<Quality> {"density":10,"unique_branches":{"magic_setparam":[[619,2,619,28,0,0,4,0]],"apprentice_load":[[1561,7,1561,11,0,0,4,1],[1565,10,1565,36,0,0,4,0],[1565,10,1565,36,0,0,4,1],[1566,8,1566,27,0,0,4,0],[1597,7,1597,14,0,0,4,1]]},"library_calls":["magic_getpath","magic_open","magic_setparam","magic_getparam","magic_load","magic_load_buffers","magic_compile","magic_error","magic_buffer","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_setparam","magic_getparam","magic_load","magic_load_buffers","magic_compile","magic_error","magic_buffer","magic_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from the fuzzer data
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Get magic database path
    const char *magic_path = magic_getpath("/usr/share/misc/magic", 0);
    
    // Create magic cookie
    magic_t magic_cookie = magic_open(MAGIC_NONE);
    if (!magic_cookie) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set magic parameter - buffer size limit
    size_t buffer_limit = 1024;
    magic_setparam(magic_cookie, MAGIC_PARAM_INDIR_MAX, &buffer_limit);
    
    // Get parameter back to verify
    size_t retrieved_limit = 0;
    magic_getparam(magic_cookie, MAGIC_PARAM_INDIR_MAX, &retrieved_limit);
    
    // Load magic database
    if (magic_load(magic_cookie, magic_path) != 0) {
        // If loading fails, try loading from buffer
        void *buffers[1] = {(void *)data};
        size_t sizes[1] = {size};
        magic_load_buffers(magic_cookie, buffers, sizes, 1);
    }
    
    // Try to compile the magic database
    if (magic_compile(magic_cookie, "compiled_output") != 0) {
        // Check error
        const char *error_msg = magic_error(magic_cookie);
    }
    
    // Test magic buffer with the input data
    const char *result = magic_buffer(magic_cookie, data, size);
    
    // Clean up
    magic_close(magic_cookie);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
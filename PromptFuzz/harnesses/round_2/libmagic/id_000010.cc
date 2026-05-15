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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"mkdbname":[[3528,34,3528,42,0,0,4,1],[3533,6,3533,14,0,0,4,1]]},"library_calls":["magic_getpath","magic_open","magic_setparam","magic_getparam","magic_load","magic_load","magic_close","magic_descriptor","magic_buffer","magic_compile","magic_error","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_setparam","magic_getparam","magic_load","magic_load","magic_descriptor","magic_buffer","magic_compile","magic_error","magic_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get the default magic database path
    const char *magic_path = magic_getpath(nullptr, 0);
    if (!magic_path) {
        // If we can't get default path, use a fallback
        magic_path = "/usr/share/misc/magic.mgc";
    }
    
    // 2. Create a magic cookie with flags
    magic_t cookie = magic_open(MAGIC_NONE | MAGIC_ERROR | MAGIC_MIME);
    if (!cookie) {
        return 0;
    }
    
    // 3. Set a parameter (buffer size limit)
    // Let's use a reasonable limit based on input size
    size_t max_buffer_size = (size > 1024 * 1024) ? 1024 * 1024 : size;
    magic_setparam(cookie, MAGIC_PARAM_BYTES_MAX, &max_buffer_size);
    
    // 4. Get the current parameter value to verify
    size_t current_max_size;
    magic_getparam(cookie, MAGIC_PARAM_BYTES_MAX, &current_max_size);
    
    // 5. Load the magic database
    if (magic_load(cookie, magic_path) != 0) {
        // If loading fails, try to compile from source
        magic_load(cookie, nullptr); // Try default paths
    }
    
    // 6. Create a FILE* from the input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Get file descriptor for reading
    int fd = fuzz_fileno(in_file);
    
    // 8. Use magic_descriptor to identify the file
    const char *result = magic_descriptor(cookie, fd);
    
    // 9. Also use magic_buffer with the same data
    // First rewind the file to read from beginning
    fseek(in_file, 0, SEEK_SET);
    
    // Read data into buffer
    uint8_t *buffer = (uint8_t *)malloc(size);
    if (buffer) {
        fread(buffer, 1, size, in_file);
        const char *buffer_result = magic_buffer(cookie, buffer, size);
        free(buffer);
    }
    
    // 10. Create output file for compilation
    FILE *out_file = fopen("output.mgc", "wb");
    if (out_file) {
        // 11. Compile the magic database
        magic_compile(cookie, "output.mgc");
        assert_file_closed(&out_file);;
    }
    
    // 12. Check for errors
    const char *error = magic_error(cookie);
    if (error) {
        // Error handling (in fuzzing we just note it)
    }
    
    // 13. Clean up resources
    assert_file_closed(&in_file);;
    magic_close(cookie);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
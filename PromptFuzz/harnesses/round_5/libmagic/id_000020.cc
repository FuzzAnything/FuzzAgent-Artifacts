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
//<ID> 273
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"looks_ucs16":[[502,3,502,14,0,0,4,0]]},"library_calls":["magic_open","magic_load_buffers","magic_getpath","magic_getflags","magic_compile","magic_list","magic_buffer","magic_error","magic_close"],"critical_calls":["magic_open","magic_load_buffers","magic_getpath","magic_getflags","magic_compile","magic_list","magic_buffer","magic_error","magic_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create a magic cookie
    magic_t cookie = magic_open(MAGIC_NONE);
    if (!cookie) {
        return 0;
    }

    // 2. Use the input data as a magic database buffer
    // Prepare buffer pointers for magic_load_buffers
    void *buffer = const_cast<void*>(reinterpret_cast<const void*>(data));
    size_t buffer_sizes[1] = {size};
    void *buffers[1] = {buffer};
    
    // Load the magic database from the input buffer
    int load_result = magic_load_buffers(cookie, buffers, buffer_sizes, 1);
    
    // 3. Get the default magic database path (for reference)
    const char *default_path = magic_getpath(nullptr, 0);
    
    // 4. Get current flags
    int current_flags = magic_getflags(cookie);
    
    // 5. Try to compile the loaded magic database to a file
    // Note: This might fail if the input data isn't a valid magic database
    int compile_result = magic_compile(cookie, "output_file.mgc");
    
    // 6. List the magic entries to a file
    FILE *list_file = fopen("output_file.list", "w");
    if (list_file) {
        // magic_list expects a filename, not a FILE*
        assert_file_closed(&list_file);;
        int list_result = magic_list(cookie, "output_file.list");
    }
    
    // 7. Use a small portion of the input data as a buffer to test magic identification
    // We'll use up to 1024 bytes or the entire input if smaller
    size_t test_size = size > 1024 ? 1024 : size;
    if (test_size > 0) {
        const char *buffer_result = magic_buffer(cookie, data, test_size);
        // buffer_result could be nullptr if identification fails
    }
    
    // 8. Check for any errors that occurred during operations
    const char *error_msg = magic_error(cookie);
    
    // 9. Clean up - close the magic cookie
    magic_close(cookie);
    
    assert_file_closed(&list_file);
	assert_file_name_closed("output_file.mgc");
	return 0;
}
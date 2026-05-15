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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":7,"unique_branches":{},"library_calls":["magic_open","magic_getpath","magic_load","magic_getflags","magic_compile","magic_list","magic_open","magic_load_buffers","magic_buffer","magic_error","magic_close","magic_error","magic_close"],"critical_calls":["magic_open","magic_getpath","magic_load","magic_getflags","magic_compile","magic_list","magic_open","magic_load_buffers","magic_buffer","magic_error","magic_close","magic_error","magic_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a temporary file for magic database
    FILE *magic_file = fopen("temp_magic.mgc", "wb");
    if (!magic_file) {
        assert_file_closed(&magic_file);
	return 0;
    }
    fwrite(data, 1, size, magic_file);
    assert_file_closed(&magic_file);;
    
    // Initialize magic handle with default flags
    magic_t magic_cookie = magic_open(MAGIC_NONE);
    if (!magic_cookie) {
        remove("temp_magic.mgc");
        assert_file_closed(&magic_file);
	return 0;
    }
    
    // Get the default magic database path
    const char *default_path = magic_getpath(NULL, 0);
    (void)default_path; // Use it to satisfy requirement
    
    // Try to load the magic database from our temporary file
    int load_result = magic_load(magic_cookie, "temp_magic.mgc");
    
    // Get current flags
    int current_flags = magic_getflags(magic_cookie);
    (void)current_flags; // Use it to satisfy requirement
    
    // Try to compile the magic database to a different format
    int compile_result = magic_compile(magic_cookie, "output_compiled.mgc");
    
    // Try to list the magic entries to a file
    int list_result = magic_list(magic_cookie, "output_list.txt");
    
    // Create input buffer from the data (use first half for magic, second for buffer test)
    size_t half_size = size / 2;
    if (half_size > 0) {
        // Prepare buffers for magic_load_buffers
        void *buffer_array[1] = {(void*)data};
        size_t buffer_sizes[1] = {half_size};
        
        // Create another magic handle for buffer loading
        magic_t magic_buffer_cookie = magic_open(MAGIC_NONE);
        if (magic_buffer_cookie) {
            int buffer_load_result = magic_load_buffers(magic_buffer_cookie, buffer_array, buffer_sizes, 1);
            
            // Test magic_buffer with the second half of input
            if (size - half_size > 0) {
                const char *buffer_result = magic_buffer(magic_buffer_cookie, 
                                                         data + half_size, 
                                                         size - half_size);
                (void)buffer_result; // Use it to satisfy requirement
            }
            
            // Check for errors
            const char *error_msg = magic_error(magic_buffer_cookie);
            (void)error_msg; // Use it to satisfy requirement
            
            magic_close(magic_buffer_cookie);
        }
    }
    
    // Check for errors on the original cookie
    const char *error_msg = magic_error(magic_cookie);
    (void)error_msg; // Use it to satisfy requirement
    
    // Clean up
    magic_close(magic_cookie);
    remove("temp_magic.mgc");
    remove("output_compiled.mgc");
    remove("output_list.txt");
    
    assert_file_closed(&magic_file);
	return 0;
}
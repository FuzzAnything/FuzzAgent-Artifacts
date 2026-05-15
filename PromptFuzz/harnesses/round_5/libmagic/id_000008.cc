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
//<ID> 145
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["magic_getpath","magic_open","magic_load","magic_load_buffers","magic_close","magic_descriptor","magic_buffer","magic_list","magic_file","magic_error","magic_compile","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_load","magic_load_buffers","magic_descriptor","magic_buffer","magic_list","magic_file","magic_error","magic_compile","magic_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Get the path to the magic database
    const char *magic_db_path = magic_getpath(NULL, 0);
    if (!magic_db_path) {
        return 0;
    }

    // 2. Create a magic handle
    magic_t magic_cookie = magic_open(MAGIC_NONE);
    if (!magic_cookie) {
        return 0;
    }

    // 3. Try to load the magic database from file
    int load_result = magic_load(magic_cookie, NULL);
    
    // 4. Also try loading from input buffer (simulating compiled magic data)
    // We'll use a portion of input as magic buffer
    size_t magic_buffer_size = size > 100 ? 100 : size;
    void *magic_buffers[1] = {const_cast<void*>(static_cast<const void*>(data))};
    size_t magic_lengths[1] = {magic_buffer_size};
    magic_load_buffers(magic_cookie, magic_buffers, magic_lengths, 1);

    // 5. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        magic_close(magic_cookie);
        assert_file_closed(&in_file);
	return 0;
    }
    int fd = fuzz_fileno(in_file);

    // 6. Use magic_descriptor to identify the file descriptor
    const char *descriptor_result = magic_descriptor(magic_cookie, fd);
    
    // 7. Also use magic_buffer with the input data
    const char *buffer_result = magic_buffer(magic_cookie, data, size);
    
    // 8. Create an output file for magic_list
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // 9. List magic entries to file
        magic_list(magic_cookie, "output_file");
        
        // Also try magic_file with the output file
        const char *file_result = magic_file(magic_cookie, "output_file");
        
        assert_file_closed(&out_file);;
    }

    // 10. Check for any errors
    const char *error_msg = magic_error(magic_cookie);
    
    // 11. Try to compile the magic database (this will likely fail with input data)
    magic_compile(magic_cookie, "compiled_magic.mgc");

    // Cleanup
    assert_file_closed(&in_file);;
    magic_close(magic_cookie);
    
    // Remove temporary files
    remove("output_file");
    remove("compiled_magic.mgc");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
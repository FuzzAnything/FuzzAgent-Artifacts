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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"magiccheck":[[2208,3,2208,11,0,0,4,0]]},"library_calls":["magic_getpath","magic_open","magic_getflags","magic_setflags","magic_close","magic_load_buffers","magic_load","magic_errno","magic_close","magic_descriptor","magic_buffer","magic_list","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_getflags","magic_setflags","magic_load_buffers","magic_load","magic_errno","magic_descriptor","magic_buffer","magic_list","magic_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if no data
    if (size == 0) {
        return 0;
    }

    // Step 1: Get magic database path
    const char *magic_path = magic_getpath(NULL, 0);
    if (!magic_path) {
        // Continue anyway - magic_open will handle default paths
    }

    // Step 2: Create magic cookie with default flags
    magic_t magic_cookie = magic_open(MAGIC_NONE);
    if (!magic_cookie) {
        return 0;
    }

    // Step 3: Get current flags
    int current_flags = magic_getflags(magic_cookie);

    // Step 4: Set additional flags for better detection
    // Use MAGIC_CONTINUE to return all matches, not just first
    int new_flags = current_flags | MAGIC_CONTINUE;
    if (magic_setflags(magic_cookie, new_flags) != 0) {
        magic_close(magic_cookie);
        return 0;
    }

    // Step 5: Load magic database from input buffer
    // The input data is treated as a magic database file
    void *buffer_array[1] = { (void *)data };
    size_t buffer_sizes[1] = { size };
    
    if (magic_load_buffers(magic_cookie, buffer_array, buffer_sizes, 1) != 0) {
        // If loading fails, try with default database
        // This ensures we can still test other APIs
        magic_load(magic_cookie, NULL);
    }

    // Step 6: Check for errors after loading
    int magic_error_code = magic_errno(magic_cookie);
    if (magic_error_code != 0) {
        // Continue anyway to test other functions
    }

    // Step 7: Create a FILE* from input data for descriptor testing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        magic_close(magic_cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // Step 8: Get file descriptor and use magic_descriptor
    int fd = fuzz_fileno(in_file);
    const char *descriptor_result = magic_descriptor(magic_cookie, fd);
    if (descriptor_result) {
        // Result available, but we don't need to use it for fuzzing
    }

    // Step 9: Use magic_buffer on the same input data
    const char *buffer_result = magic_buffer(magic_cookie, data, size);
    if (buffer_result) {
        // Result available, but we don't need to use it for fuzzing
    }

    // Step 10: List magic entries to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // magic_list writes to a file, so we need to use the filename
        assert_file_closed(&out_file);;  // Close so magic_list can open it
        if (magic_list(magic_cookie, "output_file") != 0) {
            // List failed, but continue
        }
        // Reopen to check if file was created/modified
        out_file = fopen("output_file", "rb");
        if (out_file) {
            assert_file_closed(&out_file);;
        }
    }

    // Step 11: Clean up resources
    assert_file_closed(&in_file);;
    magic_close(magic_cookie);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
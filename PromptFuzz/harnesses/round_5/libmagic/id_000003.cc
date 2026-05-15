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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["magic_open","magic_close","magic_close","magic_load_buffers","magic_close","magic_close","magic_descriptor","magic_error","magic_list","magic_getflags","magic_close"],"critical_calls":["magic_open","magic_load_buffers","magic_descriptor","magic_error","magic_list","magic_getflags","magic_close"],"visited":0}
/*We'll create a fuzz driver that uses the specified libmagic APIs to achieve an event where we:
1. Load magic database from input buffer data
2. Use a file descriptor to identify a file's type
3. List magic entries to a file
4. Check for errors and flags

Here's the implementation:

*/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Return early if we don't have enough data for basic operations
    if (size < 100) {
	return 0;
	}
	

    // Create magic cookie
    magic_t mymagic = magic_open(MAGIC_NONE);
    if (mymagic == NULL) {
        return 0;
    }

    // Event 1: Load magic database from buffer
    // We'll split the input data into two buffers for magic_load_buffers
    size_t mid_point = size / 2;
    void *buffers[2];
    size_t buffer_sizes[2];
    
    // First buffer: first half of data
    buffers[0] = malloc(mid_point);
    if (!buffers[0]) {
        magic_close(mymagic);
        return 0;
    }
    memcpy(buffers[0], data, mid_point);
    buffer_sizes[0] = mid_point;
    
    // Second buffer: second half of data
    buffers[1] = malloc(size - mid_point);
    if (!buffers[1]) {
        free(buffers[0]);
        magic_close(mymagic);
        return 0;
    }
    memcpy(buffers[1], data + mid_point, size - mid_point);
    buffer_sizes[1] = size - mid_point;
    
    // Load magic database from buffers
    int load_result = magic_load_buffers(mymagic, buffers, buffer_sizes, 2);
    
    // Event 2: Create a file from input data and get its descriptor
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == NULL) {
        free(buffers[0]);
        free(buffers[1]);
        magic_close(mymagic);
        assert_file_closed(&in_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        free(buffers[0]);
        free(buffers[1]);
        magic_close(mymagic);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Use magic_descriptor to identify the file type
    const char *description = magic_descriptor(mymagic, fd);
    
    // Event 3: Check for errors
    const char *error_msg = magic_error(mymagic);
    
    // Event 4: List magic entries to output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file != NULL) {
        // We need to close the file first as magic_list expects a filename
        assert_file_closed(&out_file);;
        
        // List magic entries to the file
        int list_result = magic_list(mymagic, "output_file");
        (void)list_result; // Suppress unused variable warning
    }
    
    // Event 5: Get current flags
    int current_flags = magic_getflags(mymagic);
    (void)current_flags; // Suppress unused variable warning
    
    // Consume the API outputs to prevent optimization
    if (description) {
        volatile const char *desc = description;
        (void)desc;
    }
    
    if (error_msg) {
        volatile const char *err = error_msg;
        (void)err;
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    free(buffers[0]);
    free(buffers[1]);
    magic_close(mymagic);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
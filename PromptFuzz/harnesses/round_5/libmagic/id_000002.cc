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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["magic_open","magic_getflags","magic_load_buffers","magic_error","magic_close","magic_close","magic_close","magic_descriptor","magic_list","magic_error","magic_getflags","magic_close"],"critical_calls":["magic_open","magic_getflags","magic_load_buffers","magic_descriptor","magic_list","magic_error","magic_getflags","magic_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libmagic APIs to achieve a scenario where we load a magic database from input data, then use it to identify files and list entries:

*/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a magic cookie
    magic_t cookie = magic_open(0);
    if (!cookie) {
        return 0;
    }

    // Get current flags
    int flags = magic_getflags(cookie);
    (void)flags; // Use flags to avoid unused variable warning

    // Load magic database from input buffer
    // Note: data should contain compiled magic file format
    void *buffer = (void *)data;
    size_t buffer_size = size;
    
    if (magic_load_buffers(cookie, &buffer, &buffer_size, 1) != 0) {
        // If loading fails, get error message
        const char *error = magic_error(cookie);
        (void)error; // Use error to avoid unused variable warning
        magic_close(cookie);
        return 0;
    }

    // Create a temporary input file from the data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for the input file
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        magic_close(cookie);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Use magic_descriptor to identify the file type
    const char *description = magic_descriptor(cookie, fd);
    (void)description; // Use description to avoid unused variable warning

    // Create output file for magic_list
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // List magic entries to file
        magic_list(cookie, "output_file");
        assert_file_closed(&out_file);;
    }

    // Check for any errors after operations
    const char *last_error = magic_error(cookie);
    (void)last_error; // Use last_error to avoid unused variable warning

    // Get flags again to see if they changed
    int final_flags = magic_getflags(cookie);
    (void)final_flags; // Use final_flags to avoid unused variable warning

    // Clean up
    assert_file_closed(&in_file);;
    magic_close(cookie);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
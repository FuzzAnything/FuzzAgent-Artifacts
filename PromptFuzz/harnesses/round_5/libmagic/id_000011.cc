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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["magic_open","magic_getflags","magic_close","magic_close","magic_load_buffers","magic_errno","magic_close","magic_descriptor","magic_errno","magic_buffer","magic_errno","magic_compile","magic_check","magic_close"],"critical_calls":["magic_open","magic_getflags","magic_load_buffers","magic_errno","magic_descriptor","magic_errno","magic_buffer","magic_errno","magic_compile","magic_check","magic_close"],"visited":1}
/*Here's a step-by-step C++ fuzz driver that uses all the specified libmagic APIs to test file type identification functionality:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Ensure we have enough data for meaningful operations
    if (size < 10) {
        return 0;
    }

    magic_t cookie = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    const char *result = nullptr;
    int ret = 0;
    int flags = 0;

    // 1. Open magic database
    cookie = magic_open(MAGIC_NONE);
    if (!cookie) {
        return 0;
    }

    // 2. Get current flags
    flags = magic_getflags(cookie);

    // 3. Create a buffer to hold magic database data
    // Split input into two parts: one for magic database, one for test data
    size_t magic_data_size = size / 2;
    size_t test_data_size = size - magic_data_size;
    
    if (magic_data_size == 0 || test_data_size == 0) {
        magic_close(cookie);
        return 0;
    }

    // Prepare buffers for magic_load_buffers
    void* magic_buffers[1];
    size_t magic_sizes[1];
    
    // Allocate and copy first half of input for magic database
    uint8_t* magic_data = static_cast<uint8_t*>(malloc(magic_data_size));
    if (!magic_data) {
        magic_close(cookie);
        return 0;
    }
    memcpy(magic_data, data, magic_data_size);
    
    magic_buffers[0] = magic_data;
    magic_sizes[0] = magic_data_size;

    // 4. Load magic database from buffers
    ret = magic_load_buffers(cookie, magic_buffers, magic_sizes, 1);
    
    // Check for errors
    if (ret == -1) {
        int err = magic_errno(cookie);
        // Continue anyway to test other APIs
    }

    // 5. Create input file from second half of data
    in_file = fmemopen((void*)(data + magic_data_size), test_data_size, "rb");
    if (!in_file) {
        free(magic_data);
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Get file descriptor and use magic_descriptor
    int fd = fuzz_fileno(in_file);
    if (fd != -1) {
        result = magic_descriptor(cookie, fd);
        // Check if an error occurred
        int err = magic_errno(cookie);
    }

    // 7. Use magic_buffer with test data
    result = magic_buffer(cookie, data + magic_data_size, test_data_size);
    // Check for errors again
    int err = magic_errno(cookie);

    // 8. Try to compile the magic database to a file
    out_file = fopen("output_file", "wb");
    if (out_file) {
        ret = magic_compile(cookie, "output_file");
        assert_file_closed(&out_file);;
        remove("output_file");
    }

    // 9. Check the magic database
    ret = magic_check(cookie, nullptr);

    // Clean up
    free(magic_data);
    assert_file_closed(&in_file);;
    magic_close(cookie);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
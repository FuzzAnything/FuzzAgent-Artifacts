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
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["magic_getpath","magic_open","magic_getflags","magic_setflags","magic_close","magic_close","magic_load_buffers","magic_errno","magic_load","magic_close","magic_descriptor","magic_buffer","magic_list","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_getflags","magic_setflags","magic_load_buffers","magic_errno","magic_load","magic_descriptor","magic_buffer","magic_list","magic_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get the default magic database path
    const char *magic_path = magic_getpath(NULL, 0);
    if (!magic_path) {
        return 0;
    }

    // 2. Open a magic cookie
    magic_t cookie = magic_open(MAGIC_NONE);
    if (!cookie) {
        return 0;
    }

    // 3. Get current flags
    int flags = magic_getflags(cookie);

    // 4. Set some flags (add MAGIC_CONTINUE for testing)
    int new_flags = flags | MAGIC_CONTINUE;
    if (magic_setflags(cookie, new_flags) != 0) {
        magic_close(cookie);
        return 0;
    }

    // 5. Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Get file descriptor for reading
    int fd = fuzz_fileno(in_file);

    // 7. Load magic database from buffer
    // We need to create a buffer array for magic_load_buffers
    void *buffers[1] = {(void *)data};
    size_t sizes[1] = {size};
    
    // Try to load from buffer (this may fail if data isn't valid magic database)
    int load_result = magic_load_buffers(cookie, buffers, sizes, 1);
    
    // 8. Check for errors
    int err = magic_errno(cookie);
    if (err != 0) {
        // If loading from buffer failed, try loading from default database
        if (magic_load(cookie, NULL) != 0) {
            assert_file_closed(&in_file);;
            magic_close(cookie);
            assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
        }
    }

    // 9. Use magic_descriptor to identify the file
    const char *descriptor_result = magic_descriptor(cookie, fd);
    
    // 10. Reset file pointer to beginning
    rewind(in_file);
    
    // 11. Read some data from file for magic_buffer
    std::vector<uint8_t> buffer_data(std::min(size, (size_t)4096));
    size_t read_size = fread(buffer_data.data(), 1, buffer_data.size(), in_file);
    
    // 12. Use magic_buffer to identify the buffer
    const char *buffer_result = magic_buffer(cookie, buffer_data.data(), read_size);

    // 13. Create output file for magic_list
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // 14. List magic entries to file
        magic_list(cookie, "output_file");
        assert_file_closed(&out_file);;
    }

    // 15. Clean up resources
    assert_file_closed(&in_file);;
    magic_close(cookie);

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
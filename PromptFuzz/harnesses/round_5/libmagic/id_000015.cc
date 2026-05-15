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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 7
//<Quality> {"density":11,"unique_branches":{"parse":[[2190,4,2190,12,0,0,4,0],[2191,4,2191,12,0,0,4,0],[2194,4,2194,12,0,0,4,0],[2208,4,2208,12,0,0,4,0]],"uncompressbzlib":[[679,6,679,17,0,0,4,1],[688,6,688,17,0,0,4,1]],"uncompressgzipped":[[584,6,584,12,0,0,4,0]]},"library_calls":["magic_getpath","magic_open","magic_getflags","magic_setflags","magic_close","magic_load_buffers","magic_check","magic_list","magic_buffer","magic_descriptor","magic_getflags","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_getflags","magic_setflags","magic_load_buffers","magic_check","magic_list","magic_buffer","magic_descriptor","magic_getflags","magic_close"],"visited":1}
/**/


// Include libmagic headers

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early exit if input is too small to be meaningful
    if (size < 2) {
	return 0;
	}
	

    // 1. Get the default magic database path
    const char *magic_path = magic_getpath(nullptr, 0);
    
    // 2. Create a magic cookie
    magic_t cookie = magic_open(0);
    if (!cookie) {
        return 0;
    }

    // 3. Set some flags
    int current_flags = magic_getflags(cookie);
    magic_setflags(cookie, current_flags | MAGIC_COMPRESS | MAGIC_CONTINUE);

    // 4. Create temporary input file from the data buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }
    int fd = fuzz_fileno(in_file);

    // 5. Prepare buffer for magic_load_buffers
    // We'll use the entire input as one buffer
    void *buffers[1] = {(void *)data};
    size_t sizes[1] = {size};
    
    // 6. Attempt to load magic from the input buffer
    // This simulates loading compiled magic data
    magic_load_buffers(cookie, buffers, sizes, 1);

    // 7. Check if the loaded magic database is valid
    int check_result = magic_check(cookie, "input_file");

    // 8. List magic entries to an output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // magic_list writes to a file - we'll use the output file
        magic_list(cookie, "output_file");
        assert_file_closed(&out_file);;
    }

    // 9. Use magic_buffer to identify data type
    const char *buffer_result = magic_buffer(cookie, data, size);

    // 10. Use magic_descriptor to identify file descriptor type
    const char *descriptor_result = magic_descriptor(cookie, fd);

    // 11. Get current flags again to verify they were set
    int updated_flags = magic_getflags(cookie);

    // Clean up resources
    assert_file_closed(&in_file);;
    
    // Remove the output file we created
    remove("output_file");
    
    // Close the magic cookie
    magic_close(cookie);

    // Use the results to prevent optimization
    if (magic_path) {
        volatile const char *path_volatile = magic_path;
        (void)path_volatile;
    }
    if (buffer_result) {
        volatile const char *buf_volatile = buffer_result;
        (void)buf_volatile;
    }
    if (descriptor_result) {
        volatile const char *desc_volatile = descriptor_result;
        (void)desc_volatile;
    }
    volatile int check_volatile = check_result;
    volatile int flags_volatile = updated_flags;
    (void)check_volatile;
    (void)flags_volatile;

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}
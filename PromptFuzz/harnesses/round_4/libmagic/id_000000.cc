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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"magic_list":[[358,6,358,16,0,0,4,1]]},"library_calls":["magic_version","magic_open","magic_close","magic_load_buffers","magic_error","magic_close","magic_compile","magic_list","magic_check","magic_error","magic_close"],"critical_calls":["magic_version","magic_open","magic_load_buffers","magic_compile","magic_list","magic_check","magic_error","magic_close"],"visited":1}
/**/


// Include libmagic headers

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<42) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    // Early return if no data
    if (size == 0) {
        return 0;
    }

    // 1. Get libmagic version
    int version = magic_version();
    // We'll use version to decide on flags (just as an example)
    int flags = version > 0 ? MAGIC_NONE : MAGIC_NONE;

    // 2. Create a magic cookie
    magic_t cookie = magic_open(flags);
    if (cookie == nullptr) {
        // Can't proceed without cookie
        return 0;
    }

    // 3. Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // 4. Get file descriptor for the input file
    int fd = fuzz_fileno(in_file);
    
    // 5. Prepare buffers for magic_load_buffers
    // We'll use the input data as a single buffer
    void *buffers[1] = {(void *)data};
    size_t sizes[1] = {size};
    
    // 6. Load magic database from buffers
    int load_result = magic_load_buffers(cookie, buffers, sizes, 1);
    
    // 7. Check for errors after loading
    if (load_result != 0) {
        const char *error_msg = magic_error(cookie);
        // Clean up and return
        assert_file_closed(&in_file);;
        magic_close(cookie);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 8. Compile the magic database to a file
    // Using a fixed output file name as per instruction
    int compile_result = magic_compile(cookie, fuzz_str_1);
    
    // 9. List the magic database to a file
    int list_result = magic_list(cookie, fuzz_str_2);
    
    // 10. Check the magic database for consistency
    int check_result = magic_check(cookie, fuzz_str_3);
    
    // 11. Get error message (even if no error, to ensure API is called)
    const char *error_msg = magic_error(cookie);
    
    // 12. Clean up resources
    assert_file_closed(&in_file);;
    magic_close(cookie);
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
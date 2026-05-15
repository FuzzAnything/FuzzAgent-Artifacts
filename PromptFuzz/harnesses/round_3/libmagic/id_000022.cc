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
//<ID> 505
//<Prompt> []
/*<Combination>: [
*/
//<score> 19.5, nr_unique_branch: 2
//<Quality> {"density":13,"unique_branches":{"buffer_fill":[[78,12,78,49,0,0,4,0]],"check_format_type":[[2658,4,2658,19,0,0,4,0]]},"library_calls":["magic_version","magic_open","magic_getflags","magic_setflags","magic_error","magic_errno","magic_close","magic_close","magic_compile","magic_load_buffers","magic_file","magic_descriptor","magic_getpath","magic_errno","magic_error","magic_close"],"critical_calls":["magic_version","magic_open","magic_getflags","magic_setflags","magic_compile","magic_load_buffers","magic_file","magic_descriptor","magic_getpath","magic_errno","magic_error","magic_close"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified libmagic APIs to achieve the event of compiling, loading, and testing a magic database from input data:

*/


// Include libmagic headers

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size < 10) {
        // Need at least some data to work with
        return 0;
    }

    // First, get the libmagic version
    int version = magic_version();
    
    // Open a magic cookie
    magic_t cookie = magic_open(fuzz_int32_t_1);
    if (cookie == nullptr) {
        // Failed to open magic cookie
        return 0;
    }

    // Set some flags - try to set multiple flags for better coverage
    int flags = magic_getflags(cookie);
    if (magic_setflags(cookie, flags | MAGIC_CONTINUE | MAGIC_MIME) != 0) {
        // Error setting flags
        const char *error_msg = magic_error(cookie);
        int err = magic_errno(cookie);
        magic_close(cookie);
        return 0;
    }

    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor
    int fd = fuzz_fileno(in_file);
    
    // Write input data to a file for magic_compile
    const char *temp_db_name = "temp_magic_db";
    FILE *temp_db = fopen(temp_db_name, "wb");
    if (temp_db != nullptr) {
        fwrite(data, 1, size, temp_db);
        assert_file_closed(&temp_db);;
        
        // Try to compile the magic database
        if (magic_compile(cookie, fuzz_str_2) == 0) {
            // Compilation succeeded, now try to load it
            // Prepare buffer for magic_load_buffers
            void *buffers[1] = {(void *)data};
            size_t sizes[1] = {size};
            
            if (magic_load_buffers(cookie, buffers, sizes, 1) == 0) {
                // Successfully loaded, now test some operations
                
                // Test magic_file with a test file
                const char *test_file_name = "test_input";
                FILE *test_file = fopen(test_file_name, "wb");
                if (test_file != nullptr) {
                    // Write some test data
                    uint8_t test_data[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}; // PNG header
                    fwrite(test_data, 1, sizeof(test_data), test_file);
                    assert_file_closed(&test_file);;
                    
                    // Try to identify the file
                    const char *result = magic_file(cookie, test_file_name);
                    if (result != nullptr) {
                        // Result obtained
                    }
                    
                    // Clean up test file
                    remove(test_file_name);
                }
                
                // Also test with the descriptor
                if (fd >= 0) {
                    const char *desc_result = magic_descriptor(cookie, fd);
                    if (desc_result != nullptr) {
                        // Result obtained
                    }
                }
            }
        }
        
        // Clean up temp file
        remove(temp_db_name);
    }

    // Try to get magic database path
    const char *db_path = magic_getpath(nullptr, fuzz_int32_t_3);
    if (db_path != nullptr) {
        // Path obtained
    }

    // Check for any accumulated errors
    int magic_err = magic_errno(cookie);
    if (magic_err != 0) {
        const char *error_msg = magic_error(cookie);
    }

    // Clean up
    if (in_file != nullptr) {
        assert_file_closed(&in_file);;
    }
    magic_close(cookie);

    assert_file_closed(&temp_db);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
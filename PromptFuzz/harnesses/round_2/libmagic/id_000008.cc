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
//<ID> 40
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["magic_open","magic_getflags","magic_setflags","magic_load","magic_load_buffers","magic_buffer","magic_descriptor","magic_descriptor","magic_open","magic_getflags","magic_setflags","magic_load","magic_buffer","magic_close","magic_close"],"critical_calls":["magic_open","magic_getflags","magic_setflags","magic_load","magic_load_buffers","magic_buffer","magic_descriptor","magic_descriptor","magic_open","magic_getflags","magic_setflags","magic_load","magic_buffer","magic_close","magic_close"],"visited":0}
/*We'll create a fuzz driver that tests libmagic's ability to identify file types using both in-memory buffers and file descriptors. Here's the step-by-step implementation:

*/


// Include libmagic headers

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size == 0) {
        return 0;
    }

    // Initialize libmagic
    magic_t cookie = magic_open(MAGIC_NONE);
    if (cookie == nullptr) {
        return 0;
    }

    // Use magic_getflags - get current flags
    int flags = magic_getflags(cookie);
    
    // Use magic_setflags - set some flags for better detection
    // MAGIC_MIME_TYPE = 0x000010, MAGIC_MIME_ENCODING = 0x000400
    magic_setflags(cookie, flags | 0x000010 | 0x000400);

    // Create a temporary file for the database
    FILE *db_file = fopen("magic_db", "wb");
    if (db_file != nullptr) {
        // Write some minimal magic database content
        // Basic magic database structure
        const char *magic_db_content = 
            "0 string PK\\003\\004 ZIP archive\n"
            "0 string \\x1f\\x8b\\x08 gzip compressed data\n"
            "0 string %PDF PDF document\n"
            "0 string \\x89PNG\\x0d\\x0a\\x1a\\x0a PNG image\n";
        
        fwrite(magic_db_content, 1, strlen(magic_db_content), db_file);
        assert_file_closed(&db_file);;
        
        // Use magic_load - load magic database from file
        magic_load(cookie, "magic_db");
    }

    // Use magic_load_buffers - load additional magic rules from buffer
    // Prepare buffer array
    void *buffer_array[1];
    size_t buffer_sizes[1];
    
    // Create a minimal magic buffer
    const char *buffer_magic = 
        "0 string \\xff\\xd8\\xff JPEG image\n"
        "0 string GIF8 GIF image\n";
    
    buffer_array[0] = malloc(strlen(buffer_magic));
    if (buffer_array[0] != nullptr) {
        memcpy(buffer_array[0], buffer_magic, strlen(buffer_magic));
        buffer_sizes[0] = strlen(buffer_magic);
        
        // Load magic from buffers
        magic_load_buffers(cookie, buffer_array, buffer_sizes, 1);
        
        // Free the allocated buffer
        free(buffer_array[0]);
    }

    // Use magic_buffer - identify data from buffer
    const char *buffer_result = magic_buffer(cookie, data, size);
    
    // Create a FILE* from the input data for testing
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file != nullptr) {
	int fuzz_fd_1 = fuzz_fileno(in_file);

        // Use magic_descriptor - identify data from file descriptor
        const char *desc_result = magic_descriptor(cookie, fuzz_fd_1);
        
        // Close the file
        assert_file_closed(&in_file);;
    }

    // Create a simple test file to verify magic_descriptor works
    FILE *test_file = fopen("test_input", "wb");
    if (test_file != nullptr) {
        // Write a small portion of data to test file
        size_t write_size = (size > 100) ? 100 : size;
        fwrite(data, 1, write_size, test_file);
        assert_file_closed(&test_file);;
        
        // Test magic_descriptor with the test file
        FILE *read_test = fopen("test_input", "rb");
        if (read_test != nullptr) {
	int fuzz_fd_0 = fuzz_fileno(read_test);

            const char *test_result = magic_descriptor(cookie, fuzz_fd_0);
            assert_file_closed(&read_test);;
        }
        
        // Clean up test file
        remove("test_input");
    }

    // Test with another cookie to ensure flags work correctly
    magic_t cookie2 = magic_open(MAGIC_SYMLINK | MAGIC_COMPRESS);
    if (cookie2 != nullptr) {
        // Test flag operations
        int flags2 = magic_getflags(cookie2);
        magic_setflags(cookie2, flags2 | MAGIC_MIME);
        
        // Load with default database (if available)
        magic_load(cookie2, nullptr);
        
        // Test buffer identification
        const char *result2 = magic_buffer(cookie2, data, (size > 10) ? 10 : size);
        
        // Clean up
        magic_close(cookie2);
    }

    // Clean up resources
    magic_close(cookie);
    
    // Remove temporary files
    remove("magic_db");
    remove("output_file");  // In case it was created

    assert_file_closed(&test_file);
	assert_file_closed(&in_file);
	assert_file_closed(&db_file);
	assert_file_name_closed("output_file");
	return 0;
}
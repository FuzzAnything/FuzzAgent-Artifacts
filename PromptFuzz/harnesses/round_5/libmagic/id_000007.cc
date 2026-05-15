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
//<ID> 140
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 7
//<Quality> {"density":8,"unique_branches":{"load_1":[[1371,8,1371,22,0,0,4,0],[1374,17,1374,37,0,0,4,0],[1374,17,1374,37,0,0,4,1],[1375,10,1375,45,0,0,4,0],[1375,10,1375,45,0,0,4,1],[1376,10,1377,27,0,0,4,1],[1380,9,1380,29,0,0,4,0]]},"library_calls":["magic_open","magic_getpath","magic_getflags","magic_setflags","magic_setparam","magic_load","magic_check","magic_compile","magic_list","magic_descriptor","magic_close"],"critical_calls":["magic_open","magic_getpath","magic_getflags","magic_setflags","magic_setparam","magic_load","magic_check","magic_compile","magic_list","magic_descriptor","magic_close"],"visited":0}
/*Looking at the list of APIs, I can see they can be used together to create a scenario where we:
1. Open a magic cookie
2. Set parameters and flags
3. Load and compile a magic database from input
4. Check and list the compiled database
5. Use the compiled database to identify data from a file descriptor
6. Get the path to the magic database

Here's the fuzz driver implementing this scenario:

*/


// Include libmagic headers

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0; // No data to process
    }

    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    magic_t magic_cookie = nullptr;
    int result = 0;
    const char *path = nullptr;
    const char *description = nullptr;
    char *temp_buffer = nullptr;
    
    // 1. Create input file from fuzzing data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create output file for compiled magic database
    out_file = fopen("output.mgc", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Open magic cookie with flags
    magic_cookie = magic_open(MAGIC_NONE | MAGIC_CHECK);
    if (!magic_cookie) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get path to default magic database
    path = magic_getpath(NULL, 0);
    if (path) {
        // Store path for potential use
        // (just calling the API as required)
    }
    
    // 5. Get current flags
    int flags = magic_getflags(magic_cookie);
    
    // 6. Set some flags (add MAGIC_COMPRESS flag)
    flags |= MAGIC_COMPRESS;
    result = magic_setflags(magic_cookie, flags);
    
    // 7. Set a parameter (buffer size)
    size_t buffer_size = 1024;
    result = magic_setparam(magic_cookie, MAGIC_PARAM_BYTES_MAX, &buffer_size);
    
    // 8. Create a temporary file with the input data to load as magic database
    FILE *magic_source = fopen("input_file", "wb");
    if (magic_source) {
        fwrite(data, 1, size, magic_source);
        assert_file_closed(&magic_source);;
    }
    
    // 9. Load magic database from file
    result = magic_load(magic_cookie, "input_file");
    
    // 10. Check if the loaded magic database is valid
    result = magic_check(magic_cookie, "input_file");
    
    if (result == 0) {
        // 11. Compile the magic database to binary format
        result = magic_compile(magic_cookie, "output.mgc");
        
        // 12. List magic entries to a file
        FILE *list_file = fopen("list_output.txt", "w");
        if (list_file) {
            // Magic list writes to the given file
            result = magic_list(magic_cookie, "list_output.txt");
            assert_file_closed(&list_file);;
        }
    }
    
    // 13. Use magic_descriptor to identify the content of our input file
    int fd = fuzz_fileno(in_file);
    if (fd >= 0) {
        // Reset file pointer to beginning
        fseek(in_file, 0, SEEK_SET);
        description = magic_descriptor(magic_cookie, fd);
        
        if (description) {
            // Store the description (just using the API as required)
            // We could print it or use it somehow
        }
    }
    
    // Clean up
    if (magic_cookie) {
        magic_close(magic_cookie);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    // Remove temporary files
    remove("input_file");
    remove("output.mgc");
    remove("list_output.txt");
    
    assert_file_closed(&magic_source);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}
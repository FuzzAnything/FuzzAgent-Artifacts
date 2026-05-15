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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["magic_open","magic_getflags","magic_setflags","magic_close","magic_load_buffers","magic_errno","magic_close","magic_getparam","magic_file","magic_buffer","magic_errno","magic_close"],"critical_calls":["magic_open","magic_getflags","magic_setflags","magic_load_buffers","magic_getparam","magic_file","magic_buffer","magic_errno","magic_close"],"visited":0}
/**/


// Include libmagic headers

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return if input is too small for meaningful processing
    if (size < 16) {
        return 0;
    }

    // 1. Create magic cookie with flags
    magic_t cookie = magic_open(MAGIC_CONTINUE | MAGIC_ERROR);
    if (!cookie) {
        return 0;
    }

    // 2. Set flags on the cookie
    int current_flags = magic_getflags(cookie);
    magic_setflags(cookie, current_flags | MAGIC_MIME);

    // 3. Prepare buffer for loading magic database
    // Split input data: first part for database, second part for testing
    size_t db_size = size / 2;
    size_t test_size = size - db_size;
    
    if (db_size == 0 || test_size == 0) {
        magic_close(cookie);
        return 0;
    }

    // Create buffer array for magic_load_buffers
    void *buffers[1] = {(void *)data};  // Use first half as database
    size_t sizes[1] = {db_size};

    // 4. Load magic database from buffer
    if (magic_load_buffers(cookie, buffers, sizes, 1) != 0) {
        // If loading fails, try to get error
        int err = magic_errno(cookie);
        (void)err; // Use variable to avoid unused warning
        
        magic_close(cookie);
        return 0;
    }

    // 5. Get parameter from magic cookie
    int param_value;
    if (magic_getparam(cookie, MAGIC_PARAM_INDIR_MAX, &param_value) != 0) {
        // Handle error if needed
    }

    // 6. Create temporary file with test data for magic_file
    const char *filename = "input_file";
    FILE *out_file = fopen(filename, "wb");
    if (out_file) {
        fwrite(data + db_size, 1, test_size, out_file);
        assert_file_closed(&out_file);;
    }

    // 7. Use magic_file with the created file
    if (out_file) {
        const char *file_result = magic_file(cookie, filename);
        (void)file_result; // Use variable to avoid unused warning
        
        // Clean up temporary file
        remove(filename);
    }

    // 8. Use magic_buffer with the test data portion
    const char *buffer_result = magic_buffer(cookie, data + db_size, test_size);
    (void)buffer_result; // Use variable to avoid unused warning

    // 9. Check for any errors that occurred
    int final_err = magic_errno(cookie);
    (void)final_err; // Use variable to avoid unused warning

    // 10. Clean up magic cookie
    magic_close(cookie);

    assert_file_closed(&out_file);
	assert_file_name_closed("input_file");
	return 0;
}
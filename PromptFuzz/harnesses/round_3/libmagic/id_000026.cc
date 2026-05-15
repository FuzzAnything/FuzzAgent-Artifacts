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
//<ID> 622
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":10,"unique_branches":{"mget":[[72,59,72,76,49,0,4,0]]},"library_calls":["magic_getpath","magic_open","magic_load_buffers","magic_getflags","magic_close","magic_descriptor","magic_file","magic_errno","magic_error","magic_setflags","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_load_buffers","magic_getflags","magic_descriptor","magic_file","magic_errno","magic_error","magic_setflags","magic_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 2) {
        return 0; // Not enough data to process meaningfully
    }

    // 1. Get magic database path
    const char *magic_path = magic_getpath(NULL, fuzz_int32_t_1);
    
    // 2. Open magic cookie
    magic_t cookie = magic_open(fuzz_int32_t_2);
    if (!cookie) {
        return 0;
    }
    
    // 3. Split input into two parts: first for magic database buffer, second for file content
    size_t magic_data_size = size / 2;
    size_t file_data_size = size - magic_data_size;
    
    // Create buffer array for magic database
    void *buffers[1] = {(void *)data};
    size_t sizes[1] = {magic_data_size};
    
    // 4. Load magic database from buffer
    int load_result = magic_load_buffers(cookie, buffers, sizes, 1);
    
    // 5. Get current flags
    int current_flags = magic_getflags(cookie);
    
    // 6. Create temporary input file with second half of data
    FILE *in_file = fmemopen((void *)(data + magic_data_size), file_data_size, "rb");
    if (!in_file) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Get file descriptor
    int fd = fuzz_fileno(in_file);
    
    // 8. Identify file using descriptor
    const char *descriptor_result = magic_descriptor(cookie, fd);
    
    // 9. Reset file pointer for file-based identification
    rewind(in_file);
    
    // Create a temporary file on disk for file-based identification
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file) {
        fwrite(data + magic_data_size, 1, file_data_size, temp_file);
        assert_file_closed(&temp_file);;
    }
    
    // 10. Identify file using filename
    const char *file_result = magic_file(cookie, "input_file");
    
    // 11. Get error information
    int error_number = magic_errno(cookie);
    const char *error_string = magic_error(cookie);
    
    // 12. Set flags (restore current flags)
    magic_setflags(cookie, current_flags);
    
    // Cleanup
    assert_file_closed(&in_file);;
    magic_close(cookie);
    
    // Remove temporary file
    remove("input_file");
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}
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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 5
//<Quality> {"density":14,"unique_branches":{"file_getbuffer":[[557,6,557,23,0,0,4,0]],"magic_getparam":[[661,2,661,28,0,0,4,0],[682,2,682,28,0,0,4,1]],"magic_setparam":[[619,2,619,28,0,0,4,0],[640,2,640,28,0,0,4,1]]},"library_calls":["magic_open","magic_getflags","magic_setflags","magic_setparam","magic_getparam","magic_close","magic_load_buffers","magic_load","magic_compile","magic_descriptor","magic_file","magic_errno","magic_error","magic_close"],"critical_calls":["magic_open","magic_getflags","magic_setflags","magic_setparam","magic_getparam","magic_load_buffers","magic_load","magic_compile","magic_descriptor","magic_file","magic_errno","magic_error","magic_close"],"visited":1}
/*Here's a fuzz driver that uses all the specified libmagic APIs to achieve the event of identifying file types from input data:

*/


extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<26) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
        return 0;
    }

    // 1. Open magic database
    magic_t cookie = magic_open(fuzz_int32_t_1);
    if (cookie == nullptr) {
        return 0;
    }

    // 2. Get current flags
    int flags = magic_getflags(cookie);
    
    // 3. Set some flags (toggle between different flag sets)
    int new_flags = flags ^ MAGIC_CONTINUE;  // Toggle CONTINUE flag
    magic_setflags(cookie, new_flags);
    
    // 4. Set parameter (buffer size limit)
    size_t param_value = size;
    magic_setparam(cookie, fuzz_int32_t_2, &param_value);
    
    // 5. Get parameter back to verify
    size_t retrieved_param;
    magic_getparam(cookie, fuzz_int32_t_3, &retrieved_param);
    
    // 6. Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Get file descriptor for the input
    int fd = fuzz_fileno(in_file);
    
    // 8. Load magic database from buffer
    // We'll use the input data itself as a magic database buffer
    // This is unusual but valid for fuzzing
    void* buffers[1] = {(void*)data};
    size_t buffer_sizes[1] = {size};
    int load_result = magic_load_buffers(cookie, buffers, buffer_sizes, 1);
    
    // 9. If loading failed, try loading from default database
    if (load_result == -1) {
        // Try to load from default location
        magic_load(cookie, nullptr);
    }
    
    // 10. Try to compile the magic database to a file
    // We'll use "output_file" as specified
    FILE *out_file = fopen("output_file", "wb");
    if (out_file != nullptr) {
        // Write some data to the file first
        fwrite(data, 1, size > 100 ? 100 : size, out_file);
        assert_file_closed(&out_file);;
    }
    
    // Try to compile magic database
    magic_compile(cookie, fuzz_str_4);
    
    // 11. Use magic_descriptor to identify the input file descriptor
    const char* desc_result = magic_descriptor(cookie, fd);
    
    // 12. Also try magic_file on a file
    // Create a temporary file with the input data
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file != nullptr) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
        
        // Use magic_file to identify it
        const char* file_result = magic_file(cookie, "input_file");
        
        // Clean up temporary file
        remove("input_file");
    }
    
    // 13. Check for errors
    int err = magic_errno(cookie);
    if (err != 0) {
        const char* error_msg = magic_error(cookie);
        // Error message is available but we don't need to do anything with it for fuzzing
    }
    
    // 14. Clean up
    assert_file_closed(&in_file);;
    magic_close(cookie);
    
    // Clean up output file if it exists
    remove("output_file");
    
    assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
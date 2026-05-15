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
//<ID> 466
//<Prompt> []
/*<Combination>: [
*/
//<score> 58.666668, nr_unique_branch: 19
//<Quality> {"density":16,"unique_branches":{"lzmacmp":[[148,6,148,13,0,0,4,1],[148,17,148,32,0,0,4,1]],"do_core_note":[[830,7,830,18,0,0,4,0],[830,22,830,50,0,0,4,1],[839,23,839,59,0,0,4,0],[852,2,852,22,0,0,4,0],[853,7,853,38,0,0,4,1],[900,7,900,26,0,0,4,0],[900,30,900,52,0,0,4,1]],"file_tryelf":[[1924,9,1924,32,0,0,4,0],[66,7,66,32,19,0,4,0]],"do_pax_note":[[67,29,67,54,2,0,4,1]],"methodname":[[1066,2,1066,16,0,0,4,0]],"uncompresszlib":[[640,20,640,38,0,0,4,1]],"uncompressbuf":[[1172,7,1172,15,0,0,4,1],[1226,6,1226,14,0,0,4,1]],"handledesc":[[972,6,972,14,0,0,4,0]],"getdecompressor":[[1103,2,1103,16,0,0,4,0]],"uncompressxzlib":[[720,23,720,44,0,0,4,1]]},"library_calls":["magic_open","magic_getpath","magic_getflags","magic_setflags","magic_setparam","magic_getparam","magic_close","magic_close","magic_load_buffers","magic_check","magic_list","magic_buffer","magic_descriptor","magic_file","magic_error","magic_errno","magic_close"],"critical_calls":["magic_open","magic_getpath","magic_getflags","magic_setflags","magic_setparam","magic_getparam","magic_load_buffers","magic_check","magic_list","magic_buffer","magic_descriptor","magic_file","magic_error","magic_errno","magic_close"],"visited":2}
/**/



extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Early return for insufficient data
    if (size < 4) {
        return 0;
    }

    // 1. Create magic cookie
    magic_t magic_cookie = magic_open(MAGIC_NONE);
    if (!magic_cookie) {
        return 0;
    }

    // 2. Get magic database path
    const char *db_path = magic_getpath(nullptr, 0);
    (void)db_path; // Use to avoid unused variable warning

    // 3. Get current flags
    int current_flags = magic_getflags(magic_cookie);
    
    // 4. Set new flags (toggle between different flag combinations)
    int new_flags = current_flags ^ MAGIC_COMPRESS;
    magic_setflags(magic_cookie, new_flags);

    // 5. Set and get parameters
    // Set MAGIC_PARAM_BYTES_MAX
    size_t bytes_max = 1024 * 1024; // 1MB
    magic_setparam(magic_cookie, MAGIC_PARAM_BYTES_MAX, &bytes_max);
    
    // Get MAGIC_PARAM_BYTES_MAX back
    size_t retrieved_bytes_max = 0;
    magic_getparam(magic_cookie, MAGIC_PARAM_BYTES_MAX, &retrieved_bytes_max);

    // 6. Create a FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        magic_close(magic_cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // 7. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        magic_close(magic_cookie);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 8. Prepare buffer for magic_load_buffers
    // Use the input data as a magic database buffer
    void *buffer_array[1] = {(void *)data};
    size_t buffer_sizes[1] = {size};
    
    // 9. Load magic database from buffer
    int load_result = magic_load_buffers(magic_cookie, buffer_array, buffer_sizes, 1);
    
    // 10. Check the loaded database
    int check_result = magic_check(magic_cookie, nullptr);
    
    // 11. List magic entries to file
    // First, rewind the input file
    rewind(in_file);
    int list_result = magic_list(magic_cookie, "output_file");
    
    // 12. Use magic_buffer to identify the input data
    const char *buffer_result = magic_buffer(magic_cookie, data, size);
    if (buffer_result) {
        // Write result to output file
        fwrite(buffer_result, strlen(buffer_result), 1, out_file);
    }

    // 13. Get file descriptor for input file
    int fd = fuzz_fileno(in_file);
    
    // 14. Use magic_descriptor to identify the file
    const char *descriptor_result = magic_descriptor(magic_cookie, fd);
    if (descriptor_result) {
        fwrite(descriptor_result, strlen(descriptor_result), 1, out_file);
    }

    // 15. Use magic_file to identify a file
    // Write input data to a temporary file first
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file) {
        fwrite(data, size, 1, temp_file);
        assert_file_closed(&temp_file);;
        
        const char *file_result = magic_file(magic_cookie, "input_file");
        if (file_result) {
            fwrite(file_result, strlen(file_result), 1, out_file);
        }
        
        // Remove temporary file
        remove("input_file");
    }

    // 16. Check for errors
    const char *error_msg = magic_error(magic_cookie);
    if (error_msg) {
        fwrite(error_msg, strlen(error_msg), 1, out_file);
    }

    // 17. Get error number
    int errno_val = magic_errno(magic_cookie);
    (void)errno_val; // Use to avoid unused variable warning

    // 18. Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    // 19. Close magic cookie
    magic_close(magic_cookie);

    assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
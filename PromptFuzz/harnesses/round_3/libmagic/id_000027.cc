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
//<ID> 625
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 30
//<Quality> {"density":14,"unique_branches":{"moffset":[[897,2,897,18,0,0,4,0]],"cvt_double":[[1139,3,1139,18,1,0,4,0],[1142,3,1142,20,1,0,4,0]],"cvt_float":[[1139,3,1139,18,1,0,4,0],[1145,3,1145,23,1,0,4,0]],"file_printable":[[850,19,850,29,0,0,4,1],[855,7,855,22,0,0,4,0]],"mget":[[1686,4,1686,21,0,0,4,0],[1702,4,1702,20,0,0,4,0],[1802,8,1802,61,0,0,4,1],[1814,7,1815,50,0,0,4,1],[72,30,72,40,18,0,4,0],[72,30,72,40,44,0,4,1],[72,59,72,76,44,0,4,1],[109,22,109,25,45,0,4,0],[109,22,109,25,79,0,4,0],[109,22,109,25,82,0,4,1],[72,30,72,40,87,0,4,1],[72,59,72,76,87,0,4,1],[109,22,109,25,88,0,4,0],[109,22,109,25,88,0,4,1]],"file_regcomp":[[753,16,753,41,0,0,4,1]],"do_ops":[[1517,3,1517,21,0,0,4,0]],"varexpand":[[543,16,543,29,0,0,4,0]],"file_pstring_length_size":[[3691,2,3691,19,0,0,4,0]],"handle_annotation":[[2540,21,2540,50,0,0,4,0],[2544,7,2544,57,0,0,4,0]],"file_pstring_get_length":[[3729,2,3729,19,0,0,4,0],[3743,6,3743,51,0,0,4,0],[3745,7,3745,24,0,0,4,1]]},"library_calls":["magic_getpath","magic_open","magic_getflags","magic_setflags","magic_getflags","magic_load_buffers","magic_errno","magic_error","magic_load","magic_close","magic_file","magic_descriptor","magic_buffer","magic_list","magic_close"],"critical_calls":["magic_getpath","magic_open","magic_getflags","magic_setflags","magic_getflags","magic_load_buffers","magic_errno","magic_error","magic_load","magic_file","magic_descriptor","magic_buffer","magic_list","magic_close"],"visited":3}
/**/


// Include libmagic headers

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
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





    // Early return if no data
    if (size == 0) {
        return 0;
    }

    // 1. Get magic database path
    const char *magic_path = magic_getpath(nullptr, fuzz_int32_t_1);
    if (magic_path) {
        // Could use magic_path if needed
    }

    // 2. Create magic cookie with default flags
    magic_t cookie = magic_open(fuzz_int32_t_2);
    if (cookie == nullptr) {
        return 0;
    }

    // 3. Set and get flags
    int original_flags = magic_getflags(cookie);
    // Try setting some common flags
    int new_flags = original_flags | MAGIC_MIME;
    magic_setflags(cookie, new_flags);
    int current_flags = magic_getflags(cookie);

    // 4. Load magic database from buffer
    // Prepare buffer array for magic_load_buffers
    void *buffer_ptr = const_cast<void*>(static_cast<const void*>(data));
    size_t buffer_size = size;
    void **buffers = &buffer_ptr;
    size_t *sizes = &buffer_size;
    
    int load_result = magic_load_buffers(cookie, buffers, sizes, 1);
    
    // 5. Check for errors
    int err = magic_errno(cookie);
    const char *error_msg = magic_error(cookie);
    
    // If loading from buffer failed, try loading default database
    if (load_result != 0) {
        magic_load(cookie, nullptr); // Load default database
    }

    // 6. Create input file from data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        magic_close(cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // 7. Get file descriptor for magic_descriptor
    int fd = fuzz_fileno(in_file);
    
    // 8. Use magic_file with a temporary file
    // First, create a temporary file with the input data
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
        
        // Use magic_file to identify the file
        const char *file_result = magic_file(cookie, "input_file");
        if (file_result) {
            // File type identified
        }
    }

    // 9. Use magic_descriptor with the fmemopen file descriptor
    const char *descriptor_result = magic_descriptor(cookie, fd);
    if (descriptor_result) {
        // Descriptor identified
    }

    // 10. Use magic_buffer with the input data
    const char *buffer_result = magic_buffer(cookie, data, size);
    if (buffer_result) {
        // Buffer identified
    }

    // 11. Use magic_list to output magic rules
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        int list_result = magic_list(cookie, "output_file");
        assert_file_closed(&out_file);;
    }

    // 12. Clean up
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    magic_close(cookie);
    
    assert_file_closed(&out_file);
	assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_fd_closed(fd);
	return 0;
}
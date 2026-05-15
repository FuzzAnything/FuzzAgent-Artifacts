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
//<ID> 436
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 22
//<Quality> {"density":17,"unique_branches":{"magic_getparam":[[661,2,661,28,0,0,4,0],[682,2,682,28,0,0,4,1]],"file_buffer":[[422,8,422,26,0,0,4,1]],"file_or_fd":[[444,2,444,9,0,0,4,0]],"file_fsmagic":[[162,7,162,30,0,0,4,0]],"do_core_note":[[67,29,67,54,4,0,4,1]],"do_os_note":[[705,6,705,32,0,0,4,0],[705,36,705,47,0,0,4,0],[710,7,710,58,0,0,4,1],[718,7,718,28,0,0,4,1],[67,29,67,54,23,0,4,1],[67,6,67,25,30,0,4,0],[67,29,67,54,30,0,4,0]],"doshn":[[1565,3,1565,20,0,0,4,0],[1567,4,1567,17,0,0,4,1],[1568,4,1568,19,0,0,4,1],[1569,4,1569,17,0,0,4,1],[1570,4,1570,15,0,0,4,1],[1571,4,1571,17,0,0,4,1],[1573,4,1573,11,0,0,4,0]],"do_memtag_note":[[775,6,775,31,0,0,4,1],[67,29,67,54,2,0,4,0]]},"library_calls":["magic_open","magic_getpath","magic_getflags","magic_setflags","magic_getflags","magic_setparam","magic_getparam","magic_load_buffers","magic_compile","magic_check","magic_list","magic_buffer","magic_descriptor","magic_file","magic_error","magic_errno","magic_close"],"critical_calls":["magic_open","magic_getpath","magic_getflags","magic_setflags","magic_getflags","magic_setparam","magic_getparam","magic_load_buffers","magic_compile","magic_check","magic_list","magic_buffer","magic_descriptor","magic_file","magic_error","magic_errno","magic_close"],"visited":2}
/*Here's a C++ fuzz driver that uses all the specified libmagic APIs to process the input data as if it were libmagic's output data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // 1. Open magic database
    magic_t magic_cookie = magic_open(MAGIC_NONE | MAGIC_ERROR);
    if (!magic_cookie) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 2. Get magic database path
    const char *magic_path = magic_getpath(NULL, 0);
    if (magic_path) {
        // Could use magic_path if needed
    }

    // 3. Set and get flags
    int original_flags = magic_getflags(magic_cookie);
    magic_setflags(magic_cookie, MAGIC_CONTINUE | MAGIC_ERROR);
    int new_flags = magic_getflags(magic_cookie);

    // 4. Set and get parameters (using MAGIC_PARAM_INDIR_MAX as example)
    size_t indir_max = 50;
    magic_setparam(magic_cookie, MAGIC_PARAM_INDIR_MAX, &indir_max);
    
    size_t retrieved_indir_max = 0;
    magic_getparam(magic_cookie, MAGIC_PARAM_INDIR_MAX, &retrieved_indir_max);

    // 5. Load magic database from buffer
    // We'll use the input data as if it were a magic database buffer
    void *buffer_array[] = {(void *)data};
    size_t buffer_sizes[] = {size};
    int load_result = magic_load_buffers(magic_cookie, buffer_array, buffer_sizes, 1);
    
    // 6. Try to compile the magic database
    if (load_result == 0) {
        magic_compile(magic_cookie, "compiled.mgc");
    }

    // 7. Check the magic database
    magic_check(magic_cookie, magic_path ? magic_path : ".");

    // 8. List magic entries to output file
    fseek(out_file, 0, SEEK_SET);
    magic_list(magic_cookie, "output_file");

    // 9. Use magic_buffer on the input data
    const char *buffer_result = magic_buffer(magic_cookie, data, size);

    // 10. Use magic_descriptor on the input file descriptor
    fseek(in_file, 0, SEEK_SET);
    const char *descriptor_result = magic_descriptor(magic_cookie, in_fd);

    // 11. Create a temporary file and use magic_file
    const char *temp_filename = "temp_input_file";
    FILE *temp_file = fopen(temp_filename, "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
        
        const char *file_result = magic_file(magic_cookie, temp_filename);
        
        // Clean up temp file
        remove(temp_filename);
    }

    // 12. Get error information
    const char *error_msg = magic_error(magic_cookie);
    int errno_val = magic_errno(magic_cookie);

    // 13. Clean up
    magic_close(magic_cookie);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Clean up output file
    remove("output_file");
    remove("compiled.mgc");

    assert_file_closed(&temp_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
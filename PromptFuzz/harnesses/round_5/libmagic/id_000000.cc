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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["magic_open","magic_getflags","magic_setflags","magic_load","magic_close","magic_getparam","magic_check","magic_compile","magic_descriptor","magic_file","magic_close"],"critical_calls":["magic_open","magic_getflags","magic_setflags","magic_load","magic_getparam","magic_check","magic_compile","magic_descriptor","magic_file","magic_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
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
        return 0;
    }

    // Create a FILE* from input data
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

    // Get file descriptor for input file
    int fd = fuzz_fileno(in_file);

    // 1. Open magic cookie
    magic_t magic_cookie = magic_open(MAGIC_MIME | MAGIC_CHECK);
    if (!magic_cookie) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 2. Set flags
    int current_flags = magic_getflags(magic_cookie);
    magic_setflags(magic_cookie, current_flags | MAGIC_CONTINUE);

    // 3. Load default magic database
    if (magic_load(magic_cookie, NULL) != 0) {
        magic_close(magic_cookie);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 4. Get parameter example
    int param_value = 0;
    magic_getparam(magic_cookie, MAGIC_PARAM_INDIR_MAX, &param_value);

    // 5. Check magic database
    magic_check(magic_cookie, NULL);

    // 6. Compile magic database to output file
    magic_compile(magic_cookie, "output_magic");

    // 7. Use magic_descriptor with input file descriptor
    const char *descriptor_result = magic_descriptor(magic_cookie, fd);
    if (descriptor_result) {
        // Write result to output file
        fwrite(descriptor_result, strlen(descriptor_result), 1, out_file);
    }

    // 8. Reset file pointer and use magic_file
    rewind(in_file);
    assert_file_closed(&in_file);;
    in_file = NULL;
    
    // Create a new file with input data for magic_file
    FILE *input_file = fopen("input_file", "wb");
    if (input_file) {
        fwrite(data, size, 1, input_file);
        assert_file_closed(&input_file);;
        
        const char *file_result = magic_file(magic_cookie, "input_file");
        if (file_result) {
            fwrite(file_result, strlen(file_result), 1, out_file);
        }
        
        // Remove temporary file
        remove("input_file");
    }

    // Cleanup
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    magic_close(magic_cookie);
    
    // Remove output files
    remove("output_file");
    remove("output_magic");
    
    assert_file_closed(&input_file);
	assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
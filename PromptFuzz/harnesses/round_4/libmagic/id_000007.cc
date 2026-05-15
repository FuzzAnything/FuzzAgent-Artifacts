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
//<ID> 334
//<Prompt> []
/*<Combination>: [
*/
//<score> 78, nr_unique_branch: 21
//<Quality> {"density":6,"unique_branches":{"file_varint2uintmax_t":[[3651,8,3651,24,0,0,4,1]],"set_test_type":[[1223,2,1223,19,0,0,4,0]],"apprentice_1":[[501,6,501,17,0,0,4,1]],"file_ascmagic_with_encoding":[[141,6,141,14,0,0,4,1]],"apprentice_map":[[3371,6,3371,40,0,0,4,1],[3375,6,3375,65,0,0,4,1]],"parse":[[2176,4,2176,12,0,0,4,0],[2180,4,2180,12,0,0,4,0],[2190,4,2190,12,0,0,4,0],[309,3,309,22,59,0,4,0],[317,3,317,20,66,0,4,0]],"apprentice_magic_strength_1":[[957,2,957,19,0,0,4,0]],"json_isspace":[[83,2,83,11,0,0,4,0]],"check_format_type":[[2674,17,2674,27,18,0,4,0]],"parse_string_modifier":[[1945,3,1945,11,0,0,4,0],[2004,3,2004,25,0,0,4,0],[2006,4,2006,21,0,0,4,1],[2007,4,2007,19,0,0,4,1],[2009,4,2009,11,0,0,4,0]],"nonmagic":[[866,9,866,16,0,0,4,0]],"get_standard_integer_type":[[399,3,399,11,0,0,4,0]]},"library_calls":["magic_open","magic_getpath","magic_check","magic_compile","magic_buffer","magic_errno","magic_close"],"critical_calls":["magic_open","magic_getpath","magic_check","magic_compile","magic_buffer","magic_errno","magic_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<34) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create FILE* from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 1. Open magic database
    magic_t magic_cookie = magic_open(fuzz_int32_t_1);
    if (!magic_cookie) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 2. Get default magic file path (just to call the API)
    const char *default_path = magic_getpath(NULL, fuzz_int32_t_2);
    (void)default_path; // Mark as used

    // 3. Check magic database (using input as magic file content)
    // First create a temporary file with magic data
    FILE *magic_file = fopen("input_file", "wb");
    if (magic_file) {
        fwrite(data, 1, size, magic_file);
        assert_file_closed(&magic_file);;
        
        // Check the magic database file
        int check_result = magic_check(magic_cookie, fuzz_str_3);
        (void)check_result; // Mark as used
        
        // Compile the magic database
        int compile_result = magic_compile(magic_cookie, fuzz_str_4);
        (void)compile_result; // Mark as used
        
        // Clean up temporary file
        remove("input_file");
    }

    // 4. Use magic_buffer to identify the input data
    const char *result = magic_buffer(magic_cookie, data, size);
    (void)result; // Mark as used

    // 5. Check for errors
    int err = magic_errno(magic_cookie);
    (void)err; // Mark as used

    // 6. Clean up
    magic_close(magic_cookie);
    assert_file_closed(&in_file);;

    assert_file_closed(&magic_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
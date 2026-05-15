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
//<score> 44, nr_unique_branch: 7
//<Quality> {"density":11,"unique_branches":{"apprentice_list":[[1175,25,1175,36,0,0,4,0],[1175,25,1175,36,0,0,4,1],[1176,22,1176,43,0,0,4,1]],"apprentice_1":[[521,6,521,25,0,0,4,0],[522,15,522,29,0,0,4,0],[522,15,522,29,0,0,4,1]],"file_apprentice":[[812,2,812,16,0,0,4,0]]},"library_calls":["magic_open","magic_version","magic_close","magic_close","magic_close","magic_close","magic_load_buffers","magic_error","magic_compile","magic_list","magic_check","magic_close"],"critical_calls":["magic_open","magic_version","magic_load_buffers","magic_error","magic_compile","magic_list","magic_check","magic_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified libmagic APIs to achieve the event of compiling, listing, and checking a magic database from input data:

*/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<52) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Create a magic cookie
    magic_t magic_cookie = magic_open(fuzz_int32_t_1);
    if (magic_cookie == nullptr) {
        return 0;
    }

    // Get libmagic version
    int version = magic_version();
    (void)version; // Use version to avoid unused variable warning

    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file == nullptr) {
        magic_close(magic_cookie);
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for the input file
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        magic_close(magic_cookie);
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create output files for compile and list operations
    FILE *compile_out = fopen("output_compile", "wb");
    FILE *list_out = fopen("output_list", "wb");
    if (compile_out == nullptr || list_out == nullptr) {
        if (compile_out) assert_file_closed(&compile_out);;
        if (list_out) assert_file_closed(&list_out);;
        assert_file_closed(&in_file);;
        magic_close(magic_cookie);
        assert_file_closed(&list_out);
	assert_file_closed(&compile_out);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Prepare buffer for magic_load_buffers
    void *buffer = malloc(size);
    if (buffer == nullptr) {
        assert_file_closed(&compile_out);;
        assert_file_closed(&list_out);;
        assert_file_closed(&in_file);;
        magic_close(magic_cookie);
        assert_file_closed(&list_out);
	assert_file_closed(&compile_out);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    memcpy(buffer, data, size);
    
    void *buffers[] = {buffer};
    size_t sizes[] = {size};
    
    // Load magic data from buffer
    int load_result = magic_load_buffers(magic_cookie, buffers, sizes, 1);
    
    // Check for errors after loading
    const char *error_msg = magic_error(magic_cookie);
    (void)error_msg; // Use error message to avoid unused variable warning

    // Compile the loaded magic data to output file
    int compile_result = magic_compile(magic_cookie, fuzz_str_2);
    
    // List magic entries to output file
    int list_result = magic_list(magic_cookie, fuzz_str_3);
    
    // Check the compiled database
    int check_result = magic_check(magic_cookie, fuzz_str_4);

    // Cleanup resources
    free(buffer);
    assert_file_closed(&in_file);;
    assert_file_closed(&compile_out);;
    assert_file_closed(&list_out);;
    magic_close(magic_cookie);

    assert_file_closed(&list_out);
	assert_file_closed(&compile_out);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
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
//<ID> 521
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 2
//<Quality> {"density":18,"unique_branches":{"file_buffer":[[502,7,502,34,0,0,4,1]],"file_default":[[301,7,301,36,0,0,4,1]]},"library_calls":["magic_open","magic_setflags","magic_close","magic_load","magic_errno","magic_open","magic_close","magic_setflags","magic_close","magic_close","magic_load_buffers","magic_errno","magic_descriptor","magic_descriptor","magic_errno","magic_errno","magic_close","magic_close"],"critical_calls":["magic_open","magic_setflags","magic_load","magic_errno","magic_open","magic_setflags","magic_load_buffers","magic_errno","magic_descriptor","magic_descriptor","magic_errno","magic_errno","magic_close","magic_close"],"visited":0}
/**/


// Include the libmagic header

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for the input file
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // First magic cookie - for loading from file
    magic_t magic_cookie1 = magic_open(fuzz_int32_t_1);
    if (!magic_cookie1) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set flags for first cookie
    if (magic_setflags(magic_cookie1, fuzz_int32_t_2) != 0) {
        magic_close(magic_cookie1);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Try to load magic database from default location
    // This might fail in fuzzing environment, but we call it anyway
    if (magic_load(magic_cookie1, NULL) != 0) {
        // Check error but continue
        magic_errno(magic_cookie1);
    }
    
    // Second magic cookie - for loading from buffer
    magic_t magic_cookie2 = magic_open(fuzz_int32_t_3);
    if (!magic_cookie2) {
        magic_close(magic_cookie1);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set flags for second cookie
    if (magic_setflags(magic_cookie2, fuzz_int32_t_4) != 0) {
        magic_close(magic_cookie1);
        magic_close(magic_cookie2);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Prepare buffer for magic_load_buffers
    // We'll use the input data as the magic database buffer
    void *buffer = (void *)data;
    size_t buffer_size = size;
    
    // Try to load magic database from buffer
    // This is the main event: using the input data as a magic database
    if (magic_load_buffers(magic_cookie2, &buffer, &buffer_size, 1) != 0) {
        // Check error but continue
        magic_errno(magic_cookie2);
    }
    
    // Now use both magic cookies to identify the file descriptor
    // This simulates using the loaded magic database to identify a file
    const char *result1 = magic_descriptor(magic_cookie1, fd);
    if (result1) {
        // Do something with the result to prevent optimization
        volatile const char *unused1 = result1;
        (void)unused1;
    }
    
    const char *result2 = magic_descriptor(magic_cookie2, fd);
    if (result2) {
        // Do something with the result to prevent optimization
        volatile const char *unused2 = result2;
        (void)unused2;
    }
    
    // Check for errors on both cookies
    magic_errno(magic_cookie1);
    magic_errno(magic_cookie2);
    
    // Clean up resources
    magic_close(magic_cookie1);
    magic_close(magic_cookie2);
    
    // Close the input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
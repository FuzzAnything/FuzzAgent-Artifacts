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
#include <multi.h>
}
//<ID> 1223
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_msprintf","curl_multi_waitfds","curl_multi_socket_action","curl_easy_pause","curl_easy_reset","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_msprintf","curl_multi_waitfds","curl_multi_socket_action","curl_easy_pause","curl_easy_reset","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
        return 0;
    }

    // Create FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create FILE* for output data
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create multi handle and easy handle
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    
    if (!multi_handle || !easy_handle) {
        if (easy_handle) curl_easy_cleanup(easy_handle);
        if (multi_handle) curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set up easy handle options
    // Use input file descriptor for reading
    int input_fd = fuzz_fileno(in_file);
    int output_fd = fuzz_fileno(out_file);
    
    // Configure easy handle with file protocol for fuzzing
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
    
    // Use a dummy URL (file protocol)
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Prepare buffer for curl_msprintf
    char buffer[256];
    
    // Use curl_msprintf to format a string with data size
    curl_msprintf(buffer, "Fuzzing with %zu bytes", size);

    // Create curl_waitfd structure for curl_multi_waitfds
    struct curl_waitfd wait_fds[2];
    wait_fds[0].fd = input_fd;
    wait_fds[0].events = CURL_WAIT_POLLIN;
    wait_fds[0].revents = 0;
    
    wait_fds[1].fd = output_fd;
    wait_fds[1].events = CURL_WAIT_POLLOUT;
    wait_fds[1].revents = 0;

    // Use curl_multi_waitfds
    unsigned int fd_count = 0;
    curl_multi_waitfds(multi_handle, wait_fds, 2, &fd_count);

    // Use curl_multi_socket (simulate with input_fd)
    int running_handles = 0;
    curl_multi_socket(multi_handle, (curl_socket_t)input_fd, &running_handles);

    // Pause the transfer
    curl_easy_pause(easy_handle, CURLPAUSE_ALL);

    // Reset the easy handle
    curl_easy_reset(easy_handle);

    // Set up easy handle again for different operation
    // Configure for writing to output file
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file://output_file");

    // Re-add to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Perform one more multi operation
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);

    // Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(output_fd);
	assert_fd_closed(input_fd);
	return 0;
}
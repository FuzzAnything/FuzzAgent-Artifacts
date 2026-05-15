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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 0
//<Quality> {"density":24,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_msprintf","curl_multi_assign","curl_multi_perform","curl_multi_socket_action","curl_easy_pause","curl_easy_send","curl_easy_pause","curl_multi_wakeup","curl_pushheader_bynum","curl_easy_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_msprintf","curl_multi_assign","curl_multi_perform","curl_multi_socket_action","curl_easy_pause","curl_easy_send","curl_easy_pause","curl_multi_wakeup","curl_pushheader_bynum","curl_easy_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Custom callback for curl_mprintf
static int  id_000003_my_printf_callback(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvprintf(format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a multi handle and easy handle
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    
    if (!multi_handle || !easy_handle) {
        if (easy_handle) curl_easy_cleanup(easy_handle);
        if (multi_handle) curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Set up easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file://input_file");
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Use curl_msprintf to format a message
    char buffer[256];
    curl_msprintf(buffer, "Fuzzing with %zu bytes of data\n", size);
    
    // Use curl_mvprintf through our callback
     id_000003_my_printf_callback("Starting transfer: %s", buffer);
    
    // Assign socket to multi handle (simulating socket assignment)
    curl_multi_assign(multi_handle, (curl_socket_t)in_fd, NULL);
    
    int running_handles = 0;
    
    // Perform initial multi perform
    curl_multi_perform(multi_handle, &running_handles);
    
    // Use curl_multi_socket_action with our file descriptor
    curl_multi_socket_action(multi_handle, (curl_socket_t)in_fd, 
                           fuzz_int32_t_1, &running_handles);
    
    // Pause the easy handle
    curl_easy_pause(easy_handle, CURLPAUSE_RECV);
    
    // Send some data (simulating WebSocket or other protocol)
    size_t sent = 0;
    const char *test_data = "Test";
    curl_easy_send(easy_handle, fuzz_str_2, static_cast<size_t>(fuzz_str_sz_2), &sent);
    
    // Resume the easy handle
    curl_easy_pause(easy_handle, CURLPAUSE_CONT);
    
    // Wake up multi handle
    curl_multi_wakeup(multi_handle);
    
    // Simulate push headers (dummy structure)
    struct curl_pushheaders *push_headers = NULL;
    // Note: In real usage, this would be populated by CURLMOPT_PUSHFUNCTION
    // For fuzzing, we'll just call the function with NULL
    char *push_header = curl_pushheader_bynum(push_headers, fuzz_uint64_t_3);
    
    // Perform the transfer
    CURLcode res = curl_easy_perform(easy_handle);
    
    // Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
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
//<ID> 764
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"curl_multi_setopt":[[3256,27,3256,67,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_multi_add_handle","curl_multi_setopt","curl_ws_start_frame","curl_multi_socket_action","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_setopt","curl_ws_start_frame","curl_multi_socket_action","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Create multi handle - required for curl_multi_socket
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create easy handle for WebSocket operations
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set WebSocket options on the easy handle
    // First, configure it as a WebSocket connection
    curl_easy_setopt(easy_handle, CURLOPT_WS_OPTIONS, (long)CURLWS_RAW_MODE);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Set multi handle options using curl_multi_setopt
    // Example: disable pipelining to keep it simple
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, 0L);
    
    // Start a WebSocket frame using data from input
    if (size >= sizeof(unsigned int) + sizeof(curl_off_t)) {
        unsigned int ws_flags;
        curl_off_t frame_len;
        
        // Read flags and frame length from the beginning of input data
        memcpy(&ws_flags, data, sizeof(unsigned int));
        memcpy(&frame_len, data + sizeof(unsigned int), sizeof(curl_off_t));
        
        // Start WebSocket frame
        curl_ws_start_frame(easy_handle, ws_flags, frame_len);
    }
    
    int running_handles = 0;
    
    // Try both socket functions with different file descriptors
    // Use input file descriptor as a socket (simulated)
    curl_multi_socket(multi_handle, (curl_socket_t)in_fd, &running_handles);
    
    // Also try with output file descriptor
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // Cleanup order is important
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Close files
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Cleanup libcurl
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
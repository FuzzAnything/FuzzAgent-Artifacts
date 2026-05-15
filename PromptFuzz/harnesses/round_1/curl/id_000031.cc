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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_mprintf","curl_msnprintf","curl_mfprintf","curl_maprintf","curl_strequal","curl_ws_recv","curl_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_mprintf","curl_msnprintf","curl_mfprintf","curl_maprintf","curl_strequal","curl_ws_recv","curl_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Initialize curl global state
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
    
    // Initialize curl handles
    CURL *curl = curl_easy_init();
    CURLM *multi_handle = curl_multi_init();
    
    if (curl && multi_handle) {
        // Set up WebSocket options
        curl_easy_setopt(curl, CURLOPT_WS_OPTIONS, CURLWS_RAW_MODE);
        
        // Add curl handle to multi handle
        curl_multi_add_handle(multi_handle, curl);
        
        // Create waitfd structures for multi_poll
        struct curl_waitfd extra_fds[2];
        extra_fds[0].fd = in_fd;
        extra_fds[0].events = CURL_WAIT_POLLIN;
        extra_fds[1].fd = out_fd;
        extra_fds[1].events = CURL_WAIT_POLLOUT;
        
        int ret;
        // Use multi_poll to wait for activity
        curl_multi_poll(multi_handle, extra_fds, 2, fuzz_int32_t_1, &ret);
        
        // Read from input file into buffer
        char *input_str = (char *)malloc(size + 1);
        if (input_str) {
            fread(input_str, 1, size, in_file);
            input_str[size] = '\0';
            
            // Use curl_mprintf family of functions
            char buffer1[256];
            int n1 = curl_mprintf("Input size: %zu", size);
            
            char buffer2[256];
            int n2 = curl_msnprintf(buffer2, sizeof(buffer2), 
                                   "Data: %.*s", (int)(size > 100 ? 100 : size), input_str);
            
            // Use curl_mfprintf to write to output file
            int n3 = curl_mfprintf(out_file, "Formatted output: %s\n", buffer2);
            
            // Use curl_maprintf for dynamic allocation
            char *dynamic_str = curl_maprintf("Dynamic string with size: %zu", size);
            if (dynamic_str) {
                // Use curl_strequal for comparison
                int equal = curl_strequal(dynamic_str, buffer2);
                
                // WebSocket receive simulation
                char ws_buffer[1024];
                size_t bytes_received = 0;
                const struct curl_ws_frame *meta = NULL;
                
                // Simulate receiving WebSocket data from the input
                CURLcode ws_res = curl_ws_recv(curl, ws_buffer, 
                                              sizeof(ws_buffer) < size ? sizeof(ws_buffer) : size,
                                              &bytes_received, &meta);
                
                // Clean up dynamically allocated string
                curl_free(dynamic_str);
            }
            
            free(input_str);
        }
        
        // Perform multi handle cleanup
        curl_multi_remove_handle(multi_handle, curl);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(curl);
    } else {
        if (curl) curl_easy_cleanup(curl);
        if (multi_handle) curl_multi_cleanup(multi_handle);
    }
    
    // Cleanup files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Cleanup curl global state
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
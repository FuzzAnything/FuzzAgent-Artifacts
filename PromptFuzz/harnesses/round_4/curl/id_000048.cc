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
#include <mprintf.h>
}
//<ID> 890
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 3
//<Quality> {"density":14,"unique_branches":{"curl_multi_get_offt":[[3834,3,3834,31,0,0,4,0],[3836,8,3836,9,0,0,4,0],[3836,13,3836,25,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_cleanup","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_reset","curl_easy_setopt","curl_multi_get_offt","curl_multi_waitfds","curl_ws_recv","curl_easy_getinfo","curl_easy_getinfo","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_reset","curl_easy_setopt","curl_multi_get_offt","curl_multi_waitfds","curl_ws_recv","curl_easy_getinfo","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl globally (once per fuzzing session)
    static bool initialized = false;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = true;
    }
    
    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE* to write output data
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
    
    // Initialize easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Initialize multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up easy handle with some options using input data
    // Use the first few bytes to determine URL
    if (size > 0) {
        // Create a simple URL based on input
        char url[256];
        snprintf(url, sizeof(url), "http://example.com/test?data=%02x", data[0]);
        curl_easy_setopt(easy, CURLOPT_URL, url);
    }
    
    // Set up WebSocket options if we have enough data
    if (size >= 2) {
        // Set WebSocket options
        curl_easy_setopt(easy, CURLOPT_WS_OPTIONS, CURLWS_RAW_MODE);
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // Call curl_easy_reset (resets all options to default)
    curl_easy_reset(easy);
    
    // Re-set some basic options after reset
    curl_easy_setopt(easy, CURLOPT_URL, "ws://example.com/ws");
    
    // Try to get offset information from multi handle
    curl_off_t offset_value = 0;
    if (size >= 4) {
        // Use first 4 bytes to determine which info to get
        CURLMinfo_offt info_type = static_cast<CURLMinfo_offt>(data[0] % 2); // 0 or 1
        curl_multi_get_offt(multi, info_type, &offset_value);
    }
    
    // Set up waitfd structure using input data
    struct curl_waitfd ufds[2];
    unsigned int fd_count = 0;
    
    if (size >= 8) {
        // Use input bytes to populate waitfd structures
        ufds[0].fd = in_fd;
        ufds[0].events = static_cast<short>(data[1] & 0x03); // 0-3
        ufds[0].revents = 0;
        
        ufds[1].fd = out_fd;
        ufds[1].events = static_cast<short>(data[2] & 0x03); // 0-3
        ufds[1].revents = 0;
        
        // Call curl_multi_waitfds
        unsigned int size_param = 2;
        curl_multi_waitfds(multi, ufds, size_param, &fd_count);
    }
    
    // Try to receive WebSocket data using input as buffer
    if (size >= 16) {
        // Allocate buffer for WebSocket receive
        size_t buffer_size = size > 1024 ? 1024 : size;
        void *ws_buffer = malloc(buffer_size);
        if (ws_buffer) {
            size_t recv_len = 0;
            const struct curl_ws_frame *meta = nullptr;
            
            // Copy some input data to buffer
            size_t copy_size = buffer_size > size ? size : buffer_size;
            memcpy(ws_buffer, data, copy_size);
            
            // Try to receive WebSocket data
            CURLcode ws_result = curl_ws_recv(easy, ws_buffer, buffer_size, &recv_len, &meta);
            
            free(ws_buffer);
        }
    }
    
    // Get information from easy handle
    if (size >= 1) {
        long response_code = 0;
        // Use different info types based on input
        CURLINFO info_type = CURLINFO_RESPONSE_CODE;
        if (data[0] % 2 == 0) {
            info_type = CURLINFO_EFFECTIVE_URL;
            char *effective_url = nullptr;
            curl_easy_getinfo(easy, info_type, &effective_url);
        } else {
            curl_easy_getinfo(easy, info_type, &response_code);
        }
    }
    
    // Clean up
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
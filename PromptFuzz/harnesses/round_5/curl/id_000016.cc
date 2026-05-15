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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 2
//<Quality> {"density":16,"unique_branches":{"curl_easy_init":[[338,6,338,18,0,0,4,0],[340,8,340,14,0,0,4,1]]},"library_calls":["curl_global_trace","curl_easy_init","curl_easy_duphandle","curl_easy_cleanup","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_multi_add_handle","curl_multi_timeout","curl_getdate","curl_multi_socket_action","curl_ws_start_frame","curl_ws_send","curl_easy_nextheader","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_trace","curl_easy_init","curl_easy_duphandle","curl_multi_init","curl_multi_add_handle","curl_multi_timeout","curl_getdate","curl_multi_socket_action","curl_ws_start_frame","curl_ws_send","curl_easy_nextheader","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize global tracing first
    curl_global_trace("all");
    
    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get a file descriptor from the input file
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Initialize curl easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Duplicate the handle for WebSocket operations
    CURL *dup_curl = curl_easy_duphandle(curl);
    if (!dup_curl) {
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Create multi handle for timeout and socket operations
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(dup_curl);
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Add the duplicated handle to multi handle
    curl_multi_add_handle(multi_handle, dup_curl);
    
    // Get timeout value (not used for actual waiting in fuzzing)
    long timeout_ms = 0;
    curl_multi_timeout(multi_handle, &timeout_ms);
    
    // Parse date from part of the input data (first 100 bytes or less)
    size_t date_len = (size > 100) ? 100 : size;
    char *date_str = (char *)malloc(date_len + 1);
    if (date_str) {
        memcpy(date_str, data, date_len);
        date_str[date_len] = '\0';
        time_t parsed_time = curl_getdate(date_str, NULL);
        free(date_str);
    }
    
    // Use socket operation with our file descriptor
    int running_handles = 0;
    curl_multi_socket(multi_handle, fd, &running_handles);
    
    // Prepare WebSocket frame data
    size_t ws_data_size = (size > 1024) ? 1024 : size;
    uint8_t *ws_data = (uint8_t *)malloc(ws_data_size);
    if (ws_data) {
        memcpy(ws_data, data, ws_data_size);
        
        // Start WebSocket frame
        curl_ws_start_frame(dup_curl, CURLWS_TEXT, ws_data_size);
        
        // Send WebSocket frame
        size_t sent = 0;
        curl_ws_send(dup_curl, ws_data, ws_data_size, &sent, 0, CURLWS_TEXT);
        
        free(ws_data);
    }
    
    // Try to get headers from the easy handle
    struct curl_header *header = curl_easy_nextheader(curl, CURLH_HEADER, 0, NULL);
    
    // Clean up in reverse order
    curl_multi_remove_handle(multi_handle, dup_curl);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(dup_curl);
    curl_easy_cleanup(curl);
    assert_file_closed(&in_file);;
    
    // Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
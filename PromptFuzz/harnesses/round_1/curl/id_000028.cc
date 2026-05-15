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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 3.6666667, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_escape","curl_free","curl_easy_option_by_name","curl_easy_setopt","curl_multi_add_handle","curl_multi_assign","curl_multi_wakeup","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_escape","curl_free","curl_easy_option_by_name","curl_easy_setopt","curl_multi_add_handle","curl_multi_assign","curl_multi_wakeup","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize curl globally
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create a file from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor from input file
    int fd = fuzz_fileno(in_file);
    curl_socket_t sockfd = fd;

    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Use curl_escape on part of the input data
    // Ensure we don't exceed bounds
    size_t escape_len = size > 100 ? 100 : size;
    char *escaped = curl_escape((const char *)data, escape_len);
    if (escaped) {
        curl_free(escaped);
    }

    // Use curl_easy_option_by_name with a string from input data
    const struct curl_easyoption *opt = nullptr;
    if (size > 10) {
        // Create null-terminated string for option name lookup
        char opt_name[11];
        memcpy(opt_name, data, 10);
        opt_name[10] = '\0';
        opt = curl_easy_option_by_name(opt_name);
        // Option may be null if name doesn't exist - that's OK
    }

    // Prepare for WebSocket receive
    void *ws_buffer = nullptr;
    size_t ws_buflen = 0;
    size_t ws_recv = 0;
    const struct curl_ws_frame *meta = nullptr;
    
    // Set up WebSocket options on easy handle
    curl_easy_setopt(easy, CURLOPT_WS_OPTIONS, CURLWS_RAW_MODE);
    
    // Add easy handle to multi handle
    CURLMcode mres = curl_multi_add_handle(multi, easy);
    if (mres == CURLM_OK) {
        // Assign socket to multi handle
        mres = curl_multi_assign(multi, sockfd, nullptr);
        
        // Wake up multi handle
        mres = curl_multi_wakeup(multi);
        
        // Try to receive WebSocket data
        if (size > 50) {
            ws_buflen = size > 1024 ? 1024 : size;
            ws_buffer = malloc(ws_buflen);
            if (ws_buffer) {
                // Use curl_ws_recv (will likely fail without actual connection)
                CURLcode wres = curl_ws_recv(easy, ws_buffer, ws_buflen, &ws_recv, &meta);
                // Result is ignored as this is fuzzing
                free(ws_buffer);
            }
        }
        
        // Remove handle from multi
        curl_multi_remove_handle(multi, easy);
    }

    // Clean up
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    assert_file_closed(&in_file);;
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
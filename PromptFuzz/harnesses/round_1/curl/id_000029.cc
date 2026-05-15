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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"ipv6_parse":[[413,34,413,38,0,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_escape","curl_easy_setopt","curl_free","curl_easy_option_by_name","curl_easy_setopt","curl_ws_recv","curl_multi_add_handle","curl_multi_wakeup","curl_multi_assign","curl_multi_perform","curl_multi_info_read","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_escape","curl_easy_setopt","curl_free","curl_easy_option_by_name","curl_easy_setopt","curl_ws_recv","curl_multi_add_handle","curl_multi_wakeup","curl_multi_assign","curl_multi_perform","curl_multi_info_read","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary headers for curl
extern "C" {
}

// Custom function to read from input buffer for ws_recv callback
static size_t  id_000029_read_callback(char *buffer, size_t size, size_t nitems, void *instream) {
    // This would be used if we needed a callback for curl_mime_data_cb
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 1. Use curl_escape with input data
    // Create a null-terminated string from input data for curl_escape
    // Use at most size-1 bytes to ensure null termination if size > 0
    char *input_str = nullptr;
    if (size > 0) {
        size_t str_len = size < 1024 ? size : 1024; // Limit length
        input_str = (char *)malloc(str_len + 1);
        if (input_str) {
            memcpy(input_str, data, str_len);
            input_str[str_len] = '\0';
            
            // Use curl_escape
            char *escaped = curl_escape(input_str, (int)str_len);
            if (escaped) {
                // We could use escaped string for something
                // For example, set as URL (though might not be valid)
                curl_easy_setopt(easy_handle, CURLOPT_URL, escaped);
                curl_free(escaped);
            }
            free(input_str);
        }
    }
    
    // 2. Use curl_easy_option_by_name
    // Look up the CURLOPT_URL option
    const struct curl_easyoption *url_option = 
        curl_easy_option_by_name("CURLOPT_URL");
    if (url_option) {
        // Option found, we could use it
        // For example, set URL using the option ID
        curl_easy_setopt(easy_handle, url_option->id, "http://example.com");
    }
    
    // 3. Create a FILE* for input data using fmemopen
    FILE *in_file = nullptr;
    if (size > 0) {
        in_file = fmemopen((void *)data, size, "rb");
    }
    
    // 4. Set up for curl_ws_recv (though we don't have real WebSocket connection)
    // We'll prepare buffers and structures but actual recv will likely fail
    char ws_buffer[1024];
    size_t bytes_recv = 0;
    const struct curl_ws_frame *meta = nullptr;
    
    // Try to receive WebSocket data (will likely fail without proper setup)
    CURLcode ws_res = curl_ws_recv(easy_handle, ws_buffer, sizeof(ws_buffer), 
                                   &bytes_recv, &meta);
    // Result is ignored since we're just testing API calls
    
    // 5. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // 6. Use curl_multi_wakeup
    curl_multi_wakeup(multi_handle);
    
    // 7. Use curl_multi_assign with a socket descriptor
    // Create a socket descriptor from input file if available
    curl_socket_t sockfd = CURL_SOCKET_BAD;
    if (in_file) {
        sockfd = (curl_socket_t)fuzz_fileno(in_file);
        if (sockfd >= 0) {
            // Assign some arbitrary data (could be input data pointer)
            void *sockp = (void *)data;
            curl_multi_assign(multi_handle, sockfd, sockp);
        }
    }
    
    // Perform some multi operations (non-blocking)
    int still_running = 0;
    if (multi_handle && easy_handle) {
        curl_multi_perform(multi_handle, &still_running);
        
        // Check for completed transfers
        int msgs_in_queue = 0;
        CURLMsg *msg = nullptr;
        while ((msg = curl_multi_info_read(multi_handle, &msgs_in_queue))) {
            if (msg->msg == CURLMSG_DONE) {
                // Transfer completed
                CURL *easy = msg->easy_handle;
                CURLcode res = msg->data.result;
                // Process result if needed
            }
        }
    }
    
    // Clean up
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    // Remove and cleanup easy handle
    if (multi_handle && easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    curl_global_cleanup();
    
    return 0;
}
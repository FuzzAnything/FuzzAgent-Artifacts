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
//<ID> 792
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["curl_global_init","curl_version_info","curl_url","curl_url_set","curl_url_dup","curl_msnprintf","curl_msnprintf","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_fdset","curl_multi_socket_action","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version_info","curl_url","curl_url_set","curl_url_dup","curl_msnprintf","curl_msnprintf","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_fdset","curl_multi_socket_action","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to achieve an event that involves version info retrieval, URL duplication, formatted printing, multi-handle socket operations, and WebSocket data reception:

*/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 1. Get version info using curl_version_info
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    
    // 2. Create a URL handle and duplicate it
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Set a sample URL
        curl_url_set(url_handle, CURLUPART_URL, fuzz_str_1, fuzz_uint32_t_2);
        
        // Duplicate the URL handle
        CURLU *dup_url = curl_url_dup(url_handle);
        
        // Format version info using curl_msnprintf
        char buffer[512];
        int printed = curl_msnprintf(buffer, sizeof(buffer),
            "Curl v%s (host: %s), SSL: %s, Protocols: ",
            version_info->version,
            version_info->host,
            version_info->ssl_version);
        
        // Append protocols if available
        if (version_info->protocols) {
            for (int i = 0; version_info->protocols[i]; i++) {
                curl_msnprintf(buffer + printed, sizeof(buffer) - printed,
                    "%s ", version_info->protocols[i]);
                printed = strlen(buffer);
            }
        }
        
        // 3. Create WebSocket connection and multi handle
        CURL *curl = curl_easy_init();
        if (curl && dup_url) {
            // Set WebSocket URL
            curl_easy_setopt(curl, CURLOPT_URL, "ws://example.com/chat");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L); // WebSocket mode
            
            // Create multi handle
            CURLM *multi_handle = curl_multi_init();
            if (multi_handle) {
                // Add easy handle to multi handle
                curl_multi_add_handle(multi_handle, curl);
                
                int running_handles = 0;
                
                // 4. Setup fd_set for select
                fd_set read_fd_set, write_fd_set, exc_fd_set;
                FD_ZERO(&read_fd_set);
                FD_ZERO(&write_fd_set);
                FD_ZERO(&exc_fd_set);
                int max_fd = 0;
                
                // Get file descriptors
                curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, 
                               &exc_fd_set, &max_fd);
                
                // 5. Process socket activity
                curl_socket_t sockfd = 0; // Dummy socket for demonstration
                if (max_fd > 0) {
                    // In real usage, you'd get actual socket from easy handle
                    // For fuzzing, we'll use a dummy socket
                    curl_multi_socket(multi_handle, sockfd, &running_handles);
                }
                
                // 6. Try to receive WebSocket data from input
                if (size > 0) {
                    // Create buffer for WebSocket reception
                    void *ws_buffer = malloc(size);
                    if (ws_buffer) {
                        memcpy(ws_buffer, data, size);
                        
                        size_t recv_len = 0;
                        const struct curl_ws_frame *meta = nullptr;
                        
                        // Attempt to receive WebSocket data
                        CURLcode res = curl_ws_recv(curl, ws_buffer, size, 
                                                  &recv_len, &meta);
                        
                        // If we got metadata, check some fields
                        if (meta) {
                            // Access metadata fields (read-only)
                            volatile int age = meta->age;
                            volatile int flags = meta->flags;
                            (void)age;
                            (void)flags;
                        }
                        
                        free(ws_buffer);
                    }
                }
                
                // Cleanup multi handle
                curl_multi_remove_handle(multi_handle, curl);
                curl_multi_cleanup(multi_handle);
            }
            
            // Cleanup easy handle
            curl_easy_cleanup(curl);
        }
        
        // Cleanup URL handles
        if (dup_url) curl_url_cleanup(dup_url);
        curl_url_cleanup(url_handle);
    }
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}
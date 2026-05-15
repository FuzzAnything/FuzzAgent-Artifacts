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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["curl_global_init","curl_version_info","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_name","curl_mime_type","curl_mime_data","curl_easy_setopt","curl_multi_add_handle","curl_multi_notify_enable","curl_multi_perform","curl_multi_timeout","curl_multi_fdset","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version_info","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_type","curl_mime_data","curl_easy_setopt","curl_multi_add_handle","curl_multi_notify_enable","curl_multi_perform","curl_multi_timeout","curl_multi_fdset","curl_multi_socket_action","curl_multi_info_read","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Get version info
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Parse input data to extract fields
    // First 256 bytes for mime name, next 256 for mime type
    size_t name_len = (size > 256) ? 256 : size;
    size_t type_len = (size > 512) ? 256 : (size > 256 ? size - 256 : 0);
    
    std::string mime_name, mime_type;
    if (name_len > 0) {
        mime_name.assign(reinterpret_cast<const char*>(data), name_len);
        // Ensure null-terminated
        mime_name.push_back('\0');
    }
    
    if (type_len > 0) {
        mime_type.assign(reinterpret_cast<const char*>(data + 256), type_len);
        // Ensure null-terminated
        mime_type.push_back('\0');
    }
    
    // Set mime name and type using the parsed data
    if (!mime_name.empty()) {
        curl_mime_name(part, mime_name.c_str());
    }
    
    if (!mime_type.empty()) {
        curl_mime_type(part, mime_type.c_str());
    }
    
    // Set mime data - use the entire input data
    curl_mime_data(part, reinterpret_cast<const char*>(data), size);
    
    // Set the mime data to easy handle
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Enable notifications for the multi handle
    curl_multi_notify_enable(multi_handle, CURLMOPT_SOCKETFUNCTION);
    
    // Drive the multi handle
    int still_running = 0;
    int repeats = 0;
    
    do {
        CURLMcode mc;
        int numfds = 0;
        
        // Perform the multi handle operations
        mc = curl_multi_perform(multi_handle, &still_running);
        
        if (mc == CURLM_OK) {
            // Get timeout
            long timeout = 0;
            curl_multi_timeout(multi_handle, &timeout);
            
            // Wait for activity or timeout
            if (timeout >= 0) {
                struct timeval wait_time;
                wait_time.tv_sec = timeout / 1000;
                wait_time.tv_usec = (timeout % 1000) * 1000;
                
                fd_set fdread, fdwrite, fdexcep;
                int maxfd = -1;
                
                FD_ZERO(&fdread);
                FD_ZERO(&fdwrite);
                FD_ZERO(&fdexcep);
                
                // Get file descriptors
                curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);
                
                if (maxfd == -1) {
                    // No file descriptors, wait a bit
                    struct timeval wait = {0, 100000}; // 100ms
                    select(0, NULL, NULL, NULL, &wait);
                } else {
                    // Wait for activity on file descriptors
                    select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &wait_time);
                }
            }
            
            // Trigger socket actions
            for (int i = 0; i < 3; i++) { // Try a few times
                curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, fuzz_int32_t_1, &still_running);
                if (!still_running) break;
            }
            
            // Check for messages
            int msgs_in_queue = 0;
            CURLMsg *msg = NULL;
            while ((msg = curl_multi_info_read(multi_handle, &msgs_in_queue))) {
                if (msg->msg == CURLMSG_DONE) {
                    // Transfer completed
                    still_running = 0;
                }
            }
            
        } else {
            // Error in multi perform
            break;
        }
        
        repeats++;
        if (repeats > 100) break; // Safety limit
        
    } while (still_running);
    
    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    return 0;
}
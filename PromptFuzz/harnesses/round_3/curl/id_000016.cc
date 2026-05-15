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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["curl_global_init","curl_version","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_notify_disable","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data","curl_easy_setopt","curl_strnequal","curl_easy_setopt","curl_easy_send","curl_multi_perform","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_multi_notify_disable","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data","curl_easy_setopt","curl_strnequal","curl_easy_setopt","curl_easy_send","curl_multi_perform","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Ensure we have minimal input data
    if (size < 1) {
	return 0;
	}
	

    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Get curl version string
    char *version_str = curl_version();
    
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
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Disable notifications on multi handle
    // Using 0 as notification ID (CURLM_NOTIFY_DISABLED)
    curl_multi_notify_disable(multi_handle, fuzz_uint32_t_1);
    
    // Create a mime part for headers testing
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Create headers list from input data
            struct curl_slist *headers = nullptr;
            
            // Use input data to create header strings
            // We'll create up to 3 headers from the input
            size_t header_len = size / 3;
            if (header_len > 0) {
                // First header
                char *header1 = (char*)malloc(header_len + 1);
                if (header1) {
                    memcpy(header1, data, header_len);
                    header1[header_len] = '\0';
                    headers = curl_slist_append(headers, header1);
                    free(header1);
                }
                
                // Second header
                if (size >= header_len * 2) {
                    char *header2 = (char*)malloc(header_len + 1);
                    if (header2) {
                        memcpy(header2, data + header_len, header_len);
                        header2[header_len] = '\0';
                        headers = curl_slist_append(headers, header2);
                        free(header2);
                    }
                }
                
                // Third header (remaining data)
                if (size > header_len * 2) {
                    size_t remaining = size - header_len * 2;
                    char *header3 = (char*)malloc(remaining + 1);
                    if (header3) {
                        memcpy(header3, data + header_len * 2, remaining);
                        header3[remaining] = '\0';
                        headers = curl_slist_append(headers, header3);
                        free(header3);
                    }
                }
                
                // Set headers on mime part (take ownership)
                curl_mime_headers(part, headers, 1);
            }
            
            // Set some mime data from input
            if (size > 0) {
                // Use first 100 bytes or size whichever is smaller
                size_t data_len = size > 100 ? 100 : size;
                char *mime_data = (char*)malloc(data_len);
                if (mime_data) {
                    memcpy(mime_data, data, data_len);
                    curl_mime_data(part, mime_data, data_len);
                    free(mime_data);
                }
            }
        }
        
        // Set mime data to easy handle
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }
    
    // Compare version string with input data using curl_strnequal
    // Use first min(version_str_len, size) bytes for comparison
    size_t compare_len = strlen(version_str);
    if (compare_len > size) compare_len = size;
    
    if (compare_len > 0) {
        curl_strnequal(version_str, (const char*)data, compare_len);
    }
    
    // Try to send data using curl_easy_send
    // We need to set up the easy handle for sending first
    // Set a dummy URL
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost");
    
    // Create a buffer for sending
    size_t send_size = size > 1024 ? 1024 : size;
    char *send_buffer = (char*)malloc(send_size);
    if (send_buffer) {
        memcpy(send_buffer, data, send_size);
        
        // Try to send (this will likely fail since we're not connected,
        // but we're testing the API call)
        size_t sent = 0;
        curl_easy_send(easy_handle, send_buffer, send_size, &sent);
        
        free(send_buffer);
    }
    
    // Perform a single multi handle action
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);
    
    // Clean up
    if (mime) {
        curl_mime_free(mime);
    }
    
    // Remove and cleanup easy handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    
    // Cleanup multi handle
    curl_multi_cleanup(multi_handle);
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}
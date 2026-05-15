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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_version","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_mime_headers","curl_slist_append","curl_mime_headers","curl_multi_notify_disable","curl_easy_setopt","curl_multi_add_handle","curl_easy_send","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_version","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_multi_notify_disable","curl_easy_setopt","curl_multi_add_handle","curl_easy_send","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<35) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);

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

    // 1. Use curl_version() to get version string
    const char *version_str = curl_version();
    
    // 2. Use curl_strnequal() to compare version string with part of input
    // Use first min(size, strlen(version_str)) bytes for comparison
    size_t compare_len = (size < strlen(version_str)) ? size : strlen(version_str);
    int str_equal = curl_strnequal(version_str, reinterpret_cast<const char*>(data), compare_len);

    // 3. Create a mime part and set headers using curl_mime_headers()
    // Create a mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Create headers list from input data
            struct curl_slist *headers = NULL;
            
            // Use input data to create header strings
            // Split input into multiple headers by newline if possible
            std::vector<char> input_copy(data, data + size);
            input_copy.push_back('\0');
            
            char *token = strtok(input_copy.data(), "\n");
            while (token && strlen(token) > 0) {
                headers = curl_slist_append(headers, token);
                token = strtok(NULL, "\n");
            }
            
            // Set headers for mime part (take ownership)
            if (headers) {
                curl_mime_headers(part, headers, 1); // 1 = take ownership
            } else {
                // If no headers were created, use a default
                headers = curl_slist_append(NULL, fuzz_str_1);
                if (headers) {
                    curl_mime_headers(part, headers, fuzz_int32_t_2);
                }
            }
        }
    }

    // 4. Use curl_multi_notify_disable() to disable notifications
    // We'll disable all notifications (0 = disable all)
    curl_multi_notify_disable(multi_handle, fuzz_uint32_t_3);

    // 5. Use curl_easy_send() to send data
    // First, we need to set up the easy handle for sending
    // Create a dummy URL for testing
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:8080");
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Try to send data
    size_t sent = 0;
    if (size > 0) {
        // We'll use the input data as what we want to send
        curl_easy_send(easy_handle, data, size, &sent);
    }

    // Clean up
    if (mime) {
        curl_mime_free(mime);
    }
    
    // Remove easy handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Global cleanup
    curl_global_cleanup();

    return 0;
}
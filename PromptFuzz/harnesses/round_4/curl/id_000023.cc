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
//<ID> 228
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_get","curl_free","curl_getdate","curl_multi_init","curl_url_cleanup","curl_global_cleanup","curl_multi_notify_enable","curl_easy_init","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_get","curl_free","curl_getdate","curl_multi_init","curl_multi_notify_enable","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize curl globally
    CURLcode global_ret = curl_global_init(CURL_GLOBAL_ALL);
    if (global_ret != CURLE_OK) {
        return 0;
    }

    // 2. Create URL handle and parse URL from input data
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Use first part of input as URL string (null-terminated)
    size_t url_len = size < 100 ? size : 100;
    char *url_str = (char *)malloc(url_len + 1);
    if (!url_str) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_str, data, url_len);
    url_str[url_len] = '\0';

    // Set URL components
    curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_1);
    
    // Get back the URL to test curl_url_get
    char *retrieved_url = nullptr;
    curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, fuzz_uint32_t_2);
    if (retrieved_url) {
        curl_free(retrieved_url);
    }

    // 3. Parse date from input data (using curl_getdate)
    time_t parsed_time = curl_getdate(url_str, nullptr);

    // 4. Create multi handle and enable notifications
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        free(url_str);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Enable notifications (using a valid notification type)
    curl_multi_notify_enable(multi_handle, CURLMOPT_SOCKETFUNCTION);

    // 5. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        free(url_str);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // Set some easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 100L);

    // 6. Create mime structure and add parts
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use remaining input data for mime data
            size_t mime_data_size = size > url_len ? size - url_len : 0;
            const char *mime_data = (const char *)(data + url_len);
            
            if (mime_data_size > 0) {
                curl_mime_data(part, mime_data, mime_data_size);
            }
        }
        
        // Set mime to easy handle
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }

    // 7. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // 8. Perform multi transfer
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    // 9. Wait for activity (short timeout for fuzzing)
    struct curl_waitfd extra_fds[1];
    int ret = 0;
    curl_multi_wait(multi_handle, extra_fds, 0, fuzz_int32_t_3, &ret);

    // 10. Perform again to process any pending operations
    curl_multi_perform(multi_handle, &running_handles);

    // 11. Clean up in reverse order
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    free(url_str);
    curl_url_cleanup(url_handle);
    curl_global_cleanup();

    return 0;
}
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
//<ID> 458
//<Prompt> []
/*<Combination>: [
*/
//<score> 8, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["curl_global_init","curl_getenv","curl_free","curl_share_init","curl_share_setopt","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_easy_escape","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_free","curl_share_init","curl_share_setopt","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_easy_escape","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize curl globally
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // 2. Get environment variable
    char *env_var = curl_getenv("HOME");
    if (env_var) {
        curl_free(env_var);
    }

    // 3. Create a share object
    CURLSH *share = curl_share_init();
    if (share) {
        // Set share option
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    }

    // 4. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        if (share) curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // 5. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        if (share) curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // 6. Set easy handle options using input data
    // Create a null-terminated string from input data for URL
    char *url = nullptr;
    if (size > 0) {
        url = static_cast<char*>(malloc(size + 1));
        if (url) {
            memcpy(url, data, size);
            url[size] = '\0';
            
            // Set URL option
            curl_easy_setopt(easy_handle, CURLOPT_URL, url);
            free(url);
        }
    }

    // Set other options
    curl_easy_setopt(easy_handle, CURLOPT_SHARE, share);
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);

    // 7. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // 8. Perform socket operations
    int running_handles = 0;
    curl_multi_socket_all(multi_handle, &running_handles);

    // 9. Use curl_easy_escape with input data
    if (size > 0) {
        // Create string for escaping
        char *input_str = static_cast<char*>(malloc(size + 1));
        if (input_str) {
            memcpy(input_str, data, size);
            input_str[size] = '\0';
            
            // Escape the string
            char *escaped = curl_easy_escape(easy_handle, input_str, static_cast<int>(size));
            if (escaped) {
                curl_free(escaped);
            }
            free(input_str);
        }
    }

    // 10. Remove easy handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);

    // 11. Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    if (share) {
        curl_share_cleanup(share);
    }
    curl_global_cleanup();

    return 0;
}
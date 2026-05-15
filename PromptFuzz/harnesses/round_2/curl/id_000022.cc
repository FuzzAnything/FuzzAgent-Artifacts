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
//<ID> 315
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 1
//<Quality> {"density":14,"unique_branches":{"curl_easy_strerror":[[47,3,47,27,0,0,4,0]]},"library_calls":["curl_global_init_mem","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_unescape","curl_getenv","curl_msnprintf","curl_multi_perform","curl_multi_wakeup","curl_multi_info_read","curl_easy_strerror","curl_msnprintf","curl_free","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_unescape","curl_getenv","curl_msnprintf","curl_multi_perform","curl_multi_wakeup","curl_multi_info_read","curl_easy_strerror","curl_msnprintf","curl_free","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Headers needed for curl APIs

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize curl with custom memory functions
    CURLcode global_init_res = curl_global_init_mem(
        CURL_GLOBAL_ALL,
        (curl_malloc_callback)malloc,
        (curl_free_callback)free,
        (curl_realloc_callback)realloc,
        (curl_strdup_callback)strdup,
        (curl_calloc_callback)calloc
    );
    
    if (global_init_res != CURLE_OK) {
        return 0;
    }
    
    // 2. Create a multi handle for performing multiple transfers
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 3. Create an easy handle for a single transfer
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Add easy handle to multi handle
    CURLMcode multi_add_res = curl_multi_add_handle(multi_handle, easy_handle);
    if (multi_add_res != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 5. Use the input data as URL-encoded string to unescape
    char *unescaped_string = nullptr;
    if (size > 0) {
        // Convert input to C-string for processing
        char *input_string = (char *)malloc(size + 1);
        if (input_string) {
            memcpy(input_string, data, size);
            input_string[size] = '\0';
            
            // Unescape the URL-encoded string
            unescaped_string = curl_unescape(input_string, size);
            free(input_string);
        }
    }
    
    // 6. Get environment variable (using a common one)
    char *env_var = curl_getenv("PATH");
    
    // 7. Format a message using curl_msnprintf
    char buffer[1024];
    int printed = 0;
    if (unescaped_string) {
        printed = curl_msnprintf(buffer, sizeof(buffer), 
                                 "Unescaped: %s\nEnv PATH: %s", 
                                 unescaped_string, 
                                 env_var ? env_var : "(null)");
    }
    
    // 8. Perform multi handle operations
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);
    
    // 9. Wake up the multi handle (though it might not be sleeping)
    curl_multi_wakeup(multi_handle);
    
    // 10. Read info from multi handle
    int msgs_in_queue = 0;
    CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    
    // 11. If there was an error, get error string
    const char *error_str = nullptr;
    if (msg && msg->data.result != CURLE_OK) {
        error_str = curl_easy_strerror(msg->data.result);
    }
    
    // 12. If we got an error string, print it
    if (error_str) {
        char error_buffer[512];
        curl_msnprintf(error_buffer, sizeof(error_buffer), 
                       "Error: %s", error_str);
    }
    
    // 13. Clean up
    if (env_var) {
        // Note: curl_getenv returns a pointer that must be freed with curl_free
        curl_free(env_var);
    }
    
    if (unescaped_string) {
        curl_free(unescaped_string);
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
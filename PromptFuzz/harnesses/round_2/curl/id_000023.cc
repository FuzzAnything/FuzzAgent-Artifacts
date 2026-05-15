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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 3
//<Quality> {"density":16,"unique_branches":{"curl_multi_info_read":[[2985,6,2985,39,0,0,4,1]],"curl_easy_strerror":[[108,3,108,25,0,0,4,0],[138,3,138,36,0,0,4,0]]},"library_calls":["curl_global_init_mem","curl_global_cleanup","curl_getenv","curl_msnprintf","curl_multi_init","curl_free","curl_global_cleanup","curl_easy_init","curl_unescape","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wakeup","curl_multi_info_read","curl_easy_strerror","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_getenv","curl_msnprintf","curl_multi_init","curl_easy_init","curl_unescape","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wakeup","curl_multi_info_read","curl_easy_strerror","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_free","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // 1. Initialize curl with custom memory callbacks
    CURLcode init_result = curl_global_init_mem(CURL_GLOBAL_ALL,
        [](size_t sz) -> void* { return malloc(sz); },   // malloc callback
        [](void* p) { free(p); },                       // free callback
        [](void* p, size_t sz) -> void* { return realloc(p, sz); }, // realloc callback
        [](const char* s) -> char* { return strdup(s); }, // strdup callback
        [](size_t n, size_t sz) -> void* { return calloc(n, sz); } // calloc callback
    );
    
    if (init_result != CURLE_OK) {
	return 0;
	}
	
    
    // 2. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get environment variable
    char *env_var = curl_getenv("HOME");
    
    // 4. Format string using curl_msnprintf
    char formatted[1024];
    int fmt_len = curl_msnprintf(formatted, sizeof(formatted), 
                                 "Data size: %zu, Home: %s", size, 
                                 env_var ? env_var : "NULL");
    
    // 5. Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        if (env_var) curl_free(env_var);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Create an easy handle and set it up
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Use input data as the URL (first part as null-terminated string)
        char *url = nullptr;
        size_t url_len = (size < 256) ? size : 256;
        if (url_len > 0) {
            url = (char*)malloc(url_len + 1);
            if (url) {
                memcpy(url, data, url_len);
                url[url_len] = '\0';
                
                // Unescape the URL portion
                char *unescaped_url = curl_unescape(url, url_len);
                if (unescaped_url) {
                    curl_easy_setopt(easy_handle, CURLOPT_URL, unescaped_url);
                    curl_free(unescaped_url);
                } else {
                    curl_easy_setopt(easy_handle, CURLOPT_URL, url);
                }
                free(url);
            }
        }
        
        // Set input file for upload
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
        curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
        
        // Add to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // Perform the transfer
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
        
        // 7. Wake up multi handle
        curl_multi_wakeup(multi_handle);
        
        // 8. Read info from multi handle
        int msgs_in_queue = 0;
        CURLMsg *msg = nullptr;
        do {
            msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
            if (msg) {
                if (msg->msg == CURLMSG_DONE) {
                    // 9. Get error string
                    const char *error_str = curl_easy_strerror(msg->data.result);
                    // Use the error string somehow
                    (void)error_str;
                }
            }
        } while (msg);
        
        // Clean up easy handle
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    // 10. Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // 11. Clean up environment variable
    if (env_var) curl_free(env_var);
    
    // 12. Close input file
    assert_file_closed(&in_file);;
    
    // 13. Create an output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write formatted string to output
        fwrite(formatted, 1, fmt_len > 0 ? fmt_len : 0, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 14. Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
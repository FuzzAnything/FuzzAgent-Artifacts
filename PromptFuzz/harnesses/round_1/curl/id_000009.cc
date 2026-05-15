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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 6.6666665, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["curl_global_init","curl_getenv","curl_url","curl_url_set","curl_formadd","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_easy_init","curl_url_get","curl_easy_setopt","curl_free","curl_multi_init","curl_multi_add_handle","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_perform","curl_easy_strerror","curl_easy_cleanup","curl_mime_free","curl_formfree","curl_url_cleanup","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_url","curl_url_set","curl_formadd","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_easy_init","curl_url_get","curl_easy_setopt","curl_free","curl_multi_init","curl_multi_add_handle","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_perform","curl_easy_strerror","curl_easy_cleanup","curl_mime_free","curl_formfree","curl_url_cleanup","curl_free","curl_global_cleanup"],"visited":2}
/**/



extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<28) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    // Initialize libcurl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // 1. Get environment variable
    char *env_var = curl_getenv(fuzz_str_1);
    
    // 2. Create URL handle
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Set URL path using environment variable or fallback
        const char *path = env_var ? env_var : "/";
        curl_url_set(url_handle, CURLUPART_PATH, path, fuzz_uint32_t_2);
    }
    
    // 3. Create form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    if (size > 0) {
        // Create a string from input data for form field
        char *field_data = (char *)malloc(size + 1);
        if (field_data) {
            memcpy(field_data, data, size);
            field_data[size] = '\0';
            
            // Add form field
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, "fuzz_field",
                        CURLFORM_COPYCONTENTS, field_data,
                        CURLFORM_CONTENTSLENGTH, (long)size,
                        CURLFORM_END);
            
            free(field_data);
        }
    }
    
    // 4. Create MIME structure and add part
    curl_mime *mime = curl_mime_init(NULL);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use input data for MIME part
            curl_mime_data(part, (const char *)data, size);
            curl_mime_name(part, fuzz_str_3);
        }
    }
    
    // 5. Create CURL easy handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Set URL from URL handle
        if (url_handle) {
            char *url = NULL;
            curl_url_get(url_handle, CURLUPART_URL, &url, fuzz_uint32_t_4);
            if (url) {
                curl_easy_setopt(easy_handle, CURLOPT_URL, url);
                curl_free(url);
            }
        }
        
        // 6. Create CURL multi handle
        CURLM *multi_handle = curl_multi_init();
        if (multi_handle) {
            // Add easy handle to multi handle
            curl_multi_add_handle(multi_handle, easy_handle);
            
            int running_handles = 0;
            
            // 7. Perform socket operations
            curl_multi_socket_all(multi_handle, &running_handles);
            
            // 8. Try to send data
            if (size > 0) {
                size_t sent = 0;
                curl_easy_send(easy_handle, data, 
                              size > 1024 ? 1024 : size, &sent);
            }
            
            // 9. Try to receive data
            char recv_buf[1024];
            size_t received = 0;
            curl_easy_recv(easy_handle, recv_buf, sizeof(recv_buf), &received);
            
            // 10. Clean up multi handle
            curl_multi_remove_handle(multi_handle, easy_handle);
            curl_multi_cleanup(multi_handle);
        }
        
        // 11. Check for errors and get error string
        CURLcode res = curl_easy_perform(easy_handle);
        if (res != CURLE_OK) {
            const char *error_str = curl_easy_strerror(res);
            // Error string is available but we don't need to use it in fuzzing
        }
        
        // 12. Clean up easy handle
        curl_easy_cleanup(easy_handle);
    }
    
    // 13. Clean up MIME
    if (mime) {
        curl_mime_free(mime);
    }
    
    // 14. Clean up form
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // 15. Clean up URL handle
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    // 16. Free environment variable string
    if (env_var) {
        curl_free(env_var);
    }
    
    // 17. Global cleanup
    curl_global_cleanup();
    
    return 0;
}
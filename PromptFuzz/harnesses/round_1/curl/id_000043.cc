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
//<ID> 498
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["curl_global_init","curl_getenv","curl_free","curl_url","curl_url_set","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_perform","curl_easy_strerror","curl_mime_free","curl_easy_cleanup","curl_formfree","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_free","curl_url","curl_url_set","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_perform","curl_easy_strerror","curl_mime_free","curl_easy_cleanup","curl_formfree","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include curl headers

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<29) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Split the input data for different purposes
    size_t url_part_size = size / 3;
    size_t form_data_size = size / 3;
    size_t mime_data_size = size - 2 * (size / 3);
    
    const uint8_t *url_data = data;
    const uint8_t *form_data = data + url_part_size;
    const uint8_t *mime_data = data + url_part_size + form_data_size;
    
    // 1. Initialize curl library
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Get environment variable
    char *home_env = curl_getenv(fuzz_str_1);
    if (home_env) {
        curl_free(home_env);
    }
    
    // 3. Create and set URL
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Create null-terminated string for URL path
        char *url_path = (char *)malloc(url_part_size + 1);
        if (url_path) {
            memcpy(url_path, url_data, url_part_size);
            url_path[url_part_size] = '\0';
            curl_url_set(url_handle, CURLUPART_PATH, url_path, fuzz_uint32_t_2);
            free(url_path);
        }
    }
    
    // 4. Create form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *last_post = NULL;
    
    // Create null-terminated string for form data
    char *form_str = (char *)malloc(form_data_size + 1);
    if (form_str && form_data_size > 0) {
        memcpy(form_str, form_data, form_data_size);
        form_str[form_data_size] = '\0';
        
        curl_formadd(&formpost, &last_post,
                     CURLFORM_COPYNAME, "fuzzfield",
                     CURLFORM_COPYCONTENTS, form_str,
                     CURLFORM_END);
    }
    if (form_str) free(form_str);
    
    // 5. Create and initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // 6. Create mime structure
        curl_mime *mime = curl_mime_init(easy_handle);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Create null-terminated string for mime data
                char *mime_str = (char *)malloc(mime_data_size + 1);
                if (mime_str && mime_data_size > 0) {
                    memcpy(mime_str, mime_data, mime_data_size);
                    mime_str[mime_data_size] = '\0';
                    
                    curl_mime_data(part, mime_str, mime_data_size);
                    curl_mime_name(part, fuzz_str_3);
                    
                    free(mime_str);
                }
            }
            
            // Set mime as POSTFIELDS
            curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
        }
        
        // Set URL (using a dummy URL since we can't get full URL from CURLU handle)
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost/");
        
        // 7. Create multi handle and add easy handle
        CURLM *multi_handle = curl_multi_init();
        if (multi_handle) {
            curl_multi_add_handle(multi_handle, easy_handle);
            
            int running_handles = 0;
            curl_multi_socket_all(multi_handle, &running_handles);
            
            // 8. Try to send and receive (though connection won't succeed)
            size_t sent = 0;
            char send_buffer[] = "test";
            curl_easy_send(easy_handle, fuzz_str_4, static_cast<size_t>(fuzz_str_sz_4), &sent);
            
            char recv_buffer[1024];
            size_t received = 0;
            curl_easy_recv(easy_handle, recv_buffer, sizeof(recv_buffer), &received);
            
            // 9. Clean up multi handle
            curl_multi_remove_handle(multi_handle, easy_handle);
            curl_multi_cleanup(multi_handle);
        }
        
        // 10. Try to perform request (will fail but exercise the API)
        CURLcode res = curl_easy_perform(easy_handle);
        const char *error_str = curl_easy_strerror(res);
        
        // 11. Clean up easy handle and mime
        if (mime) {
            curl_mime_free(mime);
        }
        curl_easy_cleanup(easy_handle);
    }
    
    // 12. Clean up form data
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // 13. Clean up URL handle
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    // 14. Global cleanup
    curl_global_cleanup();
    
    return 0;
}
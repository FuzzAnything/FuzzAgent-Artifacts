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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_easy_init","curl_url_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_mime_addpart","curl_formadd","curl_getenv","curl_free","curl_url_set","curl_multi_init","curl_mime_free","curl_formfree","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_easy_send","curl_easy_recv","curl_easy_strerror","curl_easy_strerror","curl_multi_socket_all","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_formfree","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_formadd","curl_getenv","curl_free","curl_url_set","curl_multi_init","curl_multi_add_handle","curl_easy_send","curl_easy_recv","curl_easy_strerror","curl_easy_strerror","curl_multi_socket_all","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_formfree","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/



extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<30) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeCharArray(char, fuzz_char_array_3, fuzz_char_array_size_3, fdp);
	
		const void * fuzzer_var_curl_easy_send_22_1 = fuzz_char_array_3;
		size_t fuzzer_size_curl_easy_send_22_1 = static_cast<size_t>(fuzz_char_array_size_3);//fuzzer shim end}




    // 1. Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Create a URL handle using curl_url()
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 3. Create a mime structure and add a mime part
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_easy_cleanup(easy);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    curl_mimepart *mimepart = curl_mime_addpart(mime);
    
    // 4. Create a form using curl_formadd()
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Use first 64 bytes as form name (if available)
    size_t form_name_len = size > 64 ? 64 : size;
    char *form_name = (char*)malloc(form_name_len + 1);
    if (form_name) {
        memcpy(form_name, data, form_name_len);
        form_name[form_name_len] = 0;
        
        curl_formadd(&formpost, &lastptr,
                     CURLFORM_COPYNAME, "fuzz_field",
                     CURLFORM_COPYCONTENTS, form_name,
                     CURLFORM_END);
        
        free(form_name);
    }
    
    // 5. Use curl_getenv() to check an environment variable
    char *env_var = curl_getenv(fuzz_str_1);
    if (env_var) {
        // Could use env_var for something, but just free it
        curl_free(env_var);
    }
    
    // 6. Try to parse input as URL and set it in url_handle
    if (size > 0) {
        // Treat data as a string for URL (null-terminate a copy)
        size_t url_len = size < 256 ? size : 256;
        char *url_str = (char*)malloc(url_len + 1);
        if (url_str) {
            memcpy(url_str, data, url_len);
            url_str[url_len] = 0;
            
            // Try to set the URL components
            curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_2);
            
            free(url_str);
        }
    }
    
    // 7. Create multi handle for socket operations
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_mime_free(mime);
        curl_formfree(formpost);
        curl_easy_cleanup(easy);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 8. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy);
    
    // 9. Attempt socket operations (will likely fail but API is called)
    int running_handles = 0;
    
    // Try curl_easy_send() with a small buffer
    size_t sent = 0;
    char dummy_buffer[16] = {0};
    CURLcode send_result = curl_easy_send(easy, fuzzer_var_curl_easy_send_22_1, fuzzer_size_curl_easy_send_22_1, &sent);
    
    // Try curl_easy_recv() with a small buffer
    size_t received = 0;
    char recv_buffer[16] = {0};
    CURLcode recv_result = curl_easy_recv(easy, recv_buffer, sizeof(recv_buffer), &received);
    
    // 10. Use curl_easy_strerror() to get error strings
    const char *send_error_str = curl_easy_strerror(send_result);
    const char *recv_error_str = curl_easy_strerror(recv_result);
    
    // 11. Try multi socket operations
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // 12. Perform multi operations briefly
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);
    
    // 13. Clean up everything in reverse order
    
    // Remove easy handle from multi
    curl_multi_remove_handle(multi_handle, easy);
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Clean up mime
    curl_mime_free(mime);
    
    // Clean up form
    curl_formfree(formpost);
    
    // Clean up easy handle
    curl_easy_cleanup(easy);
    
    // Clean up URL handle
    curl_url_cleanup(url_handle);
    
    // Final global cleanup
    curl_global_cleanup();
    
    return 0;
}
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
//<ID> 496
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["curl_global_init","curl_getenv","curl_free","curl_url","curl_global_cleanup","curl_url_set","curl_formadd","curl_easy_init","curl_formfree","curl_url_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_multi_init","curl_mime_free","curl_formfree","curl_url_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_easy_perform","curl_easy_strerror","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_mime_free","curl_formfree","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_free","curl_url","curl_url_set","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_easy_send","curl_easy_recv","curl_easy_perform","curl_easy_strerror","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_mime_free","curl_formfree","curl_url_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Include curl headers

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<22) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // 1. Initialize curl globally
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	

    // 2. Get environment variable
    char *env_var = curl_getenv(fuzz_str_1);
    if (env_var) curl_free(env_var);

    // 3. Create URL handle and set URL parts using input data
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Use first 100 bytes for URL path (or all if smaller)
    size_t url_len = size < 100 ? size : 100;
    char *url_part = new char[url_len + 1];
    memcpy(url_part, data, url_len);
    url_part[url_len] = '\0';
    
    curl_url_set(url_handle, CURLUPART_PATH, url_part, fuzz_uint32_t_2);
    delete[] url_part;

    // 4. Create HTTP form using input data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Use next 100 bytes for form data (or what remains)
    size_t form_data_len = (size - url_len) > 100 ? 100 : size - url_len;
    if (form_data_len > 0) {
        uint8_t *form_data_start = (uint8_t *)data + url_len;
        char *form_field = new char[form_data_len + 1];
        memcpy(form_field, form_data_start, form_data_len);
        form_field[form_data_len] = '\0';
        
        curl_formadd(&formpost, &lastptr,
                    CURLFORM_COPYNAME, "fuzzfield",
                    CURLFORM_COPYCONTENTS, form_field,
                    CURLFORM_END);
        delete[] form_field;
    }

    // 5. Create mime structure and add parts using input data
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_formfree(formpost);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use remaining bytes for mime data
            size_t mime_data_offset = url_len + form_data_len;
            size_t mime_data_len = size - mime_data_offset;
            
            if (mime_data_len > 0) {
                char *mime_data = new char[mime_data_len];
                memcpy(mime_data, data + mime_data_offset, mime_data_len);
                
                curl_mime_name(part, fuzz_str_3);
                curl_mime_data(part, mime_data, mime_data_len);
                
                delete[] mime_data;
            }
        }
    }

    // 6. Set up multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_mime_free(mime);
        curl_formfree(formpost);
        curl_url_cleanup(url_handle);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // 7. Configure easy handle with form/mime data
    if (formpost) {
        curl_easy_setopt(easy, CURLOPT_HTTPPOST, formpost);
    }
    
    // Also set a dummy URL for the easy handle
    curl_easy_setopt(easy, CURLOPT_URL, "http://localhost/test");

    // 8. Add easy handle to multi handle
    CURLMcode multi_add_res = curl_multi_add_handle(multi_handle, easy);
    
    // 9. Try socket operation (though no actual sockets exist)
    int running_handles = 0;
    curl_multi_socket_all(multi_handle, &running_handles);

    // 10. Try to send and receive data using the easy handle
    // Create a buffer for send/recv operations
    char buffer[1024];
    size_t n = 0;
    
    // Use part of input data for send
    size_t send_len = size < 512 ? size : 512;
    curl_easy_send(easy, data, send_len, &n);
    
    // Try to receive
    curl_easy_recv(easy, buffer, sizeof(buffer), &n);

    // 11. Try to perform the easy handle operation
    CURLcode perform_res = curl_easy_perform(easy);
    
    // Get error string for the result
    if (perform_res != CURLE_OK) {
        const char *error_str = curl_easy_strerror(perform_res);
        // error_str can be used for logging but we don't output in fuzzing
    }

    // 12. Remove handle from multi handle
    curl_multi_remove_handle(multi_handle, easy);

    // 13. Cleanup everything
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy);
    curl_mime_free(mime);
    curl_formfree(formpost);
    curl_url_cleanup(url_handle);
    curl_global_cleanup();

    return 0;
}
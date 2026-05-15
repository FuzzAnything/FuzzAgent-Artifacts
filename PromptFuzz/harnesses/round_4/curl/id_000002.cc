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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["curl_multi_init","curl_url","curl_multi_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_url_set","curl_url_dup","curl_url_cleanup","curl_multi_cleanup","curl_easy_init","curl_url_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_strequal","curl_mime_data","curl_easy_setopt","curl_formadd","curl_formfree","curl_multi_add_handle","curl_multi_timeout","curl_multi_perform","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_multi_cleanup"],"critical_calls":["curl_multi_init","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_strequal","curl_mime_data","curl_easy_setopt","curl_formadd","curl_formfree","curl_multi_add_handle","curl_multi_timeout","curl_multi_perform","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_multi_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<17) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // 2. Create a URL handle and duplicate it
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Use input data to create a URL string (ensuring null termination)
    size_t url_len = size > 255 ? 255 : size;
    char *url_str = (char *)malloc(url_len + 1);
    if (!url_str) {
        curl_url_cleanup(url_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    memcpy(url_str, data, url_len);
    url_str[url_len] = '\0';

    // Set URL in handle
    curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_1);
    
    // Duplicate the URL handle
    CURLU *dup_url = curl_url_dup(url_handle);
    if (!dup_url) {
        free(url_str);
        curl_url_cleanup(url_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // 3. Create and use MIME for form data
    // Create an easy handle for MIME
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_url_cleanup(dup_url);
        free(url_str);
        curl_url_cleanup(url_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Create MIME structure
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use part of input data as mime type string
            size_t mime_type_len = size > 100 ? 100 : size;
            char *mime_type = (char *)malloc(mime_type_len + 1);
            if (mime_type) {
                memcpy(mime_type, data, mime_type_len);
                mime_type[mime_type_len] = '\0';
                
                // Use curl_mime_type API
                curl_mime_type(part, mime_type);
                
                // Use curl_strequal to compare mime type
                const char *compare_type = "text/plain";
                int equal = curl_strequal(mime_type, fuzz_str_2);
                (void)equal; // Prevent unused variable warning
                
                free(mime_type);
            }
            
            // Add some data to the mime part
            if (size > 0) {
                curl_mime_data(part, (const char *)data, size > 50 ? 50 : size);
            }
        }
        
        // Set MIME data to easy handle
        curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    }

    // 4. Create and free old-style form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Create a simple form using a substring of input data
    size_t form_data_len = size > 30 ? 30 : size;
    char *form_data = (char *)malloc(form_data_len + 1);
    if (form_data) {
        memcpy(form_data, data, form_data_len);
        form_data[form_data_len] = '\0';
        
        // Note: curl_formadd is deprecated but we need to create a form to free it
        // We'll create a minimal form
        curl_formadd(&formpost, &lastptr,
                     CURLFORM_COPYNAME, "field1",
                     CURLFORM_COPYCONTENTS, form_data,
                     CURLFORM_END);
        
        free(form_data);
    }
    
    // Use curl_formfree API
    if (formpost) {
        curl_formfree(formpost);
    }

    // 5. Add easy handle to multi handle and get timeout
    curl_multi_add_handle(multi_handle, easy);
    
    long timeout_ms = 0;
    // Use curl_multi_timeout API
    CURLMcode timeout_res = curl_multi_timeout(multi_handle, &timeout_ms);
    (void)timeout_res; // Prevent unused variable warning

    // 6. Perform a single multi perform to process
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);

    // 7. Cleanup in reverse order
    if (mime) {
        curl_mime_free(mime);
    }
    
    curl_multi_remove_handle(multi_handle, easy);
    curl_easy_cleanup(easy);
    
    curl_url_cleanup(dup_url);
    free(url_str);
    curl_url_cleanup(url_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}
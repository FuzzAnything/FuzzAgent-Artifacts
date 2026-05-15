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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"set_url_scheme":[[1642,6,1642,29,0,0,4,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_multi_timeout","curl_multi_cleanup","curl_global_cleanup","curl_url","curl_multi_cleanup","curl_global_cleanup","curl_url_set","curl_url_dup","curl_url_cleanup","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_name","curl_mime_free","curl_strequal","curl_ws_send","curl_easy_reset","curl_easy_cleanup","curl_formfree","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_multi_timeout","curl_url","curl_url_set","curl_url_dup","curl_url_cleanup","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_name","curl_mime_free","curl_strequal","curl_ws_send","curl_easy_reset","curl_easy_cleanup","curl_formfree","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to process input data:

*/


extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




    // Initialize curl globally
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Check multi timeout
    long timeout_ms;
    if (curl_multi_timeout(multi_handle, &timeout_ms) != CURLM_OK) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Use a portion of input data for URL parts
    size_t url_part_size = size > 100 ? 100 : size;
    char *url_part = (char *)malloc(url_part_size + 1);
    if (url_part) {
        memcpy(url_part, data, url_part_size);
        url_part[url_part_size] = '\0';
        
        // Set URL part (scheme)
        curl_url_set(url_handle, CURLUPART_SCHEME, url_part, fuzz_uint32_t_1);
        
        // Duplicate URL handle
        CURLU *url_dup = curl_url_dup(url_handle);
        if (url_dup) {
            curl_url_cleanup(url_dup);
        }
        
        free(url_part);
    }

    // Create form data
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    if (size > 50) {
        char *field_name = (char *)malloc(26);
        char *field_value = (char *)malloc(26);
        if (field_name && field_value) {
            memcpy(field_name, data, 25);
            field_name[25] = '\0';
            memcpy(field_value, data + 25, 25);
            field_value[25] = '\0';
            
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, field_name,
                        CURLFORM_COPYCONTENTS, field_value,
                        CURLFORM_END);
            
            free(field_name);
            free(field_value);
        }
    }

    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Create mime structure
        curl_mime *mime = curl_mime_init(easy_handle);
        if (mime) {
            // Add mime part
            curl_mimepart *mimepart = curl_mime_addpart(mime);
            if (mimepart) {
                // Use a portion of input for mime type
                size_t mime_type_size = size > 30 ? 30 : size;
                char *mime_type = (char *)malloc(mime_type_size + 1);
                if (mime_type) {
                    memcpy(mime_type, data, mime_type_size);
                    mime_type[mime_type_size] = '\0';
                    
                    curl_mime_type(mimepart, mime_type);
                    free(mime_type);
                }
                
                // Use another portion for mime name
                if (size > 60) {
                    size_t mime_name_size = size > 90 ? 30 : size - 60;
                    char *mime_name = (char *)malloc(mime_name_size + 1);
                    if (mime_name) {
                        memcpy(mime_name, data + 60, mime_name_size);
                        mime_name[mime_name_size] = '\0';
                        
                        curl_mime_name(mimepart, mime_name);
                        free(mime_name);
                    }
                }
            }
            
            // Free mime structure
            curl_mime_free(mime);
        }
        
        // String comparison using input data
        if (size > 120) {
            char *str1 = (char *)malloc(61);
            char *str2 = (char *)malloc(61);
            if (str1 && str2) {
                memcpy(str1, data + 60, 60);
                str1[60] = '\0';
                memcpy(str2, data + 120, size - 120 > 60 ? 60 : size - 120);
                str2[size - 120 > 60 ? 60 : size - 120] = '\0';
                
                curl_strequal(str1, str2);
                
                free(str1);
                free(str2);
            }
        }
        
        // WebSocket send using remaining input data
        if (size > 180) {
            size_t ws_data_size = size - 180;
            const void *ws_buffer = data + 180;
            size_t sent = 0;
            
            // Note: This will likely fail since we don't have a real WebSocket connection,
            // but we call the API as requested
            curl_ws_send(easy_handle, ws_buffer, ws_data_size, &sent, fuzz_int64_t_2, fuzz_uint32_t_3);
        }
        
        // Reset easy handle
        curl_easy_reset(easy_handle);
        
        // Clean up easy handle
        curl_easy_cleanup(easy_handle);
    }

    // Clean up form
    if (formpost) {
        curl_formfree(formpost);
    }

    // Clean up URL handle
    curl_url_cleanup(url_handle);

    // Clean up multi handle
    curl_multi_cleanup(multi_handle);

    // Global cleanup
    curl_global_cleanup();

    return 0;
}
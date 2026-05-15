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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 2, nr_unique_branch: 0
//<Quality> {"density":6,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_multi_timeout","curl_url","curl_multi_cleanup","curl_global_cleanup","curl_url_set","curl_url_dup","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_strequal","curl_mime_free","curl_easy_cleanup","curl_formfree","curl_url_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_multi_timeout","curl_url","curl_url_set","curl_url_dup","curl_formadd","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_strequal","curl_mime_free","curl_easy_cleanup","curl_formfree","curl_url_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 2) {
	return 0;
	}
	
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create a multi handle for timeout operations
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Check multi handle timeout (part 1)
    long timeout_ms = 0;
    curl_multi_timeout(multi_handle, &timeout_ms);
    
    // Create initial URL handle from input data
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Use first portion of data as URL string
    size_t url_len = size / 4;
    if (url_len > 0) {
        char *url_str = (char*)malloc(url_len + 1);
        if (url_str) {
            memcpy(url_str, data, url_len);
            url_str[url_len] = '\0';
            curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_1);
            free(url_str);
        }
    }
    
    // Duplicate the URL handle (part 2)
    CURLU *dup_url = curl_url_dup(url_handle);
    
    // Create a form from input data
    struct curl_httppost *form = NULL;
    struct curl_httppost *last_post = NULL;
    
    // Use second portion of data for form field name
    size_t name_len = (size - size/4) / 3;
    if (name_len > 0) {
        char *field_name = (char*)malloc(name_len + 1);
        if (field_name) {
            memcpy(field_name, data + size/4, name_len);
            field_name[name_len] = '\0';
            
            // Use third portion for form field value
            size_t value_len = (size - size/4 - name_len) / 2;
            if (value_len > 0) {
                char *field_value = (char*)malloc(value_len + 1);
                if (field_value) {
                    memcpy(field_value, data + size/4 + name_len, value_len);
                    field_value[value_len] = '\0';
                    
                    // Add form field
                    curl_formadd(&form, &last_post,
                                CURLFORM_COPYNAME, field_name,
                                CURLFORM_COPYCONTENTS, field_value,
                                CURLFORM_END);
                    
                    free(field_value);
                }
            }
            free(field_name);
        }
    }
    
    // Create a MIME part and set its type using the last portion of data
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_mime *mime = curl_mime_init(easy_handle);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Use remaining data for MIME type
                size_t mime_offset = size/4 + name_len + ((size - size/4 - name_len)/2);
                size_t mime_len = size - mime_offset;
                if (mime_len > 0) {
                    char *mime_type = (char*)malloc(mime_len + 1);
                    if (mime_type) {
                        memcpy(mime_type, data + mime_offset, mime_len);
                        mime_type[mime_len] = '\0';
                        
                        // Set MIME type (part 3)
                        curl_mime_type(part, mime_type);
                        
                        // Compare MIME type strings (part 4)
                        if (size >= 2) {
                            char *test_str1 = (char*)malloc(3);
                            char *test_str2 = (char*)malloc(3);
                            if (test_str1 && test_str2) {
                                memcpy(test_str1, data, 2);
                                test_str1[2] = '\0';
                                memcpy(test_str2, data + (size > 2 ? 1 : 0), 2);
                                test_str2[2] = '\0';
                                curl_strequal(test_str1, test_str2);
                                free(test_str1);
                                free(test_str2);
                            }
                        }
                        
                        free(mime_type);
                    }
                }
            }
            curl_mime_free(mime);
        }
        curl_easy_cleanup(easy_handle);
    }
    
    // Free the form (part 5)
    if (form) {
        curl_formfree(form);
    }
    
    // Clean up URL handles
    if (dup_url) {
        curl_url_cleanup(dup_url);
    }
    curl_url_cleanup(url_handle);
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Final global cleanup
    curl_global_cleanup();
    
    return 0;
}
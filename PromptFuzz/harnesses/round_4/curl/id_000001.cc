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
//<ID> 2
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 6
//<Quality> {"density":6,"unique_branches":{"curl_url_dup":[[1304,8,1304,19,3,0,4,0],[1306,10,1306,23,3,0,4,1],[1304,8,1304,19,4,0,4,0],[1306,10,1306,23,4,0,4,1],[1304,8,1304,19,7,0,4,0],[1306,10,1306,23,7,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_multi_timeout","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_free","curl_easy_cleanup","curl_formadd","curl_formfree","curl_strequal","curl_url_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_multi_timeout","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_free","curl_easy_cleanup","curl_formadd","curl_formfree","curl_strequal","curl_url_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Get multi handle timeout
    long timeout_ms;
    CURLMcode mres = curl_multi_timeout(multi_handle, &timeout_ms);
    (void)mres; // Result intentionally ignored for fuzzing
    
    // Create URL handle and duplicate it
    CURLU *url_handle = curl_url();
    CURLU *url_dup = NULL;
    if (url_handle) {
        // Set some URL parts using input data
        if (size > 0) {
            // Create null-terminated string from input data
            char *url_str = (char*)malloc(size + 1);
            if (url_str) {
                memcpy(url_str, data, size);
                url_str[size] = '\0';
                
                // Try to set URL
                curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_1);
                free(url_str);
            }
        }
        
        // Duplicate the URL handle
        url_dup = curl_url_dup(url_handle);
    }
    
    // Create and use mime type
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_mime *mime = curl_mime_init(easy_handle);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Use input data for mime type (limited to reasonable length)
                size_t mime_len = size > 100 ? 100 : size;
                char *mime_type = (char*)malloc(mime_len + 1);
                if (mime_type) {
                    memcpy(mime_type, data, mime_len);
                    mime_type[mime_len] = '\0';
                    
                    // Set mime type
                    curl_mime_type(part, mime_type);
                    free(mime_type);
                }
            }
            curl_mime_free(mime);
        }
        curl_easy_cleanup(easy_handle);
    }
    
    // Create and free form
    struct curl_httppost *form = NULL;
    struct curl_httppost *last = NULL;
    
    // Add some form fields using input data
    if (size > 0) {
        // Create field name from first part of data
        size_t name_len = size > 50 ? 50 : size/2;
        char *field_name = (char*)malloc(name_len + 1);
        if (field_name) {
            memcpy(field_name, data, name_len);
            field_name[name_len] = '\0';
            
            // Create field content from second part of data
            size_t content_len = size > name_len ? size - name_len : 0;
            if (content_len > 0) {
                char *field_content = (char*)malloc(content_len + 1);
                if (field_content) {
                    memcpy(field_content, data + name_len, content_len);
                    field_content[content_len] = '\0';
                    
                    // Try to add form field
                    curl_formadd(&form, &last,
                                CURLFORM_COPYNAME, field_name,
                                CURLFORM_COPYCONTENTS, field_content,
                                CURLFORM_END);
                    free(field_content);
                }
            }
            free(field_name);
        }
    }
    
    // Free the form
    curl_formfree(form);
    
    // String comparison using input data
    if (size >= 2) {
        // Create two strings from different parts of input
        size_t split = size / 2;
        char *str1 = (char*)malloc(split + 1);
        char *str2 = (char*)malloc(size - split + 1);
        
        if (str1 && str2) {
            memcpy(str1, data, split);
            str1[split] = '\0';
            
            memcpy(str2, data + split, size - split);
            str2[size - split] = '\0';
            
            // Compare strings (case-insensitive)
            int equal = curl_strequal(str1, str2);
            (void)equal; // Result intentionally ignored for fuzzing
            
            free(str1);
            free(str2);
        } else {
            if (str1) free(str1);
            if (str2) free(str2);
        }
    }
    
    // Cleanup
    if (url_dup) {
        curl_url_cleanup(url_dup);
    }
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    return 0;
}
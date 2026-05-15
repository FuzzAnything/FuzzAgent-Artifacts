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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_global_cleanup","curl_mime_name","curl_mime_type","curl_slist_append","curl_mime_headers","curl_formadd","curl_mime_data","curl_easy_setopt","curl_easy_reset","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_type","curl_slist_append","curl_mime_headers","curl_formadd","curl_mime_data","curl_easy_setopt","curl_easy_reset","curl_formfree","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include curl headers

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Create mime structure
    curl_mime *mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Create mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Parse input data for mime configuration
    // Use first portion for mime name (up to 64 bytes or until null)
    size_t name_len = 0;
    while (name_len < size && name_len < 64 && data[name_len] != 0) {
        name_len++;
    }
    char *mime_name = (char*)malloc(name_len + 1);
    if (mime_name) {
        memcpy(mime_name, data, name_len);
        mime_name[name_len] = 0;
        
        // Set mime name using input data
        curl_mime_name(part, mime_name);
        free(mime_name);
    }

    // Use next portion for mime type (skip null byte if present)
    size_t type_offset = (name_len < size && data[name_len] == 0) ? name_len + 1 : name_len;
    if (type_offset < size) {
        size_t type_len = 0;
        size_t pos = type_offset;
        while (pos < size && type_len < 64 && data[pos] != 0) {
            type_len++;
            pos++;
        }
        
        char *mime_type = (char*)malloc(type_len + 1);
        if (mime_type) {
            memcpy(mime_type, data + type_offset, type_len);
            mime_type[type_len] = 0;
            
            // Set mime type using input data
            curl_mime_type(part, mime_type);
            free(mime_type);
        }
    }

    // Create headers list for mime part
    struct curl_slist *headers = NULL;
    // Use a portion of input data as header value
    size_t header_offset = type_offset;
    if (header_offset < size) {
        size_t header_len = 0;
        size_t pos = header_offset;
        while (pos < size && header_len < 128 && data[pos] != 0) {
            header_len++;
            pos++;
        }
        
        if (header_len > 0) {
            char *header_str = (char*)malloc(header_len + 12);
            if (header_str) {
                memcpy(header_str, "X-Custom: ", 10);
                memcpy(header_str + 10, data + header_offset, header_len);
                header_str[10 + header_len] = 0;
                
                headers = curl_slist_append(headers, header_str);
                free(header_str);
            }
        }
    }

    // Set mime headers (take ownership = 1)
    if (headers) {
        curl_mime_headers(part, headers, 1);
    }

    // Create form data (older API) for comparison
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Use a portion of input data for form field
    size_t form_offset = header_offset;
    if (form_offset < size) {
        size_t form_len = (size - form_offset) > 128 ? 128 : (size - form_offset);
        
        char *form_data = (char*)malloc(form_len + 1);
        if (form_data) {
            memcpy(form_data, data + form_offset, form_len);
            form_data[form_len] = 0;
            
            // Add form field (simplified - actual curl_formadd requires careful parameter handling)
            // Note: This demonstrates the API usage but may not create valid form
            curl_formadd(&formpost, &lastptr,
                        CURLFORM_COPYNAME, "fuzzfield",
                        CURLFORM_COPYCONTENTS, form_data,
                        CURLFORM_END);
            
            free(form_data);
        }
    }

    // Set mime data using remaining input
    size_t data_offset = form_offset;
    if (data_offset < size) {
        size_t remaining = size - data_offset;
        curl_mime_data(part, (const char*)(data + data_offset), remaining);
    }

    // Set mime to easy handle
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    // Reset easy handle (this will clear the mime setting)
    curl_easy_reset(curl);

    // Free form data
    curl_formfree(formpost);

    // Cleanup - note: mime headers were taken ownership by curl_mime_headers
    curl_mime_free(mime);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
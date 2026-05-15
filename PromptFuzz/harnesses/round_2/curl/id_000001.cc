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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"slist_get_last":[[39,9,39,19,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_global_cleanup","curl_mime_name","curl_mime_type","curl_mime_type","curl_mime_type","curl_mime_type","curl_mime_data","curl_slist_append","curl_slist_append","curl_mime_headers","curl_formadd","curl_formfree","curl_easy_reset","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_type","curl_mime_data","curl_slist_append","curl_slist_append","curl_mime_headers","curl_formadd","curl_formfree","curl_easy_reset","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to handle MIME data from the input:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 2) {
        return 0;
    }

    CURL *curl = nullptr;
    curl_mime *mime = nullptr;
    curl_mimepart *part = nullptr;
    struct curl_slist *headers = nullptr;
    struct curl_httppost *formpost = nullptr;
    struct curl_httppost *lastpost = nullptr;

    // Initialize libcurl
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    // Create easy handle
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Create MIME structure
    mime = curl_mime_init(curl);
    if (!mime) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Add a MIME part
    part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }

    // Set MIME part name (use first part of data as name)
    size_t name_len = data[0] % (size > 1 ? size - 1 : 1) + 1;
    if (name_len > size) name_len = size;
    char *name = (char *)malloc(name_len + 1);
    if (name) {
        memcpy(name, data, name_len);
        name[name_len] = '\0';
        curl_mime_name(part, name);
        free(name);
    }

    // Set MIME type (use a fixed type for simplicity)
    const char *mime_type = "application/octet-stream";
    if (size > 2) {
        // Use a portion of data to determine MIME type
        size_t type_len = data[1] % 16 + 1;
        if (type_len + 2 < size) {
            char *dynamic_type = (char *)malloc(type_len + 1);
            if (dynamic_type) {
                memcpy(dynamic_type, data + 2, type_len);
                dynamic_type[type_len] = '\0';
                curl_mime_type(part, dynamic_type);
                free(dynamic_type);
            } else {
                curl_mime_type(part, mime_type);
            }
        } else {
            curl_mime_type(part, mime_type);
        }
    } else {
        curl_mime_type(part, mime_type);
    }

    // Set MIME data from input
    size_t data_offset = name_len + 2;
    if (data_offset < size) {
        size_t data_size = size - data_offset;
        char *mime_data = (char *)malloc(data_size + 1);
        if (mime_data) {
            memcpy(mime_data, data + data_offset, data_size);
            mime_data[data_size] = '\0';
            curl_mime_data(part, mime_data, data_size);
            free(mime_data);
        }
    }

    // Create and set headers
    const char *header_strings[] = {
        "Content-Type: application/octet-stream",
        "Content-Disposition: form-data",
        nullptr
    };
    
    for (int i = 0; header_strings[i] != nullptr; i++) {
        struct curl_slist *new_header = curl_slist_append(headers, header_strings[i]);
        if (new_header) {
            headers = new_header;
        }
    }
    
    // Add a custom header from input data
    if (size > 10) {
        size_t header_len = data[2] % 32 + 1;
        if (header_len + 3 < size) {
            char *custom_header = (char *)malloc(header_len + 32);
            if (custom_header) {
                strcpy(custom_header, "X-Custom-Header: ");
                memcpy(custom_header + 17, data + 3, header_len);
                custom_header[17 + header_len] = '\0';
                struct curl_slist *new_header = curl_slist_append(headers, custom_header);
                if (new_header) {
                    headers = new_header;
                }
                free(custom_header);
            }
        }
    }
    
    // Set MIME headers (take ownership)
    curl_mime_headers(part, headers, 1);

    // Create a dummy form (for curl_formfree)
    formpost = lastpost = nullptr;
    // Add a simple form field
    curl_formadd(&formpost, &lastpost,
                 CURLFORM_COPYNAME, "field1",
                 CURLFORM_COPYCONTENTS, "value1",
                 CURLFORM_END);

    // Free the form (curl_formfree API)
    if (formpost) {
        curl_formfree(formpost);
        formpost = nullptr;
    }

    // Reset the easy handle (curl_easy_reset API)
    curl_easy_reset(curl);

    // Cleanup
    if (mime) {
        curl_mime_free(mime);
    }
    if (curl) {
        curl_easy_cleanup(curl);
    }
    
    // Note: headers are already freed by curl_mime_headers with take_ownership=1
    curl_global_cleanup();

    return 0;
}
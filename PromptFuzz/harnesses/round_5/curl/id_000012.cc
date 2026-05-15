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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_slist_append","curl_mime_headers","curl_mime_free","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_notify_enable","curl_easy_reset","curl_easy_upkeep","curl_easy_send","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_slist_append","curl_mime_headers","curl_mime_free","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_notify_enable","curl_easy_reset","curl_easy_upkeep","curl_easy_send","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 1. Create and duplicate a CURLU handle
    CURLU *url_handle = curl_url();
    CURLU *dup_url_handle = nullptr;
    if (url_handle) {
        // Set a URL part from input data
        char url_buf[256];
        size_t url_len = size > 255 ? 255 : size;
        memcpy(url_buf, data, url_len);
        url_buf[url_len] = '\0';
        curl_url_set(url_handle, CURLUPART_URL, url_buf, 0);
        
        // Duplicate the URL handle
        dup_url_handle = curl_url_dup(url_handle);
    }

    // 2. Create easy handle for curl operations
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // 3. Create mime structure and set filename
        curl_mime *mime = curl_mime_init(easy_handle);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Use first 100 bytes as filename (or less if input is smaller)
                char filename_buf[101];
                size_t filename_len = size > 100 ? 100 : size;
                memcpy(filename_buf, data, filename_len);
                filename_buf[filename_len] = '\0';
                curl_mime_filename(part, filename_buf);
                
                // 4. Set headers for mime part
                struct curl_slist *headers = nullptr;
                char header_buf[256];
                size_t header_len = size > 255 ? 255 : size;
                memcpy(header_buf, data, header_len);
                header_buf[header_len] = '\0';
                headers = curl_slist_append(headers, header_buf);
                if (headers) {
                    curl_mime_headers(part, headers, 1); // take ownership
                }
            }
            curl_mime_free(mime);
        }

        // 5. Set some options for the easy handle
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        
        // 6. Create multi handle for notifications
        CURLM *multi_handle = curl_multi_init();
        if (multi_handle) {
            // Add easy handle to multi handle
            curl_multi_add_handle(multi_handle, easy_handle);
            
            // 7. Enable notifications on multi handle
            // Using CURL_POLL_IN notification (0x1)
            curl_multi_notify_enable(multi_handle, 0x1);
            
            // 8. Reset the easy handle
            curl_easy_reset(easy_handle);
            
            // 9. Perform upkeep on easy handle
            curl_easy_upkeep(easy_handle);
            
            // 10. Send data using easy handle
            // Prepare buffer from remaining input data
            size_t send_size = size > 1024 ? 1024 : size;
            uint8_t *send_buf = (uint8_t*)malloc(send_size);
            if (send_buf) {
                memcpy(send_buf, data, send_size);
                size_t sent = 0;
                // Note: This will likely fail since we're not connected,
                // but we're calling the API as requested
                curl_easy_send(easy_handle, send_buf, send_size, &sent);
                free(send_buf);
            }
            
            // Clean up multi handle
            curl_multi_remove_handle(multi_handle, easy_handle);
            curl_multi_cleanup(multi_handle);
        }
        
        // Clean up easy handle
        curl_easy_cleanup(easy_handle);
    }

    // Clean up URL handles
    if (dup_url_handle) {
        curl_url_cleanup(dup_url_handle);
    }
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Clean up global curl
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
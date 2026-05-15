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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 14, nr_unique_branch: 2
//<Quality> {"density":14,"unique_branches":{"cpool_get_instance":[[263,13,263,24,0,0,4,1]],"Curl_cpool_upkeep":[[752,6,752,12,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_url","curl_url_dup","curl_url_cleanup","curl_url_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_slist_append","curl_mime_headers","curl_mime_free","curl_multi_notify_enable","curl_easy_send","curl_easy_upkeep","curl_easy_reset","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_url","curl_url_dup","curl_url_cleanup","curl_url_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_slist_append","curl_mime_headers","curl_mime_free","curl_multi_notify_enable","curl_easy_send","curl_easy_upkeep","curl_easy_reset","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":2}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 5. Create URL handle and duplicate it
    CURLU *url_handle = curl_url();
    if (url_handle) {
        CURLU *dup_url = curl_url_dup(url_handle);
        if (dup_url) {
            curl_url_cleanup(dup_url);
        }
        curl_url_cleanup(url_handle);
    }
    
    // 6. Create mime structure and set filename
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use first 256 bytes or less as filename
            size_t filename_len = size > 256 ? 256 : size;
            char *filename_buf = (char *)malloc(filename_len + 1);
            if (filename_buf) {
                memcpy(filename_buf, data, filename_len);
                filename_buf[filename_len] = '\0';
                curl_mime_filename(part, filename_buf);
                free(filename_buf);
            }
            
            // Set headers for mime part
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
            if (headers) {
                curl_mime_headers(part, headers, 1); // Take ownership
            }
        }
        curl_mime_free(mime);
    }
    
    // 7. Enable notifications on multi handle
    // Using a valid notification type (CURLM_NOTIFY_CONNECT = 1)
    curl_multi_notify_enable(multi_handle, 1);
    
    // 8. Send data using easy handle
    if (size > 0) {
        size_t sent = 0;
        // Create a buffer to send (use first 1024 bytes or available size)
        size_t send_size = size > 1024 ? 1024 : size;
        void *send_buf = malloc(send_size);
        if (send_buf) {
            memcpy(send_buf, data, send_size);
            curl_easy_send(easy_handle, send_buf, send_size, &sent);
            free(send_buf);
        }
    }
    
    // 9. Perform upkeep on easy handle
    curl_easy_upkeep(easy_handle);
    
    // 10. Reset easy handle
    curl_easy_reset(easy_handle);
    
    // 11. Add easy handle to multi handle and perform
    if (multi_handle && easy_handle) {
        curl_multi_add_handle(multi_handle, easy_handle);
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
        
        // Remove handle from multi
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    // 12. Cleanup
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
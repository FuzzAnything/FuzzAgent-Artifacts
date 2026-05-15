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
//<ID> 100
//<Prompt> []
/*<Combination>: [
*/
//<score> 10.666667, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"Curl_mntfy_enable":[[150,6,150,34,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_url","curl_url_set","curl_url_dup","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_url_get","curl_easy_setopt","curl_free","curl_multi_add_handle","curl_multi_notify_enable","curl_easy_send","curl_easy_upkeep","curl_easy_reset","curl_multi_perform","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_url","curl_url_set","curl_url_dup","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_url_get","curl_easy_setopt","curl_free","curl_multi_add_handle","curl_multi_notify_enable","curl_easy_send","curl_easy_upkeep","curl_easy_reset","curl_multi_perform","curl_mime_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":2}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int fd_in = fuzz_fileno(in_file);

    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
    }

    // Create URL handle and duplicate it
    CURLU *url = curl_url();
    CURLU *url_dup = nullptr;
    if (url) {
        // Set some URL parts (using first part of data as URL string)
        size_t url_len = size > 100 ? 100 : size;
        char *url_str = (char*)malloc(url_len + 1);
        if (url_str) {
            memcpy(url_str, data, url_len);
            url_str[url_len] = '\0';
            
            // Set URL
            curl_url_set(url, CURLUPART_URL, url_str, 0);
            
            // Duplicate URL
            url_dup = curl_url_dup(url);
            
            free(url_str);
        }
    }

    // Create mime structure
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        // Create mime part
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Set filename using second part of data
            size_t filename_len = size > 50 ? 50 : size/2;
            char *filename = (char*)malloc(filename_len + 1);
            if (filename) {
                size_t offset = size > 100 ? 100 : 0;
                size_t copy_len = (size - offset) > filename_len ? filename_len : (size - offset);
                if (copy_len > 0) {
                    memcpy(filename, data + offset, copy_len);
                    filename[copy_len] = '\0';
                    curl_mime_filename(part, filename);
                }
                free(filename);
            }

            // Create headers list for mime part
            struct curl_slist *headers = nullptr;
            size_t header_len = size > 30 ? 30 : size/3;
            char *header = (char*)malloc(header_len + 20);
            if (header) {
                size_t offset = size > 150 ? 150 : 0;
                size_t copy_len = (size - offset) > header_len ? header_len : (size - offset);
                if (copy_len > 0) {
                    memcpy(header, data + offset, copy_len);
                    header[copy_len] = '\0';
                    strcat(header, ": value");
                    headers = curl_slist_append(headers, header);
                }
                free(header);
            }
            
            // Set headers for mime part (take ownership)
            if (headers) {
                curl_mime_headers(part, headers, 1);
            }
        }
    }

    // Set up curl options
    if (mime) {
        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);
    }
    
    // Set URL if we have one
    if (url) {
        char *url_out = nullptr;
        if (curl_url_get(url, CURLUPART_URL, &url_out, 0) == CURLUE_OK) {
            if (url_out) {
                curl_easy_setopt(curl, CURLOPT_URL, url_out);
                curl_free(url_out);
            }
        }
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi, curl);

    // Enable notifications on multi handle
    curl_multi_notify_enable(multi, CURL_POLL_IN | CURL_POLL_OUT);

    // Create buffer for sending
    size_t send_buf_len = size > 1024 ? 1024 : size;
    void *send_buf = malloc(send_buf_len);
    if (send_buf) {
        memcpy(send_buf, data, send_buf_len > size ? size : send_buf_len);
        
        // Try to send data (this may fail since we're not actually connected)
        size_t sent = 0;
        curl_easy_send(curl, send_buf, send_buf_len, &sent);
        
        free(send_buf);
    }

    // Perform upkeep on easy handle
    curl_easy_upkeep(curl);

    // Reset easy handle
    curl_easy_reset(curl);

    // Perform some multi operations
    int running_handles = 0;
    curl_multi_perform(multi, &running_handles);

    // Clean up
    if (mime) {
        curl_mime_free(mime);
    }
    
    curl_multi_remove_handle(multi, curl);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(curl);
    
    if (url_dup) {
        curl_url_cleanup(url_dup);
    }
    if (url) {
        curl_url_cleanup(url);
    }
    
    assert_file_closed(&in_file);;
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(fd_in);
	return 0;
}
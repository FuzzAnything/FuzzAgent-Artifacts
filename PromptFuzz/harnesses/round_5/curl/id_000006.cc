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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 5
//<Quality> {"density":20,"unique_branches":{"FormInfoCopyField":[[127,8,127,12,0,0,4,1]],"FormAdd":[[407,5,407,33,0,0,4,0],[313,26,313,31,9,0,4,1]],"FormAddCheck":[[230,9,230,29,0,0,4,0],[231,9,231,42,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_free","curl_url","curl_url_set","curl_url_get","curl_free","curl_url_cleanup","curl_unescape","curl_free","curl_easy_unescape","curl_free","curl_easy_pause","curl_easy_pause","curl_multi_fdset","curl_multi_socket_action","curl_formadd","curl_formfree","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_free","curl_url","curl_url_set","curl_url_get","curl_free","curl_url_cleanup","curl_unescape","curl_free","curl_easy_unescape","curl_free","curl_easy_pause","curl_easy_pause","curl_multi_fdset","curl_multi_socket_action","curl_formadd","curl_formfree","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = out_file ? fuzz_fileno(out_file) : -1;
    
    // 1. Create multi handle and setup
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    
    if (multi_handle && easy_handle) {
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // 2. Create mime part and set encoder
        curl_mime *mime = curl_mime_init(easy_handle);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Use part of data as encoding type
                const char *encoding = size > 0 ? "gzip" : "identity";
                curl_mime_encoder(part, encoding);
            }
            curl_mime_free(mime);
        }
        
        // 3. URL parsing and getting parts
        CURLU *url = curl_url();
        if (url) {
            // Set a test URL
            curl_url_set(url, CURLUPART_URL, "https://example.com/path", 0);
            
            // Get URL parts
            char *url_part = nullptr;
            unsigned int flags = 0;
            if (size > 10) {
                // Use first 10 bytes to determine flags
                flags = data[0] % 8;
            }
            curl_url_get(url, CURLUPART_HOST, &url_part, flags);
            if (url_part) {
                curl_free(url_part);
            }
            curl_url_cleanup(url);
        }
        
        // 4. Unescaping strings
        if (size > 0) {
            // Simple unescape
            char *unescaped1 = curl_unescape((const char*)data, 
                                           size > 100 ? 100 : (int)size);
            if (unescaped1) {
                curl_free(unescaped1);
            }
            
            // Easy unescape
            int outlength = 0;
            char *unescaped2 = curl_easy_unescape(easy_handle, (const char*)data,
                                                size > 50 ? 50 : (int)size, &outlength);
            if (unescaped2) {
                curl_free(unescaped2);
            }
        }
        
        // 5. Pause and resume easy handle
        curl_easy_pause(easy_handle, CURLPAUSE_ALL);
        curl_easy_pause(easy_handle, CURLPAUSE_CONT);
        
        // 6. Multi fdset operations
        fd_set read_fd_set, write_fd_set, exc_fd_set;
        FD_ZERO(&read_fd_set);
        FD_ZERO(&write_fd_set);
        FD_ZERO(&exc_fd_set);
        
        if (in_fd >= 0) {
            FD_SET(in_fd, &read_fd_set);
        }
        if (out_fd >= 0) {
            FD_SET(out_fd, &write_fd_set);
        }
        
        int max_fd = (in_fd > out_fd) ? in_fd : out_fd;
        if (max_fd < 0) max_fd = 0;
        
        curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, 
                        &exc_fd_set, &max_fd);
        
        // 7. Multi socket operations
        int running_handles = 0;
        if (in_fd >= 0) {
            curl_multi_socket(multi_handle, in_fd, &running_handles);
        }
        
        // 8. Form operations
        struct curl_httppost *form = nullptr;
        struct curl_httppost *last_post = nullptr;
        
        // Try to add form data if we have enough input
        if (size > 20) {
            const char *name = "test_field";
            const char *content = (const char*)data;
            int content_len = size > 100 ? 100 : (int)size;
            
            curl_formadd(&form, &last_post,
                        CURLFORM_COPYNAME, name,
                        CURLFORM_COPYCONTENTS, content,
                        CURLFORM_CONTENTSLENGTH, content_len,
                        CURLFORM_END);
        }
        
        // Free the form
        curl_formfree(form);
        
        // Perform multi operations
        int still_running = 0;
        curl_multi_perform(multi_handle, &still_running);
        
        // Cleanup multi handle
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_cleanup(multi_handle);
    }
    
    // Cleanup easy handle
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    // Close files
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    
    // Cleanup curl
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}
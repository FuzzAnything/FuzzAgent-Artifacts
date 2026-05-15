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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 0
//<Quality> {"density":21,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_free","curl_url","curl_url_set","curl_url_get","curl_free","curl_url_cleanup","curl_unescape","curl_free","curl_easy_unescape","curl_free","curl_easy_pause","curl_easy_pause","curl_multi_fdset","curl_formadd","curl_formfree","curl_multi_add_handle","curl_easy_setopt","curl_multi_socket_action","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_free","curl_url","curl_url_set","curl_url_get","curl_free","curl_url_cleanup","curl_unescape","curl_free","curl_easy_unescape","curl_free","curl_easy_pause","curl_easy_pause","curl_multi_fdset","curl_formadd","curl_formfree","curl_multi_add_handle","curl_easy_setopt","curl_multi_socket_action","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // Initialize curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // Create a FILE* from input data
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
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 1. Create and use curl_mime_encoder
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use first 10 bytes as encoding string
            char encoding[11] = {0};
            size_t enc_len = size > 10 ? 10 : size;
            memcpy(encoding, data, enc_len);
            curl_mime_encoder(part, encoding);
        }
        curl_mime_free(mime);
    }
    
    // 2. Create and use curl_url_get
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Use middle portion of data as URL part
        size_t url_offset = size > 20 ? 10 : 0;
        size_t url_len = size > 20 ? (size - 10) : size;
        char *url_part = (char*)malloc(url_len + 1);
        if (url_part) {
            memcpy(url_part, data + url_offset, url_len);
            url_part[url_len] = '\0';
            
            // Set the URL
            curl_url_set(url_handle, CURLUPART_URL, url_part, 0);
            
            // Get a part from URL
            char *host_part = nullptr;
            CURLUcode url_res = curl_url_get(url_handle, CURLUPART_HOST, &host_part, 0);
            if (host_part) curl_free(host_part);
            
            free(url_part);
        }
        curl_url_cleanup(url_handle);
    }
    
    // 3. Use curl_unescape and curl_easy_unescape
    // Find a null-terminated string in data for unescaping
    size_t str_start = 0;
    while (str_start < size && data[str_start] == 0) str_start++;
    
    if (str_start < size) {
        size_t str_len = 0;
        while (str_start + str_len < size && data[str_start + str_len] != 0) {
            str_len++;
        }
        
        if (str_len > 0) {
            // Use curl_unescape
            char *unescaped1 = curl_unescape((const char*)(data + str_start), (int)str_len);
            if (unescaped1) curl_free(unescaped1);
            
            // Use curl_easy_unescape
            int outlength = 0;
            char *unescaped2 = curl_easy_unescape(easy_handle, (const char*)(data + str_start), 
                                                 (int)str_len, &outlength);
            if (unescaped2) curl_free(unescaped2);
        }
    }
    
    // 4. Use curl_easy_pause
    // Pause and resume the handle
    curl_easy_pause(easy_handle, CURLPAUSE_ALL);
    curl_easy_pause(easy_handle, CURLPAUSE_CONT);
    
    // 5. Use curl_multi_fdset
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    int max_fd = 0;
    
    // Add our file descriptors to the sets
    FD_SET(in_fd, &read_fd_set);
    FD_SET(out_fd, &write_fd_set);
    max_fd = (in_fd > out_fd) ? in_fd : out_fd;
    
    // Call curl_multi_fdset
    curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
    
    // 6. Use curl_formfree
    struct curl_httppost *formpost = nullptr;
    struct curl_httppost *lastptr = nullptr;
    
    // Try to create a simple form
    curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, "field1",
                CURLFORM_COPYCONTENTS, "value1",
                CURLFORM_END);
    
    // Free the form
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // 7. Use curl_multi_socket
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Set some options on the easy handle
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file://input_file");
    
    // Try to use the socket interface
    int running_handles = 0;
    curl_multi_socket(multi_handle, CURL_SOCKET_TIMEOUT, &running_handles);
    
    // Perform a simple multi operation
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);
    
    // Clean up multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up curl
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
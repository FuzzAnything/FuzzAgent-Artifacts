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
//<ID> 471
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 0
//<Quality> {"density":20,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_init","curl_mime_addpart","curl_mime_addpart","curl_strnequal","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_setopt","curl_multi_setopt","curl_multi_timeout","curl_multi_fdset","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_init","curl_mime_addpart","curl_mime_addpart","curl_strnequal","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_setopt","curl_multi_setopt","curl_multi_timeout","curl_multi_fdset","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create mime structure for the easy handle
    curl_mime *mime = curl_mime_init(easy_handle);
    curl_mime *submime = curl_mime_init(easy_handle);
    
    if (mime && submime) {
        // Create mime parts
        curl_mimepart *part = curl_mime_addpart(mime);
        curl_mimepart *subpart = curl_mime_addpart(submime);
        
        if (part && subpart) {
            // Add data to subpart from input file
            fseek(in_file, 0, SEEK_SET);
            fseek(out_file, 0, SEEK_SET);
            
            // Read a portion of data for string comparison
            size_t str_len = size > 100 ? 100 : size;
            char str1[101], str2[101];
            fread(str1, 1, str_len, in_file);
            fread(str2, 1, str_len, in_file);
            
            // Use curl_strnequal for string comparison
            int str_equal = curl_strnequal(str1, str2, str_len);
            
            // Set mime data from remaining input
            fseek(in_file, 0, SEEK_SET);
            char *buffer = new char[size];
            fread(buffer, 1, size, in_file);
            
            curl_mime_data(subpart, buffer, size);
            
            // Set subparts for the main part
            CURLcode res = curl_mime_subparts(part, submime);
            
            // Set up easy handle with mime data
            if (res == CURLE_OK) {
                curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
                curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
                
                // Add easy handle to multi handle
                curl_multi_add_handle(multi_handle, easy_handle);
                
                // Set multi handle options using curl_multi_setopt
                curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 10L);
                curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 10L);
                
                // Get timeout using curl_multi_timeout
                long timeout;
                CURLMcode mres = curl_multi_timeout(multi_handle, &timeout);
                
                // Set up file descriptor sets for select()
                fd_set read_fd_set, write_fd_set, exc_fd_set;
                FD_ZERO(&read_fd_set);
                FD_ZERO(&write_fd_set);
                FD_ZERO(&exc_fd_set);
                
                // Add our file descriptors
                FD_SET(in_fd, &read_fd_set);
                FD_SET(out_fd, &write_fd_set);
                
                int max_fd = (in_fd > out_fd) ? in_fd : out_fd;
                
                // Use curl_multi_fdset to get file descriptors from multi handle
                mres = curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, 
                                       &exc_fd_set, &max_fd);
                
                // Perform some operations
                int running_handles = 0;
                curl_multi_perform(multi_handle, &running_handles);
                
                // Clean up multi handle
                curl_multi_remove_handle(multi_handle, easy_handle);
            }
            
            delete[] buffer;
        }
        
        // Clean up mime structures
        if (submime) {
            // submime will be freed automatically when parent mime is freed
            // because it was set as subparts
        }
        if (mime) {
            curl_mime_free(mime);
        }
    }

    // Clean up
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up global state
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
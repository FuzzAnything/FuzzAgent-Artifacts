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
//<ID> 472
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"Curl_mime_set_subparts":[[1462,10,1462,26,0,0,4,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_setopt","curl_multi_setopt","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_multi_timeout","curl_multi_fdset","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_multi_setopt","curl_multi_setopt","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_multi_timeout","curl_multi_fdset","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl globally
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle to use with multi handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create FILE* for writing output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // 1. Use curl_multi_setopt - set some options on the multi handle
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 10L);

    // 2. Use curl_strnequal - compare strings from input data
    // Extract two strings from the input data for comparison
    const char *str1 = (const char *)data;
    size_t str1_len = size > 0 ? 1 : 0;  // Use first byte as string if available
    const char *str2 = (const char *)(data + (size > 1 ? 1 : 0));
    size_t str2_len = size > 1 ? 1 : 0;  // Use second byte as string if available
    
    if (str1_len > 0 && str2_len > 0) {
        int equal = curl_strnequal(str1, str2, str1_len < str2_len ? str1_len : str2_len);
        (void)equal;  // Use the result to avoid unused variable warning
    }

    // 3. Use curl_mime_subparts - create mime structure with subparts
    curl_mime *mime = curl_mime_init(easy_handle);
    curl_mimepart *part = curl_mime_addpart(mime);
    curl_mimepart *subpart = curl_mime_addpart(mime);
    
    if (mime && part && subpart) {
        // Set some data on the subpart
        if (size > 0) {
            curl_mime_data(subpart, (const char *)data, 
                          size > 1024 ? 1024 : size);  // Limit to 1024 bytes
        }
        
        // Attach subpart to main part
        curl_mime_subparts(part, mime);
    }

    // 4. Use curl_multi_timeout - get timeout from multi handle
    long timeout_ms;
    CURLMcode timeout_rc = curl_multi_timeout(multi_handle, &timeout_ms);
    (void)timeout_rc;  // Use the result to avoid unused variable warning

    // 5. Use curl_multi_fdset - get file descriptor sets
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    int max_fd = -1;
    
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    
    // Add our file descriptors to the sets
    if (in_fd >= 0) {
        FD_SET(in_fd, &read_fd_set);
        if (in_fd > max_fd) max_fd = in_fd;
    }
    
    if (out_fd >= 0) {
        FD_SET(out_fd, &write_fd_set);
        if (out_fd > max_fd) max_fd = out_fd;
    }
    
    // Get fdset from multi handle
    CURLMcode fdset_rc = curl_multi_fdset(multi_handle, &read_fd_set, 
                                         &write_fd_set, &exc_fd_set, &max_fd);
    (void)fdset_rc;  // Use the result to avoid unused variable warning

    // Consume some data from input file
    if (in_fd >= 0 && FD_ISSET(in_fd, &read_fd_set)) {
        char buffer[1024];
        size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
        if (bytes_read > 0 && out_fd >= 0 && FD_ISSET(out_fd, &write_fd_set)) {
            fwrite(buffer, 1, bytes_read, out_file);
        }
    }

    // Perform a dummy multi perform to exercise the multi handle
    int running_handles = 0;
    if (easy_handle) {
        // Set some easy handle options
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L);  // HEAD request
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // Perform multi handle operations
        curl_multi_perform(multi_handle, &running_handles);
        
        // Remove easy handle from multi handle
        curl_multi_remove_handle(multi_handle, easy_handle);
    }

    // Clean up resources
    if (mime) {
        curl_mime_free(mime);
    }
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    curl_global_cleanup();
    
    // Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
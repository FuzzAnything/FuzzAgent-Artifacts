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
//<ID> 731
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 2
//<Quality> {"density":19,"unique_branches":{"curl_easy_duphandle":[[1015,6,1015,39,0,0,4,0],[1019,8,1019,45,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_duphandle","curl_multi_get_offt","curl_easy_header","curl_multi_assign","curl_easy_strerror","curl_multi_perform","curl_multi_info_read","curl_multi_remove_handle","curl_multi_add_handle","curl_multi_remove_handle","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_duphandle","curl_multi_get_offt","curl_easy_header","curl_multi_assign","curl_easy_strerror","curl_multi_perform","curl_multi_info_read","curl_multi_remove_handle","curl_multi_add_handle","curl_multi_remove_handle","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

// Custom types from curl
typedef long curl_off_t;
typedef void CURLM;
typedef void CURL;
typedef int curl_socket_t;

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a memory file from input data
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
    
    // Initialize easy and multi handles
    CURL *easy = curl_easy_init();
    CURL *easy_dup = nullptr;
    CURLM *multi = curl_multi_init();
    
    if (!easy || !multi) {
        if (easy) curl_easy_cleanup(easy);
        if (multi) curl_multi_cleanup(multi);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up easy handle with input data
    // Use the input data as a URL string (truncate to reasonable length)
    size_t url_len = size > 4096 ? 4096 : size;
    char *url = (char*)malloc(url_len + 1);
    if (url) {
        memcpy(url, data, url_len);
        url[url_len] = '\0';
        curl_easy_setopt(easy, CURLOPT_URL, url);
        free(url);
    }
    
    // Set input file for upload
    curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    
    // Set output file for download
    curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // 1. Duplicate the easy handle (API #5)
    easy_dup = curl_easy_duphandle(easy);
    
    // 2. Get offset value from multi handle (API #1)
    curl_off_t offset_value = 0;
    if (multi) {
        // Try to get socket offset info (use CURLMINFO_OFFT_SOCK which is 0)
        curl_multi_get_offt(multi, (CURLMinfo_offt)0, &offset_value);
    }
    
    // 3. Try to get a header from easy handle (API #2)
    struct curl_header *header_out = nullptr;
    if (easy) {
        // Try to get Content-Type header
        curl_easy_header(easy, "Content-Type", 0, 
                        CURLH_HEADER, 0, &header_out);
    }
    
    // 4. Assign socket data to multi handle (API #3)
    if (multi) {
        // Create a dummy socket pointer using the offset value
        void *sockp = (void*)(intptr_t)offset_value;
        curl_multi_assign(multi, (curl_socket_t)in_fd, sockp);
    }
    
    // 5. Get error string for a sample error code (API #4)
    const char *error_str = curl_easy_strerror(CURLE_OK);
    
    // Perform a minimal multi operation
    int still_running = 0;
    if (multi) {
        curl_multi_perform(multi, &still_running);
        
        // Process any pending messages
        int msgs_in_queue = 0;
        CURLMsg *msg = curl_multi_info_read(multi, &msgs_in_queue);
        
        // Remove handles from multi
        curl_multi_remove_handle(multi, easy);
        if (easy_dup) {
            curl_multi_add_handle(multi, easy_dup);
            curl_multi_remove_handle(multi, easy_dup);
        }
    }
    
    // Clean up duplicated handle
    if (easy_dup) {
        curl_easy_cleanup(easy_dup);
    }
    
    // Clean up original handles
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Global cleanup
    curl_global_cleanup();
    
    // Ensure the error string is referenced to prevent optimization
    if (error_str) {
        volatile const char *dummy = error_str;
        (void)dummy;
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
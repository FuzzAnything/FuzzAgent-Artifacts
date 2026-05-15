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
//<ID> 998
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 2
//<Quality> {"density":22,"unique_branches":{"multi_addtimeout":[[3589,10,3589,18,0,0,4,0]],"Curl_llist_insert_next":[[91,8,91,10,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_easy_duphandle","curl_mime_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_unescape","curl_strequal","curl_free","curl_easy_reset","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_easy_duphandle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_unescape","curl_strequal","curl_free","curl_easy_reset","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize global curl
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // Create a file from input data for reading
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
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create duplicate handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        if (dup_handle) curl_easy_cleanup(dup_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Add mime part
    curl_mimepart *mime_part = curl_mime_addpart(mime);
    if (mime_part) {
        // Use part of input data for mime (first 100 bytes or less)
        size_t mime_data_size = size > 100 ? 100 : size;
        char *mime_data = (char *)malloc(mime_data_size + 1);
        if (mime_data) {
            memcpy(mime_data, data, mime_data_size);
            mime_data[mime_data_size] = '\0';
            
            // Set mime data (using curl_mime_data from exported APIs)
            curl_mime_data(mime_part, mime_data, mime_data_size);
            
            free(mime_data);
        }
    }

    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost/test");
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
    
    // Use input file descriptor for reading
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    
    // Use output file for writing
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);

    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        // Add easy handle to multi handle
        CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
        
        int running_handles = 0;
        if (add_res == CURLM_OK) {
            // Perform multi handle operations
            curl_multi_perform(multi_handle, &running_handles);
            
            // Remove handle from multi
            curl_multi_remove_handle(multi_handle, easy_handle);
        }
        
        // Clean up multi handle
        curl_multi_cleanup(multi_handle);
    }

    // Test curl_unescape with part of input data
    if (size > 0) {
        size_t unescape_size = size > 50 ? 50 : size;
        char *to_unescape = (char *)malloc(unescape_size + 1);
        if (to_unescape) {
            memcpy(to_unescape, data, unescape_size);
            to_unescape[unescape_size] = '\0';
            
            char *unescaped = curl_unescape(to_unescape, unescape_size);
            if (unescaped) {
                // Test curl_strequal
                int equal_result = curl_strequal(to_unescape, unescaped);
                (void)equal_result; // Use result to avoid unused variable warning
                
                // Free unescaped string
                curl_free(unescaped);
            }
            free(to_unescape);
        }
    }

    // Reset easy handle
    curl_easy_reset(easy_handle);

    // Clean up mime
    curl_mime_free(mime);

    // Clean up duplicate handle if created
    if (dup_handle) {
        curl_easy_cleanup(dup_handle);
    }

    // Clean up easy handle
    curl_easy_cleanup(easy_handle);

    // Close files
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;

    // Global cleanup
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
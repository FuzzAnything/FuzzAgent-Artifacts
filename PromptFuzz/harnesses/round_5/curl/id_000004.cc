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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 13
//<Quality> {"density":21,"unique_branches":{"curl_multi_notify_disable":[[4069,6,4069,24,0,0,4,1],[83,5,83,8,1,0,4,0],[83,12,83,43,1,0,4,0]],"Curl_client_read":[[1191,6,1191,29,0,0,4,1],[1197,6,1197,31,0,0,4,1]],"file_upload":[[310,6,310,34,0,0,4,1],[337,8,337,18,0,0,4,0]],"cr_in_read":[[663,6,663,25,0,0,4,1],[675,3,675,9,0,0,4,0],[676,8,676,29,0,0,4,1],[714,3,714,10,0,0,4,1],[726,8,726,27,0,0,4,1]],"Curl_mntfy_disable":[[158,6,158,34,0,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_notify_disable","curl_multi_perform","curl_multi_strerror","curl_multi_info_read","curl_easy_escape","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_notify_disable","curl_multi_perform","curl_multi_strerror","curl_multi_info_read","curl_easy_escape","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":2}
/**/


// Include necessary curl headers

// Helper function for curl_mvsprintf
int  id_000004_my_vsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return ret;
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
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
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used

    // Set up easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);

    // Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    if (add_res != CURLM_OK) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Disable notifications (just to use the API)
    curl_multi_notify_disable(multi_handle, 0);

    // Perform multi operations
    int running_handles = 0;
    CURLMcode perform_res = curl_multi_perform(multi_handle, &running_handles);
    
    // Get multi error string
    const char *error_str = curl_multi_strerror(perform_res);
    
    // Read messages from multi handle
    int msgs_in_queue = 0;
    CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    
    // Use curl_easy_escape on part of the input data
    // Ensure we don't exceed input bounds
    size_t escape_len = size > 100 ? 100 : size;
    char *escaped = curl_easy_escape(easy_handle, (const char *)data, (int)escape_len);
    
    // Use curl_mvsprintf via helper to format a message
    char buffer[512];
    if (escaped) {
         id_000004_my_vsprintf(buffer, "Multi perform result: %s, Escaped data: %s", 
                   error_str ? error_str : "Unknown", escaped);
        // Write the formatted string to output file
        fwrite(buffer, 1, strlen(buffer), out_file);
        curl_free(escaped);
    }

    // Clean up
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove handle from multi
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    // Cleanup handles
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
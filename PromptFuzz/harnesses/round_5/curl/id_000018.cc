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
//<ID> 335
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"curl_easy_strerror":[[47,3,47,27,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_easy_perform","curl_easy_strerror","curl_strnequal","curl_mprintf","curl_msprintf","curl_msnprintf","curl_multi_info_read","curl_easy_strerror","curl_mprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_easy_perform","curl_easy_strerror","curl_strnequal","curl_mprintf","curl_msprintf","curl_msnprintf","curl_multi_info_read","curl_easy_strerror","curl_mprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE* for writing output
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
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
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
        curl_easy_cleanup(curl);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set up easy handle options
    // Use the input file as URL data source
    curl_easy_setopt(curl, CURLOPT_URL, "file://input_file");
    
    // Set up write function to output file
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out_file);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, curl);
    
    // Perform the transfer using multi interface
    int still_running = 1;
    while (still_running) {
        CURLMcode mc = curl_multi_perform(multi_handle, &still_running);
        if (!still_running) break;
        
        // Wait for activity
        mc = curl_multi_wait(multi_handle, NULL, 0, 1000, NULL);
    }
    
    // Perform with easy interface (for curl_easy_perform API)
    CURLcode res = curl_easy_perform(curl);
    
    // Get error string for the result
    const char *error_str = curl_easy_strerror(res);
    
    // Use curl_strnequal to compare error strings
    const char *expected_error = "No error";
    int equal_result = curl_strnequal(error_str, expected_error, strlen(expected_error));
    
    // Use curl_mprintf to print to stdout
    curl_mprintf("Transfer completed with result: %d\n", res);
    
    // Use curl_msprintf to format into buffer
    char buffer1[256];
    curl_msprintf(buffer1, "Error string: %s", error_str);
    
    // Use curl_msnprintf with limited length
    char buffer2[256];
    curl_msnprintf(buffer2, sizeof(buffer2), "String equal: %d", equal_result);
    
    // Read messages from multi handle
    int msgs_in_queue = 0;
    CURLMsg *msg = NULL;
    while ((msg = curl_multi_info_read(multi_handle, &msgs_in_queue))) {
        if (msg->msg == CURLMSG_DONE) {
            // Process completed transfer
            const char *msg_error_str = curl_easy_strerror(msg->data.result);
            curl_mprintf("Message result: %s\n", msg_error_str);
        }
    }
    
    // Clean up
    curl_multi_remove_handle(multi_handle, curl);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(curl);
    
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove output file
    remove("output_file");
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
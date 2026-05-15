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
//<ID> 336
//<Prompt> []
/*<Combination>: [
*/
//<score> 132, nr_unique_branch: 34
//<Quality> {"density":24,"unique_branches":{"Curl_infof":[[319,4,319,10,2,0,4,0],[319,14,319,33,2,0,4,0],[320,5,320,24,2,0,4,0]],"curl_multi_add_handle":[[327,4,327,40,16,0,4,1],[319,14,319,33,17,0,4,0],[320,5,320,24,17,0,4,0]],"multi_warn_debug":[[319,14,319,33,4,0,4,0],[320,5,320,24,4,0,4,0],[319,14,319,33,5,0,4,0],[320,5,320,24,5,0,4,0]],"Curl_failf":[[180,6,180,23,0,0,4,0],[187,8,187,29,0,0,4,1]],"curl_multi_remove_handle":[[327,4,327,40,21,0,4,1],[319,14,319,33,22,0,4,0],[320,5,320,24,22,0,4,0]],"trc_end_buf":[[113,6,113,39,0,0,4,1],[113,24,113,29,0,0,4,0],[120,11,120,16,0,0,4,0]],"trc_infof":[[251,6,251,10,0,0,4,1],[253,6,253,12,0,0,4,1],[90,3,90,51,1,0,4,1],[319,4,319,10,9,0,4,0],[319,14,319,33,9,0,4,0],[320,5,320,24,9,0,4,0]],"setopt_long_bool":[[478,3,478,27,0,0,4,0]],"trc_write":[[51,6,51,23,0,0,4,0],[52,8,52,24,0,0,4,1],[63,7,63,25,0,0,4,0],[64,7,64,31,0,0,4,1],[65,7,65,30,0,0,4,1],[69,7,69,14,0,0,4,1]],"setopt_long_http":[[1073,3,1073,30,0,0,4,0],[1074,8,1074,30,0,0,4,1],[1114,3,1114,10,0,0,4,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_perform","curl_easy_strerror","curl_strnequal","curl_mprintf","curl_msprintf","curl_msnprintf","curl_multi_info_read","curl_easy_strerror","curl_msnprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_perform","curl_easy_strerror","curl_strnequal","curl_mprintf","curl_msprintf","curl_msnprintf","curl_multi_info_read","curl_easy_strerror","curl_msnprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Custom callback to read from our memory buffer
static size_t  id_000019_read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    size_t *offset = (size_t *)userdata;
    size_t total_size = size * nitems;
    
    // We'll simulate reading from a fixed buffer
    static const char *test_data = "test data";
    size_t test_len = strlen(test_data);
    
    if (*offset < test_len) {
        size_t to_copy = (total_size < (test_len - *offset)) ? total_size : (test_len - *offset);
        memcpy(buffer, test_data + *offset, to_copy);
        *offset += to_copy;
        return to_copy;
    }
    return 0; // EOF
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    CURL *curl = nullptr;
    CURLM *multi_handle = nullptr;
    int msgs_in_queue = 0;
    char buffer[256];
    int result = 0;
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    
    // Use the input data to set various options
    // We'll interpret the first byte as a simple option selector
    uint8_t option_selector = data[0] % 4;
    
    // Set some easy options based on input
    switch (option_selector) {
        case 0:
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            break;
        case 1:
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            break;
        case 2:
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            break;
        case 3:
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
            break;
    }
    
    // Set a dummy URL (won't actually connect in fuzzing)
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/");
    
    // Set up read callback with offset
    size_t read_offset = 0;
    curl_easy_setopt(curl, CURLOPT_READFUNCTION,  id_000019_read_callback);
    curl_easy_setopt(curl, CURLOPT_READDATA, &read_offset);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, curl);
    
    // Perform the transfer (this will fail since we're not actually connecting,
    // but we want to exercise the API)
    CURLcode perform_result = curl_easy_perform(curl);
    
    // Get error string
    const char *error_str = curl_easy_strerror(perform_result);
    
    // Use curl_strnequal to compare error strings
    int str_equal = curl_strnequal(error_str, "No error", 8);
    
    // Use curl_mprintf family to format strings
    result = curl_mprintf("CURL result: %s\n", error_str);
    
    // Use curl_msprintf
    result = curl_msprintf(buffer, "Error code: %d", perform_result);
    
    // Use curl_msnprintf with size limit
    size_t max_len = sizeof(buffer) < size ? sizeof(buffer) : size;
    if (max_len > 0) {
        // Use input data as format string (safely limited)
        result = curl_msnprintf(buffer, max_len, "%.*s", (int)(max_len - 1), data);
    }
    
    // Try to read messages from multi handle
    CURLMsg *msg = nullptr;
    do {
        msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        if (msg && msg->msg == CURLMSG_DONE) {
            // Format message info
            const char *msg_error = curl_easy_strerror(msg->data.result);
            curl_msnprintf(buffer, sizeof(buffer), "Multi msg: %s", msg_error);
        }
    } while (msg);
    
    // Clean up
    curl_multi_remove_handle(multi_handle, curl);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}
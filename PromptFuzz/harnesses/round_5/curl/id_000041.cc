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
//<ID> 807
//<Prompt> []
/*<Combination>: [
*/
//<score> 138, nr_unique_branch: 10
//<Quality> {"density":23,"unique_branches":{"ipv6_parse":[[413,10,413,30,0,0,4,0],[413,34,413,38,0,0,4,0],[413,42,413,55,0,0,4,0]],"Curl_cw_out_is_paused":[[464,6,464,13,0,0,4,0]],"curl_easy_pause":[[1154,6,1154,38,0,0,4,0]],"Curl_xfer_pause_send":[[894,6,894,13,0,0,4,0],[894,6,894,13,0,0,4,1],[894,17,894,45,0,0,4,1]],"Curl_pgrsSendPause":[[237,6,237,13,0,0,4,0],[237,6,237,13,0,0,4,1]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_set","curl_easy_init","curl_url_cleanup","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_multi_add_handle","curl_maprintf","curl_free","curl_multi_poll","curl_multi_strerror","curl_multi_socket_action","curl_easy_pause","curl_easy_perform","curl_easy_strerror","curl_strnequal","curl_easy_pause","curl_easy_perform","curl_url_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_easy_init","curl_multi_init","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_multi_add_handle","curl_maprintf","curl_free","curl_multi_poll","curl_multi_strerror","curl_multi_socket_action","curl_easy_pause","curl_easy_perform","curl_easy_strerror","curl_strnequal","curl_easy_pause","curl_easy_perform","curl_url_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize global curl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // 2. Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Parse URL from input data (assuming first part of data contains URL)
    // Find first null terminator or use size as limit
    size_t url_len = 0;
    while (url_len < size && data[url_len] != 0) {
        url_len++;
    }
    
    if (url_len > 0) {
        char *url_str = new char[url_len + 1];
        memcpy(url_str, data, url_len);
        url_str[url_len] = '\0';
        
        // Set URL part
        curl_url_set(url_handle, CURLUPART_URL, url_str, 0);
        delete[] url_str;
    }

    // 4. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // 5. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // 6. Set options for easy handle
    // Get URL from CURLU handle
    char *url_part = nullptr;
    if (curl_url_get(url_handle, CURLUPART_URL, &url_part, 0) == CURLUE_OK) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, url_part);
        curl_free(url_part);
    }

    // Set other options
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    
    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor for reading
        int fd = fuzz_fileno(in_file);
        
        // 7. Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // 8. Use curl_maprintf for formatting
        char *formatted_str = curl_maprintf("Fuzzing with size: %zu", size);
        if (formatted_str) {
            curl_free(formatted_str);
        }
        
        // 9. Multi poll (non-blocking)
        int ret = 0;
        curl_multi_poll(multi_handle, nullptr, 0, 1000, &ret);
        
        // 10. Get error string for multi (just for demonstration)
        const char *multi_error = curl_multi_strerror(CURLM_OK);
        
        // 11. Socket action (with timeout socket)
        int running_handles = 0;
        curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, 0, &running_handles);
        
        // 12. Pause easy handle
        curl_easy_pause(easy_handle, CURLPAUSE_ALL);
        
        // 13. First easy perform (should fail due to pause)
        CURLcode easy_result = curl_easy_perform(easy_handle);
        
        // 14. Get easy error string
        const char *easy_error = curl_easy_strerror(easy_result);
        
        // 15. String comparison
        const char *s1 = "test";
        const char *s2 = "test";
        int str_equal = curl_strnequal(s1, s2, 4);
        
        // Clean up file
        assert_file_closed(&in_file);;
        
        // Second easy perform (unpaused)
        curl_easy_pause(easy_handle, CURLPAUSE_CONT);
        curl_easy_perform(easy_handle);
    }

    // 16. Clean up URL handle
    curl_url_cleanup(url_handle);
    
    // 17. Clean up multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // 18. Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
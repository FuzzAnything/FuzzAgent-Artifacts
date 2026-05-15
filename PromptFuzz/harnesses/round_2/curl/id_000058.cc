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
//<ID> 1083
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 6
//<Quality> {"density":11,"unique_branches":{"curl_url_dup":[[1304,8,1304,19,3,0,4,0],[1306,10,1306,23,3,0,4,1],[1304,8,1304,19,4,0,4,0],[1306,10,1306,23,4,0,4,1],[1304,8,1304,19,7,0,4,0],[1306,10,1306,23,7,0,4,1]]},"library_calls":["curl_global_trace","curl_version_info","curl_url","curl_url_set","curl_url_dup","curl_multi_init","curl_url_cleanup","curl_url_cleanup","curl_easy_init","curl_multi_cleanup","curl_url_cleanup","curl_url_cleanup","curl_multi_add_handle","curl_easy_option_next","curl_easy_option_next","curl_ws_start_frame","curl_easy_header","curl_easy_nextheader","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_trace","curl_version_info","curl_url","curl_url_set","curl_url_dup","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_easy_option_next","curl_easy_option_next","curl_ws_start_frame","curl_easy_header","curl_easy_nextheader","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize curl globally with tracing enabled using input data as config
    if (size > 0) {
        // Use first byte as trace configuration flag (simplified)
        char config[2] = {(char)data[0], '\0'};
        curl_global_trace(config);
    }
    
    // 2. Get curl version information
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    if (!version_info) {
        return 0;
    }
    
    // 3. Create a base URL from input data
    CURLU *base_url = curl_url();
    if (!base_url) {
        return 0;
    }
    
    // Try to parse input data as URL (simplified - use first valid part)
    size_t url_len = 0;
    while (url_len < size && data[url_len] != 0 && url_len < 1024) {
        url_len++;
    }
    
    if (url_len > 0) {
        char *url_str = (char*)malloc(url_len + 1);
        if (url_str) {
            memcpy(url_str, data, url_len);
            url_str[url_len] = '\0';
            curl_url_set(base_url, CURLUPART_URL, url_str, 0);
            free(url_str);
        }
    }
    
    // 4. Duplicate the URL
    CURLU *dup_url = curl_url_dup(base_url);
    
    // 5. Create a multi handle for WebSocket operations
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        if (dup_url) curl_url_cleanup(dup_url);
        curl_url_cleanup(base_url);
        return 0;
    }
    
    // 6. Create an easy handle for WebSocket
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi_handle);
        if (dup_url) curl_url_cleanup(dup_url);
        curl_url_cleanup(base_url);
        return 0;
    }
    
    // 7. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy);
    
    // 8. Iterate through easy options (using curl_easy_option_next)
    const struct curl_easyoption *option = curl_easy_option_next(NULL);
    int option_count = 0;
    while (option && option_count < 10) { // Limit to 10 options for performance
        option = curl_easy_option_next(option);
        option_count++;
    }
    
    // 9. Start WebSocket frame using remaining input data
    size_t remaining_data = (size > url_len + 1) ? size - (url_len + 1) : 0;
    const uint8_t *ws_data = (remaining_data > 0) ? data + url_len + 1 : data;
    
    if (remaining_data > 0) {
        // Set up WebSocket frame with input data
        unsigned int flags = CURLWS_BINARY;
        if (remaining_data > 1) {
            flags |= (ws_data[0] & 0x0F); // Use first byte for frame flags
        }
        
        curl_off_t frame_len = (curl_off_t)(remaining_data > 1024 ? 1024 : remaining_data);
        curl_ws_start_frame(easy, flags, frame_len);
        
        // 10. Get headers from easy handle
        struct curl_header *header = NULL;
        unsigned int origin = CURLH_HEADER;
        int request = 0;
        
        // Get first header
        curl_easy_header(easy, "Test-Header", 0, origin, request, &header);
        
        // Get next headers using curl_easy_nextheader
        if (header) {
            struct curl_header *next_header = curl_easy_nextheader(easy, origin, request, header);
            (void)next_header; // Use to avoid unused variable warning
        }
    }
    
    // 11. Perform multi socket operations
    int running_handles = 0;
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // 12. Clean up all resources
    curl_multi_remove_handle(multi_handle, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi_handle);
    
    if (dup_url) {
        curl_url_cleanup(dup_url);
    }
    curl_url_cleanup(base_url);
    
    // 13. Global cleanup
    curl_global_cleanup();
    
    return 0;
}
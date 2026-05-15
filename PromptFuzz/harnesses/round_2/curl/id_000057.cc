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
//<ID> 1081
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 6
//<Quality> {"density":7,"unique_branches":{"curl_url_dup":[[1304,8,1304,19,11,0,4,0],[1306,10,1306,23,11,0,4,1]],"curl_easy_option_next":[[70,8,70,18,0,0,4,1]],"curl_url_set":[[1969,10,1969,12,0,0,4,0],[1969,16,1969,44,0,0,4,1],[1974,12,1974,14,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_version_info","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_easy_option_next","curl_easy_setopt","curl_slist_append","curl_easy_setopt","curl_easy_nextheader","curl_ws_start_frame","curl_slist_free_all","curl_multi_init","curl_multi_add_handle","curl_multi_socket_all","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version_info","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_easy_option_next","curl_easy_setopt","curl_slist_append","curl_easy_setopt","curl_easy_nextheader","curl_ws_start_frame","curl_slist_free_all","curl_multi_init","curl_multi_add_handle","curl_multi_socket_all","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize curl globally first
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 1. Use curl_global_trace with config from input data
    // Ensure null-terminated string for config
    char *trace_config = nullptr;
    if (size > 0) {
        size_t config_len = size > 1024 ? 1024 : size;
        trace_config = (char *)malloc(config_len + 1);
        if (trace_config) {
            memcpy(trace_config, data, config_len);
            trace_config[config_len] = '\0';
            curl_global_trace(trace_config);
            free(trace_config);
        }
    }
    
    // 2. Get version info
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    
    // 3. Create a URL handle and duplicate it
    CURLU *url_handle = curl_url();
    CURLU *url_dup = nullptr;
    if (url_handle) {
        // Use part of input data as URL component (ensure null-terminated)
        size_t url_len = size > 512 ? 512 : size;
        char *url_part = (char *)malloc(url_len + 1);
        if (url_part) {
            memcpy(url_part, data, url_len);
            url_part[url_len] = '\0';
            // Set URL part (e.g., host)
            curl_url_set(url_handle, CURLUPART_HOST, url_part, 0);
            free(url_part);
        }
        url_dup = curl_url_dup(url_handle);
    }
    
    // 4. Create easy handle and set up WebSocket
    CURL *easy_handle = curl_easy_init();
    struct curl_header *header = nullptr;
    
    if (easy_handle) {
        // Set some options using curl_easy_option_next
        const struct curl_easyoption *option = nullptr;
        while ((option = curl_easy_option_next(option)) != nullptr) {
            // Just iterate through options - we could set some but keep simple
            if (option->id == CURLOPT_URL) {
                curl_easy_setopt(easy_handle, CURLOPT_URL, "ws://localhost:8080");
            }
        }
        
        // Add a custom header for testing
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "X-Fuzz-Test: true");
        curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
        
        // 5. Get headers using curl_easy_nextheader
        // Note: This typically requires a performed request, but we'll attempt
        header = curl_easy_nextheader(easy_handle, CURLH_HEADER, 0, nullptr);
        
        // 6. Start WebSocket frame using input data length
        // Convert size to curl_off_t for frame length
        curl_off_t frame_len = (curl_off_t)(size % 65536); // Reasonable frame size
        curl_ws_start_frame(easy_handle, CURLWS_BINARY, frame_len);
        
        // Clean up headers
        curl_slist_free_all(headers);
    }
    
    // 7. Create multi handle and use curl_multi_socket_all
    CURLM *multi_handle = curl_multi_init();
    int running_handles = 0;
    
    if (multi_handle && easy_handle) {
        curl_multi_add_handle(multi_handle, easy_handle);
        curl_multi_socket_all(multi_handle, &running_handles);
        
        // Clean up multi handle
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_cleanup(multi_handle);
    }
    
    // 8. Create FILE* from input data for potential reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Read some bytes to demonstrate usage
        int fd = fuzz_fileno(in_file);
        char buf[1024];
        size_t bytes_read = fread(buf, 1, sizeof(buf), in_file);
        assert_file_closed(&in_file);;
    }
    
    // 9. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write version info to demonstrate usage
        if (version_info) {
            fprintf(out_file, "Curl version: %s\n", version_info->version);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up all resources
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    if (url_dup) {
        curl_url_cleanup(url_dup);
    }
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
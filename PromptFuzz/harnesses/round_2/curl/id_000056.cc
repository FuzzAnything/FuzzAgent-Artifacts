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
//<ID> 1080
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"trc_apply_level_by_name":[[600,8,600,59,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_version_info","curl_global_cleanup","curl_url","curl_global_cleanup","curl_url_set","curl_url_dup","curl_easy_init","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_ws_start_frame","curl_easy_option_next","curl_easy_option_next","curl_multi_add_handle","curl_multi_socket_all","curl_easy_nextheader","curl_easy_nextheader","curl_slist_free_all","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version_info","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_multi_init","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_ws_start_frame","curl_easy_option_next","curl_easy_option_next","curl_multi_add_handle","curl_multi_socket_all","curl_easy_nextheader","curl_easy_nextheader","curl_slist_free_all","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include curl headers

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // 1. Use curl_global_trace with input data as config string
    // Create null-terminated string from input data
    char *trace_config = (char *)malloc(size + 1);
    if (!trace_config) {
	return 0;
	}
	
    memcpy(trace_config, data, size);
    trace_config[size] = '\0';
    
    // Call curl_global_trace with the config
    CURLcode trace_result = curl_global_trace(trace_config);
    free(trace_config);
    
    // 2. Get version info
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    if (!version_info) {
        curl_global_cleanup();
        return 0;
    }
    
    // 3. Create and duplicate URL
    CURLU *url = curl_url();
    if (!url) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set a dummy URL part first
    curl_url_set(url, CURLUPART_URL, "http://example.com", 0);
    
    // Duplicate the URL
    CURLU *dup_url = curl_url_dup(url);
    
    // 4. Create easy handle for WebSocket operations
    CURL *easy = curl_easy_init();
    if (!easy) {
        if (dup_url) curl_url_cleanup(dup_url);
        curl_url_cleanup(url);
        curl_global_cleanup();
        return 0;
    }
    
    // 5. Set up a multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        if (dup_url) curl_url_cleanup(dup_url);
        curl_url_cleanup(url);
        curl_global_cleanup();
        return 0;
    }
    
    // 6. Add headers to iterate through them later
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Header1: Value1");
    headers = curl_slist_append(headers, "Header2: Value2");
    curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers);
    
    // 7. Start a WebSocket frame using input data for flags and frame length
    // Use the first 8 bytes for frame_len (if available) and next 4 for flags
    curl_off_t frame_len = 0;
    unsigned int flags = 0;
    
    if (size >= sizeof(curl_off_t)) {
        memcpy(&frame_len, data, sizeof(curl_off_t));
    } else {
        frame_len = (curl_off_t)size;
    }
    
    if (size >= sizeof(curl_off_t) + sizeof(unsigned int)) {
        memcpy(&flags, data + sizeof(curl_off_t), sizeof(unsigned int));
    }
    
    // Limit frame length to prevent excessive memory allocation
    if (frame_len > 65536) frame_len = 65536;
    
    // Start WebSocket frame
    CURLcode ws_result = curl_ws_start_frame(easy, flags, frame_len);
    
    // 8. Iterate through easy options using curl_easy_option_next
    const struct curl_easyoption *option = curl_easy_option_next(NULL);
    int option_count = 0;
    while (option && option_count < 10) { // Limit to 10 options to avoid infinite loops
        option = curl_easy_option_next(option);
        option_count++;
    }
    
    // 9. Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // 10. Call curl_multi_socket_all
    int running_handles = 0;
    CURLMcode multi_result = curl_multi_socket_all(multi, &running_handles);
    
    // 11. Iterate through headers using curl_easy_nextheader
    struct curl_header *header = NULL;
    header = curl_easy_nextheader(easy, CURLH_HEADER, 0, header);
    while (header) {
        header = curl_easy_nextheader(easy, CURLH_HEADER, 0, header);
    }
    
    // 12. Create FILE* from input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        assert_file_closed(&in_file);;
    }
    
    // 13. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some data to output file
        if (size > 0) {
            fwrite(data, 1, size < 100 ? size : 100, out_file);
        }
        assert_file_closed(&out_file);;
    }
    
    // Clean up in reverse order
    if (headers) curl_slist_free_all(headers);
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    if (dup_url) curl_url_cleanup(dup_url);
    curl_url_cleanup(url);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
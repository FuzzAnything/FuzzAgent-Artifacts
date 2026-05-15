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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 52.5, nr_unique_branch: 23
//<Quality> {"density":21,"unique_branches":{"setopt_long_http":[[1073,3,1073,30,0,0,4,1],[1114,3,1114,10,0,0,4,0]],"curl_ws_start_frame":[[1879,6,1879,27,0,0,4,0]],"setopt_long_proxy":[[1036,3,1036,25,0,0,4,1],[1041,3,1041,25,0,0,4,1],[1043,3,1043,25,0,0,4,1],[1048,3,1048,27,0,0,4,1],[1053,3,1053,10,0,0,4,0]],"setopt_long_proto":[[1134,3,1134,28,0,0,4,1],[1141,3,1141,21,0,0,4,1],[1149,3,1149,30,0,0,4,1],[1155,3,1155,27,0,0,4,1],[1161,3,1161,26,0,0,4,1],[1167,3,1167,32,0,0,4,1],[1171,3,1171,39,0,0,4,1],[1177,3,1177,30,0,0,4,1],[1185,3,1185,28,0,0,4,1],[1187,3,1187,32,0,0,4,1],[1192,3,1192,32,0,0,4,1],[1209,3,1209,25,0,0,4,1],[1212,3,1212,31,0,0,4,1],[1216,3,1216,26,0,0,4,0],[1221,3,1221,10,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_trace","curl_global_sslset","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_easy_escape","curl_easy_setopt","curl_easy_setopt","curl_ws_start_frame","curl_slist_append","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_multi_socket_all","curl_easy_perform","curl_easy_header","curl_easy_nextheader","curl_free","curl_slist_free_all","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_global_sslset","curl_easy_init","curl_multi_init","curl_multi_add_handle","curl_easy_escape","curl_easy_setopt","curl_easy_setopt","curl_ws_start_frame","curl_slist_append","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_multi_socket_all","curl_easy_perform","curl_easy_header","curl_easy_nextheader","curl_free","curl_slist_free_all","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create FILE* for output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Set global trace configuration using input data
    if (size > 10) {
        char trace_config[64];
        size_t trace_len = size > 63 ? 63 : size;
        memcpy(trace_config, data, trace_len);
        trace_config[trace_len] = '\0';
        curl_global_trace(trace_config);
    }
    
    // Set SSL backend
    curl_sslbackend id = CURLSSLBACKEND_NONE;
    const char *name = "openssl";
    const curl_ssl_backend ***avail = NULL;
    curl_global_sslset(id, name, avail);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
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
	return 0;
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, curl);
    
    // Prepare string for escaping - use first part of input
    char *escaped_string = NULL;
    if (size > 20) {
        char string_to_escape[128];
        size_t escape_len = size > 127 ? 127 : size;
        memcpy(string_to_escape, data, escape_len);
        string_to_escape[escape_len] = '\0';
        
        // Escape the string
        escaped_string = curl_easy_escape(curl, string_to_escape, (int)escape_len);
        
        // Use escaped string for URL
        if (escaped_string) {
            char url[256];
            snprintf(url, sizeof(url), "ws://example.com/%s", escaped_string);
            curl_easy_setopt(curl, CURLOPT_URL, url);
        }
    }
    
    // Set WebSocket options
    curl_easy_setopt(curl, CURLOPT_WS_OPTIONS, CURLWS_RAW_MODE);
    
    // Start WebSocket frame using input data
    if (size > 50) {
        // Use part of input as frame length
        curl_off_t frame_len = 0;
        if (size >= sizeof(frame_len)) {
            memcpy(&frame_len, data + 10, sizeof(frame_len));
            frame_len = frame_len % 1000; // Limit size
        }
        
        // Start frame with flags from input
        unsigned int flags = CURLWS_TEXT;
        if (size > 20) flags = data[20] % 8;
        
        curl_ws_start_frame(curl, flags, frame_len);
    }
    
    // Add some headers to have something to iterate through
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "X-Custom-Header: Value1");
    headers = curl_slist_append(headers, "X-Custom-Header: Value2");
    if (size > 30) {
        char header_buf[128];
        size_t header_len = size > 100 ? 100 : size - 30;
        snprintf(header_buf, sizeof(header_buf), "X-Data-Header: %.*s", 
                (int)header_len, (const char*)(data + 30));
        headers = curl_slist_append(headers, header_buf);
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    // Perform the easy handle through multi interface
    int running_handles = 0;
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // Try to perform easy handle directly as well
    curl_easy_perform(curl);
    
    // Get file descriptor for output file
    int out_fd = fuzz_fileno(out_file);
    (void)out_fd; // Use in some way
    
    // Process headers
    struct curl_header *header = NULL;
    unsigned int origin = CURLH_HEADER;
    int request = 0;
    
    // Get first header
    curl_easy_header(curl, "X-Custom-Header", 0, origin, request, &header);
    
    // Iterate through headers
    while ((header = curl_easy_nextheader(curl, origin, request, header)) != NULL) {
        // Process header - in fuzzing we just access it
        if (header->name && header->value) {
            // Write header info to output file
            fprintf(out_file, "%s: %s\n", header->name, header->value);
        }
    }
    
    // Cleanup
    if (escaped_string) curl_free(escaped_string);
    curl_slist_free_all(headers);
    curl_multi_remove_handle(multi_handle, curl);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(curl);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	return 0;
}
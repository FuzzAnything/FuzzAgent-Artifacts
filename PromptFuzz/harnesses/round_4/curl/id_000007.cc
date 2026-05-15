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
//<ID> 42
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"curl_multi_get_offt":[[3852,3,3852,29,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_mime_free","curl_easy_escape","curl_free","curl_multi_add_handle","curl_multi_socket_action","curl_multi_get_offt","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_mime_free","curl_easy_escape","curl_free","curl_multi_add_handle","curl_multi_socket_action","curl_multi_get_offt","curl_ws_recv","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize curl globally with tracing
    curl_global_init(CURL_GLOBAL_ALL);
    // Use a portion of input for trace config (up to 100 bytes or size)
    size_t trace_len = size > 100 ? 100 : size;
    char* trace_config = (char*)malloc(trace_len + 1);
    memcpy(trace_config, data, trace_len);
    trace_config[trace_len] = '\0';
    curl_global_trace(trace_config);
    free(trace_config);
    
    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    // 1. Use curl_mime_init and related APIs
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        // Create a mime part
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use some input data for mime name
            size_t name_len = size > 50 ? 50 : size/2;
            char* mime_name = (char*)malloc(name_len + 1);
            memcpy(mime_name, data, name_len);
            mime_name[name_len] = '\0';
            curl_mime_name(part, mime_name);
            free(mime_name);
            
            // Use some input data for mime data
            size_t mime_data_len = size > 100 ? 100 : size;
            char* mime_data = (char*)malloc(mime_data_len + 1);
            size_t offset = name_len < size - 100 ? name_len : 0;
            size_t copy_len = size - offset > mime_data_len ? mime_data_len : size - offset;
            memcpy(mime_data, data + offset, copy_len);
            mime_data[copy_len] = '\0';
            curl_mime_data(part, mime_data, copy_len);
            free(mime_data);
        }
        curl_mime_free(mime);
    }
    
    // 2. Use curl_easy_escape with input data
    // Use a portion of input for escaping
    size_t escape_len = size > 200 ? 200 : size;
    char* to_escape = (char*)malloc(escape_len + 1);
    size_t escape_offset = size > 300 ? 100 : 0;
    size_t escape_copy = size - escape_offset > escape_len ? escape_len : size - escape_offset;
    memcpy(to_escape, data + escape_offset, escape_copy);
    to_escape[escape_copy] = '\0';
    
    char *escaped = curl_easy_escape(easy, to_escape, escape_copy);
    free(to_escape);
    if (escaped) {
        curl_free(escaped);
    }
    
    // 3. Use curl_multi_add_handle and curl_multi_socket_action
    curl_multi_add_handle(multi, easy);
    
    int running_handles = 0;
    // Simulate socket action with a dummy socket (using input data to determine events)
    curl_socket_t dummy_socket = (curl_socket_t)(data[0] % 256); // Use first byte for socket
    int ev_bitmask = (size > 1) ? (data[1] % 4) : 0; // Use second byte for event mask
    
    curl_multi_socket_action(multi, dummy_socket, ev_bitmask, &running_handles);
    
    // 4. Use curl_multi_get_offt
    curl_off_t offset_value = 0;
    // Use CURLM_INFO_OFFT_RECV (value 5) or other available constant
    // We'll use 5 as an example for CURLM_INFO_OFFT_RECV
    curl_multi_get_offt(multi, (CURLMinfo_offt)5, &offset_value);
    
    // 5. Use curl_ws_recv with input data as buffer
    // Allocate buffer for WebSocket receive
    size_t ws_buf_len = size > 512 ? 512 : size;
    char *ws_buffer = (char*)malloc(ws_buf_len);
    size_t ws_recv_len = 0;
    const struct curl_ws_frame *ws_meta = NULL;
    
    // Try to receive WebSocket data (will likely fail since not in WS mode)
    curl_ws_recv(easy, ws_buffer, ws_buf_len, &ws_recv_len, &ws_meta);
    free(ws_buffer);
    
    // Clean up
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    curl_global_cleanup();
    
    return 0;
}
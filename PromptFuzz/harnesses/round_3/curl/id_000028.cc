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
//<ID> 403
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"multi_runsingle":[[2558,15,2558,46,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_trace","curl_multi_init","curl_easy_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_formadd","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_multi_wakeup","curl_easy_pause","curl_ws_recv","curl_multi_perform","curl_slist_free_all","curl_formfree","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_multi_init","curl_easy_init","curl_formadd","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_multi_wakeup","curl_easy_pause","curl_ws_recv","curl_multi_perform","curl_slist_free_all","curl_formfree","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // 1. Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Setup trace configuration
    // Use first 100 bytes or less as trace config string
    size_t trace_len = size > 100 ? 100 : size;
    char trace_config[101];
    memcpy(trace_config, data, trace_len);
    trace_config[trace_len] = '\0';
    curl_global_trace(trace_config);
    
    // 3. Create multi handle and easy handle
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    
    if (!multi_handle || !easy_handle) {
        if (easy_handle) curl_easy_cleanup(easy_handle);
        if (multi_handle) curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Create form data using curl_formadd
    struct curl_httppost *formpost = nullptr;
    struct curl_httppost *lastptr = nullptr;
    
    // Use data for form field names and contents
    size_t form_data_len = size > 50 ? 50 : size;
    char field_name[51];
    char field_content[51];
    
    memcpy(field_name, data, form_data_len);
    field_name[form_data_len] = '\0';
    
    // Use middle portion for content if enough data
    size_t content_offset = size / 2;
    size_t content_len = (size - content_offset) > 50 ? 50 : (size - content_offset);
    if (content_len > 0) {
        memcpy(field_content, data + content_offset, content_len);
        field_content[content_len] = '\0';
    } else {
        strcpy(field_content, "test");
    }
    
    curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, field_name,
                CURLFORM_COPYCONTENTS, field_content,
                CURLFORM_END);
    
    // 5. Create socket list using curl_slist_append
    struct curl_slist *header_list = nullptr;
    char header[101];
    size_t header_len = size > 100 ? 100 : size;
    memcpy(header, data, header_len);
    header[header_len] = '\0';
    header_list = curl_slist_append(header_list, header);
    
    // 6. Setup WebSocket options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "ws://example.com/ws");
    curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, header_list);
    
    // 7. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // 8. Create a FILE* from input data for WebSocket receive buffer
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        
        // 9. Use curl_multi_socket with the file descriptor
        int running_handles = 0;
        curl_multi_socket(multi_handle, fd, &running_handles);
        
        // 10. Wake up multi handle
        curl_multi_wakeup(multi_handle);
        
        // 11. Pause the easy handle
        curl_easy_pause(easy_handle, CURLPAUSE_RECV | CURLPAUSE_SEND);
        
        // 12. Try WebSocket receive (will likely fail but API is called)
        char ws_buffer[1024];
        size_t bytes_received = 0;
        const struct curl_ws_frame *ws_meta = nullptr;
        
        // Try to receive any available WebSocket data
        curl_ws_recv(easy_handle, ws_buffer, sizeof(ws_buffer), 
                     &bytes_received, &ws_meta);
        
        assert_file_closed(&in_file);;
    }
    
    // 13. Perform multi handle operations briefly
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);
    
    // 14. Cleanup
    curl_slist_free_all(header_list);
    curl_formfree(formpost);
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
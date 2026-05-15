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
//<ID> 41
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["curl_global_trace","curl_easy_init","curl_multi_init","curl_easy_cleanup","curl_mime_init","curl_easy_escape","curl_free","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_multi_add_handle","curl_multi_socket_action","curl_multi_get_offt","curl_ws_recv","curl_mime_free","curl_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup"],"critical_calls":["curl_global_trace","curl_easy_init","curl_multi_init","curl_mime_init","curl_easy_escape","curl_multi_add_handle","curl_multi_socket_action","curl_multi_get_offt","curl_ws_recv","curl_mime_free","curl_free","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup"],"visited":0}
/**/


// Curl headers would be included here
// Using the provided APIs and types

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data

    // 1. Initialize curl with tracing
    const char *trace_config = "http/2,http/3";
    curl_global_trace(fuzz_str_1);
    
    // 2. Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
	return 0;
	}
	
    
    // 3. Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        return 0;
    }
    
    // 4. Initialize mime structure
    curl_mime *mime = curl_mime_init(easy);
    
    // 5. Escape a string using part of input data
    // Use first 100 bytes or size, whichever is smaller
    size_t escape_len = size > 100 ? 100 : size;
    char *escaped = curl_easy_escape(easy, (const char*)data, escape_len);
    
    // 6. Create a file from input data for socket operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        if (escaped) curl_free(escaped);
        if (mime) curl_mime_free(mime);
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Get file descriptor for socket operations
    curl_socket_t sockfd = fuzz_fileno(in_file);
    
    // 8. Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // 9. Perform socket action with file descriptor
    int running_handles = 0;
    // Use a bitmask from data (mask to valid values: 1=in, 2=out, 4=err)
    int ev_bitmask = data[0] & 0x07;
    if (ev_bitmask == 0) ev_bitmask = 1; // Ensure at least input
    
    curl_multi_socket_action(multi, sockfd, ev_bitmask, &running_handles);
    
    // 10. Get offset value from multi handle
    curl_off_t offset_value = 0;
    // CURLMINFO_OFFT_SOCKET would be defined in headers
    // Using 0 as info parameter (socket offset)
    curl_multi_get_offt(multi, (CURLMinfo_offt)0, &offset_value);
    
    // 11. Attempt WebSocket receive (will likely fail but exercise the API)
    char ws_buffer[1024];
    size_t bytes_received = 0;
    const struct curl_ws_frame *ws_meta = NULL;
    
    // Use a small buffer from input
    size_t ws_buf_len = size > 512 ? 512 : size;
    curl_ws_recv(easy, ws_buffer, ws_buf_len, &bytes_received, &ws_meta);
    
    // 12. Clean up resources
    assert_file_closed(&in_file);;
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (escaped) {
        curl_free(escaped);
    }
    
    // Remove handle from multi and cleanup
    curl_multi_remove_handle(multi, easy);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);
    
    assert_file_closed(&in_file);
	assert_fd_closed(sockfd);
	return 0;
}
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
//<ID> 760
//<Prompt> []
/*<Combination>: [
*/
//<score> 28, nr_unique_branch: 3
//<Quality> {"density":14,"unique_branches":{"curl_multi_setopt":[[3255,3,3255,27,0,0,4,0],[3256,27,3256,67,0,0,4,0],[3264,3,3264,28,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_setopt","curl_ws_start_frame","curl_multi_socket_action","curl_multi_socket_all","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_setopt","curl_ws_start_frame","curl_multi_socket_action","curl_multi_socket_all","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Create output FILE*
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Get file descriptor for reading
    int read_fd = fuzz_fileno(in_file);
    
    // 5. Initialize multi handle (curl_multi_init)
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(read_fd);
	return 0;
    }
    
    // 6. Create an easy handle for WebSocket
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(read_fd);
	return 0;
    }
    
    // 7. Set WebSocket URL (must be set before adding to multi handle)
    // Using a dummy WebSocket URL for demonstration
    curl_easy_setopt(easy_handle, CURLOPT_URL, "ws://example.com/ws");
    curl_easy_setopt(easy_handle, CURLOPT_CONNECT_ONLY, 2L); // WebSocket mode
    
    // 8. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // 9. Set multi handle options (curl_multi_setopt)
    // Enable pipelining for demonstration
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_HTTP1 | CURLPIPE_MULTIPLEX);
    
    // 10. Start a WebSocket frame (curl_ws_start_frame)
    // Using data from input as frame content
    // Note: This is just to demonstrate the API usage
    // In real scenario, we would need proper WebSocket connection setup
    if (size > 0) {
        unsigned int flags = CURLWS_BINARY;
        curl_off_t frame_len = (curl_off_t)(size > 1024 ? 1024 : size); // Limit frame size
        curl_ws_start_frame(easy_handle, flags, frame_len);
    }
    
    // 11. Use curl_multi_socket with the file descriptor
    int running_handles = 0;
    curl_socket_t sockfd = (curl_socket_t)read_fd;
    curl_multi_socket(multi_handle, sockfd, &running_handles);
    
    // 12. Use curl_multi_socket_all
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // 13. Try to perform the transfers
    int still_running = 0;
    do {
        curl_multi_perform(multi_handle, &still_running);
    } while(still_running);
    
    // 14. Cleanup in reverse order
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // 15. Close files
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // 16. Global cleanup
    curl_global_cleanup();
    
    // 17. Remove output file if created
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(read_fd);
	return 0;
}
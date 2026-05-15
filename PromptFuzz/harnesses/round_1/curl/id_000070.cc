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
//<ID> 1407
//<Prompt> []
/*<Combination>: [
*/
//<score> 52, nr_unique_branch: 7
//<Quality> {"density":13,"unique_branches":{"formatf":[[1003,13,1003,33,0,0,4,0]],"parsefmt":[[252,14,252,25,0,0,4,1],[272,16,272,22,0,0,4,1],[274,16,274,53,0,0,4,1],[277,16,277,22,0,0,4,1]],"curl_ws_recv":[[1547,8,1547,31,0,0,4,1],[1553,8,1553,13,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_escape","curl_msnprintf","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_assign","curl_multi_waitfds","curl_ws_recv","curl_ws_meta","curl_multi_remove_handle","curl_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_escape","curl_msnprintf","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_assign","curl_multi_waitfds","curl_ws_recv","curl_ws_meta","curl_multi_remove_handle","curl_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_70(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);

    // 2. Create easy handle for WebSocket
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Create multi handle for concurrent operations
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }

    // 4. Escape a portion of input data
    size_t escape_len = size > 100 ? 100 : size;
    char *escaped_str = curl_escape((const char*)data, escape_len);
    
    // 5. Use formatted printing with input data
    char format_buffer[256];
    int format_len = size > 200 ? 200 : size;
    int printed = curl_msnprintf(format_buffer, sizeof(format_buffer), 
                                 "Input size: %zu, First bytes: %.20s", 
                                 size, escaped_str ? escaped_str : "");

    // 6. Set WebSocket options on easy handle
    curl_easy_setopt(easy, CURLOPT_URL, "ws://dummy-server.local/");
    curl_easy_setopt(easy, CURLOPT_CONNECT_ONLY, 2L); // WebSocket mode

    // 7. Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);

    // 8. Setup file descriptors for waiting
    struct curl_waitfd ufds[2];
    unsigned int fd_count = 0;
    
    // Create input FILE from fuzzing data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        ufds[0].fd = fd;
        ufds[0].events = CURL_WAIT_POLLIN;
        ufds[0].revents = 0;
        fd_count++;
        
        // 9. Assign custom data to socket in multi handle
        curl_multi_assign(multi, fd, (void*)"fuzzing_input");
    }

    // 10. Wait for file descriptors
    curl_multi_waitfds(multi, ufds, fd_count, &fd_count);

    // 11. Try to receive WebSocket data
    uint8_t ws_buffer[1024];
    size_t ws_recv_len = 0;
    const struct curl_ws_frame *meta = NULL;
    
    if (size > 0) {
        CURLcode recv_res = curl_ws_recv(easy, ws_buffer, 
                                        sizeof(ws_buffer), 
                                        &ws_recv_len, &meta);
        
        // 12. Get WebSocket metadata
        if (meta) {
            const struct curl_ws_frame *frame_meta = curl_ws_meta(easy);
            (void)frame_meta; // Use variable to avoid unused warning
        }
    }

    // 13. Remove easy handle from multi
    curl_multi_remove_handle(multi, easy);

    // 14. Cleanup allocated resources
    if (escaped_str) {
        curl_free(escaped_str);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    // 15. Final cleanup
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	return 0;
}
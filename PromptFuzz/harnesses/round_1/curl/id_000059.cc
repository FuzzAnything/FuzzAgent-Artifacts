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
//<ID> 1221
//<Prompt> []
/*<Combination>: [
*/
//<score> 240, nr_unique_branch: 28
//<Quality> {"density":30,"unique_branches":{"Curl_creader_is_paused":[[1451,9,1451,15,0,0,4,1]],"curl_easy_pause":[[1142,33,1142,44,0,0,4,1],[1146,6,1146,31,0,0,4,1],[1154,6,1154,38,0,0,4,1],[1155,6,1155,55,0,0,4,1],[1161,6,1161,36,0,0,4,0],[1168,6,1168,33,0,0,4,0],[1171,8,1171,19,0,0,4,0],[1174,10,1174,17,0,0,4,0],[1175,12,1175,42,0,0,4,1],[1181,6,1181,13,0,0,4,0],[1181,17,1181,24,0,0,4,0],[1181,28,1181,45,0,0,4,0],[1181,49,1181,60,0,0,4,0],[1183,8,1183,52,0,0,4,1],[1186,6,1186,15,0,0,4,1]],"Curl_conn_cf_cntrl":[[922,8,922,22,0,0,4,0],[922,26,922,32,0,0,4,1]],"Curl_xfer_pause_recv":[[904,6,904,13,0,0,4,0],[904,6,904,13,0,0,4,1],[904,17,904,45,0,0,4,1]],"Curl_rlimit_block":[[280,6,280,30,0,0,4,1],[285,6,285,17,0,0,4,0],[285,6,285,17,0,0,4,1]],"Curl_pgrsRecvPause":[[229,6,229,13,0,0,4,0],[229,6,229,13,0,0,4,1]],"Curl_xfer_is_blocked":[[872,6,872,16,0,0,4,0],[873,12,873,21,0,0,4,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_easy_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_msprintf","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_waitfds","curl_multi_socket_action","curl_easy_pause","curl_easy_pause","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_easy_reset","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_easy_init","curl_multi_init","curl_msprintf","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_waitfds","curl_multi_socket_action","curl_easy_pause","curl_easy_pause","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_easy_reset","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    if (size < 2) {
	return 0;
	}
	 // Need minimal data for testing
    
    // Initialize global curl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }
    
    // Create easy handles
    CURL *easy1 = curl_easy_init();
    CURL *easy2 = curl_easy_init();
    if (!easy1 || !easy2) {
        if (easy1) curl_easy_cleanup(easy1);
        if (easy2) curl_easy_cleanup(easy2);
        curl_global_cleanup();
        return 0;
    }
    
    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy1);
        curl_easy_cleanup(easy2);
        curl_global_cleanup();
        return 0;
    }
    
    // Setup output file and get its descriptor
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy1);
        curl_easy_cleanup(easy2);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }
    
    int out_fd = fuzz_fileno(out_file);
    if (out_fd < 0) {
        assert_file_closed(&out_file);;
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy1);
        curl_easy_cleanup(easy2);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	return 0;
    }
    
    // Set up easy handles with data
    // Create a null-terminated string from input for curl_msprintf
    char *input_str = (char*)malloc(size + 1);
    if (input_str) {
        memcpy(input_str, data, size);
        input_str[size] = '\0';
        
        // Use curl_msprintf - format some information about the input
        char buffer[256];
        int printed = curl_msprintf(buffer, "Input size: %zu, first byte: 0x%02x", 
                                   size, data[0]);
        
        free(input_str);
    }
    
    // Setup easy handles with some options
    curl_easy_setopt(easy1, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy1, CURLOPT_WRITEDATA, out_file);
    
    curl_easy_setopt(easy2, CURLOPT_URL, "http://example.org");
    curl_easy_setopt(easy2, CURLOPT_WRITEDATA, out_file);
    
    // Add handles to multi
    curl_multi_add_handle(multi, easy1);
    curl_multi_add_handle(multi, easy2);
    
    // Prepare for curl_multi_waitfds
    struct curl_waitfd ufds[2];
    unsigned int fd_count = 0;
    
    // Use curl_multi_waitfds
    // Note: We use out_fd as a socket fd for demonstration
    ufds[0].fd = out_fd;
    ufds[0].events = CURL_WAIT_POLLOUT;
    ufds[0].revents = 0;
    
    CURLMcode wait_result = curl_multi_waitfds(multi, ufds, 1, &fd_count);
    
    // Use curl_multi_socket - simulate socket activity on our file descriptor
    int running_handles = 0;
    curl_socket_t sock = (curl_socket_t)out_fd;
    curl_multi_socket(multi, sock, &running_handles);
    
    // Use curl_easy_pause - pause and resume one handle
    curl_easy_pause(easy1, CURLPAUSE_RECV);
    curl_easy_pause(easy1, fuzz_int32_t_1);
    
    // Write some of the input data to the output file
    if (size > 0 && out_file) {
        fwrite(data, 1, size < 1024 ? size : 1024, out_file);
        fflush(out_file);
    }
    
    // Perform a partial multi operation
    int still_running = 0;
    curl_multi_perform(multi, &still_running);
    
    // Remove handles from multi
    curl_multi_remove_handle(multi, easy1);
    curl_multi_remove_handle(multi, easy2);
    
    // Use curl_easy_reset
    curl_easy_reset(easy1);
    
    // Clean up
    if (out_file) assert_file_closed(&out_file);;
    curl_easy_cleanup(easy1);
    curl_easy_cleanup(easy2);
    curl_multi_cleanup(multi);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	return 0;
}
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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 23, nr_unique_branch: 10
//<Quality> {"density":23,"unique_branches":{"curl_share_setopt":[[281,3,281,25,0,0,4,0],[285,5,285,28,0,0,4,1],[288,5,288,31,0,0,4,1],[299,5,299,29,0,0,4,1],[309,5,309,36,0,0,4,1],[320,5,320,32,0,0,4,1],[323,5,323,12,0,0,4,0],[327,8,327,12,0,0,4,1],[331,3,331,26,0,0,4,0],[336,3,336,28,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_share_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_share_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_assign","curl_multi_notify_disable","curl_easy_nextheader","curl_multi_poll","curl_multi_perform","curl_easy_upkeep","curl_ws_send","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_share_init","curl_share_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_assign","curl_multi_notify_disable","curl_easy_nextheader","curl_multi_poll","curl_multi_perform","curl_easy_upkeep","curl_ws_send","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
	//fuzzer shim end}




    // Initialize curl globally
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create a share handle
    CURLSH *share_handle = curl_share_init();
    if (!share_handle) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set share option - using input data to determine option
    CURLSHoption share_opt = CURLSHOPT_SHARE;
    if (size > 0) {
        // Use first byte to select from valid CURLSHoption values
        uint8_t opt_selector = data[0] % 4;  // There are 4 CURLSHOPT values
        switch (opt_selector) {
            case 0: share_opt = CURLSHOPT_SHARE; break;
            case 1: share_opt = CURLSHOPT_UNSHARE; break;
            case 2: share_opt = CURLSHOPT_LOCKFUNC; break;
            case 3: share_opt = CURLSHOPT_UNLOCKFUNC; break;
        }
    }
    curl_share_setopt(share_handle, share_opt, NULL);

    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    
    // Use input file descriptor for CURLOPT_READDATA
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Assign socket using input file descriptor
    curl_multi_assign(multi_handle, (curl_socket_t)in_fd, NULL);

    // Disable a notification
    unsigned int notification = 0;  // CURLM_NOTIFY_DISABLE
    if (size > 1) {
        notification = data[1] % 2;  // 0 or 1
    }
    curl_multi_notify_disable(multi_handle, fuzz_uint32_t_1);

    // Try to get headers (even though we haven't performed a request)
    struct curl_header *header = NULL;
    unsigned int origin = 0;  // CURLH_HEADER
    int request = 0;
    struct curl_header *next_header = curl_easy_nextheader(easy_handle, fuzz_uint32_t_2, fuzz_int32_t_3, header);

    // Prepare for polling
    int running_handles = 0;
    int timeout_ms = 100;  // Short timeout for fuzzing
    int ret = 0;
    struct curl_waitfd extra_fds;
    extra_fds.fd = in_fd;
    extra_fds.events = CURL_WAIT_POLLIN;
    extra_fds.revents = 0;

    // Multi poll
    curl_multi_poll(multi_handle, &extra_fds, 1, fuzz_int32_t_4, &ret);

    // Perform multi handle operations
    curl_multi_perform(multi_handle, &running_handles);

    // Call upkeep on easy handle
    curl_easy_upkeep(easy_handle);

    // Try to send WebSocket data if we have enough input
    if (size > 10) {
        size_t sent = 0;
        curl_off_t fragsize = 0;
        unsigned int flags = CURLWS_BINARY;
        
        // Use part of input data as WebSocket payload
        size_t ws_size = size > 1024 ? 1024 : size;
        const void *ws_buffer = data;
        
        curl_ws_send(easy_handle, ws_buffer, ws_size, &sent, fuzz_int64_t_5, flags);
    }

    // Cleanup in reverse order
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_share_cleanup(share_handle);
    
    assert_file_closed(&in_file);;
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}
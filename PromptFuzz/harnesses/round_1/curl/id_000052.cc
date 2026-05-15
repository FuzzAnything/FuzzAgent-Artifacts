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
//<ID> 889
//<Prompt> []
/*<Combination>: [
*/
//<score> 17, nr_unique_branch: 3
//<Quality> {"density":34,"unique_branches":{"Curl_resolv_take_result":[[971,6,971,11,0,0,4,0],[319,4,319,10,4,0,4,0],[319,14,319,33,4,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_share_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_version","curl_getdate","curl_share_setopt","curl_share_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_unescape","curl_free","curl_easy_duphandle","curl_multi_add_handle","curl_multi_add_handle","curl_msprintf","curl_multi_poll","curl_multi_socket_action","curl_multi_perform","curl_multi_strerror","curl_easy_upkeep","curl_easy_nextheader","curl_ws_send","curl_version_info","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_share_init","curl_version","curl_getdate","curl_share_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_unescape","curl_free","curl_easy_duphandle","curl_multi_add_handle","curl_multi_add_handle","curl_msprintf","curl_multi_poll","curl_multi_socket_action","curl_multi_perform","curl_multi_strerror","curl_easy_upkeep","curl_easy_nextheader","curl_ws_send","curl_version_info","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":3}
/*Here's a comprehensive C++ fuzz driver that uses all the specified libcurl APIs:

*/


// External definitions
extern "C" {
}

// The fuzz driver function
extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Initialize curl globally
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Initialize share handle
    CURLSH *share_handle = curl_share_init();
    if (!share_handle) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Get curl version
    char *version_str = curl_version();
    
    // Get date from input data (first 100 bytes or less)
    size_t date_len = (size > 100) ? 100 : size;
    std::string date_string(reinterpret_cast<const char*>(data), date_len);
    time_t date_result = curl_getdate(date_string.c_str(), nullptr);

    // Set share options
    curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);

    // Create a temporary file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_share_cleanup(share_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor
    int fd = fuzz_fileno(in_file);

    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    
    // Use input data as URL (first null-terminated string or first 256 bytes)
    size_t url_len = 0;
    while (url_len < size && data[url_len] != 0 && url_len < 256) {
        url_len++;
    }
    std::string url_string(reinterpret_cast<const char*>(data), url_len);
    curl_easy_setopt(easy_handle, CURLOPT_URL, url_string.c_str());
    
    // Set share handle for the easy handle
    curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
    
    // Set read function to read from our file
    curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, fread);
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);

    // Unescape some data (use middle portion if available)
    int outlength = 0;
    size_t unescape_offset = size / 3;
    size_t unescape_len = (size > unescape_offset + 50) ? 50 : size - unescape_offset;
    if (unescape_len > 0) {
        char *unescaped = curl_easy_unescape(easy_handle, 
            reinterpret_cast<const char*>(data + unescape_offset), 
            unescape_len, &outlength);
        if (unescaped) {
            curl_free(unescaped);
        }
    }

    // Duplicate the easy handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);
    if (dup_handle) {
        // Add duplicated handle to multi
        curl_multi_add_handle(multi_handle, dup_handle);
    }

    // Add original handle to multi
    curl_multi_add_handle(multi_handle, easy_handle);

    // Use msprintf to format a buffer
    char buffer[256];
    int printed = curl_msprintf(buffer, "Curl version: %s, Input size: %zu", 
                               version_str ? version_str : "unknown", size);

    // Prepare for multi polling
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = fd;
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;
    
    int still_running = 1;
    int ret = 0;
    
    // Poll and perform
    while (still_running) {
        CURLMcode poll_res = curl_multi_poll(multi_handle, extra_fds, 1, fuzz_int32_t_1, &ret);
        
        // Use socket action (simulate with invalid socket -1)
        int running_handles = 0;
        curl_multi_socket(multi_handle, -1, &running_handles);
        
        CURLMcode perform_res = curl_multi_perform(multi_handle, &still_running);
        
        // Get multi error string
        const char *multi_error = curl_multi_strerror(perform_res);
        
        // Call upkeep on easy handle
        curl_easy_upkeep(easy_handle);
        
        // Break after one iteration for fuzzing
        break;
    }

    // Try to get next header (this will likely fail as we don't have actual headers)
    struct curl_header *header = curl_easy_nextheader(easy_handle, 
                                                     CURLH_HEADER, fuzz_int32_t_2, nullptr);

    // Try to send WebSocket data (last portion of input)
    size_t sent = 0;
    size_t ws_data_len = (size > 100) ? 100 : size;
    size_t ws_offset = (size > ws_data_len) ? size - ws_data_len : 0;
    CURLcode ws_res = curl_ws_send(easy_handle, 
                                  data + ws_offset, 
                                  ws_data_len, 
                                  &sent, 
                                  fuzz_int64_t_3, 
                                  CURLWS_BINARY);

    // Get version info
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);

    // Clean up duplicated handle if it was created
    if (dup_handle) {
        curl_multi_remove_handle(multi_handle, dup_handle);
        curl_easy_cleanup(dup_handle);
    }

    // Remove and cleanup original handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);

    // Cleanup multi handle
    curl_multi_cleanup(multi_handle);

    // Cleanup share handle
    curl_share_cleanup(share_handle);

    // Close the file
    assert_file_closed(&in_file);;

    // Global cleanup
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
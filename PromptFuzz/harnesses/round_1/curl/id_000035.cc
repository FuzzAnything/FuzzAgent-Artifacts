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
//<ID> 421
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":36,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_share_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_share_setopt","curl_share_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_share_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_assign","curl_multi_socket_action","curl_multi_notify_disable","curl_easy_nextheader","curl_multi_poll","curl_multi_perform","curl_multi_poll","curl_multi_perform","curl_multi_strerror","curl_easy_upkeep","curl_ws_send","curl_easy_duphandle","curl_multi_remove_handle","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_share_init","curl_share_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_assign","curl_multi_socket_action","curl_multi_notify_disable","curl_easy_nextheader","curl_multi_poll","curl_multi_perform","curl_multi_poll","curl_multi_perform","curl_multi_strerror","curl_easy_upkeep","curl_ws_send","curl_easy_duphandle","curl_multi_remove_handle","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Initialize global curl environment
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
	return 0;
	}
	

    // 2. Create handles
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    CURLSH *share_handle = curl_share_init();
    if (!share_handle) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // 3. Set share options
    CURLSHcode share_setopt_res = curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    (void)share_setopt_res; // Result may fail but we continue

    // 4. Set easy handle options using input data
    // Use first part of data as URL (null-terminated)
    size_t url_len = size < 256 ? size : 256;
    char *url = (char *)malloc(url_len + 1);
    if (!url) {
        curl_share_cleanup(share_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url, data, url_len);
    url[url_len] = '\0';

    CURLcode easy_setopt_res;
    easy_setopt_res = curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    easy_setopt_res = curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
    easy_setopt_res = curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    easy_setopt_res = curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    easy_setopt_res = curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L); // HEAD request
    
    // 5. Add easy handle to multi handle
    CURLMcode multi_add_res = curl_multi_add_handle(multi_handle, easy_handle);
    if (multi_add_res != CURLM_OK) {
        free(url);
        curl_share_cleanup(share_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // 6. Create a FILE from input data for potential use
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        
        // 7. Multi assign with dummy socket (won't be used but called)
        void *dummy_sockp = nullptr;
        CURLMcode assign_res = curl_multi_assign(multi_handle, (curl_socket_t)fd, dummy_sockp);
        (void)assign_res;
        
        // 8. Multi socket (older API)
        int running_handles = 0;
        CURLMcode socket_res = curl_multi_socket(multi_handle, (curl_socket_t)fd, &running_handles);
        (void)socket_res;
        
        assert_file_closed(&in_file);;
    }

    // 9. Disable notifications
    CURLMcode notify_disable_res = curl_multi_notify_disable(multi_handle, fuzz_uint32_t_1);
    (void)notify_disable_res;

    // 10. Get headers (if any)
    struct curl_header *header = curl_easy_nextheader(easy_handle, fuzz_uint32_t_2, fuzz_int32_t_3, nullptr);
    (void)header;

    // 11. Multi poll and perform
    int still_running = 0;
    int ret = 0;
    
    // First poll with timeout 0 (non-blocking)
    CURLMcode poll_res = curl_multi_poll(multi_handle, nullptr, 0, fuzz_int32_t_4, &ret);
    (void)poll_res;
    
    // Perform any ready transfers
    CURLMcode perform_res = curl_multi_perform(multi_handle, &still_running);
    (void)perform_res;
    
    // Poll again with short timeout
    poll_res = curl_multi_poll(multi_handle, nullptr, 0, fuzz_int32_t_5, &ret);
    
    // Perform again
    perform_res = curl_multi_perform(multi_handle, &still_running);

    // 12. Get error string for multi errors (just to call the function)
    const char *error_str = curl_multi_strerror(perform_res);
    (void)error_str;

    // 13. Call easy upkeep (for HTTP/2/3 connections)
    CURLcode upkeep_res = curl_easy_upkeep(easy_handle);
    (void)upkeep_res;

    // 14. Try to send WebSocket data (will likely fail but we call the API)
    size_t sent = 0;
    unsigned int ws_flags = 0;
    size_t ws_data_size = size > 1024 ? 1024 : size;
    CURLcode ws_send_res = curl_ws_send(easy_handle, data, ws_data_size, &sent, 
                                        (curl_off_t)ws_data_size, fuzz_uint32_t_6);
    (void)ws_send_res;

    // 15. Create duplicate handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);
    if (dup_handle) {
        // 16. Remove original handle from multi
        CURLMcode remove_res = curl_multi_remove_handle(multi_handle, easy_handle);
        (void)remove_res;
        
        // 17. Clean up duplicate
        curl_easy_cleanup(dup_handle);
    }

    // Clean up resources in reverse order
    free(url);
    
    // 18. Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // 19. Clean up multi handle
    CURLMcode multi_cleanup_res = curl_multi_cleanup(multi_handle);
    (void)multi_cleanup_res;
    
    // 20. Clean up share handle
    CURLSHcode share_cleanup_res = curl_share_cleanup(share_handle);
    (void)share_cleanup_res;
    
    // 21. Clean up global curl
    curl_global_cleanup();

    assert_file_closed(&in_file);
	return 0;
}
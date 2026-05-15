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
//<ID> 691
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 5
//<Quality> {"density":16,"unique_branches":{"urlget_url":[[1482,10,1482,19,0,0,4,0],[1487,12,1488,37,0,0,4,1],[1526,25,1526,34,0,0,4,0]],"parsefmt":[[203,10,203,21,0,0,4,0],[206,12,206,18,0,0,4,1]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_get","curl_free","curl_easy_init","curl_url_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_multi_setopt","curl_multi_add_handle","curl_escape","curl_free","curl_multi_perform","curl_multi_wait","curl_multi_socket_action","curl_easy_send","curl_ws_send","curl_multi_remove_handle","curl_easy_nextheader","curl_slist_append","curl_slist_free_all","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_get","curl_free","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_setopt","curl_multi_add_handle","curl_escape","curl_free","curl_multi_perform","curl_multi_wait","curl_multi_socket_action","curl_easy_send","curl_ws_send","curl_multi_remove_handle","curl_easy_nextheader","curl_slist_append","curl_slist_free_all","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<57) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize curl globally
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Use first part of data for URL operations
    size_t url_len = size > 100 ? 100 : size;
    char *url_str = (char*)malloc(url_len + 1);
    if (!url_str) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_str, data, url_len);
    url_str[url_len] = '\0';
    
    // Set and get URL parts
    CURLUcode url_res = curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_1);
    if (url_res == CURLUE_OK) {
        char *retrieved_url = nullptr;
        CURLUcode get_res = curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, fuzz_uint32_t_2);
        if (get_res == CURLUE_OK && retrieved_url) {
            curl_free(retrieved_url);
        }
    }
    free(url_str);
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Set some easy options
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Set multi options
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 10L);
    
    // Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    
    // Use curl_escape with part of input data
    size_t escape_len = size > 50 ? 50 : size;
    char *escape_str = (char*)malloc(escape_len + 1);
    if (escape_str) {
        memcpy(escape_str, data, escape_len);
        escape_str[escape_len] = '\0';
        char *escaped = curl_escape(escape_str, (int)escape_len);
        if (escaped) {
            curl_free(escaped);
        }
        free(escape_str);
    }
    
    // Try to perform multi operations (won't actually connect)
    int running_handles = 0;
    if (add_res == CURLM_OK) {
        curl_multi_perform(multi_handle, &running_handles);
        
        // Wait with minimal timeout
        struct curl_waitfd extra_fds[1];
        int ret = 0;
        curl_multi_wait(multi_handle, extra_fds, 0, fuzz_int32_t_3, &ret);
        
        // Try socket action (won't have real socket)
        curl_multi_socket_action(multi_handle, CURL_SOCKET_BAD, fuzz_int32_t_4, &running_handles);
    }
    
    // Try to send data (won't work without connection)
    size_t sent = 0;
    if (size > 0) {
        curl_easy_send(easy_handle, data, size > 1024 ? 1024 : size, &sent);
    }
    
    // Try WebSocket send (won't work without connection)
    if (size > 10) {
        size_t ws_sent = 0;
        curl_ws_send(easy_handle, data, size > 512 ? 512 : size, &ws_sent, fuzz_int64_t_5, fuzz_uint32_t_6);
    }
    
    // Remove handle from multi
    if (add_res == CURLM_OK) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    // Try to get headers (won't have any)
    struct curl_header *header = curl_easy_nextheader(easy_handle, fuzz_uint32_t_7, fuzz_int32_t_8, nullptr);
    
    // Create and free a slist
    struct curl_slist *slist = curl_slist_append(nullptr, fuzz_str_9);
    if (slist) {
        curl_slist_free_all(slist);
    }
    
    // Cleanup in reverse order
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    curl_url_cleanup(url_handle);
    curl_global_cleanup();
    
    return 0;
}
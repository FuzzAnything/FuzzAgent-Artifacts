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
//<ID> 1306
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 1
//<Quality> {"density":33,"unique_branches":{"multi_wait":[[1558,6,1558,20,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_version","curl_multi_init","curl_global_cleanup","curl_multi_setopt","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_share_init","curl_share_setopt","curl_url","curl_url_set","curl_url_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_duphandle","curl_easy_upkeep","curl_easy_reset","curl_easy_setopt","curl_easy_unescape","curl_free","curl_getdate","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_timeout","curl_multi_poll","curl_multi_socket_action","curl_multi_info_read","curl_multi_info_read","curl_multi_remove_handle","curl_multi_remove_handle","curl_slist_free_all","curl_easy_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version","curl_multi_init","curl_multi_setopt","curl_easy_init","curl_share_init","curl_share_setopt","curl_url","curl_url_set","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_duphandle","curl_easy_upkeep","curl_easy_reset","curl_easy_setopt","curl_easy_unescape","curl_free","curl_getdate","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_timeout","curl_multi_poll","curl_multi_socket_action","curl_multi_info_read","curl_multi_info_read","curl_multi_remove_handle","curl_multi_remove_handle","curl_slist_free_all","curl_easy_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_url_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<58) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Set up tracing using part of input data as config
    const char* trace_config = "http/2";
    if (size > 0) {
        // Use first byte to determine trace config
        switch (data[0] % 3) {
            case 0: trace_config = "http/2"; break;
            case 1: trace_config = "http/1.1"; break;
            case 2: trace_config = "tls"; break;
        }
    }
    curl_global_trace(fuzz_str_1);
    
    // Get curl version
    char *version = curl_version();
    (void)version; // Use version to avoid unused variable warning
    
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set multi handle options using input data
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)(size % 10 + 1));
    
    // Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Initialize share handle
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        // Set share options
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    }
    
    // Create URL handle
    CURLU *url_handle = curl_url();
    
    // Set URL using input data
    if (size > 10 && url_handle) {
        // Use first 10 bytes as URL part (if available)
        char url_part[11];
        size_t url_len = size > 10 ? 10 : size;
        memcpy(url_part, data, url_len);
        url_part[url_len] = '\0';
        
        curl_url_set(url_handle, CURLUPART_URL, url_part, fuzz_uint32_t_2);
    }
    
    // Create a file with input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        if (url_handle) curl_url_cleanup(url_handle);
        if (share_handle) curl_share_cleanup(share_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    
    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
    
    if (out_file) {
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    }
    
    // Create and use a linked list
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, fuzz_str_3);
    if (size > 5) {
        char header[50];
        snprintf(header, sizeof(header), "X-Data-Size: %zu", size);
        headers = curl_slist_append(headers, header);
    }
    curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    
    // Duplicate easy handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);
    
    // Perform upkeep on original handle
    curl_easy_upkeep(easy_handle);
    
    // Reset the original handle
    curl_easy_reset(easy_handle);
    
    // Re-set some options on original handle
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.org");
    
    // Unescape a string using part of input data
    if (size > 20) {
        int outlength;
        char *unescaped = curl_easy_unescape(easy_handle, 
                                           (const char *)(data + 10), 
                                           (int)(size > 30 ? 20 : size - 10), 
                                           &outlength);
        if (unescaped) {
            curl_free(unescaped);
        }
    }
    
    // Parse a date string using input data
    if (size > 40) {
        char date_str[41];
        size_t date_len = size > 40 ? 40 : size;
        memcpy(date_str, data, date_len);
        date_str[date_len] = '\0';
        time_t parsed_date = curl_getdate(date_str, NULL);
        (void)parsed_date; // Use to avoid warning
    }
    
    // Add both handles to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    if (dup_handle) {
        curl_multi_add_handle(multi_handle, dup_handle);
    }
    
    // Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Variables for multi perform
    int running_handles = 0;
    int ret = 0;
    
    // Perform multi operations
    curl_multi_perform(multi_handle, &running_handles);
    
    // Get timeout
    long timeout_ms = 0;
    curl_multi_timeout(multi_handle, &timeout_ms);
    
    // Poll for activity
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = in_fd;
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;
    
    curl_multi_poll(multi_handle, extra_fds, 1, fuzz_int32_t_4, &ret);
    
    // Socket action
    curl_multi_socket(multi_handle, (curl_socket_t)in_fd, &running_handles);
    
    // Check for messages
    int msgs_in_queue = 0;
    CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    while (msg) {
        if (msg->msg == CURLMSG_DONE) {
            // Handle completed transfer
        }
        msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    }
    
    // Remove handles from multi
    curl_multi_remove_handle(multi_handle, easy_handle);
    if (dup_handle) {
        curl_multi_remove_handle(multi_handle, dup_handle);
    }
    
    // Cleanup
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
    }
    
    assert_file_closed(&in_file);;
    
    if (dup_handle) {
        curl_easy_cleanup(dup_handle);
    }
    
    curl_easy_cleanup(easy_handle);
    
    if (share_handle) {
        curl_share_cleanup(share_handle);
    }
    
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}
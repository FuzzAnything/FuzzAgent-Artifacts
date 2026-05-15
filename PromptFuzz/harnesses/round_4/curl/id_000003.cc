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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 2
//<Quality> {"density":14,"unique_branches":{"parsedate":[[541,6,541,17,0,0,4,0],[541,21,541,55,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_url","curl_global_cleanup","curl_url_set","curl_url_set","curl_url_set","curl_url_get","curl_free","curl_getdate","curl_multi_init","curl_url_cleanup","curl_global_cleanup","curl_multi_notify_enable","curl_easy_init","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_easy_send","curl_ws_send","curl_multi_perform","curl_multi_wait","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_set","curl_url_set","curl_url_get","curl_free","curl_getdate","curl_multi_init","curl_multi_notify_enable","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_easy_send","curl_ws_send","curl_multi_perform","curl_multi_wait","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":3}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<51) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_7, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_8, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations
    
    // Initialize curl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }
    
    // Create a memory file for reading input
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for socket operations
    int fd = fuzz_fileno(in_file);
    curl_socket_t sockfd = fd;
    
    // Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 1. Use curl_url_set to set URL parts from input data
    // Use first 50 bytes as URL components
    size_t url_len = size > 50 ? 50 : size;
    char *url_part = (char *)malloc(url_len + 1);
    if (url_part) {
        memcpy(url_part, data, url_len);
        url_part[url_len] = '\0';
        
        // Set URL scheme
        curl_url_set(url_handle, CURLUPART_SCHEME, fuzz_str_1, fuzz_uint32_t_2);
        // Set URL host from input data
        curl_url_set(url_handle, CURLUPART_HOST, url_part, fuzz_uint32_t_3);
        // Set URL path
        curl_url_set(url_handle, CURLUPART_PATH, fuzz_str_4, fuzz_uint32_t_5);
        
        free(url_part);
    }
    
    // 2. Use curl_url_get to retrieve URL parts
    char *retrieved_url = nullptr;
    if (curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, fuzz_uint32_t_6) == CURLUE_OK) {
        if (retrieved_url) {
            // Use retrieved URL somehow
            curl_free(retrieved_url);
        }
    }
    
    // 3. Use curl_getdate with date string from input
    // Use next 30 bytes as date string
    if (size > 80) {
        char *date_str = (char *)malloc(31);
        if (date_str) {
            memcpy(date_str, data + 50, 30);
            date_str[30] = '\0';
            time_t parsed_time = curl_getdate(date_str, nullptr);
            free(date_str);
            (void)parsed_time; // Use result to avoid compiler warning
        }
    }
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_url_cleanup(url_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // 4. Use curl_multi_notify_enable
    // Enable socket notifications (CURL_POLL_IN)
    curl_multi_notify_enable(multi_handle, fuzz_uint32_t_7);
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_url_cleanup(url_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set some basic options for easy handle
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // 5. Use curl_multi_socket with our file descriptor
    int running_handles = 0;
    curl_multi_socket(multi_handle, sockfd, &running_handles);
    
    // 6. Use curl_easy_send (simulate sending data)
    // Use middle portion of input for sending
    if (size > 150) {
        const void *send_buffer = data + 80;
        size_t send_size = size - 80 > 100 ? 100 : size - 80;
        size_t sent_bytes = 0;
        curl_easy_send(easy_handle, send_buffer, send_size, &sent_bytes);
    }
    
    // 7. Use curl_ws_send (simulate WebSocket send)
    // Use last portion of input for WebSocket
    if (size > 200) {
        size_t ws_offset = size > 300 ? size - 100 : 180;
        const void *ws_buffer = data + ws_offset;
        size_t ws_size = size - ws_offset;
        size_t ws_sent = 0;
        curl_ws_send(easy_handle, ws_buffer, ws_size, &ws_sent, fuzz_int64_t_8, fuzz_uint32_t_9);
    }
    
    // Clean up multi handle operations
    int still_running = 1;
    while (still_running) {
        CURLMcode mc = curl_multi_perform(multi_handle, &still_running);
        if (mc != CURLM_OK) {
            break;
        }
        
        if (still_running) {
            // Wait for activity
            curl_multi_wait(multi_handle, nullptr, 0, fuzz_int32_t_10, nullptr);
        }
    }
    
    // Read any remaining data from input file
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), in_file) != nullptr) {
        // Process or ignore the data
    }
    
    // Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_url_cleanup(url_handle);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
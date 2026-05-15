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
//<ID> 589
//<Prompt> []
/*<Combination>: [
*/
//<score> 96, nr_unique_branch: 4
//<Quality> {"density":24,"unique_branches":{"tftp_send_first":[[692,8,692,14,0,0,4,0]],"cpool_foreach":[[533,11,533,15,0,0,4,1],[539,10,539,38,0,0,4,1]],"Curl_conn_upkeep":[[686,6,687,34,0,0,4,0]]},"library_calls":["curl_global_init_mem","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_wait","curl_multi_socket_action","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_send","curl_easy_recv","curl_easy_upkeep","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_easy_init","curl_multi_init","curl_multi_add_handle","curl_easy_setopt","curl_multi_wait","curl_multi_socket_action","curl_easy_send","curl_easy_recv","curl_easy_upkeep","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Custom memory functions for curl_global_init_mem
static void * id_000042_custom_malloc(size_t size) {
    return malloc(size);
}

static void  id_000042_custom_free(void *ptr) {
    free(ptr);
}

static void * id_000042_custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char * id_000042_custom_strdup(const char *str) {
    return strdup(str);
}

static void * id_000042_custom_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Initialize curl with custom memory functions
    CURLcode global_init_result = curl_global_init_mem(CURL_GLOBAL_ALL,
         id_000042_custom_malloc,
         id_000042_custom_free,
         id_000042_custom_realloc,
         id_000042_custom_strdup,
         id_000042_custom_calloc);
    
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Add easy handle to multi handle
    CURLMcode multi_add_result = curl_multi_add_handle(multi_handle, easy_handle);
    if (multi_add_result != CURLM_OK) {
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Set up easy handle with some basic options
    // Using input data to set URL (truncated to reasonable length)
    char url[256];
    size_t url_len = size < 255 ? size : 255;
    memcpy(url, data, url_len);
    url[url_len] = '\0';
    
    // Try to set URL (this may fail but that's OK for fuzzing)
    curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Prepare waitfd structure for multi_wait
    struct curl_waitfd extra_fds[2];
    extra_fds[0].fd = in_fd;
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;
    
    extra_fds[1].fd = out_fd;
    extra_fds[1].events = CURL_WAIT_POLLOUT;
    extra_fds[1].revents = 0;
    
    // Use curl_multi_wait with a short timeout
    int ret = 0;
    int timeout_ms = 1; // Very short timeout for fuzzing
    CURLMcode wait_result = curl_multi_wait(multi_handle, extra_fds, 2, fuzz_int32_t_1, &ret);
    
    // Use curl_multi_socket with the input file descriptor
    int running_handles = 0;
    curl_socket_t socket_fd = (curl_socket_t)in_fd;
    CURLMcode socket_result = curl_multi_socket(multi_handle, socket_fd, &running_handles);
    
    // Prepare a buffer for sending/receiving
    uint8_t *buffer = (uint8_t *)malloc(size);
    if (!buffer) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Copy some data to buffer for sending
    size_t copy_size = size < 1024 ? size : 1024;
    if (copy_size > 0) {
        memcpy(buffer, data, copy_size);
    }
    
    // Try to send data using curl_easy_send
    size_t sent = 0;
    CURLcode send_result = curl_easy_send(easy_handle, buffer, copy_size, &sent);
    
    // Try to receive data using curl_easy_recv
    size_t received = 0;
    CURLcode recv_result = curl_easy_recv(easy_handle, buffer, size, &received);
    
    // Perform upkeep on the easy handle
    CURLcode upkeep_result = curl_easy_upkeep(easy_handle);
    
    // Clean up
    free(buffer);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Remove the easy handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // Global cleanup
    curl_global_cleanup();
    
    // Remove output file if it exists
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
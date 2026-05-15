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
//<ID> 587
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["curl_global_init_mem","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_multi_add_handle","curl_easy_upkeep","curl_multi_perform","curl_multi_wait","curl_easy_send","curl_easy_recv","curl_multi_socket_action","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_easy_upkeep","curl_multi_perform","curl_multi_wait","curl_easy_send","curl_easy_recv","curl_multi_socket_action","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a fuzz driver that uses the specified curl APIs to simulate a client-server communication scenario:

*/


// Custom memory functions for curl_global_init_mem
static void * id_000041_my_malloc_callback(size_t size) {
    return malloc(size);
}

static void  id_000041_my_free_callback(void *ptr) {
    free(ptr);
}

static void * id_000041_my_realloc_callback(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char * id_000041_my_strdup_callback(const char *str) {
    return strdup(str);
}

static void * id_000041_my_calloc_callback(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize curl with custom memory functions
    curl_global_init_mem(CURL_GLOBAL_ALL,
                          id_000041_my_malloc_callback,
                          id_000041_my_free_callback,
                          id_000041_my_realloc_callback,
                          id_000041_my_strdup_callback,
                          id_000041_my_calloc_callback);

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Open the input data as a file
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    
    // Use fmemopen to create a FILE* from input data
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Open output file
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Set up curl easy handle for file transfer
    // We'll simulate a simple send/receive scenario
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Variables for multi interface
    int running_handles = 0;
    int ret = 0;
    
    // Create waitfd structure for the input file descriptor
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = in_fd;
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;

    // Main processing loop
    int max_iterations = 10; // Prevent infinite loops
    for (int i = 0; i < max_iterations && running_handles >= 0; i++) {
        // Perform upkeep on the easy handle
        curl_easy_upkeep(easy_handle);

        // Perform multi interface operations
        curl_multi_perform(multi_handle, &running_handles);
        
        // Wait for activity
        curl_multi_wait(multi_handle, extra_fds, 1, fuzz_int32_t_1, &ret);
        
        // If input file has data, try to send it
        if (extra_fds[0].revents & CURL_WAIT_POLLIN) {
            // Read a chunk from input file
            uint8_t buffer[1024];
            size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
            
            if (bytes_read > 0) {
                // Try to send the data
                size_t bytes_sent = 0;
                CURLcode send_result = curl_easy_send(easy_handle, buffer, bytes_read, &bytes_sent);
                
                // If send was successful, try to receive
                if (send_result == CURLE_OK && bytes_sent > 0) {
                    uint8_t recv_buffer[1024];
                    size_t bytes_received = 0;
                    CURLcode recv_result = curl_easy_recv(easy_handle, recv_buffer, sizeof(recv_buffer), &bytes_received);
                    
                    if (recv_result == CURLE_OK && bytes_received > 0) {
                        // Write received data to output file
                        fwrite(recv_buffer, 1, bytes_received, out_file);
                    }
                }
            }
        }
        
        // Process socket activity
        curl_multi_socket(multi_handle, in_fd, &running_handles);
        
        // Break if we've processed all data
        if (feof(in_file)) {
            break;
        }
    }

    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove and cleanup easy handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    
    // Cleanup multi handle
    curl_multi_cleanup(multi_handle);
    
    // Global cleanup
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
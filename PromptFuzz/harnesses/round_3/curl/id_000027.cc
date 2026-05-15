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
//<ID> 402
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 4
//<Quality> {"density":19,"unique_branches":{"xfer_may_multiplex":[[567,8,567,72,0,0,4,1]],"setopt_func":[[2557,8,2557,23,0,0,4,0]],"curl_easy_pause":[[1154,6,1154,38,0,0,4,1],[1155,6,1155,55,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_trace","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_formadd","curl_slist_append","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_multi_wakeup","curl_easy_pause","curl_ws_recv","curl_easy_pause","curl_formfree","curl_slist_free_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_multi_init","curl_easy_init","curl_formadd","curl_slist_append","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_multi_wakeup","curl_easy_pause","curl_ws_recv","curl_easy_pause","curl_formfree","curl_slist_free_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<109) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Initialize curl globally with tracing
    curl_global_init(CURL_GLOBAL_ALL);
    curl_global_trace(fuzz_str_1);
    
    // Create a multi handle for asynchronous operations
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create an easy handle for WebSocket operations
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Set up form data using curl_formadd
    struct curl_httppost *formpost = NULL;
    struct curl_httppost *lastptr = NULL;
    
    // Use part of input data as form field name and value
    if (size > 10) {
        size_t half = size / 2;
        std::vector<char> name(data, data + half);
        std::vector<char> value(data + half, data + size);
        name.push_back('\0');
        value.push_back('\0');
        
        curl_formadd(&formpost, &lastptr,
                     CURLFORM_COPYNAME, name.data(),
                     CURLFORM_COPYCONTENTS, value.data(),
                     CURLFORM_END);
    }
    
    // Create a header list using curl_slist_append
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, fuzz_str_2);
    headers = curl_slist_append(headers, fuzz_str_3);
    headers = curl_slist_append(headers, fuzz_str_4);
    
    // Set WebSocket options on the easy handle
    curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "ws://example.com/ws");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, NULL);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor for socket simulation
        curl_socket_t sockfd = fuzz_fileno(in_file);
        int running_handles = 0;
        
        // Simulate socket activity using the input file descriptor
        curl_multi_socket(multi_handle, sockfd, &running_handles);
        
        // Wake up multi handle
        curl_multi_wakeup(multi_handle);
        
        // Pause the easy handle
        curl_easy_pause(easy_handle, CURLPAUSE_RECV);
        
        // Try to receive WebSocket data
        std::vector<char> buffer(1024);
        size_t bytes_received = 0;
        const struct curl_ws_frame *meta = NULL;
        
        if (size > 0) {
            // Simulate receiving data from the input
            curl_ws_recv(easy_handle, buffer.data(), buffer.size(), 
                        &bytes_received, &meta);
            
            // Resume receiving
            curl_easy_pause(easy_handle, fuzz_int32_t_5);
        }
        
        assert_file_closed(&in_file);;
    }
    
    // Clean up form data
    if (formpost) {
        curl_formfree(formpost);
    }
    
    // Clean up headers
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    // Remove easy handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
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
//<ID> 498
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 1
//<Quality> {"density":13,"unique_branches":{"mev_sh_entry_get":[[85,6,85,26,0,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_assign","curl_multi_socket_action","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_escape","curl_maprintf","curl_maprintf","curl_maprintf","curl_multi_wait","curl_free","curl_free","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_multi_assign","curl_multi_socket_action","curl_escape","curl_maprintf","curl_multi_wait","curl_free","curl_free","curl_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Helper function for curl_mvaprintf
static char*  id_000035_my_mvaprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

// Helper function to get a socket descriptor from the input data
static curl_socket_t  id_000035_get_socket_from_data(const uint8_t* data, size_t size) {
    // Use the first sizeof(curl_socket_t) bytes as socket descriptor
    // If there's not enough data, create a default socket
    if (size >= sizeof(curl_socket_t)) {
        curl_socket_t sock;
        memcpy(&sock, data, sizeof(curl_socket_t));
        return sock;
    }
    // Return a default socket (STDIN_FILENO)
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    // Initialize CURL globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create an easy handle for demonstration
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Get socket from input data
    curl_socket_t sock =  id_000035_get_socket_from_data(data, size);
    
    // Create a dummy pointer for assignment
    void* dummy_ptr = (void*)0x12345678;
    
    // 1. Use curl_multi_assign
    CURLMcode assign_res = curl_multi_assign(multi_handle, sock, dummy_ptr);
    
    // 2. Use curl_multi_socket
    int running_handles = 0;
    CURLMcode socket_res = curl_multi_socket(multi_handle, sock, &running_handles);
    
    // Create input file from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read a string from the input file for escaping
    char* input_str = nullptr;
    if (size > 0) {
        input_str = (char*)malloc(size + 1);
        if (input_str) {
            fread(input_str, 1, size, in_file);
            input_str[size] = '\0';
        }
    }
    
    // 3. Use curl_escape (with the string from input data)
    char* escaped_str = nullptr;
    if (input_str && size > 0) {
        escaped_str = curl_escape(input_str, (int)(size < INT_MAX ? size : INT_MAX));
    }
    
    // 4. Use curl_maprintf (with escaped string or input string)
    char* maprintf_str = nullptr;
    if (escaped_str) {
        maprintf_str = curl_maprintf("Escaped: %s", escaped_str);
    } else if (input_str) {
        maprintf_str = curl_maprintf("Input: %s", input_str);
    } else {
        maprintf_str = curl_maprintf("No input data");
    }
    
    // 5. Use curl_mvaprintf (via helper function)
    char* mvaprintf_str = nullptr;
    if (maprintf_str) {
        mvaprintf_str =  id_000035_my_mvaprintf("Formatted: %s", maprintf_str);
    }
    
    // 6. Use curl_multi_wait
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = fuzz_fileno(in_file);  // Use the input file descriptor
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;
    
    int ret = 0;
    CURLMcode wait_res = curl_multi_wait(multi_handle, extra_fds, 1, fuzz_int32_t_1, &ret);
    
    // Clean up
    if (mvaprintf_str) {
        curl_free(mvaprintf_str);
    }
    if (maprintf_str) {
        curl_free(maprintf_str);
    }
    if (escaped_str) {
        curl_free(escaped_str);
    }
    if (input_str) {
        free(input_str);
    }
    
    assert_file_closed(&in_file);;
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
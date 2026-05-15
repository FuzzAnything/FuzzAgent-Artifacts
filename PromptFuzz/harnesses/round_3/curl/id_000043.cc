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
//<ID> 642
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["curl_global_init_mem","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_multi_strerror","curl_ws_send","curl_easy_reset","curl_multi_remove_handle","curl_multi_info_read","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_multi_strerror","curl_ws_send","curl_easy_reset","curl_multi_remove_handle","curl_multi_info_read","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to simulate processing curl output data and sending WebSocket messages:

*/


extern "C" {
}

// Custom memory callbacks to track allocations
static size_t  id_000043_total_allocated = 0;
static size_t  id_000043_total_freed = 0;

static void*  id_000043_custom_malloc(size_t size) {
     id_000043_total_allocated += size;
    return malloc(size);
}

static void  id_000043_custom_free(void* ptr) {
    if (ptr) {
        // We can't know the exact size, but we'll count it as freed
         id_000043_total_freed++;
        free(ptr);
    }
}

static void*  id_000043_custom_realloc(void* ptr, size_t size) {
    if (ptr)  id_000043_total_freed++;
     id_000043_total_allocated += size;
    return realloc(ptr, size);
}

static char*  id_000043_custom_strdup(const char* str) {
    size_t len = strlen(str) + 1;
     id_000043_total_allocated += len;
    return strdup(str);
}

static void*  id_000043_custom_calloc(size_t nmemb, size_t size) {
     id_000043_total_allocated += nmemb * size;
    return calloc(nmemb, size);
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
	//fuzzer shim end}




    // Ensure minimum size for meaningful operations
    if (size < 2) {
	return 0;
	}
	
    
    // 1. Initialize curl with custom memory callbacks
    CURLcode init_result = curl_global_init_mem(
        CURL_GLOBAL_DEFAULT,
         id_000043_custom_malloc,
         id_000043_custom_free,
         id_000043_custom_realloc,
         id_000043_custom_strdup,
         id_000043_custom_calloc
    );
    
    if (init_result != CURLE_OK) {
	return 0;
	}
	
    
    FILE* in_file = nullptr;
    FILE* out_file = nullptr;
    CURLM* multi_handle = nullptr;
    CURL* easy_handle = nullptr;
    
    do {
        // 2. Create input/output files using the provided data
        in_file = fmemopen((void*)data, size, "rb");
        if (!in_file) break;
        
        out_file = fopen("output_file", "wb");
        if (!out_file) break;
        
        // 3. Create multi handle and easy handle
        multi_handle = curl_multi_init();
        if (!multi_handle) break;
        
        easy_handle = curl_easy_init();
        if (!easy_handle) break;
        
        // 4. Set up easy handle with basic options
        // Use file descriptor from input file
        int in_fd = fuzz_fileno(in_file);
        int out_fd = fuzz_fileno(out_file);
        
        // Set URL (using a dummy URL since we're not actually connecting)
        curl_easy_setopt(easy_handle, CURLOPT_URL, "ws://localhost:8080/test");
        
        // Enable WebSocket support
        curl_easy_setopt(easy_handle, CURLOPT_CONNECT_ONLY, 2L); // CONNECT_ONLY for WebSocket
        
        // 5. Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // 6. Drive the multi handle
        int running_handles = 0;
        CURLMcode multi_result = curl_multi_socket_all(multi_handle, &running_handles);
        
        // 7. Use curl_multi_strerror to get error message if needed
        const char* multi_error_str = curl_multi_strerror(multi_result);
        // We'll use this string to avoid it being optimized away
        if (multi_error_str && size > 10) {
            // Just accessing the string to ensure it's used
            volatile char first_char = multi_error_str[0];
            (void)first_char;
        }
        
        // 8. Send WebSocket message using input data
        // We'll use a portion of the input data as our WebSocket message
        size_t ws_sent = 0;
        size_t ws_buflen = size > 1024 ? 1024 : size; // Limit buffer size
        
        // Allocate buffer for WebSocket message
        void* ws_buffer =  id_000043_custom_malloc(ws_buflen);
        if (!ws_buffer) break;
        
        // Copy data to buffer
        memcpy(ws_buffer, data, ws_buflen);
        
        // Try to send WebSocket message
        CURLcode ws_result = curl_ws_send(
            easy_handle,
            ws_buffer,
            ws_buflen,
            &ws_sent,
            fuzz_int64_t_1, // fragsize (0 for no fragmentation)
            CURLWS_BINARY // flags
        );
        
        // Clean up WebSocket buffer
         id_000043_custom_free(ws_buffer);
        
        // 9. Reset the easy handle
        curl_easy_reset(easy_handle);
        
        // 10. Remove easy handle from multi handle
        curl_multi_remove_handle(multi_handle, easy_handle);
        
        // 11. Process any remaining messages in multi handle
        int msgs_in_queue = 0;
        CURLMsg* msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        
        // Just to use the result, we'll check if we have messages
        if (msg && size > 20) {
            volatile CURLcode msg_result = msg->data.result;
            (void)msg_result;
        }
        
    } while (false);
    
    // 12. Clean up resources in reverse order of creation
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    if (out_file) {
        assert_file_closed(&out_file);;
        // Remove the output file we created
        remove("output_file");
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    // 13. Global cleanup
    curl_global_cleanup();
    
    return 0;
}
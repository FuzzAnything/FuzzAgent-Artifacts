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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 7, nr_unique_branch: 0
//<Quality> {"density":14,"unique_branches":{},"library_calls":["curl_global_init_mem","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_slist_append","curl_mime_headers","curl_multi_init","curl_multi_add_handle","curl_msprintf","curl_ws_send","curl_multi_waitfds","curl_multi_strerror","curl_easy_option_next","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_easy_init","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_slist_append","curl_mime_headers","curl_multi_init","curl_multi_add_handle","curl_msprintf","curl_ws_send","curl_multi_waitfds","curl_multi_strerror","curl_easy_option_next","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses all the specified curl APIs to create a WebSocket connection scenario with MIME headers and custom memory allocation:

*/


// Custom memory allocation functions
static void * id_000023_custom_malloc(size_t size) { return malloc(size); }
static void  id_000023_custom_free(void *ptr) { free(ptr); }
static void * id_000023_custom_realloc(void *ptr, size_t size) { return realloc(ptr, size); }
static char * id_000023_custom_strdup(const char *str) { return strdup(str); }
static void * id_000023_custom_calloc(size_t nmemb, size_t size) { return calloc(nmemb, size); }

extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // 1. Initialize curl with custom memory allocation (curl_global_init_mem)
    CURLcode global_init_result = curl_global_init_mem(
        CURL_GLOBAL_ALL,
         id_000023_custom_malloc,
         id_000023_custom_free,
         id_000023_custom_realloc,
         id_000023_custom_strdup,
         id_000023_custom_calloc
    );
    if (global_init_result != CURLE_OK) {
	return 0;
	}
	
    
    // Create a file from input data for various uses
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create and configure curl easy handle for WebSocket
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set URL (dummy URL since we're fuzzing)
    curl_easy_setopt(curl, CURLOPT_URL, "ws://localhost:8080/");
    
    // 3. Create MIME part and set headers (curl_mime_headers)
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use part of input data as MIME data
            size_t mime_data_size = size > 100 ? 100 : size;
            char *mime_data = (char *) id_000023_custom_malloc(mime_data_size + 1);
            if (mime_data) {
                memcpy(mime_data, data, mime_data_size);
                mime_data[mime_data_size] = '\0';
                curl_mime_data(part, mime_data, mime_data_size);
                 id_000023_custom_free(mime_data);
            }
            
            // Create headers list
            struct curl_slist *headers = NULL;
            // Use first 50 bytes as header if available
            if (size > 50) {
                char header_buf[100];
                snprintf(header_buf, sizeof(header_buf), "X-Fuzz-Header: %.50s", data);
                headers = curl_slist_append(headers, header_buf);
            }
            
            // Use curl_mime_headers API
            if (headers) {
                curl_mime_headers(part, headers, 1); // Take ownership
            }
        }
    }
    
    // 4. Create multi handle for WebSocket operations
    CURLM *multi = curl_multi_init();
    if (multi) {
        curl_multi_add_handle(multi, curl);
        
        // 5. Prepare WebSocket data buffer from input
        size_t ws_buffer_size = size > 1024 ? 1024 : size;
        void *ws_buffer =  id_000023_custom_malloc(ws_buffer_size);
        if (ws_buffer) {
            memcpy(ws_buffer, data, ws_buffer_size);
            
            // 6. Use curl_msprintf to format debug info
            char debug_buf[256];
            curl_msprintf(debug_buf, "Fuzzing with %zu bytes, WS buffer: %zu bytes", 
                         size, ws_buffer_size);
            
            // 7. Try to send WebSocket data (curl_ws_send)
            size_t sent = 0;
            CURLcode ws_result = curl_ws_send(curl, ws_buffer, ws_buffer_size, 
                                            &sent, fuzz_int64_t_1, CURLWS_BINARY);
            
            // 8. Use curl_multi_waitfds to wait for events
            struct curl_waitfd ufds[1];
            unsigned int fd_count = 0;
            int file_descriptor = fuzz_fileno(in_file);
            
            ufds[0].fd = file_descriptor;
            ufds[0].events = CURL_WAIT_POLLIN;
            ufds[0].revents = 0;
            
            CURLMcode wait_result = curl_multi_waitfds(multi, ufds, 1, &fd_count);
            
            // 9. Use curl_multi_strerror to get error string
            if (wait_result != CURLM_OK) {
                const char *error_str = curl_multi_strerror(wait_result);
                // Could log error_str if needed
            }
            
            // 10. Iterate through easy options (curl_easy_option_next)
            const struct curl_easyoption *option = NULL;
            int option_count = 0;
            while ((option = curl_easy_option_next(option)) != NULL && option_count < 10) {
                option_count++;
                // Just iterating through options, not setting them
            }
            
             id_000023_custom_free(ws_buffer);
        }
        
        // Clean up multi handle
        curl_multi_remove_handle(multi, curl);
        curl_multi_cleanup(multi);
    }
    
    // 11. Clean up MIME structure
    if (mime) {
        curl_mime_free(mime);
    }
    
    // 12. Close input file
    assert_file_closed(&in_file);;
    
    // 13. Clean up curl handle
    curl_easy_cleanup(curl);
    
    // 14. Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
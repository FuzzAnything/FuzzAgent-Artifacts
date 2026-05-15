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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 1
//<Quality> {"density":21,"unique_branches":{"multi_perform":[[2849,6,2849,21,0,0,4,1]]},"library_calls":["curl_global_init_mem","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_easy_setopt","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_ws_send","curl_msprintf","curl_msprintf","curl_multi_perform","curl_multi_strerror","curl_multi_waitfds","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data","curl_mime_free","curl_easy_option_next","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init_mem","curl_easy_init","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_ws_send","curl_msprintf","curl_multi_perform","curl_multi_strerror","curl_multi_waitfds","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data","curl_mime_free","curl_easy_option_next","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to simulate a scenario where curl is initialized with custom memory management, prepares WebSocket data, formats error messages, and handles WebSocket sending through a multi interface:

*/


// Custom memory management functions for curl_global_init_mem
static void * id_000024_custom_malloc(size_t size) { return malloc(size); }
static void  id_000024_custom_free(void *ptr) { free(ptr); }
static void * id_000024_custom_realloc(void *ptr, size_t size) { return realloc(ptr, size); }
static char * id_000024_custom_strdup(const char *str) { return strdup(str); }
static void * id_000024_custom_calloc(size_t nmemb, size_t size) { return calloc(nmemb, size); }

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* f_data, size_t f_size) {
	if(f_size<66) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int64_t, fuzz_int64_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing

    // 1. Initialize curl with custom memory management
    CURLcode global_rc = curl_global_init_mem(CURL_GLOBAL_ALL,
                                               id_000024_custom_malloc,
                                               id_000024_custom_free,
                                               id_000024_custom_realloc,
                                               id_000024_custom_strdup,
                                               id_000024_custom_calloc);
    if (global_rc != CURLE_OK) {
	return 0;
	}
	

    // Create output file for any file operations needed
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // 2. Create curl easy handle and set up for WebSocket
    CURL *curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // Use a sample URL (WebSocket URL would be needed for real WS)
    curl_easy_setopt(curl, CURLOPT_URL, "ws://example.com/dummy");

    // 3. Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(curl);
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // Add easy handle to multi
    curl_multi_add_handle(multi, curl);

    // 4. Prepare WebSocket send data
    size_t sent = 0;
    CURLcode ws_rc = curl_ws_send(curl, 
                                  data,         // Use fuzzer input as buffer
                                  size > 1000 ? 1000 : size, // Limit size
                                  &sent,
                                  fuzz_int64_t_1,            // No fragmentation
                                  CURLWS_BINARY);
    
    // 5. Format error message using curl_msprintf
    char error_buffer[256];
    if (ws_rc != CURLE_OK) {
        curl_msprintf(error_buffer, "WS send error: %d, sent: %zu bytes", 
                     ws_rc, sent);
    } else {
        curl_msprintf(error_buffer, "WS sent: %zu bytes", sent);
    }

    // 6. Get multi error string
    CURLMcode multi_rc = curl_multi_perform(multi, nullptr);
    const char *multi_error = curl_multi_strerror(multi_rc);

    // 7. Create curl_waitfd structures for curl_multi_waitfds
    struct curl_waitfd ufds[2];
    unsigned int fd_count = 0;
    
    // Use output file descriptor
    ufds[0].fd = fuzz_fileno(out_file);
    ufds[0].events = CURL_WAIT_POLLOUT;
    ufds[0].revents = 0;
    
    // Add stdin if we have input file
    if (size > 0) {
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            ufds[1].fd = fuzz_fileno(in_file);
            ufds[1].events = CURL_WAIT_POLLIN;
            ufds[1].revents = 0;
            
            // Call curl_multi_waitfds
            CURLMcode wait_rc = curl_multi_waitfds(multi, ufds, 2, &fd_count);
            
            assert_file_closed(&in_file);;
        }
    }

    // 8. Create and configure MIME part with headers
    curl_mime *mime = curl_mime_init(curl);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Create headers list
            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, fuzz_str_2);
            headers = curl_slist_append(headers, fuzz_str_3);
            
            // Use curl_mime_headers (taking ownership)
            CURLcode mime_rc = curl_mime_headers(part, headers, 1);
            
            // Set some mime data from input
            if (size > 50) {
                curl_mime_data(part, (const char *)data, 50);
            }
        }
        curl_mime_free(mime);
    }

    // 9. Iterate through easy options using curl_easy_option_next
    const struct curl_easyoption *opt = nullptr;
    int opt_count = 0;
    while ((opt = curl_easy_option_next(opt)) != nullptr && opt_count < 5) {
        opt_count++;
        // Just demonstrate we can access option info
        if (opt->name) {
            // Option name available
        }
    }

    // Cleanup
    curl_multi_remove_handle(multi, curl);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(curl);
    assert_file_closed(&out_file);;
    curl_global_cleanup();

    assert_file_closed(&out_file);
	return 0;
}
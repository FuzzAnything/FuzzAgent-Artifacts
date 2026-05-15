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
//<ID> 153
//<Prompt> []
/*<Combination>: [
*/
//<score> 34.5, nr_unique_branch: 3
//<Quality> {"density":23,"unique_branches":{"Curl_connect_only_attach":[[1254,6,1254,12,0,0,4,0]],"curl_ws_send":[[1792,21,1792,43,0,0,4,0],[1794,8,1794,14,0,0,4,0]]},"library_calls":["curl_global_init_mem","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data","curl_mime_free","curl_easy_option_next","curl_msprintf","curl_easy_option_next","curl_multi_waitfds","curl_multi_strerror","curl_ws_send","curl_msprintf","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_msprintf"],"critical_calls":["curl_global_init_mem","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data","curl_mime_free","curl_easy_option_next","curl_msprintf","curl_easy_option_next","curl_multi_waitfds","curl_multi_strerror","curl_ws_send","curl_msprintf","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_msprintf"],"visited":1}
/**/


// Custom memory callbacks for curl_global_init_mem
static void * id_000025_custom_malloc(size_t size) { return malloc(size); }
static void  id_000025_custom_free(void *ptr) { free(ptr); }
static void * id_000025_custom_realloc(void *ptr, size_t size) { return realloc(ptr, size); }
static char * id_000025_custom_strdup(const char *str) { return strdup(str); }
static void * id_000025_custom_calloc(size_t nmemb, size_t size) { return calloc(nmemb, size); }

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t* f_data, size_t f_size) {
	if(f_size<60) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    // 1. Initialize curl with custom memory functions
    CURLcode global_init_res = curl_global_init_mem(CURL_GLOBAL_ALL,
                                                     id_000025_custom_malloc,
                                                     id_000025_custom_free,
                                                     id_000025_custom_realloc,
                                                     id_000025_custom_strdup,
                                                     id_000025_custom_calloc);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // 2. Create a multi handle for non-blocking operations
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Create an easy handle for WebSocket operations
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // 4. Set up WebSocket URL (using a dummy URL since we're fuzzing)
    curl_easy_setopt(easy_handle, CURLOPT_URL, "ws://localhost:8080");
    curl_easy_setopt(easy_handle, CURLOPT_CONNECT_ONLY, 2L); // WebSocket mode

    // 5. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // 6. Create a MIME part and set headers
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Create headers list
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, fuzz_str_1);
            headers = curl_slist_append(headers, fuzz_str_2);
            
            // Use curl_mime_headers API
            curl_mime_headers(part, headers, 1); // take_ownership = 1
            
            // Set some mime data from input
            if (size > 0) {
                // Create a null-terminated string from input data
                char *str_data = (char *)malloc(size + 1);
                if (str_data) {
                    memcpy(str_data, data, size);
                    str_data[size] = '\0';
                    curl_mime_data(part, str_data, CURL_ZERO_TERMINATED);
                    free(str_data);
                }
            }
        }
        curl_mime_free(mime);
    }

    // 7. Iterate through easy options using curl_easy_option_next
    const struct curl_easyoption *option = curl_easy_option_next(NULL);
    char option_buffer[256];
    int option_count = 0;
    while (option && option_count < 5) { // Limit to 5 options
        // Use curl_msprintf to format option info
        curl_msprintf(option_buffer, "Option %d: %s", option_count, 
                     option->name ? option->name : "unnamed");
        option = curl_easy_option_next(option);
        option_count++;
    }

    // 8. Create a FILE* from input data for potential reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        
        // 9. Set up waitfd structure for curl_multi_waitfds
        struct curl_waitfd ufds[1];
        ufds[0].fd = fd;
        ufds[0].events = CURL_WAIT_POLLIN;
        ufds[0].revents = 0;
        
        unsigned int fd_count = 0;
        CURLMcode wait_res = curl_multi_waitfds(multi_handle, ufds, 1, &fd_count);
        
        // 10. Use curl_multi_strerror to get error string if needed
        if (wait_res != CURLM_OK) {
            const char *error_str = curl_multi_strerror(wait_res);
            // Could log error_str if needed
        }
        
        assert_file_closed(&in_file);;
    }

    // 11. Try to send WebSocket data if we have any
    if (size > 0) {
        size_t sent = 0;
        // Use first 8 bytes as flags if available, otherwise use default
        unsigned int flags = 0;
        if (size >= sizeof(unsigned int)) {
            memcpy(&flags, data, sizeof(unsigned int));
        }
        
        // Send the data via WebSocket
        curl_ws_send(easy_handle, data, size, &sent, fuzz_int64_t_3, fuzz_uint32_t_4);
        
        // Use curl_msprintf to format sent info
        char sent_buffer[100];
        curl_msprintf(sent_buffer, "Sent %zu of %zu bytes", sent, size);
    }

    // 12. Perform multi handle operations
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    // 13. Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    // 14. Create an output file (though not strictly needed for fuzzing)
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some info about the fuzz run
        char info_buffer[200];
        curl_msprintf(info_buffer, 
                     "Fuzz run completed. Input size: %zu, Global init result: %d", 
                     size, global_init_res);
        fwrite(info_buffer, 1, strlen(info_buffer), out_file);
        assert_file_closed(&out_file);;
    }

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
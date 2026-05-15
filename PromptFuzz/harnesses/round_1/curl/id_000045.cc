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
//<ID> 561
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":9,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_share_init","curl_share_strerror","curl_share_cleanup","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_timeout","curl_multi_socket_action","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_strnequal","curl_mime_headers","curl_slist_free_all","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_share_init","curl_share_strerror","curl_share_cleanup","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_timeout","curl_multi_socket_action","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_strnequal","curl_mime_headers","curl_slist_free_all","curl_mime_free","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize global state
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Open input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 1. Initialize share handle
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        // Use share_strerror to get error string (simulate potential error)
        const char *error_str = curl_share_strerror(CURLSHE_OK);
        (void)error_str; // Prevent unused variable warning
        
        // Clean up share handle
        curl_share_cleanup(share_handle);
    }
    
    // 2. Create multi handle for socket operations
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        // 3. Create easy handle to add to multi
        CURL *easy_handle = curl_easy_init();
        if (easy_handle) {
            // Set up a simple URL for testing
            curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost/test");
            
            // Create a file from input data for CURLOPT_READDATA
            fseek(in_file, 0, SEEK_SET);
            
            // Add easy handle to multi handle
            curl_multi_add_handle(multi_handle, easy_handle);
            
            // 4. Get timeout from multi handle
            long timeout_ms;
            CURLMcode timeout_result = curl_multi_timeout(multi_handle, &timeout_ms);
            (void)timeout_result;
            
            // 5. Process socket (using file descriptor from input file)
            int running_handles;
            int fd = fuzz_fileno(in_file);
            
            // Use the file descriptor as a socket (in real usage this would be actual socket)
            curl_multi_socket(multi_handle, fd, &running_handles);
            
            // Clean up easy handle
            curl_multi_remove_handle(multi_handle, easy_handle);
            curl_easy_cleanup(easy_handle);
        }
        
        // Clean up multi handle
        curl_multi_cleanup(multi_handle);
    }
    
    // 6. Create MIME part with headers
    // First create a MIME structure
    CURL *temp_easy = curl_easy_init();
    if (temp_easy) {
        struct curl_mime *mime = curl_mime_init(temp_easy);
        if (mime) {
            struct curl_mimepart *part = curl_mime_addpart(mime);
            if (part) {
                // Create headers list from input data
                // Use first part of input data as header string
                size_t header_len = size > 256 ? 256 : size;
                char *header_data = (char *)malloc(header_len + 1);
                if (header_data) {
                    fseek(in_file, 0, SEEK_SET);
                    fread(header_data, 1, header_len, in_file);
                    header_data[header_len] = '\0';
                    
                    // Create headers list
                    struct curl_slist *headers = NULL;
                    headers = curl_slist_append(headers, header_data);
                    
                    // Use curl_strnequal to compare header with expected value
                    const char *expected = "Content-Type:";
                    int cmp_result = curl_strnequal(header_data, fuzz_str_1, 
                                                  strlen(expected) < header_len ? strlen(expected) : header_len);
                    (void)cmp_result; // Prevent unused variable warning
                    
                    // Set headers for MIME part (take ownership = 0 to not free headers automatically)
                    if (headers) {
                        curl_mime_headers(part, headers, 0);
                        curl_slist_free_all(headers); // Free manually since take_ownership = 0
                    }
                    
                    free(header_data);
                }
            }
            curl_mime_free(mime);
        }
        curl_easy_cleanup(temp_easy);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
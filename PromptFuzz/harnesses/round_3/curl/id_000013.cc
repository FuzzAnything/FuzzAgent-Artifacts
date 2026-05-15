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
//<ID> 176
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_version","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_send","curl_multi_notify_disable","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_version","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_slist_append","curl_slist_append","curl_mime_headers","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_send","curl_multi_notify_disable","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to simulate an event involving sending MIME data with custom headers over a connection, while managing notifications and comparing version strings:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data for operations

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy and multi handles
    CURL *easy = curl_easy_init();
    CURLM *multi = curl_multi_init();
    if (!easy || !multi) {
        if (easy) curl_easy_cleanup(easy);
        if (multi) curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }
    
    // 1. Get curl version and compare with input data
    char *version = curl_version();
    size_t version_len = strlen(version);
    size_t compare_len = size > 100 ? 100 : size; // Compare first 100 bytes or less
    int version_match = curl_strnequal(version, reinterpret_cast<const char*>(data), 
                                      compare_len > version_len ? version_len : compare_len);
    (void)version_match; // Use result to avoid unused warning
    
    // 2. Create MIME structure and add headers
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Create headers from input data
            struct curl_slist *headers = nullptr;
            
            // Use first 50 bytes as header data (or less if input is smaller)
            size_t header_data_len = size > 50 ? 50 : size;
            char header_buf[256];
            snprintf(header_buf, sizeof(header_buf), "X-Custom-Header: %.*s",
                    static_cast<int>(header_data_len), data);
            
            headers = curl_slist_append(headers, header_buf);
            
            if (headers) {
                // Add more headers if we have enough data
                if (size > 100) {
                    char second_header[256];
                    snprintf(second_header, sizeof(second_header), 
                            "X-Another-Header: %.*s", 
                            static_cast<int>(size - 100 > 50 ? 50 : size - 100),
                            data + 100);
                    headers = curl_slist_append(headers, second_header);
                }
                
                // Set headers on mime part (taking ownership)
                curl_mime_headers(part, headers, 1);
                
                // Set some mime data
                size_t mime_data_len = size > 200 ? size - 200 : 0;
                if (mime_data_len > 0) {
                    curl_mime_data(part, reinterpret_cast<const char*>(data + 200), 
                                  mime_data_len);
                }
            }
        }
    }
    
    // 3. Set up easy handle for sending
    // Create a temporary file for output
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Set output to file
        curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
        
        // Set URL to a dummy value (won't actually connect in fuzzing)
        curl_easy_setopt(easy, CURLOPT_URL, "http://localhost/test");
        
        // Set MIME data if created
        if (mime) {
            curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
        }
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi, easy);
        
        // 4. Try to send data (simulate sending)
        // Use a portion of input data for sending
        size_t send_size = size > 300 ? 100 : (size > 200 ? size - 200 : 0);
        if (send_size > 0) {
            size_t sent = 0;
            // Create buffer for sending (use middle portion of input)
            const void *send_buf = data + 150;
            curl_easy_send(easy, send_buf, send_size, &sent);
            // Result ignored for fuzzing
        }
        
        // 5. Disable notifications on multi handle
        // Use a notification ID derived from input data
        unsigned int notification_id = 0;
        if (size > 0) {
            notification_id = static_cast<unsigned int>(data[0]) % 10;
        }
        curl_multi_notify_disable(multi, fuzz_uint32_t_1);
        
        // Perform a minimal multi operation
        int running_handles = 0;
        curl_multi_perform(multi, &running_handles);
        
        // Clean up file
        assert_file_closed(&out_file);;
        remove("output_file");
        
        // Remove handle from multi
        curl_multi_remove_handle(multi, easy);
    }
    
    // Clean up
    if (mime) {
        curl_mime_free(mime);
    }
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	return 0;
}
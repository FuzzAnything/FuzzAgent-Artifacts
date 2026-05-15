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
#include <mprintf.h>
}
//<ID> 802
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 2
//<Quality> {"density":6,"unique_branches":{"xfer_may_multiplex":[[566,8,566,72,0,0,4,1]],"detect_proxy":[[1897,8,1897,44,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_option_next","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_type","curl_mime_free","curl_easy_setopt","curl_ws_send","curl_multi_init","curl_multi_add_handle","curl_multi_assign","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_send","curl_url_strerror","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_option_next","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_type","curl_mime_free","curl_easy_setopt","curl_ws_send","curl_multi_init","curl_multi_add_handle","curl_multi_assign","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_send","curl_url_strerror","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs in a logical sequence to simulate WebSocket communication with MIME data handling:

*/


extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize libcurl
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a FILE pointer for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int in_fd = fuzz_fileno(in_file);
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    int out_fd = fuzz_fileno(out_file);

    // Variables to track progress
    size_t bytes_processed = 0;
    bool all_apis_called = false;

    // API 1: curl_easy_option_next - Get first easy option
    const struct curl_easyoption *first_option = curl_easy_option_next(nullptr);
    if (first_option) {
        // Use the option name (just for demonstration, not actually setting)
        const char *option_name = first_option->name;
        (void)option_name; // Avoid unused variable warning
    }

    // API 2: Create MIME data from input
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Create MIME structure
        curl_mime *mime = curl_mime_init(easy_handle);
        if (mime) {
            // Add a part to MIME
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part && size > 0) {
                // Use first half of data for MIME
                size_t mime_data_size = size / 2;
                const char *mime_data = reinterpret_cast<const char*>(data);
                
                // API 3: curl_mime_data
                CURLcode mime_result = curl_mime_data(part, mime_data, mime_data_size);
                if (mime_result == CURLE_OK) {
                    bytes_processed += mime_data_size;
                }
                
                // Set MIME type
                curl_mime_type(part, "application/octet-stream");
            }
            curl_mime_free(mime);
        }
    }

    // API 4: Setup WebSocket communication
    if (easy_handle && size > 0) {
        // Configure for WebSocket (simplified - actual setup would require more options)
        curl_easy_setopt(easy_handle, CURLOPT_URL, "ws://localhost:8080");
        
        // Use second half of data for WebSocket send
        size_t ws_data_size = size - (size / 2);
        const char *ws_data = reinterpret_cast<const char*>(data + (size / 2));
        
        // API 5: curl_ws_send
        size_t bytes_sent = 0;
        if (ws_data_size > 0) {
            CURLcode ws_result = curl_ws_send(easy_handle, ws_data, ws_data_size, 
                                              &bytes_sent, 0, CURLWS_BINARY);
            if (ws_result == CURLE_OK) {
                bytes_processed += bytes_sent;
            }
        }
    }

    // API 6: Setup multi handle and assign socket
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle && easy_handle) {
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // API 7: curl_multi_assign
        // Use the output file descriptor as a dummy socket
        curl_multi_assign(multi_handle, (curl_socket_t)out_fd, nullptr);
        
        // Perform the multi operations
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
        
        // Clean up multi handle
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_cleanup(multi_handle);
    }

    // API 8: Send data using curl_easy_send (simulating raw socket send)
    if (easy_handle && size > 0) {
        // Use a small portion of data for testing send
        size_t send_size = size > 100 ? 100 : size;
        size_t bytes_written = 0;
        CURLcode send_result = curl_easy_send(easy_handle, data, send_size, &bytes_written);
        if (send_result == CURLE_OK) {
            bytes_processed += bytes_written;
        }
    }

    // API 9: Get URL error string (for demonstration)
    CURLUcode url_error = CURLUE_OK;
    const char *error_str = curl_url_strerror(url_error);
    (void)error_str; // Avoid unused variable warning

    // Check if we used all required APIs
    all_apis_called = (first_option != nullptr) && 
                     (easy_handle != nullptr) && 
                     (multi_handle != nullptr);

    // Cleanup
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();

    // Verify we processed some data
    if (bytes_processed > 0 && all_apis_called) {
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0; // Success
    }
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0; // Still return 0 even if not all APIs were called (fuzzer might provide invalid input)
}
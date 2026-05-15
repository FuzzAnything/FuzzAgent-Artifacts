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
//<ID> 904
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 2
//<Quality> {"density":12,"unique_branches":{"curl_multi_remove_handle":[[890,8,890,39,0,0,4,1]],"out_number":[[795,16,795,40,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_easy_duphandle","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_waitfds","curl_multi_socket_action","curl_multi_remove_handle","curl_multi_remove_handle","curl_ws_meta","curl_easy_option_by_id","curl_easy_option_next","curl_easy_ssls_export","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_easy_duphandle","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_slist_append","curl_mime_headers","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_waitfds","curl_multi_socket_action","curl_multi_remove_handle","curl_multi_remove_handle","curl_ws_meta","curl_easy_option_by_id","curl_easy_option_next","curl_easy_ssls_export","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create easy handles
    CURL *easy1 = curl_easy_init();
    CURL *easy2 = curl_easy_duphandle(easy1);
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy1);
    curl_mimepart *mimepart = curl_mime_addpart(mime);
    
    // Use input data for mime part
    CURLcode mime_res = curl_mime_data(mimepart, reinterpret_cast<const char*>(data), size);
    
    // Create header list
    struct curl_slist *headers = nullptr;
    if (size > 0) {
        // Use first byte as character for header
        char header_buf[64];
        snprintf(header_buf, sizeof(header_buf), "X-Fuzz-Header: %c", data[0]);
        headers = curl_slist_append(headers, header_buf);
    }
    
    // Set headers for mime part
    if (headers) {
        curl_mime_headers(mimepart, headers, 1); // take ownership
    }
    
    // Set easy handle options
    if (easy1) {
        curl_easy_setopt(easy1, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy1, CURLOPT_MIMEPOST, mime);
        curl_easy_setopt(easy1, CURLOPT_UPLOAD, 1L);
        
        // Create input file from data
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            curl_easy_setopt(easy1, CURLOPT_READDATA, in_file);
            assert_file_closed(&in_file);;
        }
        
        // Create output file
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            curl_easy_setopt(easy1, CURLOPT_WRITEDATA, out_file);
            assert_file_closed(&out_file);;
        }
    }
    
    // Add handles to multi handle
    if (easy1 && multi_handle) {
        curl_multi_add_handle(multi_handle, easy1);
    }
    if (easy2 && multi_handle) {
        curl_multi_add_handle(multi_handle, easy2);
    }
    
    // Perform multi handle operations
    int running_handles = 0;
    if (multi_handle) {
        curl_multi_perform(multi_handle, &running_handles);
        
        // Use waitfds
        unsigned int fd_count = 0;
        struct curl_waitfd ufds[2];
        curl_multi_waitfds(multi_handle, ufds, 2, &fd_count);
        
        // Try socket action with file descriptor
        if (size > 0) {
            // Create a file from input data
            FILE *tmp_file = fmemopen((void *)data, size, "rb");
            if (tmp_file) {
                int fd = fuzz_fileno(tmp_file);
                curl_multi_socket(multi_handle, fd, &running_handles);
                assert_file_closed(&tmp_file);;
            }
        }
        
        // Remove handles
        if (easy1) {
            curl_multi_remove_handle(multi_handle, easy1);
        }
        if (easy2) {
            curl_multi_remove_handle(multi_handle, easy2);
        }
    }
    
    // Get WebSocket metadata (even though we're not using WebSocket)
    if (easy1) {
        const struct curl_ws_frame *ws_meta = curl_ws_meta(easy1);
        (void)ws_meta; // Suppress unused warning
    }
    
    // Use easy option APIs
    const struct curl_easyoption *opt = curl_easy_option_by_id(CURLOPT_URL);
    if (opt) {
        opt = curl_easy_option_next(opt);
    }
    
    // Try SSL session export (even though we're not using SSL)
    if (easy1) {
        // Create a dummy callback
        auto dummy_export = [](CURL *, void *, const char *, const unsigned char *,
                              size_t, const unsigned char *, size_t,
                              curl_off_t, int, const char *, size_t) -> CURLcode {
            return CURLE_OK;
        };
        curl_easy_ssls_export(easy1, dummy_export, nullptr);
    }
    
    // Cleanup
    if (mime) {
        curl_mime_free(mime);
    }
    
    if (easy1) {
        curl_easy_cleanup(easy1);
    }
    if (easy2) {
        curl_easy_cleanup(easy2);
    }
    
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    
    curl_global_cleanup();
    
    return 0;
}
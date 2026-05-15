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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 47.6, nr_unique_branch: 15
//<Quality> {"density":34,"unique_branches":{"value_range":[[835,6,835,26,0,0,4,1],[837,11,837,23,0,0,4,1],[839,11,839,23,0,0,4,1]],"setopt_long_bool":[[788,3,788,29,0,0,4,0]],"tcpkeepalive":[[118,16,118,39,0,0,4,0],[121,6,122,53,0,0,4,1],[195,8,196,55,0,0,4,1],[222,8,223,55,0,0,4,1],[257,8,258,55,0,0,4,1]],"Curl_node_take_elem":[[148,10,148,18,0,0,4,1]],"setopt_long_net":[[941,3,941,28,0,0,4,0],[943,8,943,15,0,0,4,0],[946,3,946,29,0,0,4,0],[948,8,948,15,0,0,4,0]],"cf_socket_open":[[1110,16,1110,39,0,0,4,0]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_get","curl_free","curl_getdate","curl_multi_init","curl_url_cleanup","curl_global_cleanup","curl_multi_notify_enable","curl_easy_init","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_type","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_get","curl_free","curl_getdate","curl_multi_init","curl_multi_notify_enable","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_url_get","curl_easy_setopt","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_type","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":4}
/*Here's a C++ fuzz driver that uses all the specified curl APIs to achieve the event of performing an HTTP POST request with multipart form data using curl's multi interface:

*/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<59) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Initialize curl global environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a URL handle
    CURLU *url = curl_url();
    if (!url) {
        curl_global_cleanup();
        return 0;
    }

    // Parse URL from input data (use first part as URL)
    size_t url_len = size > 100 ? 100 : size;
    char *url_str = (char*)malloc(url_len + 1);
    if (!url_str) {
        curl_url_cleanup(url);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_str, data, url_len);
    url_str[url_len] = '\0';
    
    CURLUcode url_set_result = curl_url_set(url, CURLUPART_URL, url_str, fuzz_uint32_t_1);
    free(url_str);
    
    // Get URL components
    char *host_part = nullptr;
    if (curl_url_get(url, CURLUPART_HOST, &host_part, fuzz_uint32_t_2) == CURLUE_OK) {
        curl_free(host_part);
    }

    // Parse date from remaining input data
    if (size > url_len) {
        char *date_str = (char*)malloc(size - url_len + 1);
        if (date_str) {
            memcpy(date_str, data + url_len, size - url_len);
            date_str[size - url_len] = '\0';
            time_t parsed_time = curl_getdate(date_str, nullptr);
            free(date_str);
        }
    }

    // Create multi handle and enable notifications
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_url_cleanup(url);
        curl_global_cleanup();
        return 0;
    }
    
    curl_multi_notify_enable(multi_handle, fuzz_uint32_t_3);

    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_url_cleanup(url);
        curl_global_cleanup();
        return 0;
    }

    // Set options for easy handle
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 1000L);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, 1000L);
    curl_easy_setopt(easy_handle, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(easy_handle, CURLOPT_TCP_KEEPINTVL, 60L);
    
    // Set URL from parsed URL
    char *full_url = nullptr;
    if (curl_url_get(url, CURLUPART_URL, &full_url, fuzz_uint32_t_4) == CURLUE_OK) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, full_url);
        curl_free(full_url);
    } else {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:8080/");
    }

    // Create multipart form data
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use input data as form field data
            char *field_data = (char*)malloc(size + 1);
            if (field_data) {
                memcpy(field_data, data, size);
                field_data[size] = '\0';
                curl_mime_data(part, field_data, size);
                free(field_data);
            }
            
            curl_mime_name(part, fuzz_str_5);
            curl_mime_type(part, fuzz_str_6);
        }
        
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Perform the multi handle operations
    int running_handles = 0;
    int retry_count = 0;
    
    do {
        CURLMcode perform_result = curl_multi_perform(multi_handle, &running_handles);
        
        if (running_handles) {
            // Wait for activity
            struct curl_waitfd extra_fds[1];
            extra_fds[0].fd = 0;
            extra_fds[0].events = CURL_WAIT_POLLIN;
            extra_fds[0].revents = 0;
            
            int numfds = 0;
            curl_multi_wait(multi_handle, extra_fds, 1, fuzz_int32_t_7, &numfds);
        }
        
        retry_count++;
    } while (running_handles && retry_count < 10);

    // Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_url_cleanup(url);
    curl_global_cleanup();

    return 0;
}
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
//<ID> 501
//<Prompt> []
/*<Combination>: [
*/
//<score> 12, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_multi_notify_enable","curl_share_init","curl_share_setopt","curl_easy_init","curl_url","curl_url_set","curl_url_set","curl_url_get","curl_free","curl_easy_setopt","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_waitfds","curl_multi_remove_handle","curl_unescape","curl_free","curl_mime_free","curl_url_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_multi_notify_enable","curl_share_init","curl_share_setopt","curl_easy_init","curl_url","curl_url_set","curl_url_set","curl_url_get","curl_free","curl_easy_setopt","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_waitfds","curl_multi_remove_handle","curl_unescape","curl_free","curl_mime_free","curl_url_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
	//fuzzer shim end}




    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create multi handle and enable notifications
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        curl_multi_notify_enable(multi_handle, CURL_POLL_IN | CURL_POLL_OUT);
    }
    
    // Create share handle and set option
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    }
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    
    // Create URL handle
    CURLU *url_handle = curl_url();
    
    // Parse input data for URL parts
    if (size > 0 && url_handle) {
        // Use first part of data for URL scheme
        size_t scheme_len = size > 10 ? 10 : size;
        char *scheme = (char *)malloc(scheme_len + 1);
        if (scheme) {
            memcpy(scheme, data, scheme_len);
            scheme[scheme_len] = '\0';
            curl_url_set(url_handle, CURLUPART_SCHEME, scheme, fuzz_uint32_t_1);
            free(scheme);
        }
        
        // Use second part for host (if enough data)
        if (size > 20) {
            size_t host_len = size > 30 ? 10 : size - 20;
            char *host = (char *)malloc(host_len + 1);
            if (host) {
                memcpy(host, data + 20, host_len);
                host[host_len] = '\0';
                curl_url_set(url_handle, CURLUPART_HOST, host, fuzz_uint32_t_2);
                
                // Get the host back
                char *retrieved_host = nullptr;
                curl_url_get(url_handle, CURLUPART_HOST, &retrieved_host, fuzz_uint32_t_3);
                if (retrieved_host) {
                    curl_free(retrieved_host);
                }
                free(host);
            }
        }
    }
    
    // Set easy handle options
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
        curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    }
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime && size > 0) {
        // Add mime part
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use data for mime content
            curl_mime_data(part, (const char *)data, size);
            curl_mime_name(part, fuzz_str_4);
            
            // Create sub-mime structure
            curl_mime *submime = curl_mime_init(easy_handle);
            if (submime) {
                curl_mimepart *subpart = curl_mime_addpart(submime);
                if (subpart && size > 10) {
                    curl_mime_data(subpart, (const char *)data + 10, 
                                  size > 10 ? size - 10 : 0);
                }
                curl_mime_subparts(part, submime);
            }
        }
        
        // Set mime to easy handle
        if (easy_handle) {
            curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
        }
    }
    
    // Add easy handle to multi handle
    if (multi_handle && easy_handle) {
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // Perform multi operations
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
        
        // Wait for file descriptors
        struct curl_waitfd ufds[1];
        unsigned int fd_count = 0;
        if (size > 0) {
            // Create a file from input data
            FILE *in_file = fmemopen((void *)data, size, "rb");
            if (in_file) {
                ufds[0].fd = fuzz_fileno(in_file);
                ufds[0].events = CURL_WAIT_POLLIN;
                ufds[0].revents = 0;
                curl_multi_waitfds(multi_handle, ufds, 1, &fd_count);
                assert_file_closed(&in_file);;
            }
        }
        
        // Remove handle from multi
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    // Test unescape function
    if (size > 0) {
        char *unescaped = curl_unescape((const char *)data, 
                                       size > 100 ? 100 : (int)size);
        if (unescaped) {
            curl_free(unescaped);
        }
    }
    
    // Clean up resources
    if (mime) {
        curl_mime_free(mime);
    }
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (share_handle) {
        curl_share_cleanup(share_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    curl_global_cleanup();
    
    return 0;
}
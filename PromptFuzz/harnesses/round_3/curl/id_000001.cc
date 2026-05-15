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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 2
//<Quality> {"density":26,"unique_branches":{"curl_share_setopt":[[218,5,218,28,0,0,4,0],[221,5,221,31,0,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_multi_notify_enable","curl_share_init","curl_share_setopt","curl_easy_init","curl_easy_setopt","curl_url","curl_url_set","curl_url_get","curl_free","curl_url_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_addpart","curl_mime_subparts","curl_mime_name","curl_mime_free","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_multi_perform","curl_easy_getinfo","curl_easy_nextheader","curl_easy_nextheader","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_multi_notify_enable","curl_share_init","curl_share_setopt","curl_easy_init","curl_easy_setopt","curl_url","curl_url_set","curl_url_get","curl_free","curl_url_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_addpart","curl_mime_subparts","curl_mime_name","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_multi_perform","curl_easy_getinfo","curl_easy_nextheader","curl_easy_nextheader","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<59) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    // Initialize global curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Enable notifications for the multi handle
    // Using CURL_POLL_IN (0x1) as notification type
    curl_multi_notify_enable(multi_handle, fuzz_uint32_t_1);
    
    // Create a share handle
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        // Set share options - share DNS cache
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    }
    
    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Set share handle for easy handle
        if (share_handle) {
            curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
        }
        
        // Create a URL handle
        CURLU *url_handle = curl_url();
        if (url_handle) {
            // Use input data to create URL string (truncate to avoid overflow)
            char url_buf[256];
            size_t url_len = size < 255 ? size : 255;
            memcpy(url_buf, data, url_len);
            url_buf[url_len] = '\0';
            
            // Set URL in URL handle
            curl_url_set(url_handle, CURLUPART_URL, url_buf, fuzz_uint32_t_2);
            
            // Get a part of the URL (host)
            char *host_part = nullptr;
            curl_url_get(url_handle, CURLUPART_HOST, &host_part, fuzz_uint32_t_3);
            
            // Free the host part if allocated
            if (host_part) {
                curl_free(host_part);
            }
            
            // Clean up URL handle
            curl_url_cleanup(url_handle);
        }
        
        // Create MIME structure
        curl_mime *mime = curl_mime_init(easy_handle);
        if (mime) {
            // Create first mime part
            curl_mimepart *part1 = curl_mime_addpart(mime);
            if (part1) {
                // Use part of input data for mime data
                size_t mime_data_size = size > 100 ? 100 : size;
                const char *mime_data = reinterpret_cast<const char*>(data);
                
                // Set mime data
                curl_mime_data(part1, mime_data, mime_data_size);
                curl_mime_name(part1, fuzz_str_4);
            }
            
            // Create second mime part (as subpart)
            curl_mime *submime = curl_mime_init(easy_handle);
            if (submime) {
                curl_mimepart *subpart = curl_mime_addpart(submime);
                if (subpart) {
                    // Use another part of input data for subpart
                    size_t sub_data_offset = size > 50 ? 50 : 0;
                    size_t sub_data_size = size > sub_data_offset ? size - sub_data_offset : 0;
                    const char *sub_data = reinterpret_cast<const char*>(data + sub_data_offset);
                    
                    curl_mime_data(subpart, sub_data, sub_data_size);
                    curl_mime_name(subpart, fuzz_str_5);
                }
                
                // Create parent part for subparts
                curl_mimepart *parent_part = curl_mime_addpart(mime);
                if (parent_part) {
                    // Set subparts for parent part
                    curl_mime_subparts(parent_part, submime);
                    curl_mime_name(parent_part, fuzz_str_6);
                }
                
                // Note: submime is now owned by parent_part
            } else {
                if (submime) curl_mime_free(submime);
            }
            
            // Set MIME data for POST
            curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
        }
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
        
        // Poll on multi handle (with timeout 1ms for immediate return)
        struct curl_waitfd extra_fds;
        int ret = 0;
        int timeout_ms = 1;
        curl_multi_poll(multi_handle, &extra_fds, 0, fuzz_int32_t_7, &ret);
        
        // Try to perform (won't actually connect due to no URL set)
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
        
        // Get info from easy handle
        char *effective_url = nullptr;
        curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);
        
        // Get headers from easy handle
        struct curl_header *header = nullptr;
        // First header
        header = curl_easy_nextheader(easy_handle, CURLH_HEADER, fuzz_int32_t_8, nullptr);
        // Try to get next header
        if (header) {
            header = curl_easy_nextheader(easy_handle, CURLH_HEADER, fuzz_int32_t_9, header);
        }
        
        // Remove easy handle from multi
        curl_multi_remove_handle(multi_handle, easy_handle);
        
        // Clean up MIME structure
        if (mime) {
            curl_mime_free(mime);
        }
        
        // Clean up easy handle
        curl_easy_cleanup(easy_handle);
    }
    
    // Clean up share handle
    if (share_handle) {
        curl_share_cleanup(share_handle);
    }
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Clean up global curl
    curl_global_cleanup();
    
    return 0;
}
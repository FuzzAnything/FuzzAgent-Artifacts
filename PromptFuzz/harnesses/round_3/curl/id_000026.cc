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
//<ID> 354
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 3
//<Quality> {"density":32,"unique_branches":{"setopt_long_bool":[[471,8,471,22,0,0,4,1],[474,13,474,38,0,0,4,1],[494,8,494,15,0,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_share_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_init","curl_share_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_notify_enable","curl_share_setopt","curl_url","curl_url_set","curl_url_set","curl_url_get","curl_easy_setopt","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_url_get","curl_strequal","curl_unescape","curl_free","curl_free","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_share_init","curl_easy_init","curl_multi_notify_enable","curl_share_setopt","curl_url","curl_url_set","curl_url_set","curl_url_get","curl_easy_setopt","curl_free","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_poll","curl_url_get","curl_strequal","curl_unescape","curl_free","curl_free","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_share_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to achieve a scenario of setting up a multi-part HTTP POST request with URL handling and shared session data:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<55) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	//fuzzer shim end}




    if (size < 16) {
	return 0;
	}
	  // Need minimum data

    // 1. Global initialization
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	

    // 2. Create multi handle, share handle, and easy handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    CURLSH *share_handle = curl_share_init();
    if (!share_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_share_cleanup(share_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // 3. Enable notifications on multi handle
    CURLMcode notify_res = curl_multi_notify_enable(multi_handle, 
                                                    CURL_POLL_IN | CURL_POLL_OUT);
    if (notify_res != CURLM_OK) {
        // Continue anyway
    }

    // 4. Set share options
    CURLSHcode share_opt_res = curl_share_setopt(share_handle, 
                                                 CURLSHOPT_SHARE, 
                                                 CURL_LOCK_DATA_COOKIE);
    if (share_opt_res != CURLSHE_OK) {
        // Continue anyway
    }

    // 5. Create and set up URL handle
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Use first 8 bytes for scheme and host
        char scheme_host[9];
        size_t scheme_len = (size > 8) ? 8 : size;
        memcpy(scheme_host, data, scheme_len);
        scheme_host[scheme_len] = '\0';
        
        // Set URL parts
        CURLUcode url_set_res;
        url_set_res = curl_url_set(url_handle, CURLUPART_SCHEME, fuzz_str_1, fuzz_uint32_t_2);
        if (url_set_res == CURLUE_OK) {
            url_set_res = curl_url_set(url_handle, CURLUPART_HOST, scheme_host, fuzz_uint32_t_3);
        }
        
        // Get URL back
        char *full_url = nullptr;
        if (url_set_res == CURLUE_OK) {
            curl_url_get(url_handle, CURLUPART_URL, &full_url, fuzz_uint32_t_4);
        }
        
        // Set URL on easy handle if we got one
        if (full_url) {
            curl_easy_setopt(easy_handle, CURLOPT_URL, full_url);
            curl_free(full_url);
        }
    }

    // 6. Create mime structure for multipart data
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        // Create main part
        curl_mimepart *part1 = curl_mime_addpart(mime);
        if (part1) {
            // Use middle part of data for mime name
            size_t name_offset = (size / 3) % (size - 8);
            if (name_offset + 8 <= size) {
                char name_buf[9];
                memcpy(name_buf, data + name_offset, 8);
                name_buf[8] = '\0';
                curl_mime_name(part1, name_buf);
            }
            
            // Use remaining data for mime content
            size_t data_offset = (2 * size / 3);
            size_t data_len = size - data_offset;
            if (data_len > 0 && data_offset < size) {
                curl_mime_data(part1, 
                              reinterpret_cast<const char*>(data + data_offset),
                              data_len);
            }
        }
        
        // Create subparts mime structure
        curl_mime *submime = curl_mime_init(easy_handle);
        if (submime) {
            curl_mimepart *subpart = curl_mime_addpart(submime);
            if (subpart) {
                // Use first 4 bytes for subpart name
                char subname[5];
                size_t subname_len = (size > 4) ? 4 : size;
                memcpy(subname, data, subname_len);
                subname[subname_len] = '\0';
                curl_mime_name(subpart, subname);
                
                // Use next 4 bytes for subpart data
                if (size >= 8) {
                    curl_mime_data(subpart, 
                                  reinterpret_cast<const char*>(data + 4),
                                  4);
                }
            }
            
            // Add subparts to main part
            if (part1) {
                curl_mime_subparts(part1, submime);
            }
        }
        
        // Set mime data on easy handle
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }

    // 7. Set additional easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECT_ONLY, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 0L);

    // 8. Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    if (add_res == CURLM_OK) {
        // 9. Perform multi poll (non-blocking)
        struct curl_waitfd extra_fds[1];
        extra_fds[0].fd = 0;
        extra_fds[0].events = CURL_WAIT_POLLIN;
        extra_fds[0].revents = 0;
        
        int ret = 0;
        CURLMcode poll_res = curl_multi_poll(multi_handle, extra_fds, 1, fuzz_int32_t_5, &ret);
        
        // 10. Get URL again for comparison
        if (url_handle) {
            char *url_part = nullptr;
            CURLUcode get_res = curl_url_get(url_handle, CURLUPART_HOST, &url_part, fuzz_uint32_t_6);
            if (get_res == CURLUE_OK && url_part) {
                // 11. Use string comparison
                int equal = curl_strequal(url_part, fuzz_str_7);
                
                // 12. Use unescape on a sample string
                const char *encoded = "test%20string";
                char *decoded = curl_unescape(fuzz_str_8, static_cast<int>(fuzz_str_sz_8));
                if (decoded) {
                    curl_free(decoded);
                }
                
                curl_free(url_part);
            }
        }
        
        // 13. Perform one iteration
        int running_handles = 0;
        CURLMcode perform_res = curl_multi_perform(multi_handle, &running_handles);
        
        // 14. Remove handle from multi
        CURLMcode remove_res = curl_multi_remove_handle(multi_handle, easy_handle);
    }

    // 15. Cleanup resources
    if (mime) {
        curl_mime_free(mime);
    }
    
    curl_easy_cleanup(easy_handle);
    curl_share_cleanup(share_handle);
    curl_multi_cleanup(multi_handle);
    
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    curl_global_cleanup();
    
    return 0;
}
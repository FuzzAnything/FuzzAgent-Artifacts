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
//<ID> 463
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 1
//<Quality> {"density":22,"unique_branches":{"curl_multi_remove_handle":[[797,6,797,18,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_global_cleanup","curl_version","curl_getdate","curl_easy_unescape","curl_free","curl_easy_duphandle","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_multi_add_handle","curl_multi_perform","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_share_init","curl_share_setopt","curl_share_setopt","curl_share_cleanup","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_version","curl_getdate","curl_easy_unescape","curl_free","curl_easy_duphandle","curl_multi_init","curl_multi_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_multi_add_handle","curl_multi_perform","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_perform","curl_share_init","curl_share_setopt","curl_share_setopt","curl_share_cleanup","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize global curl environment
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        // If global init fails, we can't proceed
        return 0;
    }
    
    // 2. Set trace configuration from input data
    // Use first 100 bytes or size if smaller for trace config
    size_t config_len = size > 100 ? 100 : size;
    char *trace_config = (char *)malloc(config_len + 1);
    if (!trace_config) {
	return 0;
	}
	
    
    memcpy(trace_config, data, config_len);
    trace_config[config_len] = '\0';
    
    CURLcode trace_res = curl_global_trace(trace_config);
    free(trace_config);
    
    // 3. Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Get curl version info
    char *version = curl_version();
    // We just need to call it, we don't need to use the result
    
    // 5. Parse date from input data
    // Use next 100 bytes or remaining size for date string
    size_t date_offset = config_len < size ? config_len : 0;
    size_t date_len = size - date_offset > 100 ? 100 : size - date_offset;
    
    char *date_str = nullptr;
    if (date_len > 0) {
        date_str = (char *)malloc(date_len + 1);
        if (date_str) {
            memcpy(date_str, data + date_offset, date_len);
            date_str[date_len] = '\0';
            time_t parsed_date = curl_getdate(date_str, nullptr);
            free(date_str);
        }
    }
    
    // 6. Unescape a URL-encoded string from input data
    // Use next 100 bytes or remaining size for escape string
    size_t escape_offset = date_offset + date_len;
    size_t escape_len = size - escape_offset > 100 ? 100 : size - escape_offset;
    
    if (escape_len > 0) {
        char *escape_str = (char *)malloc(escape_len);
        if (escape_str) {
            memcpy(escape_str, data + escape_offset, escape_len);
            int outlength = 0;
            char *unescaped = curl_easy_unescape(easy_handle, escape_str, escape_len, &outlength);
            if (unescaped) {
                curl_free(unescaped);
            }
            free(escape_str);
        }
    }
    
    // 7. Duplicate the easy handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);
    
    // 8. Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        if (dup_handle) curl_easy_cleanup(dup_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 9. Set multi handle options
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 5L);
    
    // 10. Set easy handle options using remaining input data
    // Create a FILE* from remaining input data for CURLOPT_READDATA
    size_t file_offset = escape_offset + escape_len;
    size_t file_size = size - file_offset;
    
    if (file_size > 0) {
        FILE *in_file = fmemopen((void *)(data + file_offset), file_size, "rb");
        if (in_file) {
            // Set various options on the easy handle
            curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
            curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
            curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_size);
            
            // 11. Perform connection upkeep
            curl_easy_upkeep(easy_handle);
            
            // 12. Add easy handle to multi handle
            CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
            
            // 13. Perform the multi transfers
            int running_handles = 0;
            if (add_res == CURLM_OK) {
                CURLMcode perform_res = curl_multi_perform(multi_handle, &running_handles);
                // We don't need to loop since we're just fuzzing
            }
            
            // Also add the duplicated handle if it exists
            if (dup_handle) {
                curl_easy_setopt(dup_handle, CURLOPT_URL, "http://example2.com");
                curl_easy_setopt(dup_handle, CURLOPT_READDATA, in_file);
                curl_easy_setopt(dup_handle, CURLOPT_UPLOAD, 1L);
                curl_multi_add_handle(multi_handle, dup_handle);
                curl_multi_perform(multi_handle, &running_handles);
            }
            
            assert_file_closed(&in_file);;
        }
    }
    
    // 14. Create and configure a share handle
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
        
        // 15. Clean up share handle
        curl_share_cleanup(share_handle);
    }
    
    // 16. Clean up multi handle
    if (dup_handle) {
        curl_multi_remove_handle(multi_handle, dup_handle);
    }
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    curl_multi_cleanup(multi_handle);
    
    // 17. Clean up easy handles
    if (dup_handle) {
        curl_easy_cleanup(dup_handle);
    }
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    // 18. Global cleanup
    curl_global_cleanup();
    
    return 0;
}
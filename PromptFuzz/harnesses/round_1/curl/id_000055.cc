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
//<ID> 1059
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 2
//<Quality> {"density":24,"unique_branches":{"type_url_check":[[4395,5,4395,13,0,0,4,0],[4404,5,4404,12,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_trace","curl_version","curl_getdate","curl_easy_init","curl_global_cleanup","curl_easy_unescape","curl_url","curl_url_set","curl_url_dup","curl_url_cleanup","curl_url_cleanup","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_easy_duphandle","curl_share_init","curl_share_setopt","curl_multi_init","curl_multi_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_timeout","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version","curl_getdate","curl_easy_init","curl_easy_unescape","curl_url","curl_url_set","curl_url_dup","curl_url_cleanup","curl_url_cleanup","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_easy_duphandle","curl_share_init","curl_share_setopt","curl_multi_init","curl_multi_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_timeout","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // 1. Initialize global curl with default flags
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // 2. Set global trace configuration using input data
    if (size > 0) {
        char* trace_config = (char*)malloc(size + 1);
        if (trace_config) {
            memcpy(trace_config, data, size);
            trace_config[size] = '\0';
            curl_global_trace(trace_config);
            free(trace_config);
        }
    }
    
    // 3. Get and store curl version
    char* version = curl_version();
    
    // 4. Parse date from input data
    if (size > 0) {
        char* date_str = (char*)malloc(size + 1);
        if (date_str) {
            memcpy(date_str, data, size);
            date_str[size] = '\0';
            time_t parsed_time = curl_getdate(date_str, nullptr);
            free(date_str);
        }
    }
    
    // 5. Initialize easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // 6. Unescape some data
    if (size > 0) {
        int outlength = 0;
        char* unescaped = curl_easy_unescape(curl, (const char*)data, 
                                            size < INT_MAX ? (int)size : INT_MAX, 
                                            &outlength);
        if (unescaped) {
            // Use unescaped data for URL
            if (outlength > 0) {
                // 7. Create URL object and set URL part
                CURLU* url = curl_url();
                if (url) {
                    curl_url_set(url, CURLUPART_URL, unescaped, fuzz_uint32_t_1);
                    
                    // Duplicate the URL object
                    CURLU* url_dup = curl_url_dup(url);
                    if (url_dup) {
                        curl_url_cleanup(url_dup);
                    }
                    
                    curl_url_cleanup(url);
                }
                
                // 8. Set easy handle options with unescaped data
                curl_easy_setopt(curl, CURLOPT_URL, unescaped);
            }
            curl_free(unescaped);
        }
    }
    
    // 9. Set additional easy handle options
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    
    // 10. Perform easy handle upkeep
    curl_easy_upkeep(curl);
    
    // 11. Duplicate easy handle
    CURL* curl_dup = curl_easy_duphandle(curl);
    
    // 12. Initialize share handle
    CURLSH* share = curl_share_init();
    if (share) {
        // Set share options
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    }
    
    // 13. Initialize multi handle
    CURLM* multi = curl_multi_init();
    if (multi) {
        // 14. Set multi handle options
        curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, 5L);
        
        // 15. Add easy handle to multi handle
        curl_multi_add_handle(multi, curl);
        
        // 16. Perform multi handle operations
        int running_handles = 0;
        curl_multi_perform(multi, &running_handles);
        
        // 17. Get multi handle timeout
        long timeout = 0;
        curl_multi_timeout(multi, &timeout);
        
        // 18. If we have a duplicated handle, add it too
        if (curl_dup) {
            curl_multi_add_handle(multi, curl_dup);
            curl_multi_perform(multi, &running_handles);
            
            // Remove duplicated handle
            curl_multi_remove_handle(multi, curl_dup);
        }
        
        // Remove original handle
        curl_multi_remove_handle(multi, curl);
        
        // 19. Clean up multi handle
        curl_multi_cleanup(multi);
    }
    
    // 20. Clean up share handle
    if (share) {
        curl_share_cleanup(share);
    }
    
    // 21. Clean up duplicated easy handle
    if (curl_dup) {
        curl_easy_cleanup(curl_dup);
    }
    
    // 22. Clean up original easy handle
    curl_easy_cleanup(curl);
    
    // 23. Clean up global curl
    curl_global_cleanup();
    
    return 0;
}
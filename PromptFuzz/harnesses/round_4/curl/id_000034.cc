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
//<ID> 531
//<Prompt> []
/*<Combination>: [
*/
//<score> 39, nr_unique_branch: 2
//<Quality> {"density":26,"unique_branches":{"curl_multi_remove_handle":[[797,6,797,18,0,0,4,0]],"setopt_cptr":[[2171,3,2171,21,0,0,4,0]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_easy_init","curl_url_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_maprintf","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_notify_enable","curl_url_set","curl_url_get","curl_easy_init","curl_multi_wait","curl_multi_wakeup","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_free","curl_formfree","curl_multi_cleanup","curl_easy_cleanup","curl_mime_free","curl_url_cleanup","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_url_set","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_maprintf","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_notify_enable","curl_url_set","curl_url_get","curl_easy_init","curl_multi_wait","curl_multi_wakeup","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_free","curl_formfree","curl_multi_cleanup","curl_easy_cleanup","curl_mime_free","curl_url_cleanup","curl_free","curl_global_cleanup"],"visited":1}
/**/


// Include necessary headers

// Define notification constants if not already defined
#ifndef CURLM_NOTIFY_ON_CONNECT
#define CURLM_NOTIFY_ON_CONNECT (1 << 0)
#endif

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize curl global environment
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // 2. Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // 4. Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // 5. Create mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // 6. Use input data for mime data (first half for URL, second half for mime)
    size_t url_data_len = size / 2;
    size_t mime_data_len = size - url_data_len;
    
    // Prepare URL string from first half of data
    char *url_str = nullptr;
    if (url_data_len > 0) {
        url_str = static_cast<char*>(malloc(url_data_len + 1));
        if (url_str) {
            memcpy(url_str, data, url_data_len);
            url_str[url_data_len] = '\0';
            
            // Set URL
            CURLUcode url_set_res = curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_1);
            free(url_str);
        }
    }

    // Set mime data from second half of data
    if (mime_data_len > 0) {
        const char* mime_data = reinterpret_cast<const char*>(data + url_data_len);
        CURLcode mime_data_res = curl_mime_data(part, mime_data, mime_data_len);
    }

    // 7. Create subparts mime structure
    curl_mime *subparts = curl_mime_init(easy_handle);
    if (subparts) {
        curl_mimepart *subpart = curl_mime_addpart(subparts);
        if (subpart && size > 10) {
            // Use a small portion for subpart data
            curl_mime_data(subpart, reinterpret_cast<const char*>(data), 
                          size > 100 ? 100 : size);
        }
        
        // Attach subparts to main part
        curl_mime_subparts(part, subparts);
    }

    // 8. Create formatted string using curl_maprintf
    char *formatted_str = nullptr;
    if (size > 20) {
        const char* format_str = "Data: %.*s";
        formatted_str = curl_maprintf(format_str, 10, data);
    }

    // 9. Set easy handle options
    if (url_str) {
        curl_easy_setopt(easy_handle, CURLOPT_CURLU, url_handle);
    }
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);

    // 10. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        // 11. Enable notifications
        curl_multi_notify_enable(multi_handle, CURLM_NOTIFY_ON_CONNECT);
        
        // 12. Set URL again for testing
        if (size > 30) {
            char temp_url[31];
            memcpy(temp_url, data, 30);
            temp_url[30] = '\0';
            curl_url_set(url_handle, CURLUPART_URL, temp_url, fuzz_uint32_t_2);
        }
        
        // 13. Get URL back
        char *retrieved_url = nullptr;
        curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, fuzz_uint32_t_3);
        
        // 14. Create another easy handle
        CURL *easy_handle2 = curl_easy_init();
        if (easy_handle2) {
            // 15. Multi wait
            int ret = 0;
            curl_multi_wait(multi_handle, nullptr, 0, fuzz_int32_t_4, &ret);
            
            // 16. Multi wakeup
            curl_multi_wakeup(multi_handle);
            
            // 17. Multi perform
            int running_handles = 0;
            curl_multi_perform(multi_handle, &running_handles);
            
            // 18. Remove handle
            curl_multi_remove_handle(multi_handle, easy_handle2);
            
            curl_easy_cleanup(easy_handle2);
        }
        
        // Free retrieved URL if allocated
        if (retrieved_url) {
            curl_free(retrieved_url);
        }
        
        // 19. Form free (dummy call)
        curl_formfree(nullptr);
        
        // 20. Multi cleanup
        curl_multi_cleanup(multi_handle);
    }

    // 21. Cleanup easy handle
    curl_easy_cleanup(easy_handle);
    
    // 22. Cleanup mime
    curl_mime_free(mime);
    
    // 23. Cleanup URL handle
    curl_url_cleanup(url_handle);
    
    // 24. Free formatted string if allocated
    if (formatted_str) {
        curl_free(formatted_str);
    }
    
    // 25. Global cleanup
    curl_global_cleanup();

    return 0;
}
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
//<ID> 1056
//<Prompt> []
/*<Combination>: [
*/
//<score> 8.5, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["curl_global_init","curl_global_trace","curl_version","curl_getdate","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_easy_unescape","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_easy_duphandle","curl_easy_setopt","curl_easy_cleanup","curl_share_init","curl_share_setopt","curl_multi_init","curl_multi_setopt","curl_multi_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_timeout","curl_url","curl_url_set","curl_url_set","curl_url_dup","curl_url_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version","curl_getdate","curl_easy_init","curl_easy_unescape","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_easy_duphandle","curl_easy_setopt","curl_easy_cleanup","curl_share_init","curl_share_setopt","curl_multi_init","curl_multi_setopt","curl_multi_setopt","curl_multi_add_handle","curl_multi_perform","curl_multi_timeout","curl_url","curl_url_set","curl_url_set","curl_url_dup","curl_url_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* f_data, size_t f_size) {
	if(f_size<21) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Initialize global curl
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_init_res != CURLE_OK) {
	return 0;
	}
	

    // 2. Set global trace configuration
    const char *trace_config = "all"; // Simple trace config
    curl_global_trace(fuzz_str_1);

    // 3. Get curl version
    char *version_str = curl_version();
    if (version_str) {
        // Can be used but don't need to free
    }

    // 4. Parse date from input (use first part of data as date string)
    // Find null terminator in data for date string
    size_t date_len = 0;
    while (date_len < size && data[date_len] != 0) date_len++;
    
    if (date_len > 0 && date_len < size) {
        char *date_str = new char[date_len + 1];
        memcpy(date_str, data, date_len);
        date_str[date_len] = 0;
        time_t parsed_date = curl_getdate(date_str, nullptr);
        delete[] date_str;
    }

    // 5. Create a FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 6. Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 7. URL unescape (use middle part of data)
    size_t unescape_start = date_len + 1;
    if (unescape_start < size) {
        size_t unescape_len = 0;
        while (unescape_start + unescape_len < size && 
               data[unescape_start + unescape_len] != 0) {
            unescape_len++;
        }
        
        if (unescape_len > 0 && unescape_start + unescape_len < size) {
            const char *to_unescape = (const char *)(data + unescape_start);
            int outlength = 0;
            char *unescaped = curl_easy_unescape(easy, to_unescape, unescape_len, &outlength);
            if (unescaped) {
                curl_free(unescaped);
            }
        }
    }

    // 8. Set easy handle options
    curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy, CURLOPT_URL, "file:///dev/null");
    curl_easy_setopt(easy, CURLOPT_NOBODY, 1L);
    
    // 9. Perform easy handle upkeep
    curl_easy_upkeep(easy);
    
    // 10. Duplicate easy handle
    CURL *easy_dup = curl_easy_duphandle(easy);
    if (easy_dup) {
        // Use the duplicate briefly
        curl_easy_setopt(easy_dup, CURLOPT_NOBODY, 1L);
        curl_easy_cleanup(easy_dup);
    }

    // 11. Create share handle
    CURLSH *share = curl_share_init();
    if (share) {
        // Set some share options
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_CONNECT);
    }

    // 12. Create multi handle
    CURLM *multi = curl_multi_init();
    if (multi) {
        // Set multi handle options
        curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, 5L);
        curl_multi_setopt(multi, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi, easy);
        
        // Perform multi handle operations
        int running_handles = 0;
        curl_multi_perform(multi, &running_handles);
        
        // Get multi timeout
        long timeout = 0;
        curl_multi_timeout(multi, &timeout);
        
        // Remove and cleanup will happen later
    }

    // 13. Create URL handle
    CURLU *url = curl_url();
    if (url) {
        // Set URL part from input data (use end part)
        size_t url_part_start = 0;
        for (size_t i = 0; i < size; i++) {
            if (data[i] == 0) {
                url_part_start = i + 1;
                if (url_part_start < size) break;
            }
        }
        
        if (url_part_start < size) {
            size_t url_part_len = size - url_part_start;
            if (url_part_len > 0) {
                char *url_part = new char[url_part_len + 1];
                memcpy(url_part, data + url_part_start, url_part_len);
                url_part[url_part_len] = 0;
                
                // Try to set as host
                curl_url_set(url, CURLUPART_HOST, url_part, fuzz_uint32_t_2);
                
                // Also try as scheme if it looks like one
                if (url_part_len > 4 && memcmp(url_part, "http", 4) == 0) {
                    curl_url_set(url, CURLUPART_SCHEME, fuzz_str_3, fuzz_uint32_t_4);
                }
                
                delete[] url_part;
            }
        }
        
        // Duplicate URL handle
        CURLU *url_dup = curl_url_dup(url);
        if (url_dup) {
            curl_url_cleanup(url_dup);
        }
    }

    // 14. Cleanup in proper order
    if (multi) {
        if (easy) {
            curl_multi_remove_handle(multi, easy);
        }
        curl_multi_cleanup(multi);
    }
    
    if (share) {
        curl_share_cleanup(share);
    }
    
    if (easy) {
        curl_easy_cleanup(easy);
    }
    
    if (url) {
        curl_url_cleanup(url);
    }
    
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
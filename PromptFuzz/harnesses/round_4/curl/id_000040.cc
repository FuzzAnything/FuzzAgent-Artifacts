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
//<ID> 624
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 3
//<Quality> {"density":7,"unique_branches":{"curl_mvaprintf":[[1148,6,1148,27,0,0,4,1]],"curl_url_dup":[[1304,8,1304,19,11,0,4,0],[1306,10,1306,23,11,0,4,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_maprintf","curl_free","curl_easy_recv","curl_easy_escape","curl_unescape","curl_free","curl_free","curl_url","curl_url_set","curl_url_dup","curl_url_cleanup","curl_url_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_maprintf","curl_free","curl_easy_recv","curl_easy_escape","curl_unescape","curl_free","curl_free","curl_url","curl_url_set","curl_url_dup","curl_url_cleanup","curl_url_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create a CURL handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // 1. Use curl_maprintf with input data as format string
    // Create null-terminated string from input data for format
    char *format_str = nullptr;
    if (size > 0) {
        format_str = new char[size + 1];
        memcpy(format_str, data, size);
        format_str[size] = '\0';
        
        char *formatted = curl_maprintf("%s", format_str);
        if (formatted) {
            curl_free(formatted);
        }
        delete[] format_str;
    }
    
    // 2. Use curl_easy_recv - simulate receiving data
    // For fuzzing, we'll use the input as buffer to receive into
    if (size > 0) {
        size_t n = 0;
        // Create a buffer for receiving
        void *recv_buffer = malloc(size);
        if (recv_buffer) {
            // Note: curl_easy_recv requires an active connection
            // We'll call it but it will likely fail since we're not connected
            CURLcode recv_res = curl_easy_recv(curl, recv_buffer, size, &n);
            // Result is ignored since this is fuzzing
            free(recv_buffer);
        }
    }
    
    // 3. Use curl_easy_escape with input data
    if (size > 0) {
        // Create null-terminated string for escaping
        char *escape_str = new char[size + 1];
        memcpy(escape_str, data, size);
        escape_str[size] = '\0';
        
        char *escaped = curl_easy_escape(curl, escape_str, (int)size);
        if (escaped) {
            // 4. Use curl_unescape on the escaped string
            int outlength = 0;
            char *unescaped = curl_unescape(escaped, 0);
            if (unescaped) {
                curl_free(unescaped);
            }
            curl_free(escaped);
        }
        delete[] escape_str;
    }
    
    // 5. Use curl_url_dup - create and duplicate a URL handle
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Set some URL parts using input data
        if (size > 0) {
            // Use first 100 bytes or size for URL part
            size_t url_part_size = size > 100 ? 100 : size;
            char *url_part = new char[url_part_size + 1];
            memcpy(url_part, data, url_part_size);
            url_part[url_part_size] = '\0';
            
            // Try to set as host (will fail if not valid, but that's OK for fuzzing)
            curl_url_set(url_handle, CURLUPART_HOST, url_part, fuzz_uint32_t_1);
            delete[] url_part;
        }
        
        // Duplicate the URL handle
        CURLU *dup_url = curl_url_dup(url_handle);
        if (dup_url) {
            curl_url_cleanup(dup_url);
        }
        
        curl_url_cleanup(url_handle);
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}
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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 5.5, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_global_cleanup","curl_version","curl_getdate","curl_easy_unescape","curl_easy_duphandle","curl_multi_init","curl_multi_setopt","curl_multi_setopt","curl_easy_setopt","curl_easy_upkeep","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_version","curl_getdate","curl_easy_unescape","curl_easy_duphandle","curl_multi_init","curl_multi_setopt","curl_multi_setopt","curl_easy_setopt","curl_easy_upkeep","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_multi_remove_handle","curl_free","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/*Here's a fuzz driver that uses all the specified curl APIs in a meaningful way:

*/


// Include curl headers

// Helper function to create va_list for curl_mvsprintf
static int  id_000010_wrap_curl_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimal data for meaningful operations
    
    // Initialize global curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // 1. curl_global_trace - set trace configuration
    FILE *trace_file = fopen("trace_output.txt", "w");
    if (trace_file) {
        // Use part of input as trace config
        char trace_config[256];
        size_t trace_len = size > 255 ? 255 : size;
        memcpy(trace_config, data, trace_len);
        trace_config[trace_len] = '\0';
        curl_global_trace(trace_config);
        assert_file_closed(&trace_file);;
    }
    
    // 2. Create a CURL easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        assert_file_closed(&trace_file);
	return 0;
    }
    
    // 3. curl_version - get version string
    char *version = curl_version();
    
    // 4. curl_getdate - parse date from input
    char date_str[256];
    size_t date_len = size > 255 ? 255 : size;
    memcpy(date_str, data, date_len);
    date_str[date_len] = '\0';
    
    // Try to parse date
    time_t parsed_time = curl_getdate(date_str, NULL);
    
    // 5. curl_easy_unescape - unescape a URL-encoded string
    int outlength = 0;
    char *unescaped = nullptr;
    
    // Create a string to unescape (using middle portion of data)
    size_t unescape_offset = size / 3;
    size_t unescape_len = size > unescape_offset + 100 ? 100 : size - unescape_offset;
    
    if (unescape_len > 0) {
        char *to_unescape = (char*)malloc(unescape_len + 1);
        if (to_unescape) {
            memcpy(to_unescape, data + unescape_offset, unescape_len);
            to_unescape[unescape_len] = '\0';
            
            // Convert some characters to %-encoding for unescaping
            for (size_t i = 0; i < unescape_len && i < 20; i++) {
                if (to_unescape[i] == ' ') to_unescape[i] = '+';
            }
            
            unescaped = curl_easy_unescape(easy, to_unescape, unescape_len, &outlength);
            free(to_unescape);
        }
    }
    
    // 6. curl_easy_duphandle - duplicate the easy handle
    CURL *easy_dup = curl_easy_duphandle(easy);
    
    // 7. curl_mvsprintf - formatted string output with va_list
    char buffer[1024];
    if (unescaped) {
        // Use  id_000010_wrap_curl_mvsprintf to handle va_list
         id_000010_wrap_curl_mvsprintf(buffer, 
                           "Curl version: %s, Parsed time: %ld, Unescaped: %s", 
                           version, 
                           (long)parsed_time, 
                           unescaped);
    } else {
         id_000010_wrap_curl_mvsprintf(buffer, 
                           "Curl version: %s, Parsed time: %ld", 
                           version, 
                           (long)parsed_time);
    }
    
    // 8. Create multi handle for curl_multi_setopt
    CURLM *multi = curl_multi_init();
    if (multi) {
        // 9. curl_multi_setopt - set multi handle option
        // Use some data to determine which option value to use
        long maxconn = data[0] % 100 + 1;  // Random value between 1-100
        curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, maxconn);
        
        // Also set other options based on input data
        if (size > 1) {
            long pipelining = data[1] % 3;  // 0, 1, or 2
            curl_multi_setopt(multi, CURLMOPT_PIPELINING, pipelining);
        }
    }
    
    // 10. curl_easy_upkeep - perform connection upkeep if supported
    // First set up a dummy URL to have a connection context
    if (easy_dup) {
        // Set a dummy option so upkeep has context
        curl_easy_setopt(easy_dup, CURLOPT_URL, "http://example.com");
        curl_easy_upkeep(easy_dup);
    }
    
    // If we have a multi handle and both easy handles, add them
    if (multi && easy && easy_dup) {
        curl_multi_add_handle(multi, easy);
        curl_multi_add_handle(multi, easy_dup);
        
        // Perform a single iteration
        int running_handles = 0;
        curl_multi_perform(multi, &running_handles);
        
        // Clean up from multi handle
        curl_multi_remove_handle(multi, easy);
        curl_multi_remove_handle(multi, easy_dup);
    }
    
    // Cleanup
    if (unescaped) {
        curl_free(unescaped);
    }
    
    if (easy_dup) {
        curl_easy_cleanup(easy_dup);
    }
    
    if (easy) {
        curl_easy_cleanup(easy);
    }
    
    if (multi) {
        curl_multi_cleanup(multi);
    }
    
    curl_global_cleanup();
    
    assert_file_closed(&trace_file);
	return 0;
}
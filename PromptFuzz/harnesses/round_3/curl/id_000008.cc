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
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 1
//<Quality> {"density":8,"unique_branches":{"Curl_close":[[217,31,217,42,0,0,4,0]]},"library_calls":["curl_global_trace","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_multi_init","curl_multi_timeout","curl_easy_init","curl_ws_meta","curl_easy_setopt","curl_multi_add_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_trace","curl_url","curl_url_set","curl_url_dup","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_multi_init","curl_multi_timeout","curl_easy_init","curl_ws_meta","curl_easy_setopt","curl_multi_add_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<11) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // Initialize global trace with config from input data
    char trace_config[256] = {0};
    size_t config_len = size < sizeof(trace_config) ? size : sizeof(trace_config)-1;
    memcpy(trace_config, data, config_len);
    trace_config[config_len] = '\0';
    
    CURLcode trace_res = curl_global_trace(trace_config);
    
    // Create an initial CURLU handle
    CURLU *url_handle = curl_url();
    if (url_handle) {
        // Set a URL part using input data
        char url_part[256] = {0};
        size_t url_len = size < sizeof(url_part) ? size : sizeof(url_part)-1;
        memcpy(url_part, data, url_len);
        url_part[url_len] = '\0';
        
        // Set scheme part
        curl_url_set(url_handle, CURLUPART_SCHEME, fuzz_str_1, fuzz_uint32_t_2);
        
        // Duplicate the URL handle
        CURLU *dup_url = curl_url_dup(url_handle);
        
        // Create and free a mime structure
        CURL *easy = curl_easy_init();
        if (easy) {
            // Create mime structure
            curl_mime *mime = curl_mime_init(easy);
            if (mime) {
                // Add a mime part with data from input
                curl_mimepart *part = curl_mime_addpart(mime);
                if (part) {
                    // Use input data for mime
                    size_t mime_data_len = size < 100 ? size : 100;
                    curl_mime_data(part, (const char*)data, mime_data_len);
                }
                // Free mime structure
                curl_mime_free(mime);
            }
            curl_easy_cleanup(easy);
        }
        
        // Clean up duplicated URL handle
        if (dup_url) {
            curl_url_cleanup(dup_url);
        }
        
        // Clean up original URL handle
        curl_url_cleanup(url_handle);
    }
    
    // Create a multi handle for timeout query
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        long timeout_ms = 0;
        CURLMcode timeout_res = curl_multi_timeout(multi_handle, &timeout_ms);
        
        // Create a CURL handle for WebSocket metadata
        CURL *ws_easy = curl_easy_init();
        if (ws_easy) {
            // Try to get WebSocket metadata (will be NULL since no WS connection)
            const struct curl_ws_frame *meta = curl_ws_meta(ws_easy);
            
            // Set WebSocket options (though we won't actually connect)
            curl_easy_setopt(ws_easy, CURLOPT_WS_OPTIONS, CURLWS_RAW_MODE);
            
            // Add handle to multi
            if (ws_easy) {
                curl_multi_add_handle(multi_handle, ws_easy);
            }
            
            // Clean up WebSocket handle
            curl_easy_cleanup(ws_easy);
        }
        
        // Clean up multi handle
        curl_multi_cleanup(multi_handle);
    }
    
    // Perform global cleanup
    curl_global_cleanup();
    
    return 0;
}
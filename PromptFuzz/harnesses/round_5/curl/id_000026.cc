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
//<ID> 455
//<Prompt> []
/*<Combination>: [
*/
//<score> 13, nr_unique_branch: 0
//<Quality> {"density":13,"unique_branches":{},"library_calls":["curl_global_init","curl_getenv","curl_free","curl_share_init","curl_share_setopt","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_escape","curl_easy_setopt","curl_free","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_free","curl_share_init","curl_share_setopt","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_escape","curl_easy_setopt","curl_free","curl_multi_add_handle","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Initialize global curl environment
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // 2. Get environment variable
    char *env_var = curl_getenv("HOME");
    if (env_var) {
        curl_free(env_var);
    }
    
    // 3. Create share handle and set options
    CURLSH *share = curl_share_init();
    if (share) {
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    }
    
    // 4. Create multi handle
    CURLM *multi = curl_multi_init();
    
    // 5. Create easy handle
    CURL *easy = curl_easy_init();
    
    if (easy && multi) {
        // 6. Use input data as file content
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            FILE *out_file = fopen("output_file", "wb");
            
            if (out_file) {
                // 7. Set options on easy handle
                curl_easy_setopt(easy, CURLOPT_SHARE, share);
                curl_easy_setopt(easy, CURLOPT_READDATA, in_file);
                curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
                curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
                
                // Use part of data for URL (limit to 100 bytes for safety)
                size_t url_len = size > 100 ? 100 : size;
                char *escaped_url = curl_easy_escape(easy, (const char *)data, url_len);
                if (escaped_url) {
                    // Create a dummy URL with escaped data
                    char url[256];
                    snprintf(url, sizeof(url), "file://input_file?data=%s", escaped_url);
                    curl_easy_setopt(easy, CURLOPT_URL, url);
                    curl_free(escaped_url);
                } else {
                    curl_easy_setopt(easy, CURLOPT_URL, "file://input_file");
                }
                
                // 8. Add easy handle to multi handle
                curl_multi_add_handle(multi, easy);
                
                // 9. Use multi socket interface
                int running_handles = 0;
                curl_multi_socket_all(multi, &running_handles);
                
                // 10. Cleanup file handles
                assert_file_closed(&out_file);;
            }
            assert_file_closed(&in_file);;
        }
        
        // 11. Remove easy handle from multi handle
        curl_multi_remove_handle(multi, easy);
    }
    
    // 12. Cleanup resources
    if (easy) {
        curl_easy_cleanup(easy);
    }
    
    if (multi) {
        curl_multi_cleanup(multi);
    }
    
    if (share) {
        curl_share_cleanup(share);
    }
    
    curl_global_cleanup();
    
    return 0;
}
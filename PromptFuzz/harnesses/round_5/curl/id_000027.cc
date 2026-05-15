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
//<ID> 456
//<Prompt> []
/*<Combination>: [
*/
//<score> 22, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["curl_global_init","curl_getenv","curl_free","curl_share_init","curl_global_cleanup","curl_share_setopt","curl_share_setopt","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_escape","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_getenv","curl_free","curl_share_init","curl_share_setopt","curl_share_setopt","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_escape","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a fuzzer that uses the specified curl APIs to test multi-handle transfers with shared data and URL escaping:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Initialize global curl environment
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // 2. Get environment variable
    char *env_var = curl_getenv("HOME");
    if (env_var) {
        // Free the environment variable string
        curl_free(env_var);
    }
    
    // 3. Create a share object
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Set share options - share cookies and DNS cache
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    
    // 5. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // 6. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // 7. Set easy handle options
    // Use share object
    curl_easy_setopt(easy_handle, CURLOPT_SHARE, share);
    
    // Set URL using input data (create a simple URL with the data as query parameter)
    // First, escape the input data to make it URL-safe
    char *escaped_data = curl_easy_escape(easy_handle, 
                                          reinterpret_cast<const char*>(data), 
                                          static_cast<int>(size > 100 ? 100 : size));
    
    if (escaped_data) {
        // Create a URL with the escaped data
        char url[256];
        snprintf(url, sizeof(url), "file://input_file?data=%s", escaped_data);
        
        // Set the URL
        curl_easy_setopt(easy_handle, CURLOPT_URL, url);
        
        // Free the escaped string
        curl_free(escaped_data);
    } else {
        // Use a default URL if escaping fails
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://input_file");
    }
    
    // Set other options
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    
    // Create input file from the data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        // Get file descriptor
        int fd = fuzz_fileno(in_file);
        
        // Set read function and data (simplified - in real usage you'd set CURLOPT_READFUNCTION)
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
        
        // 8. Add easy handle to multi handle
        CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
        
        if (add_res == CURLM_OK) {
            int running_handles = 0;
            
            // 9. Perform socket operations (simplified version)
            CURLMcode socket_res = curl_multi_socket_all(multi_handle, &running_handles);
            
            // In a real fuzzer, we would loop here, but for simplicity we just call it once
            // Note: curl_multi_socket_all is deprecated but we use it as requested
            
            // 10. Remove handle from multi handle
            curl_multi_remove_handle(multi_handle, easy_handle);
        }
        
        assert_file_closed(&in_file);;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write some dummy data
        fwrite(data, 1, size > 100 ? 100 : size, out_file);
        assert_file_closed(&out_file);;
    }
    
    // 11. Clean up
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_share_cleanup(share);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
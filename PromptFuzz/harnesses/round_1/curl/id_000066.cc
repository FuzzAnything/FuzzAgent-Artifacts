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
//<ID> 1304
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 1
//<Quality> {"density":19,"unique_branches":{"curl_share_setopt":[[245,5,245,36,0,0,4,0]]},"library_calls":["curl_global_init","curl_version","curl_version_info","curl_getdate","curl_share_init","curl_share_setopt","curl_share_setopt","curl_multi_init","curl_easy_init","curl_easy_unescape","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_duphandle","curl_multi_add_handle","curl_multi_add_handle","curl_msprintf","curl_multi_perform","curl_multi_poll","curl_multi_socket_action","curl_multi_strerror","curl_easy_reset","curl_multi_remove_handle","curl_multi_remove_handle","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version","curl_version_info","curl_getdate","curl_share_init","curl_share_setopt","curl_share_setopt","curl_multi_init","curl_easy_init","curl_easy_unescape","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_duphandle","curl_multi_add_handle","curl_multi_add_handle","curl_msprintf","curl_multi_perform","curl_multi_poll","curl_multi_socket_action","curl_multi_strerror","curl_easy_reset","curl_multi_remove_handle","curl_multi_remove_handle","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Include curl headers

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize global curl environment
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_init_result != CURLE_OK) {
        return 0;
    }

    // 2. Get curl version info
    char *version_str = curl_version();
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    
    // 3. Parse date from input data (use first reasonable chunk)
    const char* date_str = reinterpret_cast<const char*>(data);
    size_t date_len = size > 256 ? 256 : size;
    std::string date_input(date_str, date_len);
    time_t parsed_time = curl_getdate(date_input.c_str(), nullptr);

    // 4. Create share handle and set options
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
        curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_SSL_SESSION);
    }

    // 5. Create multi handle
    CURLM *multi_handle = curl_multi_init();

    // 6. Create easy handle
    CURL *easy_handle = curl_easy_init();
    CURL *dup_handle = nullptr;
    
    if (easy_handle && multi_handle) {
        // 7. Set easy handle options using input data
        // Use a portion of input as URL
        size_t url_len = size > 512 ? 512 : size;
        std::string url_data(reinterpret_cast<const char*>(data), url_len);
        
        // Unescape some data
        int outlength = 0;
        char* unescaped = curl_easy_unescape(easy_handle, 
                                           url_data.c_str(), 
                                           static_cast<int>(url_len), 
                                           &outlength);
        
        if (unescaped) {
            // Use unescaped data for something
            curl_free(unescaped);
        }

        // Create input file from data
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // Get file descriptor
            int fd = fuzz_fileno(in_file);
            
            // Create output file
            FILE *out_file = fopen("output_file", "wb");
            
            if (out_file) {
                // 8. Set various easy handle options
                curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
                curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
                curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
                curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
                curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
                
                // 9. Duplicate handle
                dup_handle = curl_easy_duphandle(easy_handle);
                
                // 10. Add handle to multi
                CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
                
                if (add_result == CURLM_OK && dup_handle) {
                    // Also add duplicated handle
                    curl_multi_add_handle(multi_handle, dup_handle);
                }
                
                // 11. Use msprintf
                char buffer[256];
                curl_msprintf(buffer, "Curl version: %s", version_str ? version_str : "unknown");
                
                // 12. Multi perform loop
                int running_handles = 0;
                int ret = 0;
                
                // Initial perform
                CURLMcode perform_result = curl_multi_perform(multi_handle, &running_handles);
                
                // Setup waitfd for our input file
                struct curl_waitfd wait_fd;
                wait_fd.fd = fd;
                wait_fd.events = CURL_WAIT_POLLIN;
                wait_fd.revents = 0;
                
                // Poll
                CURLMcode poll_result = curl_multi_poll(multi_handle, 
                                                       &wait_fd, 
                                                       1, // one extra fd
                                                       fuzz_int32_t_1, // timeout ms
                                                       &ret);
                
                // Socket action
                CURLMcode socket_result = curl_multi_socket_action(multi_handle, 
                                                                  fd, 
                                                                  fuzz_int32_t_2, 
                                                                  &running_handles);
                
                // Check for multi errors
                if (perform_result != CURLM_OK) {
                    const char* error_str = curl_multi_strerror(perform_result);
                    // Could log error here
                }
                
                // Close files
                assert_file_closed(&in_file);;
                assert_file_closed(&out_file);;
                
                // Remove output file (cleanup)
                remove("output_file");
            } else {
                assert_file_closed(&in_file);;
            }
        }
        
        // 13. Reset easy handle
        curl_easy_reset(easy_handle);
        
        // 14. Remove handles from multi
        if (dup_handle) {
            curl_multi_remove_handle(multi_handle, dup_handle);
        }
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    // 15. Cleanup in reverse order
    if (dup_handle) {
        curl_easy_cleanup(dup_handle);
    }
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
    if (share_handle) {
        curl_share_cleanup(share_handle);
    }
    
    curl_global_cleanup();
    
    return 0;
}
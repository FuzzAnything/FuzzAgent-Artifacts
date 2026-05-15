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
//<ID> 1301
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 0
//<Quality> {"density":30,"unique_branches":{},"library_calls":["curl_global_init","curl_version","curl_version_info","curl_share_init","curl_share_setopt","curl_share_setopt","curl_multi_init","curl_share_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_getdate","curl_easy_unescape","curl_free","curl_msprintf","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_duphandle","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_strerror","curl_multi_poll","curl_multi_socket_action","curl_multi_perform","curl_multi_socket_action","curl_multi_remove_handle","curl_easy_reset","curl_easy_cleanup","curl_multi_remove_handle","curl_easy_reset","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version","curl_version_info","curl_share_init","curl_share_setopt","curl_share_setopt","curl_multi_init","curl_easy_init","curl_getdate","curl_easy_unescape","curl_free","curl_msprintf","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_duphandle","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_strerror","curl_multi_poll","curl_multi_socket_action","curl_multi_perform","curl_multi_socket_action","curl_multi_remove_handle","curl_easy_reset","curl_easy_cleanup","curl_multi_remove_handle","curl_easy_reset","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    // 1. Initialize global curl
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }
    
    // 2. Get curl version information
    char *version_str = curl_version();
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    
    // 3. Initialize share object
    CURLSH *share = curl_share_init();
    if (share) {
        // Set share options - using cookie sharing as example
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
        curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_DNS);
    }
    
    // 4. Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        if (share) curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // 5. Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        if (share) curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }
    
    // 6. Use input data for various purposes
    if (size > 0) {
        // Create a FILE* for reading input data
        FILE *in_file = fmemopen((void *)data, size, "rb");
        if (in_file) {
            // Create output file
            FILE *out_file = fopen("output_file", "wb");
            
            // Get file descriptors
            int in_fd = fuzz_fileno(in_file);
            int out_fd = out_file ? fuzz_fileno(out_file) : -1;
            
            // Create a null-terminated string from input data for various APIs
            // Ensure we don't overflow by limiting to size
            char *input_str = (char *)malloc(size + 1);
            if (input_str) {
                memcpy(input_str, data, size);
                input_str[size] = '\0';
                
                // Use curl_getdate with input string
                time_t parsed_date = curl_getdate(input_str, NULL);
                
                // Use curl_easy_unescape
                int outlength = 0;
                char *unescaped = curl_easy_unescape(easy_handle, input_str, size, &outlength);
                if (unescaped) {
                    curl_free(unescaped);
                }
                
                // Use curl_msprintf
                char buffer[256];
                curl_msprintf(buffer, "Input size: %zu, Version: %s", size, version_str ? version_str : "unknown");
                
                free(input_str);
            }
            
            // Set easy handle options using input data
            if (size >= 10) {
                // Use first 10 bytes as a URL (just for demonstration, actual URL would need proper format)
                char url[11];
                memcpy(url, data, 10);
                url[10] = '\0';
                curl_easy_setopt(easy_handle, CURLOPT_URL, url);
            }
            
            // Set share object for easy handle
            if (share) {
                curl_easy_setopt(easy_handle, CURLOPT_SHARE, share);
            }
            
            // Set input/output file descriptors
            curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
            if (out_file) {
                curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
            }
            
            // Set other options
            curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
            curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
            
            // 7. Duplicate easy handle
            CURL *dup_handle = curl_easy_duphandle(easy_handle);
            
            // 8. Add handles to multi handle
            if (dup_handle) {
                curl_multi_add_handle(multi_handle, dup_handle);
            }
            curl_multi_add_handle(multi_handle, easy_handle);
            
            // 9. Perform multi operations
            int running_handles = 0;
            int ret = 0;
            
            // Initial perform
            CURLMcode multi_perform_res = curl_multi_perform(multi_handle, &running_handles);
            const char *multi_error = curl_multi_strerror(multi_perform_res);
            
            // Poll for activity (timeout of 100ms)
            struct curl_waitfd extra_fds[1];
            extra_fds[0].fd = in_fd;
            extra_fds[0].events = CURL_WAIT_POLLIN;
            extra_fds[0].revents = 0;
            
            curl_multi_poll(multi_handle, extra_fds, 1, fuzz_int32_t_1, &ret);
            
            // Socket action (using in_fd as socket, though it's not a real socket)
            curl_multi_socket_action(multi_handle, in_fd, fuzz_int32_t_2, &running_handles);
            
            // Another perform to complete any pending operations
            curl_multi_perform(multi_handle, &running_handles);
            
            // Another socket action
            curl_multi_socket_action(multi_handle, out_fd, fuzz_int32_t_3, &running_handles);
            
            // 10. Clean up duplicate handle first
            if (dup_handle) {
                curl_multi_remove_handle(multi_handle, dup_handle);
                curl_easy_reset(dup_handle);
                curl_easy_cleanup(dup_handle);
            }
            
            // 11. Reset and clean up original easy handle
            curl_multi_remove_handle(multi_handle, easy_handle);
            curl_easy_reset(easy_handle);
            
            // Clean up file handles
            assert_file_closed(&in_file);;
            if (out_file) {
                assert_file_closed(&out_file);;
            }
        }
    }
    
    // 12. Final cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    if (share) {
        curl_share_cleanup(share);
    }
    curl_global_cleanup();
    
    return 0;
}
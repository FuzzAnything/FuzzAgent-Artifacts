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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 73
//<Quality> {"density":18,"unique_branches":{"curl_multi_get_offt":[[3828,6,3828,24,0,0,4,1],[3830,6,3830,13,0,0,4,1],[3834,3,3834,31,0,0,4,1],[3840,3,3840,31,0,0,4,1],[3846,3,3846,31,0,0,4,1],[3849,3,3849,28,0,0,4,1],[3852,3,3852,29,0,0,4,1],[3855,3,3855,10,0,0,4,0],[83,5,83,8,1,0,4,0],[83,12,83,43,1,0,4,0]],"cpfds_add_sock":[[385,6,385,10,0,0,4,0],[385,14,385,33,0,0,4,0],[386,32,386,38,0,0,4,1]],"Curl_timeleft_now_ms":[[122,11,122,29,0,0,4,0]],"multi_wait":[[1579,13,1579,62,0,0,4,0],[1592,19,1592,26,0,0,4,0],[1624,6,1624,29,0,0,4,0],[1624,33,1624,70,0,0,4,0],[1624,33,1624,70,0,0,4,1],[1642,8,1642,15,0,0,4,1]],"Curl_cshutdn_setfds":[[439,6,439,37,0,0,4,1]],"curl_multi_setopt":[[3234,6,3234,24,0,0,4,1],[3237,6,3237,24,0,0,4,1],[3243,3,3243,31,0,0,4,1],[3246,3,3246,27,0,0,4,1],[3249,3,3249,29,0,0,4,1],[3252,3,3252,25,0,0,4,1],[3255,3,3255,27,0,0,4,1],[3258,3,3258,30,0,0,4,1],[3261,3,3261,26,0,0,4,1],[3264,3,3264,28,0,0,4,0],[3266,8,3266,24,0,0,4,0],[3269,3,3269,37,0,0,4,1],[3273,3,3273,38,0,0,4,1],[3278,3,3278,36,0,0,4,1],[3280,3,3280,44,0,0,4,1],[3282,3,3282,42,0,0,4,1],[3284,3,3284,35,0,0,4,1],[3286,3,3286,37,0,0,4,1],[3288,3,3288,39,0,0,4,1],[3295,3,3295,32,0,0,4,1],[3310,3,3310,31,0,0,4,1],[3313,3,3313,27,0,0,4,1],[3316,3,3316,36,0,0,4,1],[3341,3,3341,27,0,0,4,1],[3344,3,3344,10,0,0,4,1],[83,5,83,8,1,0,4,0],[83,12,83,43,1,0,4,0]],"curl_multi_fdset":[[1264,6,1264,24,0,0,4,1],[1267,6,1267,24,0,0,4,1],[1271,6,1271,51,0,0,4,0],[1275,10,1275,15,0,0,4,1],[1281,18,1281,26,0,0,4,0],[1281,18,1281,26,0,0,4,1],[1282,12,1282,23,0,0,4,1],[1285,12,1285,40,0,0,4,0],[1287,12,1287,41,0,0,4,1],[1289,12,1289,44,0,0,4,0],[1292,13,1292,62,0,0,4,0],[1292,13,1292,62,0,0,4,1],[83,5,83,8,1,0,4,0],[83,12,83,43,1,0,4,0]],"Curl_poll":[[242,6,242,20,0,0,4,1],[244,11,244,25,0,0,4,1],[249,6,249,12,0,0,4,0],[250,8,250,17,0,0,4,1]],"Curl_pollfds_add_ps":[[417,14,417,23,0,0,4,0],[419,8,419,37,0,0,4,0],[421,8,421,38,0,0,4,1],[423,8,423,14,0,0,4,0],[424,10,424,61,0,0,4,1]],"out_string":[[846,6,846,10,0,0,4,0],[848,8,848,18,0,0,4,0]]},"library_calls":["curl_global_init","curl_url","curl_url_set","curl_url_dup","curl_multi_init","curl_easy_init","curl_url_get","curl_easy_setopt","curl_free","curl_multi_add_handle","curl_multi_setopt","curl_strnequal","curl_multi_fdset","curl_multi_get_offt","curl_multi_wait","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_dup","curl_multi_init","curl_easy_init","curl_url_get","curl_easy_setopt","curl_free","curl_multi_add_handle","curl_multi_setopt","curl_strnequal","curl_multi_fdset","curl_multi_get_offt","curl_multi_wait","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":2}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize global curl resources
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Parse URL from input data
    const char *url_data = reinterpret_cast<const char*>(data);
    size_t url_len = size > 0 ? (data[0] % (size + 1)) : 0;
    
    // Create a URL handle and set URL from input
    CURLU *url_handle = curl_url();
    if (url_handle && url_len > 0 && url_len < size) {
        // Null-terminate the URL string
        char *url_str = static_cast<char*>(malloc(url_len + 1));
        if (url_str) {
            memcpy(url_str, url_data, url_len);
            url_str[url_len] = '\0';
            
            // Set URL in the handle
            curl_url_set(url_handle, CURLUPART_URL, url_str, 0);
            free(url_str);
        }
    }
    
    // Duplicate the URL handle
    CURLU *dup_url = curl_url_dup(url_handle);
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        // Create easy handle and add to multi
        CURL *easy_handle = curl_easy_init();
        if (easy_handle && url_handle) {
            char *url_part = nullptr;
            curl_url_get(url_handle, CURLUPART_URL, &url_part, 0);
            if (url_part) {
                curl_easy_setopt(easy_handle, CURLOPT_URL, url_part);
                curl_free(url_part);
            }
            
            // Add easy handle to multi
            curl_multi_add_handle(multi_handle, easy_handle);
            
            // Set multi options based on input data
            long max_connects = size > 1 ? data[1] : 5L;
            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_connects);
            
            // String comparison using part of input data
            const char *compare_str1 = "HTTP";
            const char *compare_str2 = "http";
            size_t n = size > 2 ? (data[2] % 5) + 1 : 4;
            int str_equal = curl_strnequal(compare_str1, compare_str2, n);
            
            // Prepare file descriptors for multi
            fd_set read_fds, write_fds, exc_fds;
            FD_ZERO(&read_fds);
            FD_ZERO(&write_fds);
            FD_ZERO(&exc_fds);
            int max_fd = -1;
            
            // Get file descriptor sets
            curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);
            
            // Get offset information
            curl_off_t offset_value = 0;
            if (size > 3) {
                // Convert to offset type (CURLINFO_OFFT_SENT = 0x300004)
                CURLMinfo_offt info_type = static_cast<CURLMinfo_offt>(
                    0x300000 + (data[3] % 10)
                );
                curl_multi_get_offt(multi_handle, info_type, &offset_value);
            }
            
            // Perform multi wait
            int num_fds = 0;
            int timeout_ms = size > 4 ? static_cast<int>(data[4] % 1000) : 100;
            CURLMcode wait_result = curl_multi_wait(
                multi_handle, nullptr, 0, timeout_ms, &num_fds
            );
            
            // Perform a simple multi perform to get some activity
            int running_handles = 0;
            curl_multi_perform(multi_handle, &running_handles);
            
            // Remove and clean up easy handle
            curl_multi_remove_handle(multi_handle, easy_handle);
            curl_easy_cleanup(easy_handle);
        }
        
        // Clean up multi handle
        curl_multi_cleanup(multi_handle);
    }
    
    // Clean up URL handles
    if (dup_url) {
        curl_url_cleanup(dup_url);
    }
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}
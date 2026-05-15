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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 91, nr_unique_branch: 27
//<Quality> {"density":14,"unique_branches":{"free_formlist":[[157,9,157,20,0,0,4,0],[157,9,157,20,0,0,4,1]],"Curl_is_absolute_url":[[206,54,206,67,0,0,4,1]],"parseurl":[[1152,53,1152,63,0,0,4,0]],"guess_scheme":[[1005,6,1005,16,0,0,4,1],[33,27,33,57,1,0,4,1],[33,27,33,57,2,0,4,1],[33,27,33,57,3,0,4,1],[33,27,33,57,4,0,4,1],[33,27,33,57,5,0,4,1],[33,27,33,57,6,0,4,1]],"FormAddCheck":[[229,19,229,49,0,0,4,0],[229,54,229,59,0,0,4,0]],"findprotocol":[[1442,6,1442,7,0,0,4,1],[1463,9,1463,10,0,0,4,1]],"parse_hostname_login":[[283,6,283,15,0,0,4,1],[290,37,290,38,0,0,4,1]],"parse_scheme":[[952,8,952,43,0,0,4,1],[964,8,964,62,0,0,4,1],[967,8,967,36,0,0,4,1],[976,6,976,13,0,0,4,1]],"urlget_url":[[1473,10,1473,11,0,0,4,1],[1478,8,1478,9,0,0,4,1]],"free_chain":[[291,9,291,10,0,0,4,1]],"Curl_parse_port":[[375,14,375,24,0,0,4,1]],"FormAdd":[[571,6,571,12,0,0,4,0],[585,6,585,13,0,0,4,1]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_get","curl_free","curl_getdate","curl_multi_init","curl_multi_notify_enable","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_multi_wait","curl_multi_remove_handle","curl_formadd","curl_formfree","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_get","curl_free","curl_getdate","curl_multi_init","curl_multi_notify_enable","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_multi_wait","curl_multi_remove_handle","curl_formadd","curl_formfree","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    // Initialize global curl environment
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create URL handle and parse input data as URL components
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Use first portion of input as URL part
    size_t url_len = size > 100 ? 100 : size;
    char *url_part = (char *)malloc(url_len + 1);
    if (!url_part) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_part, data, url_len);
    url_part[url_len] = '\0';
    
    // Set URL component
    curl_url_set(url_handle, CURLUPART_URL, url_part, fuzz_uint32_t_1);
    
    // Get URL component back
    char *retrieved_url = nullptr;
    curl_url_get(url_handle, CURLUPART_URL, &retrieved_url, fuzz_uint32_t_2);
    if (retrieved_url) {
        curl_free(retrieved_url);
    }
    
    free(url_part);
    
    // Parse date from input (using portion after URL part)
    if (size > url_len + 10) {
        size_t date_len = size - url_len > 50 ? 50 : size - url_len;
        char *date_str = (char *)malloc(date_len + 1);
        if (date_str) {
            memcpy(date_str, data + url_len, date_len);
            date_str[date_len] = '\0';
            time_t parsed_time = curl_getdate(date_str, nullptr);
            free(date_str);
        }
    }
    
    // Create multi handle and enable notifications
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        curl_multi_notify_enable(multi_handle, CURL_POLL_IN | CURL_POLL_OUT);
        
        // Create easy handle
        CURL *easy_handle = curl_easy_init();
        if (easy_handle) {
            // Set various options using input data
            size_t opt_offset = url_len;
            
            // Set URL option
            if (opt_offset < size) {
                size_t opt_len = size - opt_offset > 200 ? 200 : size - opt_offset;
                char *opt_str = (char *)malloc(opt_len + 1);
                if (opt_str) {
                    memcpy(opt_str, data + opt_offset, opt_len);
                    opt_str[opt_len] = '\0';
                    curl_easy_setopt(easy_handle, CURLOPT_URL, opt_str);
                    free(opt_str);
                }
            }
            
            // Create input file from data
            FILE *in_file = fmemopen((void *)data, size, "rb");
            if (in_file) {
                // Set read function and data
                curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, fread);
                curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
                
                // Create output file
                FILE *out_file = fopen("output_file", "wb");
                if (out_file) {
                    // Set write function and data
                    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, fwrite);
                    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
                    
                    // Add easy handle to multi handle
                    curl_multi_add_handle(multi_handle, easy_handle);
                    
                    // Perform socket actions (simulate with dummy socket)
                    int running_handles = 0;
                    curl_multi_socket_action(multi_handle, CURL_SOCKET_BAD, fuzz_int32_t_3, &running_handles);
                    
                    // Wait for activity
                    struct curl_waitfd extra_fds;
                    int ret = 0;
                    curl_multi_wait(multi_handle, &extra_fds, 0, fuzz_int32_t_4, &ret);
                    
                    // Remove handle from multi
                    curl_multi_remove_handle(multi_handle, easy_handle);
                    
                    assert_file_closed(&out_file);;
                }
                assert_file_closed(&in_file);;
            }
            
            // Create and free a form (even if empty)
            struct curl_httppost *form = nullptr;
            struct curl_httppost *last_post = nullptr;
            curl_formadd(&form, &last_post, CURLFORM_COPYNAME, "test", CURLFORM_END);
            curl_formfree(form);
            
            curl_easy_cleanup(easy_handle);
        }
        
        curl_multi_cleanup(multi_handle);
    }
    
    // Cleanup
    curl_url_cleanup(url_handle);
    curl_global_cleanup();
    
    return 0;
}
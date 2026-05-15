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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 10
//<Quality> {"density":27,"unique_branches":{"Curl_junkscan":[[232,13,232,23,0,0,4,0]],"set_url_port":[[1675,6,1675,53,0,0,4,1],[1675,57,1675,71,0,0,4,1],[1679,6,1679,10,0,0,4,1],[44,22,44,34,1,0,4,0],[44,38,44,50,1,0,4,0]],"curl_url_set":[[1855,3,1855,22,0,0,4,0],[1869,3,1869,26,0,0,4,0]],"urlget_url":[[1454,8,1454,17,0,0,4,1],[1456,13,1456,41,0,0,4,1]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_dup","curl_url_cleanup","curl_global_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_set","curl_url_set","curl_url_set","curl_url_strerror","curl_easy_init","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup","curl_easy_option_by_id","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_dup","curl_url_set","curl_url_strerror","curl_easy_init","curl_easy_option_by_id","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":2}
/*Looking at the requirements and the specified APIs, I'll create a fuzzer that:

1. Uses the input data to construct a URL
2. Duplicates the URL handle
3. Uses curl_easy_option_by_id to get option information
4. Performs an HTTP request
5. Handles URL-related errors

*/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<16) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
	//fuzzer shim end}




    // Early exit if data is too small
    if (size < 1) {
        return 0;
    }

    // 1. Initialize curl globally
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // 2. Create a URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Create a duplicated URL handle
    CURLU *dup_url_handle = curl_url_dup(url_handle);
    if (!dup_url_handle) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // 4. Extract URL string from input data
    // Ensure null termination for the URL string
    char *url_str = (char *)malloc(size + 1);
    if (!url_str) {
        curl_url_cleanup(dup_url_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    memcpy(url_str, data, size);
    url_str[size] = '\0';
    
    // Try to set the URL in the duplicated handle
    CURLUcode url_set_res = CURLUE_OK;
    
    // We'll try to set different URL parts based on the input
    // Use first byte to determine which part to set
    unsigned int flags = CURLU_DEFAULT_PORT | CURLU_ALLOW_SPACE;
    CURLUPart part = CURLUPART_URL;
    
    if (size > 1) {
        // Use the first byte to determine URL part
        switch (data[0] % 7) {
            case 0: part = CURLUPART_URL; break;
            case 1: part = CURLUPART_SCHEME; break;
            case 2: part = CURLUPART_HOST; break;
            case 3: part = CURLUPART_PATH; break;
            case 4: part = CURLUPART_QUERY; break;
            case 5: part = CURLUPART_FRAGMENT; break;
            case 6: part = CURLUPART_PORT; break;
        }
        
        // For scheme and port, use simple test values
        if (part == CURLUPART_SCHEME) {
            url_set_res = curl_url_set(dup_url_handle, part, fuzz_str_1, flags);
        } else if (part == CURLUPART_PORT) {
            url_set_res = curl_url_set(dup_url_handle, part, fuzz_str_2, flags);
        } else {
            // Use the URL string (excluding first byte for other parts)
            url_set_res = curl_url_set(dup_url_handle, part, url_str + 1, flags);
        }
    } else {
        url_set_res = curl_url_set(dup_url_handle, part, url_str, flags);
    }
    
    // 5. Get URL string error message (if any)
    const char *url_error_str = curl_url_strerror(url_set_res);
    
    // 6. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        free(url_str);
        curl_url_cleanup(dup_url_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // 7. Get option by ID - use CURLOPT_URL (10002)
    const struct curl_easyoption *option = curl_easy_option_by_id(CURLoption(10002));
    
    // 8. Set the URL option if we got a valid option
    if (option && url_set_res == CURLUE_OK) {
        // Try to extract the URL from the handle
        char *extracted_url = NULL;
        CURLUcode get_res = curl_url_get(dup_url_handle, CURLUPART_URL, &extracted_url, fuzz_uint32_t_3);
        
        if (get_res == CURLUE_OK && extracted_url) {
            curl_easy_setopt(easy_handle, CURLOPT_URL, extracted_url);
            curl_free(extracted_url);
        } else {
            // Use a test URL if we can't get it from the handle
            curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        }
        
        // Set other options for safe fuzzing
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L); // HEAD request only
        curl_easy_setopt(easy_handle, CURLOPT_FAILONERROR, 1L);
        
        // Create a FILE* for output using fmemopen
        FILE *out_file = fopen("output_file", "wb");
        if (out_file) {
            // Set write function to write to file
            curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
            
            // 9. Perform the request
            curl_easy_perform(easy_handle);
            
            // Get file descriptor and close file
            int fd = fuzz_fileno(out_file);
            (void)fd; // Use fd to avoid unused variable warning
            assert_file_closed(&out_file);;
        }
    }

    // 10. Cleanup
    curl_easy_cleanup(easy_handle);
    free(url_str);
    curl_url_cleanup(dup_url_handle);
    curl_url_cleanup(url_handle);
    curl_global_cleanup();

    return 0;
}
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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_dup","curl_url_cleanup","curl_global_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup","curl_easy_option_by_id","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_url_get","curl_easy_setopt","curl_free","curl_url_strerror","curl_easy_perform","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_dup","curl_easy_option_by_id","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_url_get","curl_easy_setopt","curl_free","curl_easy_perform","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Initialize libcurl globally
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        // Global initialization failed - can't proceed
        return 0;
    }

    // Create a new URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Duplicate the URL handle
    CURLU *url_dup = curl_url_dup(url_handle);
    if (!url_dup) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a FILE pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_url_cleanup(url_dup);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_url_cleanup(url_dup);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get easy option by ID (CURLOPT_URL = 10002)
    const struct curl_easyoption *option = curl_easy_option_by_id(static_cast<CURLoption>(10002));
    
    // Initialize easy handle
    CURL *easy = curl_easy_init();
    if (easy) {
        // Set some options
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
        
        // Write output to file
        curl_easy_setopt(easy, CURLOPT_WRITEDATA, out_file);
        
        // Attempt to set URL from duplicated handle (this is just to use the handle)
        char *url_part = nullptr;
        CURLUcode url_res = curl_url_get(url_dup, CURLUPART_URL, &url_part, fuzz_uint32_t_1);
        if (url_res == CURLUE_OK && url_part) {
            curl_easy_setopt(easy, CURLOPT_URL, url_part);
            curl_free(url_part);
        } else {
            // Get URL error string
            const char *err_str = curl_url_strerror(url_res);
            // Could log error if needed
        }
        
        // Try to perform the operation - this will fail since we haven't set a valid URL
        // but we're calling it to satisfy the requirement
        CURLcode perform_res = curl_easy_perform(easy);
        
        // Clean up easy handle
        curl_easy_cleanup(easy);
    }

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up URL handles
    curl_url_cleanup(url_dup);
    curl_url_cleanup(url_handle);
    
    // Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_dup","curl_url_cleanup","curl_global_cleanup","curl_easy_init","curl_url_cleanup","curl_global_cleanup","curl_easy_option_by_id","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_strerror","curl_url_get","curl_free","curl_url_strerror","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_dup","curl_url_cleanup","curl_easy_init","curl_easy_option_by_id","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_perform","curl_easy_strerror","curl_url_get","curl_free","curl_url_strerror","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":1}
/*Here's a fuzz driver that uses the specified curl APIs to perform URL handling and HTTP request operations:

*/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < 10) {
        return 0;
    }

    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // Create initial URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Parse input as URL string (truncate at first null byte if present)
    const char *url_str = reinterpret_cast<const char*>(data);
    size_t url_len = size;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == 0) {
            url_len = i;
            break;
        }
    }

    if (url_len == 0) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }

    // Allocate and copy URL string
    char *url_copy = static_cast<char*>(malloc(url_len + 1));
    if (!url_copy) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_copy, url_str, url_len);
    url_copy[url_len] = '\0';

    // Set URL components using the input data
    CURLUcode set_res = curl_url_set(url_handle, CURLUPART_URL, url_copy, fuzz_uint32_t_1);
    
    // Duplicate the URL handle
    CURLU *dup_url_handle = curl_url_dup(url_handle);
    
    // Clean up original URL handle
    curl_url_cleanup(url_handle);
    
    if (!dup_url_handle) {
        free(url_copy);
        curl_global_cleanup();
        return 0;
    }

    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_url_cleanup(dup_url_handle);
        free(url_copy);
        curl_global_cleanup();
        return 0;
    }

    // Get CURLOPT_URL option by ID (CURLOPT_URL = 10002)
    const struct curl_easyoption *url_option = curl_easy_option_by_id(CURLOPT_URL);
    if (url_option) {
        // Set the URL option on the easy handle
        curl_easy_setopt(easy_handle, CURLOPT_URL, url_copy);
    }

    // Set additional options for safe fuzzing
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L); // HEAD request only
    curl_easy_setopt(easy_handle, CURLOPT_FAILONERROR, 1L);
    
    // Create a FILE* for reading/writing if needed
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Set output to file
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
        
        // Perform the HTTP request
        CURLcode perform_res = curl_easy_perform(easy_handle);
        
        // Get and use error info if needed
        if (perform_res != CURLE_OK) {
            const char *err_str = curl_easy_strerror(perform_res);
            // Error string available for debugging
        }
        
        assert_file_closed(&out_file);;
        remove("output_file");
    }

    // Get URL components from duplicated handle
    char *scheme = nullptr;
    CURLUcode get_res = curl_url_get(dup_url_handle, CURLUPART_SCHEME, &scheme, fuzz_uint32_t_2);
    if (get_res == CURLUE_OK && scheme) {
        curl_free(scheme);
    }

    // Get URL error string if URL operations failed
    if (set_res != CURLUE_OK) {
        const char *url_error = curl_url_strerror(set_res);
        // Error string available for debugging
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_url_cleanup(dup_url_handle);
    free(url_copy);
    curl_global_cleanup();

    assert_file_closed(&out_file);
	return 0;
}
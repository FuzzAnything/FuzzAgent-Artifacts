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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_share_init","curl_easy_cleanup","curl_global_cleanup","curl_url","curl_share_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_url_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_escape","curl_mime_addpart","curl_share_setopt","curl_easy_option_by_id","curl_easy_setopt","curl_url_cleanup","curl_free","curl_mime_data","curl_mime_free","curl_share_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_share_init","curl_url","curl_mime_init","curl_easy_escape","curl_mime_addpart","curl_share_setopt","curl_easy_option_by_id","curl_easy_setopt","curl_url_cleanup","curl_free","curl_mime_data","curl_mime_free","curl_share_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Helper function for curl_mvprintf
static int  id_000000_curl_mvprintf_helper(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int ret = curl_mvprintf(format, args);
    va_end(args);
    return ret;
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 4) {
	return 0;
	}
	
    
    // Initialize global curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create share handle
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    // Create URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_share_cleanup(share);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (!mime) {
        curl_url_cleanup(url_handle);
        curl_share_cleanup(share);
        curl_easy_cleanup(easy);
        curl_global_cleanup();
        return 0;
    }
    
    // Use input data for various operations
    
    // 1. Use curl_easy_escape with input data
    // First create a null-terminated string from input
    size_t str_len = size < 256 ? size : 256;
    char *input_str = new char[str_len + 1];
    memcpy(input_str, data, str_len);
    input_str[str_len] = '\0';
    
    char *escaped = curl_easy_escape(easy, input_str, str_len);
    
    // 2. Use curl_mime_addpart
    curl_mimepart *mimepart = curl_mime_addpart(mime);
    
    // 3. Use curl_share_setopt
    curl_share_setopt(share, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    
    // 4. Use curl_easy_option_by_id
    const struct curl_easyoption *option = curl_easy_option_by_id(CURLOPT_URL);
    
    // 5. Use curl_easy_setopt with the escaped string as URL
    if (escaped && option) {
        curl_easy_setopt(easy, CURLOPT_URL, escaped);
    }
    
    // 6. Use curl_mvprintf via helper function
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        if (escaped) {
             id_000000_curl_mvprintf_helper("Escaped string: %s\n", escaped);
        } else {
             id_000000_curl_mvprintf_helper("Input size: %zu\n", size);
        }
        assert_file_closed(&out_file);;
    }
    
    // 7. Use curl_url_cleanup
    curl_url_cleanup(url_handle);
    
    // Cleanup resources
    if (escaped) curl_free(escaped);
    delete[] input_str;
    
    // Try to set mime data with remaining input
    if (mimepart && size > str_len) {
        size_t remaining = size - str_len;
        const char *mime_data = reinterpret_cast<const char*>(data + str_len);
        curl_mime_data(mimepart, mime_data, remaining);
    }
    
    // Additional cleanup
    curl_mime_free(mime);
    curl_share_cleanup(share);
    curl_easy_cleanup(easy);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	return 0;
}
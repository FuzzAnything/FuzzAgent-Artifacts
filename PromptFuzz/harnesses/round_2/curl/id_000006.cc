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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 135, nr_unique_branch: 8
//<Quality> {"density":27,"unique_branches":{"Curl_conn_upkeep":[[686,6,687,34,0,0,4,0]],"Curl_hash_next_element":[[372,6,372,19,0,0,4,0],[379,10,379,21,0,0,4,0]],"setopt_cptr":[[2012,3,2012,26,0,0,4,0]],"cpool_foreach":[[527,9,527,11,0,0,4,0],[533,11,533,15,0,0,4,0],[533,11,533,15,0,0,4,1],[539,10,539,38,0,0,4,1]]},"library_calls":["curl_global_init","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_data","curl_mime_encoder","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_strnequal","curl_strnequal","curl_multi_perform","curl_multi_timeout","curl_easy_nextheader","curl_easy_upkeep","curl_multi_remove_handle","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_encoder","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_strnequal","curl_strnequal","curl_multi_perform","curl_multi_timeout","curl_easy_nextheader","curl_easy_upkeep","curl_multi_remove_handle","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified curl APIs in a meaningful way:

*/


extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize CURL
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create easy handle for HTTP operations
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create multi handle for performing multiple transfers simultaneously
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create mime structure for multipart/form-data
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a mime part
    curl_mimepart *mime_part = curl_mime_addpart(mime);
    if (!mime_part) {
        curl_mime_free(mime);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Use input data as mime data
    const char *mime_data = reinterpret_cast<const char*>(data);
    size_t data_size = size > 1024 ? 1024 : size; // Limit size for safety
    curl_mime_data(mime_part, mime_data, data_size);
    
    // Set mime encoder (using data as encoding type string if it looks valid)
    if (size >= 3) {
        // Check if first few bytes look like an encoding string
        const char *potential_encoding = reinterpret_cast<const char*>(data);
        if (potential_encoding[0] >= 'a' && potential_encoding[0] <= 'z') {
            // Try to use input as encoding type (e.g., "base64", "quoted-printable")
            curl_mime_encoder(mime_part, "base64");
        }
    }

    // Set up easy handle for a request
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, easy_handle);

    // Add handle to multi handle
    CURLMcode multi_add_res = curl_multi_add_handle(multi_handle, easy_handle);
    if (multi_add_res != CURLM_OK) {
        curl_mime_free(mime);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Use curl_strnequal for comparison
    // Compare parts of the input data with known strings
    const char *test_str1 = "test";
    const char *test_str2 = "data";
    
    // Create some test strings from input data
    if (size >= 8) {
        const char *str1 = reinterpret_cast<const char*>(data);
        const char *str2 = reinterpret_cast<const char*>(data) + 4;
        int equal1 = curl_strnequal(str1, test_str1, 4);
        int equal2 = curl_strnequal(str2, test_str2, 4);
        (void)equal1; (void)equal2; // Prevent unused variable warnings
    }

    // Perform the transfer (non-blocking)
    int running_handles = 0;
    CURLMcode multi_perform_res = curl_multi_perform(multi_handle, &running_handles);

    // Get timeout from multi handle
    long timeout_ms = 0;
    CURLMcode timeout_res = curl_multi_timeout(multi_handle, &timeout_ms);
    (void)timeout_res; // Prevent unused variable warning

    // Try to get headers using curl_easy_nextheader
    // Note: This requires actual headers from a transfer, so we'll simulate
    struct curl_header *header = nullptr;
    if (running_handles > 0) {
        // Try to get headers (origin 0 = both, request 0 = not request header)
        header = curl_easy_nextheader(easy_handle, 0, 0, nullptr);
    }

    // Call curl_easy_upkeep for HTTP/2 connection upkeep
    if (running_handles > 0) {
        curl_easy_upkeep(easy_handle);
    }

    // Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_mime_free(mime);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    return 0;
}
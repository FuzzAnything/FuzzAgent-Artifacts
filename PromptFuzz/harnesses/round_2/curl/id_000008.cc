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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 27, nr_unique_branch: 0
//<Quality> {"density":27,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_easy_upkeep","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data","curl_easy_setopt","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_header","curl_easy_nextheader","curl_multi_timeout","curl_multi_perform","curl_slist_free_all","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_easy_upkeep","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data","curl_easy_setopt","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_easy_header","curl_easy_nextheader","curl_multi_timeout","curl_multi_perform","curl_slist_free_all","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs to process HTTP response data with MIME encoding and header iteration:

*/


extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 2) {
	return 0;
	}
	  // Need at least some data

    // Initialize CURL
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // 1. Use curl_easy_upkeep - perform connection upkeep
    curl_easy_upkeep(easy_handle);

    // Create a FILE pointer from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // 2. Use curl_strnequal - compare strings from input data
    // Extract two substrings from the input for comparison
    size_t compare_len = size / 4;
    if (compare_len > 0) {
        const char *str1 = (const char *)data;
        const char *str2 = (const char *)data + compare_len;
        size_t min_len = compare_len < (size - compare_len) ? compare_len : (size - compare_len);
        if (min_len > 0) {
            int equal = curl_strnequal(str1, str2, min_len);
            (void)equal; // Use result to avoid compiler warning
        }
    }

    // 3. Use curl_mime_encoder - create MIME part with encoding
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Set encoding based on input data
            if (size > 10) {
                const char *encoding = "binary"; // Default encoding
                if (data[0] % 3 == 0) encoding = "base64";
                else if (data[0] % 3 == 1) encoding = "quoted-printable";
                
                curl_mime_encoder(part, encoding);
                
                // Add some data to the MIME part
                size_t mime_data_size = size > 100 ? 100 : size;
                curl_mime_data(part, (const char *)data, mime_data_size);
            }
        }
        
        // Set MIME data to easy handle
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }

    // 4. Use curl_easy_nextheader - iterate through headers
    // First set up some dummy headers for testing
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
    
    // Create a header from input data if we have enough
    if (size > 20) {
        char header_buf[256];
        size_t header_len = size > 255 ? 255 : size;
        memcpy(header_buf, data, header_len);
        header_buf[header_len] = '\0';
        
        // Make it look like a valid header
        char full_header[300];
        snprintf(full_header, sizeof(full_header), "X-Fuzz-Data: %s", header_buf);
        headers = curl_slist_append(headers, full_header);
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    
    // Try to iterate through headers (simulated)
    struct curl_header *h = NULL;
    unsigned int origin = CURLH_HEADER;
    int request = 0;
    
    // Get first header
    curl_easy_header(easy_handle, "Content-Type", 0, origin, request, &h);
    
    // Use curl_easy_nextheader to iterate
    if (h) {
        struct curl_header *next = curl_easy_nextheader(easy_handle, origin, request, h);
        (void)next; // Use result
    }

    // 5. Use curl_multi_timeout - get timeout value
    long timeout_ms = 0;
    CURLMcode timeout_res = curl_multi_timeout(multi_handle, &timeout_ms);
    (void)timeout_res; // Use result

    // Perform a simple multi perform to process handles
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    // Cleanup
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove and cleanup handles
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
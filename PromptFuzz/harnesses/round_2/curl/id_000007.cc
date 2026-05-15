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
//<ID> 72
//<Prompt> []
/*<Combination>: [
*/
//<score> 29, nr_unique_branch: 1
//<Quality> {"density":29,"unique_branches":{"curl_mime_data":[[1282,8,1282,40,0,0,4,0]]},"library_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_multi_add_handle","curl_multi_timeout","curl_multi_perform","curl_easy_nextheader","curl_easy_upkeep","curl_multi_perform","curl_slist_free_all","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_strnequal","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_slist_append","curl_slist_append","curl_easy_setopt","curl_multi_add_handle","curl_multi_timeout","curl_multi_perform","curl_easy_nextheader","curl_easy_upkeep","curl_multi_perform","curl_slist_free_all","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses all specified curl APIs with the input data:

*/


extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 20) {
        return 0; // Too small for meaningful fuzzing
    }

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy and multi handles
    CURL *easy_handle = curl_easy_init();
    CURLM *multi_handle = curl_multi_init();
    
    if (!easy_handle || !multi_handle) {
        if (easy_handle) curl_easy_cleanup(easy_handle);
        if (multi_handle) curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
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
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Parse input data for different purposes
    // Use first part for string comparison
    size_t str_len = size > 100 ? 50 : size/2;
    const char *str1 = reinterpret_cast<const char*>(data);
    const char *str2 = reinterpret_cast<const char*>(data + str_len/2);
    
    // API 1: Use curl_strnequal for string comparison
    int str_equal = curl_strnequal(str1, str2, str_len < 50 ? str_len : 50);
    (void)str_equal; // Prevent unused variable warning

    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // API 2: Set mime encoder using part of input data
            // Use middle section for encoding type
            size_t enc_offset = size/4;
            size_t enc_len = size/8;
            char *encoding = (char*)malloc(enc_len + 1);
            if (encoding) {
                memcpy(encoding, data + enc_offset, enc_len);
                encoding[enc_len] = '\0';
                curl_mime_encoder(part, encoding);
                free(encoding);
            }
            
            // Add some data to mime part
            const char *mime_data = "fuzz data";
            curl_mime_data(part, mime_data, CURL_ZERO_TERMINATED);
        }
        
        // Set mime data to easy handle
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }

    // Set up easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file://input_file");
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    
    // Set some headers to test header iteration
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "X-Fuzz-Test: true");
    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
    curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // API 3: Get multi handle timeout
    long timeout_ms = 0;
    curl_multi_timeout(multi_handle, &timeout_ms);

    // Perform some operations
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    // API 4: Iterate through headers
    struct curl_header *header = NULL;
    unsigned int origin = 0; // CURLH_HEADER
    int request = 0;
    while ((header = curl_easy_nextheader(easy_handle, origin, request, header))) {
        // Process header
        (void)header; // Prevent unused warning
    }

    // API 5: Perform connection upkeep (useful for WebSocket, HTTP/2, etc.)
    curl_easy_upkeep(easy_handle);

    // Perform another round to potentially trigger more code paths
    if (running_handles) {
        curl_multi_perform(multi_handle, &running_handles);
    }

    // Cleanup in specific order
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    // Remove handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Cleanup easy handle
    curl_easy_cleanup(easy_handle);
    
    // Cleanup multi handle
    curl_multi_cleanup(multi_handle);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Global cleanup
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
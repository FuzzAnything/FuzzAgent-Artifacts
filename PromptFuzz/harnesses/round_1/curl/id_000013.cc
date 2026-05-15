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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 37.5, nr_unique_branch: 32
//<Quality> {"density":25,"unique_branches":{"Curl_getinfo":[[647,3,647,21,0,0,4,0],[649,8,649,19,0,0,4,0]],"getinfo_long":[[205,3,205,27,0,0,4,1],[214,3,214,28,0,0,4,1],[215,3,215,29,0,0,4,1],[224,3,224,10,0,0,4,0],[230,3,230,30,0,0,4,0],[233,3,233,33,0,0,4,1],[236,3,236,25,0,0,4,1],[246,3,246,28,0,0,4,1],[249,3,249,29,0,0,4,1],[252,3,252,33,0,0,4,1],[255,3,255,39,0,0,4,1],[262,3,262,31,0,0,4,1],[265,3,265,31,0,0,4,1],[269,3,269,32,0,0,4,1],[273,3,273,30,0,0,4,1],[277,3,277,31,0,0,4,1],[281,3,281,25,0,0,4,1],[284,3,284,29,0,0,4,1],[292,3,292,27,0,0,4,1],[304,3,304,29,0,0,4,1],[311,3,311,27,0,0,4,1],[318,3,318,28,0,0,4,1],[321,3,321,32,0,0,4,1],[329,3,329,33,0,0,4,1],[332,3,332,33,0,0,4,1],[335,3,335,31,0,0,4,1],[345,3,345,29,0,0,4,1],[364,3,364,25,0,0,4,1],[367,3,367,27,0,0,4,1],[376,3,376,10,0,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_mime_free","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_easy_header","curl_easy_getinfo","curl_unescape","curl_free","curl_easy_reset","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_mime_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_subparts","curl_easy_setopt","curl_multi_add_handle","curl_multi_socket_action","curl_easy_header","curl_easy_getinfo","curl_unescape","curl_free","curl_easy_reset","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_mime_free","curl_global_cleanup"],"visited":1}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<31) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	 // Need minimum data for meaningful operations

    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);

    // Create a multi handle for socket action
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

    // Create FILE* for reading input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a file descriptor from the input file
    int fd = fuzz_fileno(in_file);
    curl_socket_t sock = (curl_socket_t)fd;

    // Set up easy handle with some options
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    
    // Create a mime structure for multipart data
    curl_mime *mime = curl_mime_init(easy_handle);
    curl_mimepart *part = curl_mime_addpart(mime);
    
    if (part) {
        // Use first 10 bytes as mime data
        size_t data_len = size > 10 ? 10 : size;
        curl_mime_data(part, (const char *)data, data_len);
        
        // Create subparts mime structure
        curl_mime *submime = curl_mime_init(easy_handle);
        curl_mimepart *subpart = curl_mime_addpart(submime);
        
        if (subpart) {
            // Use next 10 bytes for subpart data
            size_t sub_data_len = size > 20 ? 10 : (size > 10 ? size - 10 : 0);
            const char *sub_data = size > 10 ? (const char *)(data + 10) : "";
            curl_mime_data(subpart, sub_data, sub_data_len);
            
            // Use curl_mime_subparts - one of the required APIs
            curl_mime_subparts(part, submime);
        } else {
            curl_mime_free(submime);
        }
        
        // Set mime to easy handle
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    int running_handles = 0;
    
    // Use curl_multi_socket_action - one of the required APIs
    // Since we don't have a real socket, we simulate with file descriptor
    curl_multi_socket_action(multi_handle, sock, fuzz_int32_t_1, &running_handles);

    // Try to get header information - one of the required APIs
    struct curl_header *header_out = nullptr;
    size_t index = 0;
    unsigned int origin = CURLH_HEADER;
    int request = 0;
    CURLHcode header_result = curl_easy_header(easy_handle, fuzz_str_2, fuzz_uint64_t_3, origin, fuzz_int32_t_4, &header_out);

    // Get info from easy handle - one of the required APIs
    long response_code = 0;
    curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &response_code);

    // Unescape a string - one of the required APIs
    // Create a simple URL-encoded string from input data
    size_t escape_len = size > 5 ? 5 : size;
    char *escaped = curl_unescape((const char *)data, escape_len);
    if (escaped) {
        // Use the unescaped string
        // Free the unescaped string
        curl_free(escaped);
    }

    // Reset the easy handle - one of the required APIs
    curl_easy_reset(easy_handle);

    // Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    if (mime) {
        curl_mime_free(mime);
    }
    
    assert_file_closed(&in_file);;
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
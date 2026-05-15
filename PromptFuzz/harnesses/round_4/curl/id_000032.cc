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
//<ID> 525
//<Prompt> []
/*<Combination>: [
*/
//<score> 25, nr_unique_branch: 0
//<Quality> {"density":25,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_filedata","curl_mime_encoder","curl_easy_setopt","curl_msprintf","curl_multi_add_handle","curl_multi_strerror","curl_multi_waitfds","curl_multi_socket_action","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_mime_encoder","curl_easy_setopt","curl_msprintf","curl_multi_add_handle","curl_multi_strerror","curl_multi_waitfds","curl_multi_socket_action","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t* f_data, size_t f_size) {
	if(f_size<9) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
        return 0;
    }

    // Initialize curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
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
    
    // Create a mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create a mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create a temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Write input data to a temporary file for curl_mime_filedata
    FILE *temp_file = fopen("input_file", "wb");
    if (temp_file) {
        fwrite(data, 1, size, temp_file);
        assert_file_closed(&temp_file);;
    }
    
    // 1. Use curl_mime_filedata - set file data for mime part
    CURLcode res = curl_mime_filedata(part, "input_file");
    
    // 2. Use curl_mime_encoder - set encoder for mime part
    if (res == CURLE_OK) {
        res = curl_mime_encoder(part, fuzz_str_1);
    }
    
    // Set mime data to easy handle
    if (res == CURLE_OK) {
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }
    
    // 3. Use curl_msprintf - format a string
    char buffer[256];
    int printed = curl_msprintf(buffer, "Data size: %zu, First byte: 0x%02x", 
                                size, data[0]);
    
    // Add easy handle to multi handle
    if (res == CURLE_OK) {
        CURLMcode mres = curl_multi_add_handle(multi_handle, easy_handle);
        
        // 4. Use curl_multi_strerror - get error string
        if (mres != CURLM_OK) {
            const char *err_str = curl_multi_strerror(mres);
            // Could use err_str if needed
        }
        
        // 5. Use curl_multi_waitfds - wait for file descriptors
        struct curl_waitfd ufds[1];
        unsigned int fd_count = 0;
        if (mres == CURLM_OK) {
            curl_multi_waitfds(multi_handle, ufds, 1, &fd_count);
        }
        
        // 6. Use curl_multi_socket - simulate socket activity
        int running_handles = 0;
        if (fd_count > 0) {
            // Use the file descriptor from our input file
            curl_socket_t sock = fuzz_fileno(in_file);
            curl_multi_socket(multi_handle, sock, &running_handles);
        }
        
        // Perform a minimal multi operation
        int still_running = 0;
        curl_multi_perform(multi_handle, &still_running);
        
        // Remove handle from multi
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    // Clean up
    assert_file_closed(&in_file);;
    
    // Remove temporary file
    remove("input_file");
    
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_closed(&temp_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}
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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 4
//<Quality> {"density":27,"unique_branches":{"curl_mime_filedata":[[1313,8,1313,35,0,0,4,0]],"Curl_pretransfer":[[581,17,581,48,0,0,4,0],[585,8,585,32,0,0,4,1]],"setopt_cptr":[[1956,3,1956,25,0,0,4,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_escape","curl_easy_setopt","curl_free","curl_mprintf","curl_msnprintf","curl_multi_wakeup","curl_multi_perform","curl_easy_getinfo","curl_easy_header","curl_multi_wait","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_escape","curl_easy_setopt","curl_free","curl_mprintf","curl_msnprintf","curl_multi_wakeup","curl_multi_perform","curl_easy_getinfo","curl_easy_header","curl_multi_wait","curl_mime_free","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/**/


// Include curl headers

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);

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

    // Create a file from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a file for writing output
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

    // Create a mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use input file name for mime filedata
            rewind(in_file);
            char temp_filename[] = "temp_input_file";
            FILE *temp_file = fopen(temp_filename, "wb");
            if (temp_file) {
                fwrite(data, 1, size, temp_file);
                assert_file_closed(&temp_file);;
                
                // Use curl_mime_filedata API
                curl_mime_filedata(part, temp_filename);
                
                // Clean up temp file
                remove(temp_filename);
            }
        }
        
        // Set the mime data to easy handle
        curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    }

    // Set some easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);

    // Use curl_easy_escape API
    // Create a string from first part of input data
    size_t str_len = size > 100 ? 100 : size;
    char *input_str = (char *)malloc(str_len + 1);
    if (input_str) {
        memcpy(input_str, data, str_len);
        input_str[str_len] = '\0';
        
        char *escaped_str = curl_easy_escape(easy_handle, input_str, str_len);
        free(input_str);
        
        if (escaped_str) {
            // Use escaped string somehow
            curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, escaped_str);
            curl_free(escaped_str);
        }
    }

    // Use curl_mprintf API
    char format_buffer[256];
    curl_mprintf("Fuzzing with size: %zu\n", size);
    int printed = curl_msnprintf(format_buffer, sizeof(format_buffer), 
                                 "Data: %.*s", 
                                 size > 20 ? 20 : (int)size, data);

    // Use curl_multi_wakeup API
    curl_multi_wakeup(multi_handle);

    // Perform multi operations
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    // Use curl_easy_getinfo API
    double total_time = 0;
    curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &total_time);

    // Use curl_easy_header API
    struct curl_header *header = nullptr;
    CURLHcode header_code = curl_easy_header(easy_handle, "Content-Type", 0, 
                                             CURLH_HEADER, 0, &header);

    // Use curl_multi_wait API
    int numfds = 0;
    struct curl_waitfd wait_fd;
    wait_fd.fd = fuzz_fileno(in_file);
    wait_fd.events = CURL_WAIT_POLLIN;
    wait_fd.revents = 0;
    
    curl_multi_wait(multi_handle, &wait_fd, 1, 100, &numfds);

    // Clean up
    if (mime) {
        curl_mime_free(mime);
    }
    
    // Remove easy handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // Clean up multi handle
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
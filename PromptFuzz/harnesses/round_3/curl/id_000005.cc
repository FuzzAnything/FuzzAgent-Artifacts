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
//<score> 6, nr_unique_branch: 4
//<Quality> {"density":9,"unique_branches":{"lookup":[[35,6,35,10,0,0,4,0],[38,10,38,14,0,0,4,0],[39,12,39,40,0,0,4,0],[39,12,39,40,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_global_cleanup","curl_easy_option_by_name","curl_unescape","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_free","curl_easy_cleanup","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_waitfds","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_free","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_option_by_name","curl_unescape","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_type","curl_mime_free","curl_easy_cleanup","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_multi_add_handle","curl_multi_waitfds","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_free","curl_global_cleanup"],"visited":2}
/**/


// Helper function to use curl_mvfprintf
static int  id_000005_curl_mvfprintf_wrapper(FILE *fd, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvfprintf(fd, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<6) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a file handle from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for input file
    int input_fd = fuzz_fileno(in_file);
    
    // Read a string from input for unescaping (use first part of data as string)
    size_t str_len = size > 100 ? 100 : size;
    char *input_str = (char *)malloc(str_len + 1);
    if (!input_str) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
    }
    
    fread(input_str, 1, str_len, in_file);
    input_str[str_len] = '\0';
    
    // API 1: curl_easy_option_by_name - Look up an option by name
    const struct curl_easyoption *option = curl_easy_option_by_name(fuzz_str_1);
    
    // API 2: curl_unescape - Unescape the input string
    char *unescaped_str = curl_unescape(input_str, (int)str_len);
    
    // API 3: curl_mime_type - Create a mime part and set its type
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_mime *mime = curl_mime_init(easy);
        if (mime) {
            curl_mimepart *part = curl_mime_addpart(mime);
            if (part && unescaped_str) {
                // Use the unescaped string as mime type (or part of it)
                char mime_type[100];
                snprintf(mime_type, sizeof(mime_type), "text/%s", 
                        unescaped_str ? (strlen(unescaped_str) > 50 ? "plain" : unescaped_str) : "plain");
                curl_mime_type(part, mime_type);
            }
            curl_mime_free(mime);
        }
        curl_easy_cleanup(easy);
    }
    
    // API 4: curl_multi_waitfds - Set up multi handle and wait for file descriptors
    CURLM *multi = curl_multi_init();
    if (multi) {
        // Create an easy handle and add it to multi
        CURL *easy_handle = curl_easy_init();
        if (easy_handle) {
            curl_easy_setopt(easy_handle, CURLOPT_URL, "file://input_file");
            curl_multi_add_handle(multi, easy_handle);
            
            // Prepare waitfd structure for input file descriptor
            struct curl_waitfd ufds[1];
            ufds[0].fd = input_fd;
            ufds[0].events = CURL_WAIT_POLLIN;
            ufds[0].revents = 0;
            
            unsigned int fd_count = 0;
            
            // Wait for file descriptors (non-blocking)
            curl_multi_waitfds(multi, ufds, 1, &fd_count);
            
            // Perform any pending operations
            int running_handles = 0;
            curl_multi_perform(multi, &running_handles);
            
            // Clean up multi handle
            curl_multi_remove_handle(multi, easy_handle);
            curl_easy_cleanup(easy_handle);
        }
        curl_multi_cleanup(multi);
    }
    
    // API 5: curl_mvfprintf - Write formatted output using va_list wrapper
    if (unescaped_str) {
         id_000005_curl_mvfprintf_wrapper(out_file, 
                              "Unescaped string: %s\nOption found: %s\nInput size: %zu\n",
                              unescaped_str,
                              option ? "Yes" : "No",
                              size);
    }
    
    // Clean up resources
    if (unescaped_str) {
        curl_free(unescaped_str);
    }
    free(input_str);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(input_fd);
	return 0;
}
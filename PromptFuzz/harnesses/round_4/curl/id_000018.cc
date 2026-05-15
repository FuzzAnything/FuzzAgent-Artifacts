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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 8
//<Quality> {"density":16,"unique_branches":{"curl_easy_unescape":[[175,8,175,12,0,0,4,0],[176,10,176,38,0,0,4,0]],"lookup":[[35,6,35,10,0,0,4,1],[35,14,35,16,0,0,4,0],[38,10,38,14,0,0,4,1],[43,12,43,25,0,0,4,0],[43,12,43,25,0,0,4,1],[43,29,43,60,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_multi_setopt","curl_multi_add_handle","curl_easy_option_by_id","curl_easy_ssls_import","curl_multi_waitfds","curl_multi_socket_action","curl_multi_info_read","curl_easy_escape","curl_easy_unescape","curl_free","curl_free","curl_pushheader_byname","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_multi_setopt","curl_multi_add_handle","curl_easy_option_by_id","curl_easy_ssls_import","curl_multi_waitfds","curl_multi_socket_action","curl_multi_info_read","curl_easy_escape","curl_easy_unescape","curl_free","curl_free","curl_pushheader_byname","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Helper function to create va_list for curl_mvsnprintf
static int  id_000018_call_curl_mvsnprintf(char *buffer, size_t maxlength, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<30) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Use fmemopen to create a FILE* for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }
    
    int fd = fuzz_fileno(in_file);  // Get file descriptor for input
    
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set multi handle options
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 5L);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Get easy option by ID
    const struct curl_easyoption *opt = curl_easy_option_by_id(CURLOPT_URL);
    
    // Use curl_mvsnprintf to format a string
    char buffer[1024];
    if (size > 0) {
        // Use first byte as format specifier
        char format[32];
        snprintf(format, sizeof(format), "Input: %%s (size: %%zu, first: 0x%%02x)");
         id_000018_call_curl_mvsnprintf(buffer, sizeof(buffer), format, 
                            "fuzz_data", size, data[0]);
    }
    
    // SSL session import (using parts of input data as SSL data)
    if (size >= 64) {
        const char *session_key = "fuzz_session";
        const unsigned char *shmac = data;
        size_t shmac_len = size > 32 ? 32 : size;
        const unsigned char *sdata = data + (size > 32 ? 32 : 0);
        size_t sdata_len = size > 64 ? 32 : (size > 32 ? size - 32 : 0);
        
        curl_easy_ssls_import(easy_handle, fuzz_str_1, 
                             shmac, shmac_len, 
                             sdata, sdata_len);
    }
    
    // Create curl_waitfd structure for multi_waitfds
    struct curl_waitfd ufds[1];
    ufds[0].fd = fd;
    ufds[0].events = CURL_WAIT_POLLIN;
    ufds[0].revents = 0;
    
    unsigned int fd_count = 0;
    curl_multi_waitfds(multi_handle, ufds, 1, &fd_count);
    
    // Use multi_socket with our file descriptor
    int running_handles = 0;
    curl_multi_socket(multi_handle, (curl_socket_t)fd, &running_handles);
    
    // Process multi handle messages
    int msgs_in_queue = 0;
    CURLMsg *msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    
    // Unescape a string from input data
    if (size > 0) {
        // Take up to 100 bytes for unescaping
        int unescape_len = size > 100 ? 100 : (int)size;
        char *escaped = curl_easy_escape(easy_handle, (const char *)data, unescape_len);
        if (escaped) {
            // Now unescape it
            int outlength = 0;
            char *unescaped = curl_easy_unescape(easy_handle, escaped, strlen(escaped), &outlength);
            if (unescaped) {
                // Write unescaped data to output file
                fwrite(unescaped, 1, outlength, out_file);
                curl_free(unescaped);
            }
            curl_free(escaped);
        }
    }
    
    // Simulate push headers (dummy structure since we don't have real push headers)
    struct curl_pushheaders *push_headers = nullptr;
    if (push_headers) {
        char *header = curl_pushheader_byname(push_headers, fuzz_str_2);
    }
    
    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(fd);
	return 0;
}
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
//<ID> 495
//<Prompt> []
/*<Combination>: [
*/
//<score> 15, nr_unique_branch: 0
//<Quality> {"density":15,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_unescape","curl_version_info","curl_strequal","curl_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_assign","curl_multi_poll","curl_multi_wakeup","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_unescape","curl_version_info","curl_strequal","curl_multi_assign","curl_multi_poll","curl_multi_wakeup","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Helper function to use curl_mvsprintf
static int  id_000033_my_curl_mvsprintf(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data
    
    // Initialize curl globally
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a multi handle for curl_multi operations
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create an easy handle for curl_easy_unescape
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // PART 1: Use curl_easy_unescape on input data
    // Treat first part of data as escaped string
    size_t unescape_len = size > 256 ? 256 : size;
    int outlength = 0;
    char* unescaped = curl_easy_unescape(easy_handle, 
                                         reinterpret_cast<const char*>(data), 
                                         unescape_len, 
                                         &outlength);
    
    // PART 2: Use curl_version_info
    curl_version_info_data* version_info = curl_version_info(CURLVERSION_NOW);
    
    // PART 3: Use curl_mvsprintf with version info and unescaped data
    char buffer[1024];
    if (version_info && version_info->version && unescaped) {
         id_000033_my_curl_mvsprintf(buffer, 
                         "libcurl version: %s, unescaped: %.*s", 
                         version_info->version,
                         outlength > 100 ? 100 : outlength,
                         unescaped);
    }
    
    // PART 4: Use curl_strequal to compare strings
    int strings_equal = 0;
    if (version_info && version_info->version && unescaped) {
        // Compare first 10 chars of version string with unescaped data
        char version_prefix[11] = {0};
        strncpy(version_prefix, version_info->version, 10);
        strings_equal = curl_strequal(version_prefix, 
                                     unescaped ? (outlength > 10 ? unescaped : version_prefix) : version_prefix);
    }
    
    // PART 5: Create a file from input data for multi operations
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        if (unescaped) curl_free(unescaped);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor for curl_multi_assign
    curl_socket_t sockfd = fuzz_fileno(in_file);
    
    // PART 6: Use curl_multi_assign with the file descriptor
    void* sockp = reinterpret_cast<void*>(0x12345678);  // Dummy pointer
    CURLMcode assign_result = curl_multi_assign(multi_handle, sockfd, sockp);
    
    // PART 7: Use curl_multi_poll with waitfd
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = sockfd;
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;
    
    int numfds = 0;
    CURLMcode poll_result = curl_multi_poll(multi_handle, 
                                           extra_fds, 
                                           1,  // extra_nfds
                                           0,  // timeout_ms - non-blocking
                                           &numfds);
    
    // PART 8: Use curl_multi_wakeup
    CURLMcode wakeup_result = curl_multi_wakeup(multi_handle);
    
    // Add easy handle to multi handle to simulate transfer
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Perform a quick multi operation
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);
    
    // Clean up
    curl_multi_remove_handle(multi_handle, easy_handle);
    assert_file_closed(&in_file);;
    
    // Free allocated resources
    if (unescaped) {
        curl_free(unescaped);
    }
    
    // Clean up curl handles
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	assert_fd_closed(sockfd);
	return 0;
}
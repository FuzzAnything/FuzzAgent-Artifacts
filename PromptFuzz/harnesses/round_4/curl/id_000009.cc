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
//<ID> 46
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["curl_global_init","curl_version_info","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_easy_duphandle","curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_mime_free","curl_share_init","curl_share_cleanup","curl_multi_fdset","curl_multi_waitfds","curl_multi_notify_disable","curl_easy_strerror","curl_multi_strerror","curl_multi_remove_handle","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version_info","curl_multi_init","curl_easy_init","curl_easy_duphandle","curl_multi_add_handle","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_mime_free","curl_share_init","curl_share_cleanup","curl_multi_fdset","curl_multi_waitfds","curl_multi_notify_disable","curl_easy_strerror","curl_multi_strerror","curl_multi_remove_handle","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Get version information
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    
    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create an easy handle and duplicate it
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    CURL *duplicated_handle = curl_easy_duphandle(easy_handle);
    
    // Add the duplicated handle to multi handle
    curl_multi_add_handle(multi_handle, duplicated_handle);
    
    // Create a MIME part and set filename using input data
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use input data as filename (null-terminated)
            char *filename = static_cast<char*>(malloc(size + 1));
            if (filename) {
                memcpy(filename, data, size);
                filename[size] = '\0';
                curl_mime_filename(part, filename);
                free(filename);
            }
        }
        curl_mime_free(mime);
    }
    
    // Create a share handle and clean it up
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        curl_share_cleanup(share_handle);
    }
    
    // Use fdset with multi handle
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    int max_fd = -1;
    
    curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
    
    // Use waitfds with multi handle
    struct curl_waitfd ufds[1];
    unsigned int fd_count = 0;
    
    // Create a FILE* from input data for fd operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (in_file) {
        int fd = fuzz_fileno(in_file);
        if (fd >= 0) {
            ufds[0].fd = fd;
            ufds[0].events = CURL_WAIT_POLLIN;
            ufds[0].revents = 0;
            
            curl_multi_waitfds(multi_handle, ufds, 1, &fd_count);
        }
        assert_file_closed(&in_file);;
    }
    
    // Disable notifications
    curl_multi_notify_disable(multi_handle, fuzz_uint32_t_1);
    
    // Get error strings (simulate errors for demonstration)
    const char *easy_error = curl_easy_strerror(CURLE_OK);
    const char *multi_error = curl_multi_strerror(CURLM_OK);
    
    // Clean up multi handle
    curl_multi_remove_handle(multi_handle, duplicated_handle);
    curl_easy_cleanup(duplicated_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Global cleanup
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
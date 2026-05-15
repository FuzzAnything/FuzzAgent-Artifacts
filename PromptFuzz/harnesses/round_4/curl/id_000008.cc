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
//<ID> 45
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":22,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_version_info","curl_easy_init","curl_global_cleanup","curl_easy_duphandle","curl_easy_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_easy_strerror","curl_mime_free","curl_share_init","curl_share_cleanup","curl_share_strerror","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_notify_disable","curl_multi_strerror","curl_multi_add_handle","curl_multi_strerror","curl_multi_fdset","curl_multi_strerror","curl_multi_waitfds","curl_multi_strerror","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version_info","curl_easy_init","curl_easy_duphandle","curl_easy_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_easy_strerror","curl_mime_free","curl_share_init","curl_share_cleanup","curl_share_strerror","curl_multi_init","curl_multi_notify_disable","curl_multi_strerror","curl_multi_add_handle","curl_multi_strerror","curl_multi_fdset","curl_multi_strerror","curl_multi_waitfds","curl_multi_strerror","curl_multi_perform","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Custom headers for types not automatically included
struct curl_mimepart;

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    // Initialize curl global state
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor for reading
    int infd = fuzz_fileno(in_file);

    // 1. Get version information
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    if (version_info) {
        // Access version string to ensure it's used
        const char *version_str = version_info->version;
    }

    // 2. Create easy handle and duplicate it
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(infd);
	return 0;
    }

    // Duplicate the easy handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);
    if (dup_handle) {
        // Clean up the duplicate immediately since we don't need it
        curl_easy_cleanup(dup_handle);
    }

    // 3. Create a mime structure and set filename using input data
    // Extract filename from input data (first null-terminated string)
    const char *filename = "fuzz_filename.dat";
    if (size > 0) {
        // Find null terminator in input data
        size_t filename_len = 0;
        while (filename_len < size && data[filename_len] != 0) {
            filename_len++;
        }
        
        if (filename_len > 0 && filename_len < size) {
            // Create temporary buffer for filename
            char *temp_filename = (char *)malloc(filename_len + 1);
            if (temp_filename) {
                memcpy(temp_filename, data, filename_len);
                temp_filename[filename_len] = '\0';
                
                // Create mime structure
                curl_mime *mime = curl_mime_init(easy_handle);
                if (mime) {
                    curl_mimepart *part = curl_mime_addpart(mime);
                    if (part) {
                        // Use the curl_mime_filename API
                        CURLcode res = curl_mime_filename(part, temp_filename);
                        
                        // Get error string for demonstration
                        const char *error_str = curl_easy_strerror(res);
                    }
                    curl_mime_free(mime);
                }
                free(temp_filename);
            }
        }
    }

    // 4. Create and cleanup a share handle
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        CURLSHcode share_res = curl_share_cleanup(share_handle);
        
        // Get share error string
        const char *share_error_str = curl_share_strerror(share_res);
    }

    // 5. Create multi handle for waiting operations
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(infd);
	return 0;
    }

    // 6. Disable notifications on multi handle
    CURLMcode notify_res = curl_multi_notify_disable(multi_handle, fuzz_uint32_t_1);
    
    // Get multi error string
    const char *multi_error_str = curl_multi_strerror(notify_res);

    // 7. Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    multi_error_str = curl_multi_strerror(add_res);

    // 8. Set up fdset for multi handle
    fd_set read_fds, write_fds, exc_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&exc_fds);
    
    // Add our input file descriptor to read set
    if (infd >= 0) {
        FD_SET(infd, &read_fds);
    }
    
    int max_fd = infd;
    CURLMcode fdset_res = curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);
    multi_error_str = curl_multi_strerror(fdset_res);

    // 9. Use curl_multi_waitfds
    struct curl_waitfd ufds[1];
    unsigned int fd_count = 0;
    
    if (infd >= 0) {
        ufds[0].fd = infd;
        ufds[0].events = CURL_WAIT_POLLIN;
        ufds[0].revents = 0;
        
        CURLMcode waitfds_res = curl_multi_waitfds(multi_handle, ufds, 1, &fd_count);
        multi_error_str = curl_multi_strerror(waitfds_res);
    }

    // 10. Perform a minimal multi operation
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    // 11. Clean up all resources
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);
    assert_file_closed(&in_file);;
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(infd);
	return 0;
}
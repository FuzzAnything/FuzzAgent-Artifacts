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
//<ID> 49
//<Prompt> []
/*<Combination>: [
*/
//<score> 8.5, nr_unique_branch: 0
//<Quality> {"density":17,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_version_info","curl_easy_init","curl_global_cleanup","curl_easy_duphandle","curl_easy_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_mime_free","curl_share_init","curl_share_cleanup","curl_multi_init","curl_multi_add_handle","curl_multi_fdset","curl_multi_strerror","curl_multi_waitfds","curl_multi_strerror","curl_multi_notify_disable","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_strerror","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version_info","curl_easy_init","curl_easy_duphandle","curl_easy_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_mime_free","curl_share_init","curl_share_cleanup","curl_multi_init","curl_multi_add_handle","curl_multi_fdset","curl_multi_strerror","curl_multi_waitfds","curl_multi_strerror","curl_multi_notify_disable","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_strerror","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl global state
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    // Create a file from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    int in_fd = fuzz_fileno(in_file);

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }
    int out_fd = fuzz_fileno(out_file);

    // Get curl version info
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    if (version_info) {
        // Use version info in some way (e.g., check features)
        (void)version_info->features;
    }

    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Duplicate the easy handle
    CURL *duplicated_handle = curl_easy_duphandle(easy_handle);
    if (duplicated_handle) {
        // Clean up duplicated handle immediately as we don't need it
        curl_easy_cleanup(duplicated_handle);
    }

    // Create mime structure and set filename using input data
    curl_mime *mime = curl_mime_init(easy_handle);
    if (mime) {
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use first 100 bytes or less as filename
            size_t filename_len = size < 100 ? size : 100;
            char *filename = (char *)malloc(filename_len + 1);
            if (filename) {
                memcpy(filename, data, filename_len);
                filename[filename_len] = '\0';
                curl_mime_filename(part, filename);
                free(filename);
            }
        }
        curl_mime_free(mime);
    }

    // Create share handle and clean it up
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        curl_share_cleanup(share_handle);
    }

    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (multi_handle) {
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);

        // Setup file descriptor sets for curl_multi_fdset
        fd_set read_fds, write_fds, exc_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_ZERO(&exc_fds);
        int max_fd = -1;

        // Get file descriptor sets
        CURLMcode fdset_result = curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);
        
        // Get error string for fdset result
        const char *fdset_error = curl_multi_strerror(fdset_result);
        (void)fdset_error; // Use error string to avoid unused variable warning

        // Setup waitfd structure for curl_multi_waitfds
        struct curl_waitfd ufds[2];
        ufds[0].fd = in_fd;
        ufds[0].events = CURL_WAIT_POLLIN;
        ufds[0].revents = 0;
        
        ufds[1].fd = out_fd;
        ufds[1].events = CURL_WAIT_POLLOUT;
        ufds[1].revents = 0;

        unsigned int fd_count = 0;
        
        // Wait for file descriptors
        CURLMcode wait_result = curl_multi_waitfds(multi_handle, ufds, 2, &fd_count);
        
        // Get error string for wait result
        const char *wait_error = curl_multi_strerror(wait_result);
        (void)wait_error; // Use error string to avoid unused variable warning

        // Disable notifications
        curl_multi_notify_disable(multi_handle, CURL_POLL_IN | CURL_POLL_OUT);

        // Remove and clean up easy handle from multi
        curl_multi_remove_handle(multi_handle, easy_handle);
        
        // Clean up multi handle
        curl_multi_cleanup(multi_handle);
    }

    // Get error string for a sample error code
    const char *error_str = curl_easy_strerror(CURLE_OK);
    (void)error_str; // Use error string to avoid unused variable warning

    // Clean up easy handle
    curl_easy_cleanup(easy_handle);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Cleanup global curl state
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
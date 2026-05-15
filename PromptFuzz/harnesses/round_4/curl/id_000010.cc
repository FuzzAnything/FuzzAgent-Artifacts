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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 16, nr_unique_branch: 0
//<Quality> {"density":16,"unique_branches":{},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_share_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_share_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_share_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_free","curl_share_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_free","curl_share_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_waitfds","curl_easy_duphandle","curl_easy_strerror","curl_version_info","curl_mime_filename","curl_easy_cleanup","curl_share_cleanup","curl_multi_notify_disable","curl_multi_fdset","curl_multi_strerror","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_share_init","curl_mime_init","curl_mime_addpart","curl_multi_waitfds","curl_easy_duphandle","curl_easy_strerror","curl_version_info","curl_mime_filename","curl_easy_cleanup","curl_share_cleanup","curl_multi_notify_disable","curl_multi_fdset","curl_multi_strerror","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Custom types from curl that we need
struct curl_waitfd;
struct curl_version_info_data;
typedef void CURLM;
typedef void CURL;
typedef void CURLSH;
struct curl_mimepart;

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Initialize global curl environment
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	

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

    // Create a share handle
    CURLSH *share_handle = curl_share_init();
    if (!share_handle) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_share_cleanup(share_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_share_cleanup(share_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a FILE* from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_mime_free(mime);
        curl_share_cleanup(share_handle);
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
        curl_mime_free(mime);
        curl_share_cleanup(share_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Set up waitfd structures
    struct curl_waitfd waitfds[2];
    waitfds[0].fd = in_fd;
    waitfds[0].events = CURL_WAIT_POLLIN;
    waitfds[0].revents = 0;
    waitfds[1].fd = out_fd;
    waitfds[1].events = CURL_WAIT_POLLOUT;
    waitfds[1].revents = 0;

    // Use curl_multi_waitfds
    unsigned int fd_count = 0;
    CURLMcode waitfds_res = curl_multi_waitfds(multi_handle, waitfds, 2, &fd_count);

    // Duplicate the easy handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);
    if (dup_handle) {
        // Get error string for a sample error code
        const char *error_str = curl_easy_strerror(CURLE_OK);
        
        // Get version info
        curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
        
        // Use input data for filename (truncate to reasonable length)
        size_t filename_len = (size > 100) ? 100 : size;
        char *filename = (char *)malloc(filename_len + 1);
        if (filename) {
            memcpy(filename, data, filename_len);
            filename[filename_len] = '\0';
            
            // Set mime filename using input data
            CURLcode filename_res = curl_mime_filename(part, filename);
            
            free(filename);
        }
        
        // Clean up the duplicate handle
        curl_easy_cleanup(dup_handle);
    }

    // Clean up share handle
    CURLSHcode share_cleanup_res = curl_share_cleanup(share_handle);

    // Disable notifications on multi handle
    CURLMcode notify_res = curl_multi_notify_disable(multi_handle, fuzz_uint32_t_1);

    // Use curl_multi_fdset
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    int max_fd = 0;
    CURLMcode fdset_res = curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);

    // Get error string for multi operations
    const char *multi_error_str = curl_multi_strerror(waitfds_res);

    // Clean up all resources
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
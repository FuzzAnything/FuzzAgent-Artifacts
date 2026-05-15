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
//<ID> 689
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 8
//<Quality> {"density":27,"unique_branches":{"trc_apply_level_by_category":[[617,14,617,32,0,0,4,0],[617,14,617,32,0,0,4,1],[618,8,618,17,0,0,4,0],[621,14,621,32,0,0,4,0],[621,14,621,32,0,0,4,1],[622,8,622,17,0,0,4,0]],"thrdpool_signal":[[247,8,247,19,0,0,4,0]],"trc_opt":[[641,8,641,42,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_trace","curl_version","curl_mprintf","curl_multi_init","curl_global_cleanup","curl_multi_notify_disable","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_mime_data_cb","curl_mime_addpart","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_easy_upkeep","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_strerror","curl_share_strerror","curl_url_strerror","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version","curl_mprintf","curl_multi_init","curl_multi_notify_disable","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data_cb","curl_mime_addpart","curl_mime_filedata","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_easy_upkeep","curl_multi_socket_all","curl_multi_remove_handle","curl_easy_strerror","curl_share_strerror","curl_url_strerror","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Callback functions for mime data
static size_t  id_000045_read_callback(char *buffer, size_t size, size_t nitems, void *arg) {
    FILE *file = (FILE *)arg;
    return fread(buffer, size, nitems, file);
}

static int  id_000045_seek_callback(void *arg, curl_off_t offset, int origin) {
    FILE *file = (FILE *)arg;
    return fseek(file, (long)offset, origin) != 0;
}

static void  id_000045_free_callback(void *arg) {
    // Nothing to free here since we're using fmemopen
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t* f_data, size_t f_size) {
	if(f_size<10) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Set trace configuration
    curl_global_trace(fuzz_str_1);
    
    // Get and print curl version
    char *version = curl_version();
    if (version) {
        curl_mprintf("Curl version: %s\n", version);
    }
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Disable notifications
    curl_multi_notify_disable(multi_handle, fuzz_uint32_t_2);
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Add mime part
    curl_mimepart *part = curl_mime_addpart(mime);
    if (!part) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set mime data callback
    curl_mime_data_cb(part, (curl_off_t)size,  id_000045_read_callback, 
                       id_000045_seek_callback,  id_000045_free_callback, in_file);
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Set file data for another mime part
        curl_mimepart *file_part = curl_mime_addpart(mime);
        if (file_part) {
            curl_mime_filedata(file_part, "output_file");
        }
        assert_file_closed(&out_file);;
    }
    
    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Setup fdset for polling
    fd_set read_fds, write_fds, exc_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&exc_fds);
    int max_fd = 0;
    
    curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);
    
    // Perform upkeep
    curl_easy_upkeep(easy_handle);
    
    // Get running handles count
    int running_handles = 0;
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // Remove handle from multi
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Get error strings (dummy calls to use the functions)
    const char *easy_error = curl_easy_strerror(CURLE_OK);
    const char *share_error = curl_share_strerror(CURLSHE_OK);
    const char *url_error = curl_url_strerror(CURLUE_OK);
    
    // Cleanup
    assert_file_closed(&in_file);;
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    // Free version string if needed
    if (version) {
        // Note: curl_version() returns a static string, so we don't free it
    }
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
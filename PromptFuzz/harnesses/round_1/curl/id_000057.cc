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
//<ID> 1126
//<Prompt> []
/*<Combination>: [
*/
//<score> 52.5, nr_unique_branch: 4
//<Quality> {"density":21,"unique_branches":{"Curl_client_reset":[[327,4,327,40,4,0,4,1]],"curl_multi_remove_handle":[[327,4,327,40,21,0,4,1]],"mstate":[[327,4,327,40,8,0,4,1]],"curl_multi_add_handle":[[327,4,327,40,16,0,4,1]]},"library_calls":["curl_global_init","curl_global_trace","curl_version","curl_getdate","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_easy_duphandle","curl_share_init","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_multi_setopt","curl_multi_setopt","curl_easy_reset","curl_easy_unescape","curl_free","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_timeout","curl_multi_poll","curl_multi_info_read","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version","curl_getdate","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_easy_duphandle","curl_share_init","curl_multi_init","curl_multi_setopt","curl_multi_setopt","curl_easy_reset","curl_easy_unescape","curl_free","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_timeout","curl_multi_poll","curl_multi_info_read","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" {
}

// Helper function for curl_mvprintf usage
static int  id_000057_helper_mvprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvprintf(format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t* f_data, size_t f_size) {
	if(f_size<19) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Too small for meaningful fuzzing
    
    // Initialize global curl environment
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // Set trace configuration from input data
    char trace_config[256];
    size_t trace_len = size > 255 ? 255 : size;
    memcpy(trace_config, data, trace_len);
    trace_config[trace_len] = '\0';
    curl_global_trace(trace_config);
    
    // Get and print curl version using curl_mvprintf
    char* version_str = curl_version();
    if (version_str) {
         id_000057_helper_mvprintf("CURL Version: %s\n", version_str);
    }
    
    // Parse date from input
    char date_str[256];
    size_t date_len = (size - trace_len) > 255 ? 255 : (size - trace_len);
    if (date_len > 0) {
        memcpy(date_str, data + trace_len, date_len);
        date_str[date_len] = '\0';
        time_t parsed_date = curl_getdate(date_str, NULL);
        (void)parsed_date;  // Use parsed_date to avoid unused warning
    }
    
    // Create a file from input data for reading
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a file for output
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Use in_fd to avoid unused warning
    (void)out_fd;  // Use out_fd to avoid unused warning
    
    // Initialize easy handle
    CURL* easy_handle = curl_easy_init();
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
    
    // Set some easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    
    // Use curl_easy_upkeep
    curl_easy_upkeep(easy_handle);
    
    // Duplicate the easy handle
    CURL* dup_easy_handle = curl_easy_duphandle(easy_handle);
    
    // Initialize share handle
    CURLSH* share_handle = curl_share_init();
    
    // Initialize multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        if (dup_easy_handle) curl_easy_cleanup(dup_easy_handle);
        if (share_handle) curl_share_cleanup(share_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set multi handle options
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 10L);
    
    // Reset easy handle
    curl_easy_reset(easy_handle);
    
    // Create a URL escape/unescape test
    char test_str[] = "test%20string%21";
    int outlength = 0;
    char* unescaped = curl_easy_unescape(easy_handle, fuzz_str_1, static_cast<int>(fuzz_str_sz_1), &outlength);
    if (unescaped) {
        // Do something with unescaped
        curl_free(unescaped);
    }
    
    // Add handles to multi handle
    if (easy_handle) {
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    if (dup_easy_handle) {
        curl_multi_add_handle(multi_handle, dup_easy_handle);
    }
    
    // Perform multi transfers
    int running_handles = 0;
    int max_loops = 5;  // Limit loops for fuzzing
    
    for (int i = 0; i < max_loops; i++) {
        CURLMcode mc = curl_multi_perform(multi_handle, &running_handles);
        if (mc != CURLM_OK) break;
        
        // Check for timeouts
        long timeout = 0;
        curl_multi_timeout(multi_handle, &timeout);
        
        // Prepare for polling
        struct curl_waitfd extra_fds[1];
        extra_fds[0].fd = in_fd;
        extra_fds[0].events = CURL_WAIT_POLLIN;
        extra_fds[0].revents = 0;
        
        int ret = 0;
        // Poll with timeout
        curl_multi_poll(multi_handle, extra_fds, 1, timeout > 0 ? (int)timeout : 100, &ret);
        
        // Read messages
        int msgs_in_queue = 0;
        CURLMsg* msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        if (msg && msg->msg == CURLMSG_DONE) {
            // Handle completed transfer
        }
        
        if (running_handles == 0) break;
    }
    
    // Cleanup multi handles
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    if (dup_easy_handle) {
        curl_multi_remove_handle(multi_handle, dup_easy_handle);
    }
    
    // Cleanup
    curl_multi_cleanup(multi_handle);
    if (share_handle) curl_share_cleanup(share_handle);
    if (easy_handle) curl_easy_cleanup(easy_handle);
    if (dup_easy_handle) curl_easy_cleanup(dup_easy_handle);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Final cleanup
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
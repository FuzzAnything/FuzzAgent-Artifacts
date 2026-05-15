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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 6
//<Quality> {"density":21,"unique_branches":{"curl_version":[[191,6,191,18,0,0,4,1],[265,14,265,19,0,0,4,0],[265,14,265,19,0,0,4,1],[268,8,268,25,0,0,4,1],[270,8,270,9,0,0,4,0],[270,8,270,9,0,0,4,1]]},"library_calls":["curl_global_init","curl_version","curl_global_cleanup","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_easy_duphandle","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_filename","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_add_handle","curl_multi_fdset","curl_multi_poll","curl_multi_perform","curl_ws_start_frame","curl_easy_ssls_import","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version","curl_easy_init","curl_easy_duphandle","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_filename","curl_easy_setopt","curl_multi_init","curl_multi_add_handle","curl_multi_add_handle","curl_multi_fdset","curl_multi_poll","curl_multi_perform","curl_ws_start_frame","curl_easy_ssls_import","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Helper function to use curl_mvsprintf with variable arguments
static int  id_000000_my_curl_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<44) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Initialize libcurl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // Get curl version
    char *version = curl_version();
    if (!version) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create a FILE* to read input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create a FILE* to write output data
    FILE *out_file = fopen("output_file", "wb");
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
    
    // Create easy handle
    CURL *easy = curl_easy_init();
    if (!easy) {
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
    CURL *dup_easy = curl_easy_duphandle(easy);
    if (!dup_easy) {
        curl_easy_cleanup(easy);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create a mime structure
    curl_mime *mime = curl_mime_init(easy);
    if (mime) {
        // Add a mime part
        curl_mimepart *part = curl_mime_addpart(mime);
        if (part) {
            // Use part of input data for mime data
            size_t mime_data_size = size > 100 ? 100 : size;
            CURLcode res = curl_mime_data(part, (const char *)data, mime_data_size);
            
            // Set filename for mime part
            res = curl_mime_filename(part, fuzz_str_1);
        }
        
        // Set mime data to easy handle (for potential POST)
        curl_easy_setopt(easy, CURLOPT_MIMEPOST, mime);
    }
    
    // Create a multi handle
    CURLM *multi = curl_multi_init();
    if (multi) {
        // Add both easy handles to multi handle
        curl_multi_add_handle(multi, easy);
        curl_multi_add_handle(multi, dup_easy);
        
        // Prepare fd_set structures
        fd_set read_fd_set, write_fd_set, exc_fd_set;
        FD_ZERO(&read_fd_set);
        FD_ZERO(&write_fd_set);
        FD_ZERO(&exc_fd_set);
        
        // Add our file descriptors to the sets
        FD_SET(in_fd, &read_fd_set);
        FD_SET(out_fd, &write_fd_set);
        
        int max_fd = (in_fd > out_fd) ? in_fd : out_fd;
        
        // Use multi_fdset
        CURLMcode mres = curl_multi_fdset(multi, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
        
        // Prepare poll structure
        struct curl_waitfd extra_fds[2];
        extra_fds[0].fd = in_fd;
        extra_fds[0].events = CURL_WAIT_POLLIN;
        extra_fds[0].revents = 0;
        
        extra_fds[1].fd = out_fd;
        extra_fds[1].events = CURL_WAIT_POLLOUT;
        extra_fds[1].revents = 0;
        
        int ret;
        // Use multi_poll with timeout of 0 (non-blocking)
        mres = curl_multi_poll(multi, extra_fds, 2, fuzz_int32_t_2, &ret);
        
        // Try to perform some operations
        int still_running = 0;
        curl_multi_perform(multi, &still_running);
        
        // Try to start a WebSocket frame (may fail since we're not in WS mode)
        // Use a portion of data for frame_len
        curl_off_t frame_len = 0;
        if (size >= sizeof(curl_off_t)) {
            memcpy(&frame_len, data, sizeof(curl_off_t));
            // Ensure frame_len is reasonable
            if (frame_len > 10000) frame_len = 10000;
        }
        CURLcode ws_res = curl_ws_start_frame(easy, fuzz_uint32_t_3, fuzz_int64_t_4);
        
        // Try SSL session import (may fail without proper setup)
        // Use portions of input data for parameters
        const char *session_key = "test_session";
        size_t shmac_len = size > 32 ? 32 : size;
        size_t sdata_len = size > 64 ? 64 : size;
        
        // Ensure we don't overflow
        if (shmac_len + sdata_len <= size) {
            CURLcode ssl_res = curl_easy_ssls_import(easy, fuzz_str_5, 
                                                     data, shmac_len,
                                                     data + shmac_len, sdata_len);
        }
        
        // Remove handles from multi
        curl_multi_remove_handle(multi, easy);
        curl_multi_remove_handle(multi, dup_easy);
        
        // Cleanup multi
        curl_multi_cleanup(multi);
    }
    
    // Use curl_mvsprintf with version info
    char buffer[256];
    int printed =  id_000000_my_curl_mvsprintf(buffer, "Curl version: %s, Input size: %zu", 
                                    version, size);
    
    // Write the formatted string to output file
    if (printed > 0) {
        fwrite(buffer, 1, printed, out_file);
    }
    
    // Cleanup
    if (mime) {
        curl_mime_free(mime);
    }
    
    curl_easy_cleanup(dup_easy);
    curl_easy_cleanup(easy);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
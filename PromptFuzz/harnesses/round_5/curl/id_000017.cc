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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 112, nr_unique_branch: 24
//<Quality> {"density":16,"unique_branches":{"curlx_str_until":[[57,15,57,28,0,0,4,1],[59,8,59,19,0,0,4,0],[63,6,63,10,0,0,4,0]],"trc_opt":[[630,9,630,49,0,0,4,1],[634,8,634,21,0,0,4,0],[638,13,638,26,0,0,4,0],[641,8,641,42,0,0,4,1],[643,13,643,52,0,0,4,1],[645,13,645,51,0,0,4,1],[647,13,647,49,0,0,4,1],[649,13,649,47,0,0,4,1],[656,8,656,38,0,0,4,1]],"curlx_str_nudge":[[269,6,269,21,0,0,4,0]],"trc_apply_level_by_name":[[599,14,599,32,0,0,4,0],[599,14,599,32,0,0,4,1],[600,8,600,59,0,0,4,0],[600,8,600,59,0,0,4,1],[605,14,605,32,0,0,4,0],[605,14,605,32,0,0,4,1],[606,8,606,61,0,0,4,0],[606,8,606,61,0,0,4,1]],"curlx_str_single":[[137,6,137,21,0,0,4,1]],"curlx_str_casecompare":[[251,11,251,29,0,0,4,1],[251,33,251,70,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_global_trace","curl_getdate","curl_easy_init","curl_global_cleanup","curl_easy_duphandle","curl_easy_cleanup","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_timeout","curl_multi_socket_action","curl_ws_start_frame","curl_ws_send","curl_easy_nextheader","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_getdate","curl_easy_init","curl_easy_duphandle","curl_multi_init","curl_multi_add_handle","curl_multi_timeout","curl_multi_socket_action","curl_ws_start_frame","curl_ws_send","curl_easy_nextheader","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // Create input file from memory
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);

    // Set up global tracing with first part of input as config
    size_t config_len = size > 100 ? 100 : size;
    char *trace_config = new char[config_len + 1];
    memcpy(trace_config, data, config_len);
    trace_config[config_len] = '\0';
    curl_global_trace(trace_config);
    delete[] trace_config;

    // Parse date from input (skip first config_len bytes)
    if (size > config_len) {
        size_t date_len = size - config_len > 50 ? 50 : size - config_len;
        char *date_str = new char[date_len + 1];
        memcpy(date_str, data + config_len, date_len);
        date_str[date_len] = '\0';
        time_t parsed_time = curl_getdate(date_str, nullptr);
        (void)parsed_time; // Use result to avoid unused variable warning
        delete[] date_str;
    }

    // Create easy handle and duplicate it
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    CURL *duplicated_handle = curl_easy_duphandle(easy_handle);
    if (!duplicated_handle) {
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create multi handle and add easy handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(duplicated_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    curl_multi_add_handle(multi_handle, easy_handle);

    // Get timeout from multi handle
    long timeout_ms = 0;
    curl_multi_timeout(multi_handle, &timeout_ms);

    // Use multi socket with input file descriptor
    int running_handles = 0;
    curl_multi_socket(multi_handle, (curl_socket_t)in_fd, &running_handles);

    // Prepare WebSocket frame data
    size_t ws_data_len = size > 512 ? 512 : size;
    const void *ws_buffer = static_cast<const void*>(data);

    // Start WebSocket frame
    size_t frame_len = ws_data_len > 256 ? 256 : ws_data_len;
    curl_ws_start_frame(easy_handle, CURLWS_TEXT, static_cast<curl_off_t>(frame_len));

    // Send WebSocket data
    size_t bytes_sent = 0;
    curl_ws_send(easy_handle, ws_buffer, ws_data_len, &bytes_sent, 
                 static_cast<curl_off_t>(frame_len), CURLWS_TEXT);

    // Try to get headers (may return nullptr if no headers)
    struct curl_header *header = curl_easy_nextheader(easy_handle, 
                                                     CURLH_HEADER, 0, nullptr);

    // Clean up multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);

    // Clean up easy handles
    curl_easy_cleanup(duplicated_handle);
    curl_easy_cleanup(easy_handle);

    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;

    // Global cleanup
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 6, nr_unique_branch: 0
//<Quality> {"density":12,"unique_branches":{},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_global_trace","curl_easy_init","curl_global_cleanup","curl_easy_duphandle","curl_easy_cleanup","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_wait","curl_easy_header","curl_easy_option_by_name","curl_msprintf","curl_msnprintf","curl_pushheader_bynum","curl_msprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_easy_init","curl_easy_duphandle","curl_multi_init","curl_multi_add_handle","curl_multi_wait","curl_easy_header","curl_easy_option_by_name","curl_msprintf","curl_msnprintf","curl_pushheader_bynum","curl_msprintf","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":1}
/*Here's a C++ fuzz driver that uses the specified curl APIs to achieve the event of "parsing and processing HTTP/2 server push headers with tracing enabled":

*/


// Include necessary curl headers
extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<54) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeCharArray(char, fuzz_char_array_1, fuzz_char_array_size_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_7, fdp);
	
		const char * fuzzer_var_curl_global_trace_3_0 = fuzz_char_array_1;//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Initialize curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // Create input file from fuzzer data
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

    // Extract configuration string from input data (first 5 bytes)
    char config_buf[6] = {0};
    size_t config_len = size > 5 ? 5 : size;
    memcpy(config_buf, data, config_len);
    
    // Set global trace configuration
    CURLcode trace_res = curl_global_trace(fuzzer_var_curl_global_trace_3_0);

    // Create easy handle and set some options
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

    // Duplicate the handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);
    if (!dup_handle) {
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

    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(dup_handle);
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

    // Add the duplicated handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, dup_handle);

    // Set up waitfd structure using input file descriptor
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = in_fd;
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;

    // Extract timeout from input data (next 4 bytes after config)
    int timeout_ms = 0;
    if (size >= 9) {
        memcpy(&timeout_ms, data + 5, 4);
        // Ensure reasonable timeout value
        if (timeout_ms < 0) timeout_ms = 0;
        if (timeout_ms > 10000) timeout_ms = 10000;
    }

    int ret = 0;
    // Wait for activity on the multi handle
    CURLMcode wait_res = curl_multi_wait(multi_handle, extra_fds, 1, fuzz_int32_t_2, &ret);

    // Try to get header information
    struct curl_header *header_out = nullptr;
    const char *header_name = "Content-Type";
    size_t header_index = 0;
    
    // Extract header index from input data
    if (size >= 10) {
        header_index = data[9] % 10;  // Keep index small
    }
    
    CURLHcode header_res = curl_easy_header(dup_handle, fuzz_str_3, fuzz_uint64_t_4, 
                                           CURLH_HEADER, fuzz_int32_t_5, &header_out);

    // Look up an easy option by name
    const char *option_name = "URL";
    const struct curl_easyoption *option = curl_easy_option_by_name(fuzz_str_6);
    
    // Use msprintf to format information about the option
    char buffer[256];
    if (option) {
        int print_res = curl_msprintf(buffer, "Option found: %s", option_name);
        
        // Also use msnprintf for safe formatting
        char buffer2[256];
        int print_res2 = curl_msnprintf(buffer2, sizeof(buffer2), 
                                       "Option ID: %d", option->id);
        
        // Write to output file
        if (print_res > 0) {
            fwrite(buffer, 1, print_res, out_file);
        }
        if (print_res2 > 0) {
            fwrite(buffer2, 1, print_res2, out_file);
        }
    }

    // Simulate push headers (using a dummy structure)
    // In real usage, this would come from HTTP/2 server push
    struct curl_pushheaders *push_headers = nullptr;
    
    // Try to get push header by number (even though we don't have real push headers)
    // This demonstrates the API usage
    char *push_header = curl_pushheader_bynum(push_headers, fuzz_uint64_t_7);
    
    // If we had a real push header, we could process it
    if (push_header) {
        // Process push header
        char header_buffer[256];
        int header_print_res = curl_msprintf(header_buffer, 
                                           "Push header: %s", push_header);
        if (header_print_res > 0) {
            fwrite(header_buffer, 1, header_print_res, out_file);
        }
    }

    // Clean up multi handle
    if (multi_handle) {
        curl_multi_remove_handle(multi_handle, dup_handle);
        curl_multi_cleanup(multi_handle);
    }

    // Clean up easy handles
    if (dup_handle) {
        curl_easy_cleanup(dup_handle);
    }
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }

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
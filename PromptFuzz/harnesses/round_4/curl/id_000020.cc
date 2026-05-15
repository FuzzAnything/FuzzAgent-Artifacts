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
//<ID> 216
//<Prompt> []
/*<Combination>: [
*/
//<score> 54, nr_unique_branch: 1
//<Quality> {"density":27,"unique_branches":{"lookup":[[48,13,48,20,0,0,4,1]]},"library_calls":["curl_global_trace","curl_easy_init","curl_global_cleanup","curl_easy_duphandle","curl_easy_cleanup","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_add_handle","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_wait","curl_msprintf","curl_msnprintf","curl_easy_option_by_name","curl_easy_setopt","curl_easy_header","curl_pushheader_bynum","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_trace","curl_easy_init","curl_easy_duphandle","curl_multi_init","curl_multi_add_handle","curl_multi_add_handle","curl_multi_wait","curl_msprintf","curl_msnprintf","curl_easy_option_by_name","curl_easy_setopt","curl_easy_header","curl_pushheader_bynum","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified curl APIs:

*/


// Include necessary headers for curl

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* f_data, size_t f_size) {
	if(f_size<53) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_6, fdp);
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    // Parse input data: assume first byte is config type, rest is output data
    uint8_t config_type = data[0];
    const uint8_t* output_data = data + 1;
    size_t output_size = size - 1;
    
    if (output_size < 1) {
        return 0;
    }

    // Initialize curl globally with tracing
    const char* trace_config = nullptr;
    switch (config_type % 3) {
        case 0: trace_config = "http/2"; break;
        case 1: trace_config = "ssl"; break;
        case 2: trace_config = "http/3"; break;
    }
    curl_global_trace(trace_config);

    // Create an easy handle
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Duplicate the handle
    CURL* dup_handle = curl_easy_duphandle(easy_handle);
    if (!dup_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create a multi handle for waiting
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(dup_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Add handles to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    curl_multi_add_handle(multi_handle, dup_handle);

    // Create output file to write the response data
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_remove_handle(multi_handle, dup_handle);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(dup_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // Write some output data to file (simulating curl output)
    fwrite(output_data, 1, output_size, out_file);
    fflush(out_file);
    assert_file_closed(&out_file);;

    // Reopen the file for reading
    FILE* in_file = fopen("output_file", "rb");
    if (!in_file) {
        remove("output_file");
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_multi_remove_handle(multi_handle, dup_handle);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(dup_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	return 0;
    }

    // Get file descriptor for waiting
    int fd = fuzz_fileno(in_file);
    
    // Create waitfd structure for curl_multi_wait
    struct curl_waitfd wait_fd;
    wait_fd.fd = fd;
    wait_fd.events = CURL_WAIT_POLLIN;
    wait_fd.revents = 0;

    // Wait on the file descriptor (simulating network I/O)
    int running_handles = 0;
    int ret = 0;
    curl_multi_wait(multi_handle, &wait_fd, 1, fuzz_int32_t_1, &ret);

    // Use formatted printing functions
    char buffer[256];
    char small_buffer[64];
    
    // Simulate some header processing
    const char* format_str = "Header-Name: Value-%d";
    int msprintf_result = curl_msprintf(buffer, format_str, (int)(output_size % 100));
    
    // Use msnprintf with limited buffer
    const char* header_format = "X-Test-Header: %.*s";
    int msnprintf_result = curl_msnprintf(small_buffer, sizeof(small_buffer), 
                                          header_format, 
                                          (int)(output_size % 50), 
                                          reinterpret_cast<const char*>(output_data));

    // Look up an option by name
    const char* option_name = "CURLOPT_URL";
    const struct curl_easyoption* option = curl_easy_option_by_name(fuzz_str_2);
    
    // Set the option on the easy handle if found
    if (option) {
        // Use the first part of output data as URL
        std::string url = "http://example.com/";
        if (output_size > 0) {
            url += std::string(reinterpret_cast<const char*>(output_data), 
                              std::min(output_size, (size_t)20));
        }
        curl_easy_setopt(easy_handle, option->id, url.c_str());
    }

    // Try to get headers from the easy handle
    struct curl_header* header_out = nullptr;
    CURLHcode header_result = curl_easy_header(easy_handle, fuzz_str_3, fuzz_uint64_t_4, 
                                               CURLH_HEADER, fuzz_int32_t_5, &header_out);

    // Simulate push headers (normally used in HTTP/2 server push)
    // We'll create a mock pushheaders structure
    struct curl_pushheaders* push_headers = nullptr;
    // In real usage, this would be provided by curl in a push callback
    // For fuzzing, we'll just call the function with null
    char* push_header = curl_pushheader_bynum(push_headers, fuzz_uint64_t_6);

    // Clean up
    assert_file_closed(&in_file);;
    remove("output_file");
    
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_remove_handle(multi_handle, dup_handle);
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(dup_handle);
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}
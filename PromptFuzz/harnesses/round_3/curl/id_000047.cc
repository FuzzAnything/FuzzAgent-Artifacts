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
//<ID> 877
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"curl_ws_start_frame":[[1887,6,1887,17,0,0,4,1],[1893,6,1893,9,0,0,4,0]],"parse_scheme":[[945,26,945,33,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_url","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_url_dup","curl_url_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_url_set","curl_url_set","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_option_by_id","curl_multi_socket_all","curl_easy_pause","curl_ws_start_frame","curl_easy_header","curl_easy_getinfo","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_url","curl_url_dup","curl_url_set","curl_url_set","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_easy_option_by_id","curl_multi_socket_all","curl_easy_pause","curl_ws_start_frame","curl_easy_header","curl_easy_getinfo","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Custom callback for curl_mvsprintf
static int  id_000047_my_write_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    return size * nitems; // Just accept all data
}

// Custom function to use curl_mvsprintf safely
static int  id_000047_safe_curl_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<35) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    // Initialize CURL globally
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Create output file
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

    // Initialize easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Initialize URL handle
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Duplicate URL handle
    CURLU *url_dup = curl_url_dup(url_handle);
    if (!url_dup) {
        curl_url_cleanup(url_handle);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set URL from fuzzer data (using first part as URL string)
    // We'll use up to 256 bytes for URL or less if input is smaller
    size_t url_len = (size > 256) ? 256 : size;
    char *url_str = (char *)malloc(url_len + 1);
    if (url_str) {
        memcpy(url_str, data, url_len);
        url_str[url_len] = '\0';
        
        // Set URL in both handles
        curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_1);
        curl_url_set(url_dup, CURLUPART_URL, url_str, fuzz_uint32_t_2);
        
        free(url_str);
    }

    // Set easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION,  id_000047_my_write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Get option by ID
    const struct curl_easyoption *opt = curl_easy_option_by_id(CURLOPT_URL);
    if (opt) {
        // Use curl_mvsprintf with the option name
        char buffer[256];
         id_000047_safe_curl_mvsprintf(buffer, "Testing option: %s\n", opt->name);
        // Write to output file
        fwrite(buffer, strlen(buffer), 1, out_file);
    }

    // Perform multi handle operations
    int running_handles = 0;
    curl_multi_socket_all(multi_handle, &running_handles);

    // Pause the easy handle
    curl_easy_pause(easy_handle, CURLPAUSE_ALL);

    // Start WebSocket frame (if supported)
    // We'll use a small frame length based on input size
    curl_off_t frame_len = (curl_off_t)(size % 1024);
    curl_ws_start_frame(easy_handle, CURLWS_BINARY, frame_len);

    // Get header information
    struct curl_header *header = nullptr;
    curl_easy_header(easy_handle, fuzz_str_3, fuzz_uint64_t_4, CURLH_HEADER, fuzz_int32_t_5, &header);

    // Get info from easy handle
    char *effective_url = nullptr;
    curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);

    // Clean up multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_multi_cleanup(multi_handle);

    // Clean up easy handle
    curl_easy_cleanup(easy_handle);

    // Clean up URL handles
    curl_url_cleanup(url_dup);
    curl_url_cleanup(url_handle);

    // Close files
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;

    // Global cleanup
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
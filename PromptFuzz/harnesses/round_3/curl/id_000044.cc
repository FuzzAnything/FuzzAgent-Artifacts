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
//<ID> 791
//<Prompt> []
/*<Combination>: [
*/
//<score> 99, nr_unique_branch: 2
//<Quality> {"density":33,"unique_branches":{"Curl_hash_pick":[[246,11,246,13,0,0,4,1]],"parseurlandfillconn":[[1665,31,1665,70,0,0,4,1]]},"library_calls":["curl_global_init","curl_version_info","curl_global_cleanup","curl_msnprintf","curl_url","curl_global_cleanup","curl_url_cleanup","curl_global_cleanup","curl_url_set","curl_url_dup","curl_url_cleanup","curl_global_cleanup","curl_multi_init","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup","curl_multi_socket_action","curl_ws_recv","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version_info","curl_msnprintf","curl_url","curl_url_set","curl_url_dup","curl_multi_init","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_multi_add_handle","curl_multi_fdset","curl_multi_socket_action","curl_ws_recv","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	
    
    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 1. Get curl version info using curl_version_info
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    if (!version_info) {
        curl_global_cleanup();
        return 0;
    }
    
    // 2. Create a buffer and use curl_msnprintf to write version string
    char version_buffer[256];
    int printed = curl_msnprintf(version_buffer, sizeof(version_buffer),
                                 "CURL Version: %s, SSL: %s, Protocols: %s",
                                 version_info->version,
                                 version_info->ssl_version,
                                 version_info->protocols ? *version_info->protocols : "none");
    
    // 3. Create a URL handle and duplicate it using curl_url_dup
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set URL from input data (first part)
    size_t url_len = size > 100 ? 100 : size / 2;
    char *url_str = (char*)malloc(url_len + 1);
    if (!url_str) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(url_str, data, url_len);
    url_str[url_len] = '\0';
    
    curl_url_set(url_handle, CURLUPART_URL, url_str, fuzz_uint32_t_1);
    
    // Duplicate the URL handle
    CURLU *dup_url = curl_url_dup(url_handle);
    if (!dup_url) {
        free(url_str);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Create a multi handle and easy handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_url_cleanup(dup_url);
        free(url_str);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_url_cleanup(dup_url);
        free(url_str);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Set some easy handle options
    curl_easy_setopt(easy_handle, CURLOPT_URL, url_str);
    curl_easy_setopt(easy_handle, CURLOPT_WS_OPTIONS, CURLWS_RAW_MODE);
    
    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // 5. Use curl_multi_fdset
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    int max_fd = -1;
    
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    
    CURLMcode fdset_result = curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, 
                                               &exc_fd_set, &max_fd);
    
    // 6. Use curl_multi_socket with a dummy socket
    int running_handles = 0;
    curl_socket_t dummy_socket = fuzz_fileno(stdin); // Use stdin as dummy socket
    
    // Create a file from input data for reading
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_url_cleanup(dup_url);
        free(url_str);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(dummy_socket);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_url_cleanup(dup_url);
        free(url_str);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(dummy_socket);
	return 0;
    }
    
    // Use the file descriptor from input file with curl_multi_socket
    curl_socket_t file_socket = fuzz_fileno(in_file);
    CURLMcode socket_result = curl_multi_socket(multi_handle, file_socket, &running_handles);
    
    // 7. Try to receive WebSocket data using curl_ws_recv
    // Prepare a buffer for WebSocket data
    uint8_t ws_buffer[1024];
    size_t received = 0;
    const struct curl_ws_frame *meta = nullptr;
    
    // Simulate WebSocket receive (this would normally require an established WS connection)
    CURLcode ws_result = curl_ws_recv(easy_handle, ws_buffer, sizeof(ws_buffer), 
                                      &received, &meta);
    
    // Write some data to output file based on what we processed
    if (out_file) {
        fwrite(version_buffer, 1, printed > 0 ? (size_t)printed : 0, out_file);
        fwrite(url_str, 1, strlen(url_str), out_file);
        
        // Write some of the input data to output file
        size_t write_size = size > 512 ? 512 : size;
        fwrite(data, 1, write_size, out_file);
    }
    
    // Clean up
    if (out_file) assert_file_closed(&out_file);;
    if (in_file) assert_file_closed(&in_file);;
    
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_url_cleanup(dup_url);
    free(url_str);
    curl_url_cleanup(url_handle);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(file_socket);
	assert_fd_closed(dummy_socket);
	return 0;
}
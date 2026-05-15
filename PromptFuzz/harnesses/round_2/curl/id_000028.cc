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
//<ID> 329
//<Prompt> []
/*<Combination>: [
*/
//<score> 24, nr_unique_branch: 1
//<Quality> {"density":12,"unique_branches":{"Curl_close":[[217,31,217,42,0,0,4,0]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_getenv","curl_strequal","curl_free","curl_multi_setopt","curl_multi_strerror","curl_multi_add_handle","curl_easy_upkeep","curl_multi_notify_disable","curl_multi_timeout","curl_multi_waitfds","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_getenv","curl_strequal","curl_free","curl_multi_setopt","curl_multi_strerror","curl_multi_add_handle","curl_easy_upkeep","curl_multi_notify_disable","curl_multi_timeout","curl_multi_waitfds","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Initialize libcurl
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // Create output file for writing
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        curl_global_cleanup();
        assert_file_closed(&out_file);
	return 0;
    }

    // Create input FILE* from fuzzing data
    FILE* in_file = fmemopen((void*)data, size, "rb");
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

    // Create multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Create easy handle
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set up environment variable checking
    const char* env_var = "FUZZ_ENV_VAR";
    char* env_value = curl_getenv(env_var);
    
    // Compare environment variable value with expected value using curl_strequal
    const char* expected_value = "FUZZ_VALUE";
    int env_equal = 0;
    if (env_value) {
        env_equal = curl_strequal(env_value, expected_value);
        curl_free(env_value);
    }

    // Set multi handle options using input data
    // Use first byte to determine which option to set
    if (size > 0) {
        CURLMcode setopt_res = curl_multi_setopt(multi_handle, 
            CURLMOPT_MAXCONNECTS, (long)(data[0] % 100));
        
        // Use multi_strerror to get error string if needed
        if (setopt_res != CURLM_OK) {
            const char* error_str = curl_multi_strerror(setopt_res);
            // Can optionally write error string to output
        }
    }

    // Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    
    // Perform easy handle upkeep
    CURLcode upkeep_res = curl_easy_upkeep(easy_handle);
    
    // Disable notifications
    CURLMcode notify_res = curl_multi_notify_disable(multi_handle, 0);
    
    // Get timeout
    long timeout;
    CURLMcode timeout_res = curl_multi_timeout(multi_handle, &timeout);
    
    // Prepare for waitfds
    struct curl_waitfd ufds[2];
    unsigned int fd_count = 0;
    
    // Set up file descriptors for waiting
    ufds[0].fd = in_fd;
    ufds[0].events = CURL_WAIT_POLLIN;
    ufds[0].revents = 0;
    
    ufds[1].fd = out_fd;
    ufds[1].events = CURL_WAIT_POLLOUT;
    ufds[1].revents = 0;
    
    // Wait for file descriptors
    CURLMcode wait_res = curl_multi_waitfds(multi_handle, ufds, 2, &fd_count);
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Clean up FILE* handles
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up libcurl
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	assert_file_closed(&out_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}
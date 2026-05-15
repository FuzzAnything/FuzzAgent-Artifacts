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
//<ID> 1220
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 14
//<Quality> {"density":22,"unique_branches":{"setopt_long_misc":[[1233,3,1233,29,0,0,4,1],[1238,3,1238,25,0,0,4,1],[1241,3,1241,29,0,0,4,0],[1242,8,1242,16,0,0,4,1],[1244,8,1244,30,0,0,4,0],[1245,8,1245,54,0,0,4,1],[1251,3,1251,26,0,0,4,1],[1256,3,1256,27,0,0,4,1],[1261,3,1261,28,0,0,4,1],[1265,3,1265,28,0,0,4,1],[1283,3,1283,27,0,0,4,1],[1292,3,1292,10,0,0,4,1]],"setopt_long_proto":[[1216,3,1216,26,0,0,4,1],[1221,3,1221,10,0,0,4,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_global_cleanup","curl_easy_init","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_msprintf","curl_multi_waitfds","curl_multi_socket_action","curl_easy_pause","curl_easy_reset","curl_easy_setopt","curl_easy_setopt","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_multi_add_handle","curl_msprintf","curl_multi_waitfds","curl_multi_socket_action","curl_easy_pause","curl_easy_reset","curl_easy_setopt","curl_easy_setopt","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize curl globally
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
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

    // Add easy handle to multi handle
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor from input file
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Prepare buffer for curl_msprintf
    char buffer[256];
    const char *format_string = "Data size: %zu bytes";
    
    // Use curl_msprintf to format string (API 2)
    int printed = curl_msprintf(buffer, format_string, size);
    
    // Prepare curl_waitfd structure for curl_multi_waitfds
    struct curl_waitfd wait_fd;
    wait_fd.fd = fd;
    wait_fd.events = CURL_WAIT_POLLIN;
    wait_fd.revents = 0;
    
    unsigned int fd_count = 0;
    
    // Use curl_multi_waitfds (API 1)
    CURLMcode wait_result = curl_multi_waitfds(multi_handle, &wait_fd, 1, &fd_count);
    
    int running_handles = 0;
    
    // Use curl_multi_socket (API 3)
    CURLMcode socket_result = curl_multi_socket(multi_handle, fd, &running_handles);
    
    // Use curl_easy_pause to pause the handle (API 4)
    CURLcode pause_result = curl_easy_pause(easy_handle, CURLPAUSE_ALL);
    
    // Use curl_easy_reset to reset the handle (API 5)
    curl_easy_reset(easy_handle);
    
    // Try to read some data from the file to simulate usage
    char read_buffer[1024];
    if (fread(read_buffer, 1, size > 1024 ? 1024 : size, in_file) > 0) {
        // Perform some operation with the read data
        // For example, set it as POST data for the easy handle
        curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, read_buffer);
        curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, 
                         size > 1024 ? 1024 : size);
    }

    // Clean up resources in reverse order
    assert_file_closed(&in_file);;
    
    // Remove easy handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Global cleanup
    curl_global_cleanup();

    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
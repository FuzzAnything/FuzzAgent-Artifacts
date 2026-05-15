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
//<ID> 234
//<Prompt> []
/*<Combination>: [
*/
//<score> 44, nr_unique_branch: 2
//<Quality> {"density":22,"unique_branches":{"parsedate":[[425,13,425,28,0,0,4,0],[439,19,439,34,0,0,4,0]]},"library_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_getdate","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_ssls_import","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_perform","curl_multi_poll","curl_multi_socket_action","curl_easy_recv","curl_easy_pause","curl_easy_pause","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_getdate","curl_easy_setopt","curl_easy_ssls_import","curl_multi_add_handle","curl_multi_perform","curl_multi_poll","curl_multi_socket_action","curl_easy_recv","curl_easy_pause","curl_easy_pause","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include necessary curl headers

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing

    // 1. Initialize global curl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	

    // 2. Create multi handle and easy handle
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    if (!multi_handle || !easy_handle) {
        if (easy_handle) curl_easy_cleanup(easy_handle);
        if (multi_handle) curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // 3. Parse date from input (first 50 bytes as string)
    const char *date_str = reinterpret_cast<const char*>(data);
    size_t date_len = size > 50 ? 50 : size;
    char *date_copy = static_cast<char*>(malloc(date_len + 1));
    if (!date_copy) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(date_copy, date_str, date_len);
    date_copy[date_len] = '\0';
    
    time_t parsed_time = curl_getdate(date_copy, nullptr);
    free(date_copy);

    // 4. Set up easy handle options
    // Use remaining data for SSL session import
    size_t ssl_offset = date_len;
    if (size - ssl_offset < 96) { // Need at least 96 bytes for session data
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Set up URL (dummy URL for testing)
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:8080");
    
    // Set up SSL session import
    const char *session_key = "fuzz_session";
    const unsigned char *shmac = data + ssl_offset;
    size_t shmac_len = 32;
    const unsigned char *sdata = data + ssl_offset + shmac_len;
    size_t sdata_len = 64;
    
    // Only attempt import if we have enough data
    if (size >= ssl_offset + shmac_len + sdata_len) {
        curl_easy_ssls_import(easy_handle, session_key, shmac, shmac_len, sdata, sdata_len);
    }

    // Set up FILE* for reading (using fmemopen as specified)
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    // 5. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // 6. Perform multi handle operations
    int running_handles = 0;
    int ret = 0;
    
    // Initial perform
    curl_multi_perform(multi_handle, &running_handles);
    
    // Setup poll structure
    struct curl_waitfd extra_fds;
    extra_fds.fd = fuzz_fileno(in_file);
    extra_fds.events = CURL_WAIT_POLLIN;
    extra_fds.revents = 0;
    
    // Poll with timeout
    curl_multi_poll(multi_handle, &extra_fds, 1, 100, &ret);
    
    // Socket action with timeout
    curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, 0, &running_handles);

    // 7. Try to receive data
    char buffer[1024];
    size_t n = 0;
    curl_easy_recv(easy_handle, buffer, sizeof(buffer), &n);

    // 8. Pause and resume the transfer
    curl_easy_pause(easy_handle, CURLPAUSE_RECV);
    curl_easy_pause(easy_handle, CURLPAUSE_CONT);

    // 9. Remove handle from multi handle
    curl_multi_remove_handle(multi_handle, easy_handle);

    // 10. Clean up
    assert_file_closed(&in_file);;
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    assert_file_closed(&in_file);
	return 0;
}
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
//<ID> 151
//<Prompt> []
/*<Combination>: [
*/
//<score> 90, nr_unique_branch: 14
//<Quality> {"density":18,"unique_branches":{"formatf":[[996,8,996,33,0,0,4,0],[998,10,998,20,0,0,4,1]],"Curl_is_in_callback":[[3789,18,3789,29,0,0,4,1]],"parsefmt":[[251,9,251,17,0,0,4,0],[252,14,252,25,0,0,4,0],[257,16,257,40,0,0,4,1],[280,14,281,44,0,0,4,1],[481,10,481,33,0,0,4,0],[482,12,482,25,0,0,4,0],[489,12,489,39,0,0,4,1],[491,12,491,34,0,0,4,0],[580,5,580,25,0,0,4,0]],"out_string":[[859,11,859,21,0,0,4,0],[875,16,875,20,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_cleanup","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_url","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_share_init","curl_url_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_escape","curl_free","curl_easy_recv","curl_url_set","curl_url_get","curl_free","curl_easy_ssls_export","curl_multi_strerror","curl_easy_setopt","curl_ws_recv","curl_multi_add_handle","curl_multi_socket_action","curl_free","curl_share_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_easy_init","curl_multi_init","curl_url","curl_share_init","curl_easy_escape","curl_free","curl_easy_recv","curl_url_set","curl_url_get","curl_free","curl_easy_ssls_export","curl_multi_strerror","curl_easy_setopt","curl_ws_recv","curl_multi_add_handle","curl_multi_socket_action","curl_free","curl_share_cleanup","curl_multi_remove_handle","curl_multi_cleanup","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":0}
/**/


extern "C" {
}

// Helper function for curl_mvaprintf
static char*  id_000006_my_mvaprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

// Callback for curl_easy_ssls_export
static CURLcode  id_000006_ssls_export_callback(CURL* handle, void* userptr,
                                     const char* session_key,
                                     const unsigned char* shmac,
                                     size_t shmac_len,
                                     const unsigned char* sdata,
                                     size_t sdata_len,
                                     curl_off_t sdata_offset,
                                     int is_import,
                                     const char* hostname,
                                     size_t hostname_len) {
    // Just return OK for fuzzing purposes
    return CURLE_OK;
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<12) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
	//fuzzer shim end}




    if (size < 10) {
        return 0;
    }

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    // Create file from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptor
    int fd = fuzz_fileno(in_file);

    // Create easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create multi handle
    CURLM* multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create URL handle
    CURLU* url = curl_url();
    if (!url) {
        curl_multi_cleanup(multi);
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Create share handle
    CURLSH* share = curl_share_init();
    if (!share) {
        curl_url_cleanup(url);
        curl_multi_cleanup(multi);
        curl_easy_cleanup(curl);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // Set up some data from input
    const char* test_string = (const char*)data;
    size_t str_len = size > 100 ? 100 : size;
    
    // 1. curl_easy_escape
    char* escaped = curl_easy_escape(curl, test_string, (int)str_len);
    if (escaped) {
        // Use escaped string for something
        curl_free(escaped);
    }

    // 2. curl_easy_recv
    char recv_buffer[1024];
    size_t bytes_received = 0;
    // Note: This will likely fail since we're not connected, but we call it anyway
    curl_easy_recv(curl, recv_buffer, sizeof(recv_buffer), &bytes_received);

    // 3. curl_url_get
    // First set a URL
    char url_part[256];
    snprintf(url_part, sizeof(url_part), "http://example.com/%.*s", 
             (int)(size > 50 ? 50 : size), test_string);
    curl_url_set(url, CURLUPART_URL, url_part, fuzz_uint32_t_1);
    
    // Now get a part back
    char* hostname = nullptr;
    CURLUcode url_res = curl_url_get(url, CURLUPART_HOST, &hostname, fuzz_uint32_t_2);
    if (hostname) {
        curl_free(hostname);
    }

    // 4. curl_easy_ssls_export
    curl_easy_ssls_export(curl,  id_000006_ssls_export_callback, nullptr);

    // 5. curl_multi_strerror
    // Get error string for a sample error code
    const char* error_str = curl_multi_strerror(CURLM_OK);

    // 6. curl_ws_recv
    // Set up WebSocket options
    curl_easy_setopt(curl, CURLOPT_WS_OPTIONS, CURLWS_RAW_MODE);
    
    char ws_buffer[1024];
    size_t ws_recv = 0;
    const struct curl_ws_frame* ws_meta = nullptr;
    curl_ws_recv(curl, ws_buffer, sizeof(ws_buffer), &ws_recv, &ws_meta);

    // 7. curl_multi_socket_action
    // Add easy handle to multi handle
    curl_multi_add_handle(multi, curl);
    
    int running_handles = 0;
    // Use the file descriptor from our input file
    curl_multi_socket_action(multi, fd, fuzz_int32_t_3, &running_handles);

    // 8. curl_mvaprintf
    // Format a string using input data
    char* formatted =  id_000006_my_mvaprintf("Input: %.*s, Size: %zu", 
                                   (int)(size > 20 ? 20 : size), 
                                   test_string, size);
    if (formatted) {
        curl_free(formatted);
    }

    // 9. curl_share_cleanup
    curl_share_cleanup(share);

    // Cleanup
    curl_multi_remove_handle(multi, curl);
    curl_multi_cleanup(multi);
    curl_easy_cleanup(curl);
    curl_url_cleanup(url);
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}
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
//<score> 21, nr_unique_branch: 6
//<Quality> {"density":21,"unique_branches":{"file_upload":[[310,6,310,34,0,0,4,1]],"Curl_client_read":[[1193,6,1193,29,0,0,4,1],[1199,6,1199,31,0,0,4,1]],"cr_in_read":[[663,6,663,25,0,0,4,1],[676,8,676,29,0,0,4,1],[726,8,726,27,0,0,4,1]]},"library_calls":["curl_global_init","curl_global_trace","curl_version","curl_getdate","curl_easy_unescape","curl_free","curl_easy_init","curl_global_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_easy_duphandle","curl_share_init","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_share_cleanup","curl_global_cleanup","curl_multi_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_timeout","curl_multi_poll","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version","curl_getdate","curl_easy_unescape","curl_free","curl_easy_init","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_upkeep","curl_easy_duphandle","curl_share_init","curl_multi_init","curl_multi_setopt","curl_multi_add_handle","curl_multi_add_handle","curl_multi_perform","curl_multi_timeout","curl_multi_poll","curl_multi_remove_handle","curl_multi_remove_handle","curl_multi_cleanup","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup"],"visited":3}
/**/


extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing

    // 1. Initialize global curl with default flags
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_init_res != CURLE_OK) {
	return 0;
	}
	

    // 2. Set global trace configuration using part of input data
    char trace_config[50];
    size_t trace_len = size < 50 ? size : 50;
    memcpy(trace_config, data, trace_len);
    trace_config[trace_len - 1] = '\0'; // Ensure null termination
    curl_global_trace(trace_config);

    // 3. Get curl version info
    char *version_str = curl_version();

    // 4. Parse date from input data
    char date_str[100];
    size_t date_len = size < 100 ? size : 100;
    memcpy(date_str, data, date_len);
    date_str[date_len - 1] = '\0';
    time_t parsed_time = curl_getdate(date_str, nullptr);

    // 5. Create a string to unescape (using middle portion of data)
    const char *string_to_unescape = reinterpret_cast<const char*>(data);
    int unescape_len = static_cast<int>(size < 1000 ? size : 1000);
    int outlength = 0;
    char *unescaped = curl_easy_unescape(nullptr, string_to_unescape, unescape_len, &outlength);
    if (unescaped) {
        curl_free(unescaped);
    }

    // 6. Create and set up easy handles
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create input and output files using data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }

    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set various easy options
    curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");

    // 7. Perform upkeep on easy handle
    curl_easy_upkeep(easy_handle);

    // 8. Duplicate easy handle
    CURL *dup_handle = curl_easy_duphandle(easy_handle);

    // 9. Create share handle
    CURLSH *share_handle = curl_share_init();

    // 10. Create and set up multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        if (dup_handle) curl_easy_cleanup(dup_handle);
        curl_easy_cleanup(easy_handle);
        curl_share_cleanup(share_handle);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Set multi handle options
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 10L);

    // 11. Add easy handles to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    if (dup_handle) {
        curl_multi_add_handle(multi_handle, dup_handle);
    }

    // 12. Perform multi operations
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    // 13. Get timeout and poll
    long timeout_ms = 0;
    curl_multi_timeout(multi_handle, &timeout_ms);
    
    struct curl_waitfd extra_fds[2];
    extra_fds[0].fd = fuzz_fileno(in_file);
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;
    
    extra_fds[1].fd = fuzz_fileno(out_file);
    extra_fds[1].events = CURL_WAIT_POLLOUT;
    extra_fds[1].revents = 0;
    
    int ret = 0;
    curl_multi_poll(multi_handle, extra_fds, 2, fuzz_int32_t_1, &ret);

    // 14. Clean up in reverse order
    curl_multi_remove_handle(multi_handle, easy_handle);
    if (dup_handle) {
        curl_multi_remove_handle(multi_handle, dup_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    curl_share_cleanup(share_handle);
    if (dup_handle) curl_easy_cleanup(dup_handle);
    curl_easy_cleanup(easy_handle);
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    curl_global_cleanup();

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
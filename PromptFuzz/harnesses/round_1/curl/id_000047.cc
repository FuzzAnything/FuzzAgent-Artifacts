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
//<ID> 595
//<Prompt> []
/*<Combination>: [
*/
//<score> 20.666666, nr_unique_branch: 4
//<Quality> {"density":31,"unique_branches":{"async_thrdd_get_ai":[[766,14,766,16,0,0,4,0],[767,8,767,34,0,0,4,0],[768,10,768,20,0,0,4,0],[768,10,768,20,0,0,4,1]]},"library_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_share_init","curl_version","curl_getdate","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup","curl_share_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_unescape","curl_free","curl_multi_add_handle","curl_multi_poll","curl_multi_perform","curl_multi_perform","curl_multi_strerror","curl_easy_upkeep","curl_easy_nextheader","curl_ws_send","curl_easy_duphandle","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_multi_init","curl_easy_init","curl_share_init","curl_version","curl_getdate","curl_share_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_unescape","curl_free","curl_multi_add_handle","curl_multi_poll","curl_multi_perform","curl_multi_perform","curl_multi_strerror","curl_easy_upkeep","curl_easy_nextheader","curl_ws_send","curl_easy_duphandle","curl_multi_add_handle","curl_multi_perform","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_remove_handle","curl_easy_cleanup","curl_multi_cleanup","curl_share_cleanup","curl_global_cleanup"],"visited":2}
/**/


// Include necessary headers (assuming they're available in the environment)

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* f_data, size_t f_size) {
	if(f_size<46) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_5, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_6, fdp);
	//fuzzer shim end}




    // Return early if there's insufficient data
    if (size < 10) {
	return 0;
	}
	

    // 1. Initialize global curl session
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // 2. Create multi, easy, and share handles
    CURLM *multi_handle = curl_multi_init();
    CURL *easy_handle = curl_easy_init();
    CURL *easy_handle2 = nullptr;
    CURLSH *share_handle = curl_share_init();
    
    // 3. Use curl_version() - get version string
    char *version = curl_version();
    (void)version; // Use version to prevent unused warning
    
    // 4. Use curl_getdate() - parse date from data
    // Use first 100 bytes or less as date string
    size_t date_len = size > 100 ? 100 : size;
    char *date_str = new char[date_len + 1];
    memcpy(date_str, data, date_len);
    date_str[date_len] = '\0';
    time_t parsed_date = curl_getdate(date_str, nullptr);
    (void)parsed_date; // Use parsed_date to prevent unused warning
    delete[] date_str;
    
    // 5. Create a FILE* for input data using fmemopen
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        // Cleanup and return if file creation fails
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share_handle);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 6. Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_share_cleanup(share_handle);
        curl_global_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 7. Configure share handle to share cookies
    curl_share_setopt(share_handle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
    
    // 8. Configure easy handle with various options
    if (easy_handle) {
        // Set URL from data (use first 500 bytes or less as URL)
        size_t url_len = size > 500 ? 500 : size;
        char *url = new char[url_len + 1];
        memcpy(url, data, url_len);
        url[url_len] = '\0';
        curl_easy_setopt(easy_handle, CURLOPT_URL, url);
        delete[] url;
        
        // Set other options
        curl_easy_setopt(easy_handle, CURLOPT_SHARE, share_handle);
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, in_file);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, out_file);
        curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
        
        // Use curl_easy_unescape() - unescape a sample string
        const char *sample_escaped = "Hello%20World%21";
        int outlength = 0;
        char *unescaped = curl_easy_unescape(easy_handle, fuzz_str_1, 
                                            static_cast<int>(fuzz_str_sz_1), &outlength);
        if (unescaped) {
            curl_free(unescaped);
        }
        
        // Add easy handle to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    // 9. Perform multi operations
    int running_handles = 0;
    int ret = 0;
    
    // Create pollfd structure for multi_poll
    struct curl_waitfd extra_fd;
    extra_fd.fd = fuzz_fileno(in_file);
    extra_fd.events = CURL_WAIT_POLLIN;
    extra_fd.revents = 0;
    
    // Call multi_poll and multi_perform
    curl_multi_poll(multi_handle, &extra_fd, 1, fuzz_int32_t_2, &ret);
    curl_multi_perform(multi_handle, &running_handles);
    
    // 10. Get error string if any
    CURLMcode mcode = curl_multi_perform(multi_handle, &running_handles);
    const char *multi_error = curl_multi_strerror(mcode);
    (void)multi_error; // Use multi_error to prevent unused warning
    
    // 11. Use curl_easy_upkeep()
    if (easy_handle) {
        curl_easy_upkeep(easy_handle);
    }
    
    // 12. Use curl_easy_nextheader() - try to get headers
    if (easy_handle) {
        struct curl_header *header = nullptr;
        header = curl_easy_nextheader(easy_handle, CURLH_HEADER, fuzz_int32_t_3, header);
        (void)header; // Use header to prevent unused warning
    }
    
    // 13. Use curl_ws_send() - only if this is a WebSocket handle
    // Note: This requires WebSocket setup which we don't have, so we'll call it
    // but it will likely fail gracefully
    if (easy_handle) {
        size_t sent = 0;
        const char *ws_msg = "test";
        curl_ws_send(easy_handle, fuzz_str_4, static_cast<size_t>(fuzz_str_sz_4), &sent, fuzz_int64_t_5, fuzz_uint32_t_6);
    }
    
    // 14. Duplicate easy handle
    if (easy_handle) {
        easy_handle2 = curl_easy_duphandle(easy_handle);
        
        // Add duplicated handle to multi handle
        if (easy_handle2) {
            curl_multi_add_handle(multi_handle, easy_handle2);
            curl_multi_perform(multi_handle, &running_handles);
            
            // Remove duplicated handle
            curl_multi_remove_handle(multi_handle, easy_handle2);
            curl_easy_cleanup(easy_handle2);
        }
    }
    
    // 15. Cleanup sequence
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    curl_multi_cleanup(multi_handle);
    curl_share_cleanup(share_handle);
    curl_global_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}
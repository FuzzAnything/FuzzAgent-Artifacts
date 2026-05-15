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
//<ID> 996
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 0
//<Quality> {"density":26,"unique_branches":{},"library_calls":["curl_global_init","curl_global_trace","curl_version","curl_easy_init","curl_global_cleanup","curl_easy_duphandle","curl_multi_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_share_init","curl_easy_option_by_id","curl_share_cleanup","curl_multi_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_free","curl_getdate","curl_easy_unescape","curl_free","curl_easy_setopt","curl_multi_setopt","curl_multi_add_handle","curl_easy_upkeep","curl_multi_timeout","curl_multi_poll","curl_multi_socket_action","curl_multi_perform","curl_multi_remove_handle","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_global_trace","curl_version","curl_easy_init","curl_easy_duphandle","curl_multi_init","curl_share_init","curl_easy_option_by_id","curl_mime_init","curl_mime_addpart","curl_mime_encoder","curl_mime_free","curl_getdate","curl_easy_unescape","curl_free","curl_easy_setopt","curl_multi_setopt","curl_multi_add_handle","curl_easy_upkeep","curl_multi_timeout","curl_multi_poll","curl_multi_socket_action","curl_multi_perform","curl_multi_remove_handle","curl_share_cleanup","curl_easy_cleanup","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Include curl headers

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<4) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Initialize global curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 2. Set global trace configuration using input data
    // Use first 100 bytes or less for trace config
    size_t trace_len = size > 100 ? 100 : size;
    char* trace_config = (char*)malloc(trace_len + 1);
    memcpy(trace_config, data, trace_len);
    trace_config[trace_len] = '\0';
    curl_global_trace(trace_config);
    free(trace_config);
    
    // 3. Get curl version
    char* version_str = curl_version();
    
    // 4. Create easy handle
    CURL* easy = curl_easy_init();
    if (!easy) {
        curl_global_cleanup();
        return 0;
    }
    
    // 5. Create duplicate easy handle
    CURL* easy_dup = curl_easy_duphandle(easy);
    
    // 6. Create multi handle
    CURLM* multi = curl_multi_init();
    if (!multi) {
        curl_easy_cleanup(easy);
        if (easy_dup) curl_easy_cleanup(easy_dup);
        curl_global_cleanup();
        return 0;
    }
    
    // 7. Create share handle
    CURLSH* share = curl_share_init();
    
    // 8. Get easy option by ID
    const struct curl_easyoption* opt = curl_easy_option_by_id(CURLOPT_URL);
    
    // 9. Create FILE* for input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        if (share) curl_share_cleanup(share);
        curl_multi_cleanup(multi);
        curl_easy_cleanup(easy);
        if (easy_dup) curl_easy_cleanup(easy_dup);
        curl_global_cleanup();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 10. Create MIME structure and set encoder
    curl_mime* mime = curl_mime_init(easy);
    if (mime) {
        curl_mimepart* part = curl_mime_addpart(mime);
        if (part) {
            // Use part of input data as encoding type
            size_t encoding_len = size > 20 ? 20 : size;
            char* encoding = (char*)malloc(encoding_len + 1);
            memcpy(encoding, data, encoding_len);
            encoding[encoding_len] = '\0';
            curl_mime_encoder(part, encoding);
            free(encoding);
        }
        curl_mime_free(mime);
    }
    
    // 11. Parse date from input data
    size_t date_len = size > 50 ? 50 : size;
    char* date_str = (char*)malloc(date_len + 1);
    memcpy(date_str, data, date_len);
    date_str[date_len] = '\0';
    time_t parsed_time = curl_getdate(date_str, NULL);
    free(date_str);
    
    // 12. Unescape string using input data
    size_t unescape_len = size > 30 ? 30 : size;
    char* to_unescape = (char*)malloc(unescape_len + 1);
    memcpy(to_unescape, data, unescape_len);
    to_unescape[unescape_len] = '\0';
    int outlength = 0;
    char* unescaped = curl_easy_unescape(easy, to_unescape, unescape_len, &outlength);
    if (unescaped) {
        curl_free(unescaped);
    }
    free(to_unescape);
    
    // 13. Set easy handle options
    // Use part of input as URL
    size_t url_len = size > 40 ? 40 : size;
    char* url = (char*)malloc(url_len + 1);
    memcpy(url, data, url_len);
    url[url_len] = '\0';
    curl_easy_setopt(easy, CURLOPT_URL, url);
    free(url);
    
    // 14. Set multi handle options
    curl_multi_setopt(multi, CURLMOPT_MAXCONNECTS, 5L);
    
    // 15. Add easy handle to multi handle
    curl_multi_add_handle(multi, easy);
    
    // 16. Perform upkeep on easy handle
    curl_easy_upkeep(easy);
    
    // 17. Get multi timeout
    long timeout = 0;
    curl_multi_timeout(multi, &timeout);
    
    // 18. Prepare for polling
    int ret = 0;
    int running_handles = 0;
    
    // 19. Poll with timeout (non-blocking)
    curl_multi_poll(multi, NULL, 0, timeout > 0 ? timeout : 0, &ret);
    
    // 20. Perform multi socket action (simulate socket activity)
    // Using CURL_SOCKET_TIMEOUT (-1) to trigger timeout processing
    curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, fuzz_int32_t_1, &running_handles);
    
    // 21. Perform multi operations
    curl_multi_perform(multi, &running_handles);
    
    // 22. Cleanup sequence
    curl_multi_remove_handle(multi, easy);
    
    assert_file_closed(&in_file);;
    
    if (share) {
        curl_share_cleanup(share);
    }
    
    if (easy_dup) {
        curl_easy_cleanup(easy_dup);
    }
    
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    curl_global_cleanup();
    
    assert_file_closed(&in_file);
	return 0;
}
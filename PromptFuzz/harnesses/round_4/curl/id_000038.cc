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
//<ID> 550
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 2
//<Quality> {"density":34,"unique_branches":{"curl_url_set":[[1884,24,1884,40,0,0,4,0],[1886,10,1886,16,0,0,4,1]]},"library_calls":["curl_global_init","curl_url","curl_global_cleanup","curl_url_set","curl_url_set","curl_url_set","curl_easy_init","curl_url_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_type","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_mime_free","curl_easy_cleanup","curl_url_cleanup","curl_global_cleanup","curl_multi_notify_enable","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_url","curl_url_set","curl_url_set","curl_url_set","curl_easy_init","curl_mime_init","curl_mime_addpart","curl_mime_data","curl_mime_name","curl_mime_type","curl_url_get","curl_easy_setopt","curl_free","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_multi_init","curl_multi_notify_enable","curl_multi_add_handle","curl_multi_perform","curl_multi_wait","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_url_cleanup","curl_global_cleanup"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<79) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Initialize global curl environment
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
	return 0;
	}
	
    
    // 2. Create URL handle and set URL parts
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Use first 100 bytes for URL path (or less if input is smaller)
    size_t url_len = size > 100 ? 100 : size;
    char url_path[101];
    memcpy(url_path, data, url_len);
    url_path[url_len] = '\0';
    
    curl_url_set(url_handle, CURLUPART_SCHEME, fuzz_str_1, fuzz_uint32_t_2);
    curl_url_set(url_handle, CURLUPART_HOST, fuzz_str_3, fuzz_uint32_t_4);
    curl_url_set(url_handle, CURLUPART_PATH, url_path, fuzz_uint32_t_5);
    
    // 3. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 4. Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(easy_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 5. Add mime part with data from input
    curl_mimepart *mimepart = curl_mime_addpart(mime);
    if (mimepart) {
        // Use remaining data for mime content
        const char *mime_data = reinterpret_cast<const char*>(data);
        size_t mime_data_size = size;
        
        curl_mime_data(mimepart, mime_data, mime_data_size);
        curl_mime_name(mimepart, fuzz_str_6);
        curl_mime_type(mimepart, fuzz_str_7);
    }
    
    // 6. Set easy handle options
    char *full_url = nullptr;
    curl_url_get(url_handle, CURLUPART_URL, &full_url, fuzz_uint32_t_8);
    if (full_url) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, full_url);
        curl_free(full_url);
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    
    // 7. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_mime_free(mime);
        curl_easy_cleanup(easy_handle);
        curl_url_cleanup(url_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 8. Enable notifications (using available notification type)
    curl_multi_notify_enable(multi_handle, CURLMOPT_SOCKETFUNCTION);
    
    // 9. Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // 10. Perform multi operations with timeout
    int running_handles = 0;
    int ret = 0;
    
    // Perform initial check
    curl_multi_perform(multi_handle, &running_handles);
    
    // Wait with timeout (short timeout for fuzzing)
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = 0;
    extra_fds[0].events = CURL_WAIT_POLLIN;
    extra_fds[0].revents = 0;
    
    curl_multi_wait(multi_handle, extra_fds, 1, fuzz_int32_t_9, &ret);
    
    // Perform again to process any pending operations
    curl_multi_perform(multi_handle, &running_handles);
    
    // 11. Cleanup in reverse order
    curl_multi_remove_handle(multi_handle, easy_handle);
    
    // Free resources
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_url_cleanup(url_handle);
    curl_global_cleanup();
    
    return 0;
}
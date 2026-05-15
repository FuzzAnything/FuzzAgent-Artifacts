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
//<ID> 930
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 1
//<Quality> {"density":32,"unique_branches":{"curl_easy_strerror":[[177,3,177,24,0,0,4,0]]},"library_calls":["curl_global_init","curl_version_info","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_easy_duphandle","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_filename","curl_share_init","curl_share_cleanup","curl_multi_init","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_add_handle","curl_multi_strerror","curl_multi_fdset","curl_multi_strerror","curl_multi_waitfds","curl_multi_strerror","curl_multi_notify_disable","curl_multi_strerror","curl_multi_remove_handle","curl_multi_strerror","curl_easy_option_by_name","curl_ws_start_frame","curl_easy_strerror","curl_multi_cleanup","curl_multi_strerror","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_easy_strerror","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version_info","curl_easy_init","curl_easy_duphandle","curl_mime_init","curl_mime_addpart","curl_mime_filename","curl_share_init","curl_share_cleanup","curl_multi_init","curl_multi_add_handle","curl_multi_strerror","curl_multi_fdset","curl_multi_strerror","curl_multi_waitfds","curl_multi_strerror","curl_multi_notify_disable","curl_multi_strerror","curl_multi_remove_handle","curl_multi_strerror","curl_easy_option_by_name","curl_ws_start_frame","curl_easy_strerror","curl_multi_cleanup","curl_multi_strerror","curl_mime_free","curl_easy_cleanup","curl_easy_cleanup","curl_easy_strerror","curl_global_cleanup"],"visited":0}
/**/


// Include necessary headers for libcurl

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t* f_data, size_t f_size) {
	if(f_size<10) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	

    // 1. Initialize global curl
    CURLcode global_init_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_res != CURLE_OK) {
        return 0;
    }

    // 2. Get version info
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    if (!version_info) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    // 4. Duplicate easy handle
    CURL *duplicated_handle = curl_easy_duphandle(easy_handle);
    if (!duplicated_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // 5. Create MIME structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_easy_cleanup(duplicated_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // 6. Add MIME part
    curl_mimepart *mime_part = curl_mime_addpart(mime);
    if (!mime_part) {
        curl_mime_free(mime);
        curl_easy_cleanup(duplicated_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // 7. Set MIME filename using input data
    // Use first 100 bytes or less for filename
    size_t filename_len = size > 100 ? 100 : size;
    char *filename = (char*)malloc(filename_len + 1);
    if (filename) {
        memcpy(filename, data, filename_len);
        filename[filename_len] = '\0';
        curl_mime_filename(mime_part, filename);
        free(filename);
    }

    // 8. Create share handle
    CURLSH *share_handle = curl_share_init();
    if (share_handle) {
        curl_share_cleanup(share_handle);
    }

    // 9. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_mime_free(mime);
        curl_easy_cleanup(duplicated_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // 10. Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    if (add_res != CURLM_OK) {
        const char *error_str = curl_multi_strerror(add_res);
        // error_str can be used if needed
    }

    // 11. Use fdset (even though we're not really polling)
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    int max_fd = -1;
    
    CURLMcode fdset_res = curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
    if (fdset_res != CURLM_OK) {
        const char *error_str = curl_multi_strerror(fdset_res);
    }

    // 12. Use waitfds
    struct curl_waitfd ufds[1];
    unsigned int fd_count = 0;
    CURLMcode waitfds_res = curl_multi_waitfds(multi_handle, ufds, 1, &fd_count);
    if (waitfds_res != CURLM_OK) {
        const char *error_str = curl_multi_strerror(waitfds_res);
    }

    // 13. Disable notifications
    CURLMcode notify_disable_res = curl_multi_notify_disable(multi_handle, fuzz_uint32_t_1);
    if (notify_disable_res != CURLM_OK) {
        const char *error_str = curl_multi_strerror(notify_disable_res);
    }

    // 14. Remove handle from multi
    CURLMcode remove_res = curl_multi_remove_handle(multi_handle, easy_handle);
    if (remove_res != CURLM_OK) {
        const char *error_str = curl_multi_strerror(remove_res);
    }

    // 15. Look up easy option by name
    const struct curl_easyoption *option = curl_easy_option_by_name(fuzz_str_2);
    if (!option) {
        // Option not found
    }

    // 16. Try to start WebSocket frame (will likely fail but we call it)
    CURLcode ws_res = curl_ws_start_frame(easy_handle, CURLWS_BINARY, size);
    if (ws_res != CURLE_OK) {
        const char *error_str = curl_easy_strerror(ws_res);
    }

    // 17. Clean up multi handle
    CURLMcode multi_cleanup_res = curl_multi_cleanup(multi_handle);
    if (multi_cleanup_res != CURLM_OK) {
        const char *error_str = curl_multi_strerror(multi_cleanup_res);
    }

    // 18. Clean up MIME
    curl_mime_free(mime);

    // 19. Clean up duplicated handle
    curl_easy_cleanup(duplicated_handle);

    // 20. Clean up original easy handle
    curl_easy_cleanup(easy_handle);

    // 21. Get easy error string for demonstration
    const char *easy_error_str = curl_easy_strerror(CURLE_OK);

    // 22. Clean up global curl
    curl_global_cleanup();

    return 0;
}
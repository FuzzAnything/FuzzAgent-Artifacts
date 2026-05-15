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
//<ID> 139
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 0
//<Quality> {"density":42,"unique_branches":{},"library_calls":["curl_global_init","curl_version_info","curl_global_cleanup","curl_easy_option_by_name","curl_global_cleanup","curl_easy_init","curl_global_cleanup","curl_multi_init","curl_easy_cleanup","curl_global_cleanup","curl_multi_notify_enable","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_init","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_addpart","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_name","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_mime_data","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_easy_setopt","curl_easy_ssls_import","curl_multi_add_handle","curl_mime_free","curl_multi_cleanup","curl_easy_cleanup","curl_global_cleanup","curl_multi_get_handles","curl_multi_socket_action","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"critical_calls":["curl_global_init","curl_version_info","curl_easy_option_by_name","curl_easy_init","curl_multi_init","curl_multi_notify_enable","curl_mime_init","curl_mime_addpart","curl_mime_name","curl_mime_data","curl_easy_setopt","curl_easy_setopt","curl_easy_setopt","curl_easy_ssls_import","curl_multi_add_handle","curl_multi_get_handles","curl_multi_socket_action","curl_multi_perform","curl_multi_remove_handle","curl_mime_free","curl_easy_cleanup","curl_multi_cleanup","curl_global_cleanup"],"visited":0}
/**/


// Headers for curl

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t* f_data, size_t f_size) {
	if(f_size<48) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint64_t, fuzz_uint64_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
	//fuzzer shim end}




    if (size < 100) {
        return 0; // Need minimum data for meaningful testing
    }

    // Split input data for different purposes
    const size_t ssl_data_size = size / 4;
    const size_t mime_data_size = size / 4;
    const size_t session_key_size = 32; // Fixed size for session key
    
    if (ssl_data_size < session_key_size || mime_data_size < 10) {
        return 0;
    }

    // 1. Initialize global curl environment
    CURLcode global_res = curl_global_init(CURL_GLOBAL_ALL);
    if (global_res != CURLE_OK) {
        return 0;
    }

    // 2. Get version information
    curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
    if (!version_info) {
        curl_global_cleanup();
        return 0;
    }

    // 3. Look up an easy option by name
    const struct curl_easyoption *url_option = curl_easy_option_by_name(fuzz_str_1);
    if (!url_option) {
        curl_global_cleanup();
        return 0;
    }

    // 4. Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    // 5. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // 6. Enable notifications on multi handle
    CURLMcode notify_res = curl_multi_notify_enable(multi_handle, CURL_POLL_IN | CURL_POLL_OUT);
    if (notify_res != CURLM_OK) {
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // 7. Create mime structure
    curl_mime *mime = curl_mime_init(easy_handle);
    if (!mime) {
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // 8. Add mime part
    curl_mimepart *mime_part = curl_mime_addpart(mime);
    if (!mime_part) {
        curl_mime_free(mime);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // 9. Set mime name using part of input data
    const char *mime_name = "fuzzpart";
    CURLcode mime_name_res = curl_mime_name(mime_part, fuzz_str_2);
    if (mime_name_res != CURLE_OK) {
        curl_mime_free(mime);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // 10. Set mime data using input data
    const char *mime_data = reinterpret_cast<const char*>(data + ssl_data_size);
    CURLcode mime_data_res = curl_mime_data(mime_part, mime_data, mime_data_size);
    if (mime_data_res != CURLE_OK) {
        curl_mime_free(mime);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // 11. Set various easy options
    // Set URL option
    CURLcode setopt_res = curl_easy_setopt(easy_handle, CURLOPT_URL, "file:///dev/null");
    if (setopt_res != CURLE_OK) {
        curl_mime_free(mime);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Set mime post data
    setopt_res = curl_easy_setopt(easy_handle, CURLOPT_MIMEPOST, mime);
    if (setopt_res != CURLE_OK) {
        curl_mime_free(mime);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Set verbosity off
    setopt_res = curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    
    // 12. Try to import SSL session (may fail with invalid data, that's OK)
    const char *session_key = "test_session_key";
    const unsigned char *shmac = data;
    const unsigned char *sdata = data + session_key_size;
    size_t sdata_len = ssl_data_size - session_key_size;
    
    CURLcode ssl_res = curl_easy_ssls_import(easy_handle, fuzz_str_3, 
                                             shmac, fuzz_uint64_t_4,
                                             sdata, sdata_len);
    // We don't check result - it's OK if it fails with fuzzing data

    // 13. Add easy handle to multi handle
    CURLMcode add_res = curl_multi_add_handle(multi_handle, easy_handle);
    if (add_res != CURLM_OK) {
        curl_mime_free(mime);
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // 14. Get handles from multi handle
    CURL **handles = curl_multi_get_handles(multi_handle);
    if (handles) {
        // Could iterate through handles if needed
    }

    // 15. Simulate socket action with timeout
    int running_handles = 0;
    curl_socket_t timeout_socket = CURL_SOCKET_TIMEOUT;
    CURLMcode socket_res = curl_multi_socket_action(multi_handle, timeout_socket, 
                                                    fuzz_int32_t_5, &running_handles);
    if (socket_res != CURLM_OK) {
        // Continue cleanup anyway
    }

    // 16. Perform a single iteration
    int still_running = 0;
    CURLMcode perform_res = curl_multi_perform(multi_handle, &still_running);
    if (perform_res != CURLM_OK) {
        // Continue cleanup anyway
    }

    // 17. Remove easy handle from multi handle
    CURLMcode remove_res = curl_multi_remove_handle(multi_handle, easy_handle);
    if (remove_res != CURLM_OK) {
        // Continue cleanup anyway
    }

    // 18. Clean up resources in reverse order
    curl_mime_free(mime);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/multi.h>

namespace harness_016 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for configuration and multi operations
    if (size < 64) {
        return 0; // Need minimum input for meaningful multi interface testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Create an easy handle (guidance step 1)
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Step 3: Configure easy handle with URL and options (guidance step 2)
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (!url.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    }
    
    // Set some other options that don't require network
    bool verbose = fdp.ConsumeBool();
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    
    // Set timeout to prevent hanging
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 10L);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, 10L);
    
    // Disable signal handling
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);

    // Step 4: Create multi handle (guidance step 3)
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Step 5: Add easy handle to multi handle (guidance step 4)
    CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
    if (add_result != CURLM_OK) {
        curl_multi_cleanup(multi_handle);
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Step 6: Call curl_version_info (guidance step 5 - optional helper)
    // Consume version stamp from fuzzer input
    CURLversion version_stamp = static_cast<CURLversion>(fdp.ConsumeIntegral<uint32_t>() % 5);
    curl_version_info_data* version_info = curl_version_info(version_stamp);
    (void)version_info; // Use result to prevent compiler warnings

    // Step 7: Target API 1 - curl_multi_socket_all (guidance step 6 - main target)
    // This is the deprecated API but still present with 0% coverage
    int running_handles = 0;
    CURLMcode socket_all_result = curl_multi_socket_all(multi_handle, &running_handles);
    
    // The API is deprecated but we still call it for coverage exploration
    // According to documentation, it's equivalent to curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, 0, &running_handles)
    (void)socket_all_result; // Use result to prevent compiler warnings

    // Step 8: Target API 2 - curl_multi_get_offt (guidance step 7 - secondary target)
    // Consume info type from fuzzer input
    CURLMinfo_offt info_type = static_cast<CURLMinfo_offt>(
        fdp.ConsumeIntegralInRange<uint32_t>(CURLMINFO_NONE + 1, CURLMINFO_LASTENTRY - 1));
    
    curl_off_t offset_value = 0;
    CURLMcode get_offt_result = curl_multi_get_offt(multi_handle, info_type, &offset_value);
    (void)get_offt_result; // Use result to prevent compiler warnings
    (void)offset_value; // Use result to prevent compiler warnings

    // Step 9: Read results with curl_multi_info_read (guidance step 8)
    int msgs_in_queue = 0;
    CURLMsg* msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    (void)msg; // Use result to prevent compiler warnings
    (void)msgs_in_queue; // Use result to prevent compiler warnings

    // Additional multi interface APIs for broader coverage
    // Try curl_multi_perform as well (though not in guidance, it's related)
    int still_running = 0;
    CURLMcode perform_result = curl_multi_perform(multi_handle, &still_running);
    (void)perform_result; // Use result to prevent compiler warnings
    
    // Try curl_multi_timeout
    long timeout_ms = 0;
    CURLMcode timeout_result = curl_multi_timeout(multi_handle, &timeout_ms);
    (void)timeout_result; // Use result to prevent compiler warnings

    // Step 10: Cleanup (guidance step 9)
    // Remove handle from multi
    CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)remove_result; // Use result to prevent compiler warnings
    
    // Cleanup handles
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}

} // namespace harness_016

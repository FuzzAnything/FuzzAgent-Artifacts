#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/multi.h>

namespace harness_001 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 16) {
        return 0; // Need minimum input
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Create multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create easy handle
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Consume data for configuration
    // URL string (dummy URL, we don't actually connect)
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (!url.empty()) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    }

    // Set some other options that don't require network
    bool verbose = fdp.ConsumeBool();
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, verbose ? 1L : 0L);

    // Add easy handle to multi handle
    CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
    if (add_result != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Consume data for socket operations
    // Socket descriptor (could be invalid, that's okay for testing)
    curl_socket_t socket_fd = static_cast<curl_socket_t>(fdp.ConsumeIntegral<int>());
    
    // Event bitmask for curl_multi_socket_action
    int ev_bitmask = fdp.ConsumeIntegral<int>();
    
    // Running handles pointer
    int running_handles = 0;

    // Target API 1: curl_multi_socket_action
    // This is the primary target with 622 undiscovered branches
    CURLMcode socket_action_result = curl_multi_socket_action(
        multi_handle, socket_fd, ev_bitmask, &running_handles);
    
    // Target API 2: curl_multi_socket (macro for curl_multi_socket_action with ev_bitmask=0)
    // This is the secondary target with 622 undiscovered branches
    CURLMcode socket_result = curl_multi_socket(
        multi_handle, socket_fd, &running_handles);

    // Remove handle from multi
    CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handle);
    if (remove_result != CURLM_OK) {
        // Still continue cleanup
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}

} // namespace harness_001

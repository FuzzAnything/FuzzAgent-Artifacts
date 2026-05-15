#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/urlapi.h>

namespace harness_012 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for basic configuration and socket operations
    if (size < 64) {
        return 0; // Need minimum input for meaningful multi-socket testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 1: Create multi handle (primary container for multiple transfers)
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Step 2: Create URL handle for testing curl_url_dup API
    CURLU* url_handle = curl_url();
    if (url_handle) {
        // Set some URL parts from fuzzed input
        if (fdp.remaining_bytes() > 16) {
            std::string scheme = fdp.ConsumeRandomLengthString(10);
            if (!scheme.empty() && (scheme == "http" || scheme == "https" || scheme == "ftp")) {
                curl_url_set(url_handle, CURLUPART_SCHEME, scheme.c_str(), 0);
            }
        }
        
        if (fdp.remaining_bytes() > 16) {
            std::string host = fdp.ConsumeRandomLengthString(50);
            if (!host.empty()) {
                curl_url_set(url_handle, CURLUPART_HOST, host.c_str(), 0);
            }
        }
        
        if (fdp.remaining_bytes() > 8) {
            std::string path = fdp.ConsumeRandomLengthString(100);
            if (!path.empty()) {
                curl_url_set(url_handle, CURLUPART_PATH, path.c_str(), 0);
            }
        }
        
        // Target API: curl_url_dup (62 undiscovered branches)
        CURLU* dup_url_handle = curl_url_dup(url_handle);
        if (dup_url_handle) {
            curl_url_cleanup(dup_url_handle);
        }
    }

    // Step 3: Create multiple easy handles (1-3 handles for multi-transfer testing)
    size_t num_handles = fdp.ConsumeIntegralInRange<size_t>(1, 3);
    std::vector<CURL*> easy_handles;
    std::vector<bool> handles_added;

    for (size_t i = 0; i < num_handles && fdp.remaining_bytes() > 32; i++) {
        CURL* easy_handle = curl_easy_init();
        if (!easy_handle) {
            continue;
        }
        
        // Configure each easy handle with different options
        std::string url = fdp.ConsumeRandomLengthString(256);
        if (!url.empty()) {
            // Try to use the URL handle if we have one
            if (url_handle) {
                // Get URL from the handle
                char* url_str = nullptr;
                if (curl_url_get(url_handle, CURLUPART_URL, &url_str, 0) == CURLUE_OK && url_str) {
                    curl_easy_setopt(easy_handle, CURLOPT_URL, url_str);
                    curl_free(url_str);
                } else {
                    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
                }
            } else {
                curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
            }
        }
        
        // Set various options from fuzzed input
        bool verbose = fdp.ConsumeBool();
        curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, verbose ? 1L : 0L);
        
        bool follow_location = fdp.ConsumeBool();
        curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
        
        // Add handle to multi handle
        CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
        if (add_result == CURLM_OK) {
            easy_handles.push_back(easy_handle);
            handles_added.push_back(true);
        } else {
            // Clean up if adding failed
            curl_easy_cleanup(easy_handle);
            handles_added.push_back(false);
        }
    }

    // Step 4: Test curl_multi_socket_action with various socket states
    // Primary target: curl_multi_socket_action (392 undiscovered branches)
    
    // Determine number of socket operations to perform (1-5)
    size_t num_socket_ops = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    
    for (size_t i = 0; i < num_socket_ops && fdp.remaining_bytes() > 8; i++) {
        // Consume socket descriptor (can be invalid/random for testing)
        curl_socket_t socket_fd = static_cast<curl_socket_t>(fdp.ConsumeIntegral<int>());
        
        // Consume event bitmask for socket action
        // CURL_POLL_IN = 0x01, CURL_POLL_OUT = 0x02, CURL_POLL_INOUT = 0x03, CURL_POLL_REMOVE = 0x04
        int ev_bitmask = fdp.ConsumeIntegralInRange<int>(0, 7);
        
        int running_handles = 0;
        
        // Primary API call: curl_multi_socket_action
        CURLMcode socket_action_result = curl_multi_socket_action(
            multi_handle, socket_fd, ev_bitmask, &running_handles);
        
        // Test with different combinations:
        // 1. Invalid socket descriptor
        // 2. Different event bitmasks
        // 3. Different running handle states
    }

    // Step 5: Test curl_multi_poll API
    // Related API: curl_multi_poll (2 undiscovered branches)
    if (fdp.remaining_bytes() > 8) {
        int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 1000);
        int num_fds = 0;
        
        // Test curl_multi_poll with timeout
        CURLMcode poll_result = curl_multi_poll(multi_handle, nullptr, 0, timeout_ms, &num_fds);
        
        // Test with extra file descriptors if we have enough input
        if (fdp.remaining_bytes() > 16) {
            struct curl_waitfd extra_fds[2];
            extra_fds[0].fd = static_cast<curl_socket_t>(fdp.ConsumeIntegral<int>());
            extra_fds[0].events = fdp.ConsumeIntegral<short>();
            extra_fds[0].revents = 0;
            
            extra_fds[1].fd = static_cast<curl_socket_t>(fdp.ConsumeIntegral<int>());
            extra_fds[1].events = fdp.ConsumeIntegral<short>();
            extra_fds[1].revents = 0;
            
            int poll_num_fds = 0;
            curl_multi_poll(multi_handle, extra_fds, 2, timeout_ms, &poll_num_fds);
        }
    }

    // Step 6: Test curl_multi_wakeup API
    // Related API: curl_multi_wakeup (3 undiscovered branches)
    if (fdp.remaining_bytes() > 4) {
        CURLMcode wakeup_result = curl_multi_wakeup(multi_handle);
        // Test wakeup even if previous operations failed
    }

    // Step 7: Test curl_multi_perform to get transfers started
    int running_handles = 0;
    CURLMcode perform_result = curl_multi_perform(multi_handle, &running_handles);
    
    // Step 8: Test multi_info_read to check for messages
    int msgs_in_queue = 0;
    CURLMsg* msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    while (msg != nullptr && fdp.remaining_bytes() > 4) {
        // Consume more data between message reads
        if (fdp.ConsumeBool()) {
            msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        } else {
            break;
        }
    }

    // Step 9: Clean up - remove handles from multi
    for (size_t i = 0; i < easy_handles.size(); i++) {
        if (handles_added[i]) {
            CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handles[i]);
            // Continue cleanup even if removal fails
        }
    }

    // Step 10: Clean up easy handles
    for (CURL* easy_handle : easy_handles) {
        curl_easy_cleanup(easy_handle);
    }

    // Step 11: Clean up URL handle
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }

    // Step 12: Clean up multi handle
    // Target API: curl_multi_cleanup (111 undiscovered branches)
    CURLMcode cleanup_result = curl_multi_cleanup(multi_handle);

    // Step 13: Global cleanup
    curl_global_cleanup();

    return 0;
}

} // namespace harness_012

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/multi.h>

namespace harness_014 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for configuration and waitfd operations
    if (size < 64) {
        return 0; // Need minimum input for meaningful multi-waitfds testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Create curl easy handle
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Step 3: Configure easy handle with URL from fuzzer input
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (url.empty()) {
        // Use a dummy URL if input doesn't provide one
        url = "http://localhost/";
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    
    // Set various options from fuzzed input to explore different code paths
    bool verbose = fdp.ConsumeBool();
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    
    bool follow_location = fdp.ConsumeBool();
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
    
    // Set timeouts to prevent hanging during fuzzing
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 10L);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, 10L);
    
    // Disable signal handling for fuzzing safety
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
    
    // Step 4: Initialize multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        curl_global_cleanup();
        return 0;
    }

    // Step 5: Add easy handle to multi handle
    CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
    // Continue even if add fails - we still want to test waitfds

    // Step 6: Perform multi operations
    int running_handles = 0;
    CURLMcode perform_result = curl_multi_perform(multi_handle, &running_handles);
    
    // Step 7: PRIMARY TARGET - curl_multi_waitfds (0% coverage, 26 branches)
    // First call to get descriptor count
    unsigned int fd_count = 0;
    CURLMcode waitfds_result1 = curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    
    // If we have descriptors, allocate array and call again
    if (waitfds_result1 == CURLM_OK && fd_count > 0) {
        // Limit maximum fd count for fuzzing safety
        if (fd_count > 100) {
            fd_count = 100;
        }
        
        // Allocate waitfd array
        struct curl_waitfd* ufds = (struct curl_waitfd*)malloc(fd_count * sizeof(struct curl_waitfd));
        if (ufds) {
            // Second call to get actual descriptors
            CURLMcode waitfds_result2 = curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
            
            // Test different scenarios with fuzzed input
            // 1. Test with partial array size (smaller than actual count)
            if (fdp.ConsumeBool() && fd_count > 1) {
                unsigned int partial_size = fdp.ConsumeIntegralInRange<unsigned int>(1, fd_count - 1);
                unsigned int partial_fd_count = 0;
                curl_multi_waitfds(multi_handle, ufds, partial_size, &partial_fd_count);
            }
            
            // 2. Test with zero size but non-null array pointer
            if (fdp.ConsumeBool()) {
                unsigned int zero_fd_count = 0;
                curl_multi_waitfds(multi_handle, ufds, 0, &zero_fd_count);
            }
            
            // 3. Test with null fd_count pointer
            if (fdp.ConsumeBool()) {
                curl_multi_waitfds(multi_handle, ufds, fd_count, NULL);
            }
            
            free(ufds);
        }
    }
    
    // Step 8: Test related multi APIs for comprehensive coverage
    // Test curl_multi_fdset (alternative to waitfds)
    if (fdp.ConsumeBool()) {
        fd_set read_fds, write_fds, exc_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_ZERO(&exc_fds);
        int max_fd = -1;
        curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);
    }
    
    // Test curl_multi_wait with timeout
    if (fdp.ConsumeBool()) {
        int num_fds = 0;
        int timeout_ms = fdp.ConsumeIntegralInRange<int>(-1, 100);
        curl_multi_wait(multi_handle, NULL, 0, timeout_ms, &num_fds);
    }
    
    // Step 9: Clean up multi handle (remove and cleanup)
    if (add_result == CURLM_OK) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    
    // Step 10: Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // Step 11: Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_014

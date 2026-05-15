#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_011 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for basic configuration
    if (size < 32) {
        return 0; // Need minimum input for meaningful send testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Create curl easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Step 3: Consume URL from fuzzer input
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (url.empty()) {
        // Use a dummy URL if input doesn't provide one
        url = "http://localhost/";
    }
    
    // Set the URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // Step 4: Set CONNECT_ONLY option to establish connection without transfer
    // This is required for curl_easy_send to work
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    
    // Step 5: Set timeouts to prevent hanging during fuzzing
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 10L);
    
    // Disable signal handling for fuzzing safety
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    // Step 6: Attempt to establish connection
    // Note: This will likely fail in fuzzing context (no network),
    // but we still want to test curl_easy_send API calls
    // The API has parameter validation and error handling paths we want to cover
    CURLcode perform_result = curl_easy_perform(curl);
    
    // Even if connection fails, we can still test curl_easy_send
    // as it might have initialization/parameter validation code paths
    
    // Step 7: Test curl_easy_send with various buffer sizes and data
    // Primary target: curl_easy_send (0% coverage, 8 undiscovered branches)
    
    // Determine number of send operations to perform (1-3)
    size_t num_send_ops = fdp.ConsumeIntegralInRange<size_t>(1, 3);
    
    for (size_t i = 0; i < num_send_ops && fdp.remaining_bytes() > 4; i++) {
        // Consume send buffer size from fuzzer input
        size_t send_buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        
        // Ensure we don't consume more bytes than available
        if (send_buf_size > fdp.remaining_bytes()) {
            send_buf_size = fdp.remaining_bytes();
        }
        
        if (send_buf_size > 0) {
            // Create send buffer with fuzzed data
            std::vector<uint8_t> send_buffer = fdp.ConsumeBytes<uint8_t>(send_buf_size);
            
            size_t nsent = 0;
            
            // Primary API call: curl_easy_send
            CURLcode send_result = curl_easy_send(curl, 
                                                  send_buffer.data(), 
                                                  send_buffer.size(), 
                                                  &nsent);
            
            // Test different error handling scenarios
            // 1. Send with zero buffer size (edge case)
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 1) {
                size_t zero_sent = 0;
                curl_easy_send(curl, nullptr, 0, &zero_sent);
            }
            
            // 2. Send with partial buffer (testing partial sends)
            if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
                size_t partial_buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                if (partial_buf_size <= fdp.remaining_bytes()) {
                    std::vector<uint8_t> partial_buffer = fdp.ConsumeBytes<uint8_t>(partial_buf_size);
                    size_t partial_sent = 0;
                    curl_easy_send(curl, partial_buffer.data(), partial_buffer.size(), &partial_sent);
                }
            }
        }
    }
    
    // Step 8: Test curl_easy_getinfo to check transmission status
    // as suggested in coverage guidance
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 1) {
        // Try to get some information about the connection
        long response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        
        double connect_time = 0;
        curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connect_time);
        
        char* effective_url = nullptr;
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    }
    
    // Step 9: Test alternative paths with curl_multi interface
    // as suggested in coverage guidance for multi-interface
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        CURLM* multi_handle = curl_multi_init();
        if (multi_handle) {
            // Add the easy handle to multi handle
            curl_multi_add_handle(multi_handle, curl);
            
            // Test curl_multi_socket (secondary target with 402 undiscovered branches)
            int running_handles = 0;
            curl_socket_t dummy_socket = static_cast<curl_socket_t>(fdp.ConsumeIntegral<int>());
            curl_multi_socket(multi_handle, dummy_socket, &running_handles);
            
            // Remove handle and cleanup multi
            curl_multi_remove_handle(multi_handle, curl);
            curl_multi_cleanup(multi_handle);
        }
    }
    
    // Step 10: Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_011

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_004 {


// Simple write function that does nothing (since we're using CONNECT_ONLY)
static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    // Just return the size to indicate success
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 16) {
        return 0; // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // 1. curl_global_init(CURL_GLOBAL_DEFAULT)
    CURLcode global_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // 2. curl_easy_init()
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Consume data for configuration
    // URL for connection (dummy URL to avoid actual network)
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (url.empty()) {
        // Use a dummy URL if input doesn't provide one
        url = "http://localhost/";
    }

    // 3. curl_easy_setopt with CURLOPT_URL and CURLOPT_WRITEFUNCTION
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    
    // Required for curl_easy_recv: set CONNECT_ONLY to establish connection without transfer
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    
    // Set timeouts to prevent hanging during fuzzing
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 10L);
    
    // Disable signal handling for fuzzing safety
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    // 4. curl_easy_perform
    // This will attempt to connect (but will likely fail with our dummy URL/timeout)
    CURLcode perform_result = curl_easy_perform(curl);
    
    // Even if perform fails, we can still test curl_easy_recv
    // The API might return CURLE_COULDNT_CONNECT or timeout, but that's fine for fuzzing
    
    // 5. curl_easy_recv (target API)
    // Prepare buffer for receiving data
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    if (buffer_size > fdp.remaining_bytes()) {
        buffer_size = fdp.remaining_bytes();
    }
    
    // Create buffer and consume some data for it
    std::vector<char> buffer(buffer_size);
    if (buffer_size > 0) {
        auto bytes = fdp.ConsumeBytes<uint8_t>(buffer_size);
        memcpy(buffer.data(), bytes.data(), std::min(bytes.size(), buffer_size));
    }
    
    size_t nread = 0;
    CURLcode recv_result = curl_easy_recv(curl, buffer.data(), buffer.size(), &nread);
    
    // Handle CURLE_AGAIN if returned (though unlikely with our setup)
    if (recv_result == CURLE_AGAIN) {
        // In a real application we would wait on socket, but for fuzzing we just continue
        // Try one more time with zero buffer size to test edge case
        size_t zero_read = 0;
        curl_easy_recv(curl, nullptr, 0, &zero_read);
    }
    
    // Also test with various buffer sizes including edge cases
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        size_t small_buffer_size = fdp.ConsumeIntegralInRange<size_t>(0, 16);
        std::vector<char> small_buffer(small_buffer_size);
        size_t small_nread = 0;
        curl_easy_recv(curl, small_buffer.data(), small_buffer.size(), &small_nread);
    }
    
    // Try to get socket info (CURLINFO_ACTIVESOCKET) - may fail but that's OK
    curl_socket_t sockfd;
    CURLcode info_result = curl_easy_getinfo(curl, CURLINFO_ACTIVESOCKET, &sockfd);
    (void)info_result; // Ignore result for fuzzing

    // 6. curl_easy_cleanup
    curl_easy_cleanup(curl);
    
    // 7. curl_global_cleanup
    curl_global_cleanup();

    return 0;
}

} // namespace harness_004

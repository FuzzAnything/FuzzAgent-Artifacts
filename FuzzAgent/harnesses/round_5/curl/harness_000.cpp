#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <curl/curl.h>

namespace harness_000 {
/*
 * Minimal fuzzing harness for libcurl's easy API using fuzzer-built library
 * Targets: curl_easy_init, curl_easy_setopt, curl_easy_perform, curl_easy_cleanup
 */


// Simple write callback that discards data
static size_t discard_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;  // Always report success
}

// Header callback that discards headers
static size_t discard_header_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;  // Always report success
}

// Simple fuzzer test input
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful fuzzing
    if (size < 8) {
        return 0;
    }
    
    // Simple test: use first byte as option selector
    uint8_t option_selector = data[0];
    size_t url_len = (data[1] % 64) + 1;  // URL length 1-64 bytes
    
    if (size < 2 + url_len) {
        return 0;
    }
    
    // Create URL string from input
    std::string url(reinterpret_cast<const char*>(data + 2), url_len);
    
    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // Set write callback to discard response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullptr);
    
    // Set header callback to discard headers
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, discard_header_cb);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, nullptr);
    
    // Set some basic options based on input
    if (option_selector & 0x01) {
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    }
    if (option_selector & 0x02) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    }
    if (option_selector & 0x04) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    }
    
    // Set timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    
    // Set HTTP method
    if ((option_selector & 0x08) && (size > 2 + url_len + 1)) {
        // Use POST with remaining data
        size_t post_len = size - (2 + url_len);
        const char* post_data = reinterpret_cast<const char*>(data + 2 + url_len);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_len);
    } else {
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    }
    
    // Perform the request (ignore result, we're fuzzing)
    curl_easy_perform(curl);
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_000

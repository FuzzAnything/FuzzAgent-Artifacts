#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/header.h>
#include <curl/multi.h>

namespace harness_001 {
/*
 * Fuzzing harness for libcurl's HTTP header manipulation APIs
 * Targets: curl_easy_nextheader, curl_pushheader_bynum, curl_slist_append
 * Helpers: curl_easy_init, curl_easy_setopt, curl_easy_cleanup
 * Semantic differentiation from harness_000.cpp: Focuses on header manipulation
 *   rather than basic HTTP operations
 */


// Simple write callback that discards data
static size_t discard_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;  // Always report success
}

// Header callback that discards headers
static size_t discard_header_cb(char *buffer, size_t size, size_t nitems, void *userdata) {
    (void)buffer;
    (void)userdata;
    return size * nitems;  // Always report success
}

// Mock push callback for curl_pushheader_bynum testing
static int mock_push_callback(CURL *parent,
                             CURL *easy,
                             size_t num_headers,
                             struct curl_pushheaders *headers,
                             void *userp) {
    (void)parent;
    (void)easy;
    (void)userp;
    
    // Test curl_pushheader_bynum with the provided headers
    for (size_t i = 0; i < num_headers; i++) {
        const char *header = curl_pushheader_bynum(headers, i);
        // Just access the header to test the API
        (void)header;
    }
    
    // Also test curl_pushheader_byname for good measure
    const char *path_header = curl_pushheader_byname(headers, ":path");
    (void)path_header;
    
    return CURL_PUSH_DENY; // Deny push for fuzzing context
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful fuzzing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t option_selector = fdp.ConsumeIntegral<uint8_t>();
    uint8_t num_headers = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    uint8_t max_header_length = fdp.ConsumeIntegralInRange<uint8_t>(1, 128);
    uint8_t url_length = fdp.ConsumeIntegralInRange<uint8_t>(1, 64);
    
    // Consume URL string
    std::string url = fdp.ConsumeBytesAsString(url_length);
    
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
    
    // Set timeout to prevent hanging
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    
    // Create header list using curl_slist_append
    struct curl_slist *headers = nullptr;
    for (uint8_t i = 0; i < num_headers && fdp.remaining_bytes() > 0; i++) {
        // Consume header name and value
        std::string header_name = fdp.ConsumeRandomLengthString(max_header_length);
        std::string header_value = fdp.ConsumeRandomLengthString(max_header_length);
        
        if (!header_name.empty() && !header_value.empty()) {
            std::string header = header_name + ": " + header_value;
            headers = curl_slist_append(headers, header.c_str());
        }
    }
    
    // Set headers if any were created
    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    
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
    
    // For HTTP/2 and push header testing
    if (option_selector & 0x08) {
        // Enable HTTP/2
        curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
        
        // Create a multi handle for push functionality
        CURLM *multi = curl_multi_init();
        if (multi) {
            // Set push callback
            curl_multi_setopt(multi, CURLMOPT_PUSHFUNCTION, mock_push_callback);
            curl_multi_setopt(multi, CURLMOPT_PUSHDATA, nullptr);
            
            // Add easy handle to multi handle
            curl_multi_add_handle(multi, curl);
            
            // Clean up multi handle
            curl_multi_remove_handle(multi, curl);
            curl_multi_cleanup(multi);
        }
    }
    
    // Perform the request (ignore result, we're fuzzing API usage)
    CURLcode res = curl_easy_perform(curl);
    
    // Test curl_easy_nextheader API if request was successful
    if (res == CURLE_OK) {
        struct curl_header *h = nullptr;
        struct curl_header *prev = nullptr;
        
        // Iterate through all headers using curl_easy_nextheader
        // Note: This requires headers to have been received, which may not happen
        // in fuzzing context, but we test the API call anyway
        do {
            h = curl_easy_nextheader(curl, CURLH_HEADER, -1, prev);
            if (h) {
                // Access header fields to ensure coverage
                (void)h->name;
                (void)h->value;
                (void)h->amount;
                (void)h->index;
                (void)h->origin;
                prev = h;
            }
        } while (h);
    }
    
    // Clean up headers list if created
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_001

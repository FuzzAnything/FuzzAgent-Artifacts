#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_000 {

// Simple write callback that discards data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the received data
    return size * nmemb;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for basic operations
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize curl library - must be called once
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Consume fuzzed data for various parameters
    std::string url = fdp.ConsumeRandomLengthString(256);
    int timeout = fdp.ConsumeIntegralInRange<int>(1, 60);
    bool follow_location = fdp.ConsumeBool();
    bool verbose = fdp.ConsumeBool();
    std::string user_agent = fdp.ConsumeRandomLengthString(100);
    std::string post_data = fdp.ConsumeRandomLengthString(1024);
    bool use_post = fdp.ConsumeBool();
    
    // Set basic options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // Set timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    
    // Set follow location
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
    
    // Set verbose mode
    curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    
    // Set user agent
    if (!user_agent.empty()) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());
    }
    
    // Set write callback to discard data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    
    // Set POST data if requested
    if (use_post && !post_data.empty()) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.size());
    }
    
    // Set SSL verification options (test different configurations)
    bool verify_peer = fdp.ConsumeBool();
    bool verify_host = fdp.ConsumeBool();
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verify_peer ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verify_host ? 1L : 0L);
    
    // Set connection timeout
    int connect_timeout = fdp.ConsumeIntegralInRange<int>(1, 30);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout);
    
    // Perform the request - ignore errors as we're fuzzing
    CURLcode result = curl_easy_perform(curl);
    
    // Try to get some info even if perform failed
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    
    double total_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_000

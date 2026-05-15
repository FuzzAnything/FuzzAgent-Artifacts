#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_000 {
/*
 * Fuzzing harness for libcurl's easy interface
 * Targets: curl_easy_init, curl_easy_setopt, curl_easy_perform, curl_easy_cleanup
 * Focus on HTTP/HTTPS protocol handling with various options
 */


// Write callback for handling response data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Just discard the data for fuzzing purposes
    (void)contents;
    (void)userp;
    return realsize;
}

// Read callback for providing request data
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    FuzzedDataProvider *fdp = static_cast<FuzzedDataProvider*>(userp);
    size_t buffer_size = size * nmemb;
    
    if (fdp->remaining_bytes() == 0) {
        return 0; // EOF
    }
    
    // Consume some bytes for the request body
    size_t to_read = fdp->ConsumeIntegralInRange<size_t>(0, buffer_size);
    if (to_read > 0) {
        std::vector<uint8_t> data = fdp->ConsumeBytes<uint8_t>(to_read);
        memcpy(ptr, data.data(), data.size());
        return data.size();
    }
    return 0;
}

// Progress callback
static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    (void)clientp;
    (void)dltotal;
    (void)dlnow;
    (void)ultotal;
    (void)ulnow;
    return 0; // Continue
}

// Header callback
static size_t header_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Just discard headers for fuzzing
    (void)contents;
    (void)userp;
    return realsize;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Consume various parameters from fuzzed input
    uint8_t test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Basic URL - consume from input
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (!url.empty()) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    }
    
    // Set various options based on fuzzed input
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
    
    // HTTP method
    uint8_t http_method = fdp.ConsumeIntegral<uint8_t>() % 6;
    switch (http_method) {
        case 0:
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            break;
        case 1:
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            // Set POST data if available
            if (fdp.remaining_bytes() > 0) {
                std::string post_data = fdp.ConsumeRandomLengthString(1024);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_data.size());
            }
            break;
        case 2:
            curl_easy_setopt(curl, CURLOPT_PUT, 1L);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
            curl_easy_setopt(curl, CURLOPT_READDATA, &fdp);
            break;
        case 3:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
            break;
        case 4:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "HEAD");
            break;
        case 5:
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
            break;
    }
    
    // Timeout settings
    long timeout = fdp.ConsumeIntegralInRange<long>(1, 60);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    
    // Follow redirects
    bool follow_redirects = fdp.ConsumeBool();
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow_redirects ? 1L : 0L);
    
    // SSL/TLS options
    bool verify_peer = fdp.ConsumeBool();
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verify_peer ? 1L : 0L);
    
    bool verify_host = fdp.ConsumeBool();
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verify_host ? 2L : 0L);
    
    // Verbose mode
    bool verbose = fdp.ConsumeBool();
    curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    
    // Proxy settings (if specified in input)
    if (fdp.remaining_bytes() > 10) {
        std::string proxy = fdp.ConsumeRandomLengthString(128);
        if (!proxy.empty()) {
            curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
        }
    }
    
    // User agent
    std::string user_agent = fdp.ConsumeRandomLengthString(128);
    if (!user_agent.empty()) {
        curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());
    }
    
    // HTTP headers
    struct curl_slist *headers = NULL;
    int num_headers = fdp.ConsumeIntegralInRange<int>(0, 5);
    for (int i = 0; i < num_headers && fdp.remaining_bytes() > 10; i++) {
        std::string header = fdp.ConsumeRandomLengthString(100);
        if (!header.empty()) {
            headers = curl_slist_append(headers, header.c_str());
        }
    }
    if (headers) {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    
    // Progress callback
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
    
    // Perform the request (this will likely fail since we're not connecting to real servers,
    // but it will exercise the code paths)
    curl_easy_perform(curl);
    
    // Clean up headers
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    // Get info (even if perform failed)
    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    double total_time = 0.0;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_000

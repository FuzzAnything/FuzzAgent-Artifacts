#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/urlapi.h"

namespace harness_000 {
/*
 * Fuzzing harness for libcurl targeting URL parsing and HTTP request handling
 * Harness ID: 000
 * Targets: curl_url APIs and curl_easy APIs
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally (only once, but fuzzer will call this many times)
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // ==================== URL Parsing API Testing ====================
    
    // Test 1: Basic URL parsing with curl_url APIs
    CURLU* url_handle = curl_url();
    if (url_handle) {
        // Consume various URL components from fuzzer input
        std::string scheme = fdp.ConsumeRandomLengthString(16);
        std::string host = fdp.ConsumeRandomLengthString(64);
        std::string path = fdp.ConsumeRandomLengthString(128);
        std::string query = fdp.ConsumeRandomLengthString(128);
        std::string fragment = fdp.ConsumeRandomLengthString(64);
        
        // Set URL parts using fuzzed data
        if (!scheme.empty()) {
            curl_url_set(url_handle, CURLUPART_SCHEME, scheme.c_str(), 0);
        }
        if (!host.empty()) {
            curl_url_set(url_handle, CURLUPART_HOST, host.c_str(), 0);
        }
        if (!path.empty()) {
            curl_url_set(url_handle, CURLUPART_PATH, path.c_str(), 0);
        }
        if (!query.empty()) {
            curl_url_set(url_handle, CURLUPART_QUERY, query.c_str(), 0);
        }
        if (!fragment.empty()) {
            curl_url_set(url_handle, CURLUPART_FRAGMENT, fragment.c_str(), 0);
        }
        
        // Get URL parts back
        char* url_out = nullptr;
        CURLUcode uc = curl_url_get(url_handle, CURLUPART_URL, &url_out, 0);
        if (uc == CURLUE_OK && url_out) {
            free(url_out);
        }
        
        // Test getting individual parts
        char* part_out = nullptr;
        curl_url_get(url_handle, CURLUPART_HOST, &part_out, 0);
        if (part_out) {
            free(part_out);
        }
        
        // Clean up URL handle
        curl_url_cleanup(url_handle);
    }
    
    // ==================== Easy API Testing ====================
    
    // Test 2: curl_easy APIs with various options
    CURL* easy_handle = curl_easy_init();
    if (easy_handle) {
        // Consume fuzzer data for various options
        int opt_choice = fdp.ConsumeIntegralInRange<int>(0, 10);
        std::string url = fdp.ConsumeRandomLengthString(256);
        bool verbose = fdp.ConsumeBool();
        bool follow_location = fdp.ConsumeBool();
        int timeout = fdp.ConsumeIntegralInRange<int>(0, 60);
        std::string user_agent = fdp.ConsumeRandomLengthString(128);
        std::string proxy = fdp.ConsumeRandomLengthString(128);
        
        // Set URL (required for curl_easy_perform)
        if (!url.empty()) {
            curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
        }
        
        // Set various options based on fuzzed data
        curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, verbose ? 1L : 0L);
        curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, (long)timeout);
        
        if (!user_agent.empty()) {
            curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, user_agent.c_str());
        }
        
        if (!proxy.empty()) {
            curl_easy_setopt(easy_handle, CURLOPT_PROXY, proxy.c_str());
        }
        
        // Set a write callback to discard data (since we're not actually making network calls)
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, 
                         [](void* buffer, size_t size, size_t nmemb, void* userp) -> size_t {
                             return size * nmemb;  // Just pretend we wrote everything
                         });
        
        // Try to perform the operation (will likely fail since URL may be invalid)
        // But that's okay - we're testing the parsing and setup code paths
        CURLcode res = curl_easy_perform(easy_handle);
        
        // Get some info back (even if perform failed)
        long http_code = 0;
        curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &http_code);
        
        double total_time = 0;
        curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &total_time);
        
        // Test error string function
        const char* err_str = curl_easy_strerror(res);
        
        // Clean up
        curl_easy_cleanup(easy_handle);
    }
    
    // Test 3: URL parsing with easy escape/unescape functions
    std::string to_escape = fdp.ConsumeRandomLengthString(256);
    if (!to_escape.empty()) {
        char* escaped = curl_easy_escape(nullptr, to_escape.c_str(), (int)to_escape.length());
        if (escaped) {
            // Try to unescape it back
            int outlen = 0;
            char* unescaped = curl_easy_unescape(nullptr, escaped, 0, &outlen);
            if (unescaped) {
                curl_free(unescaped);
            }
            curl_free(escaped);
        }
    }
    
    // Test 4: Create another easy handle with different options
    if (fdp.remaining_bytes() > 100) {
        CURL* easy2 = curl_easy_init();
        if (easy2) {
            std::string url2 = fdp.ConsumeRandomLengthString(200);
            if (!url2.empty()) {
                curl_easy_setopt(easy2, CURLOPT_URL, url2.c_str());
            }
            
            // Test some boolean options
            curl_easy_setopt(easy2, CURLOPT_NOSIGNAL, 1L);
            curl_easy_setopt(easy2, CURLOPT_FAILONERROR, fdp.ConsumeBool() ? 1L : 0L);
            
            // Test setting and getting error buffer
            char error_buffer[CURL_ERROR_SIZE] = {0};
            curl_easy_setopt(easy2, CURLOPT_ERRORBUFFER, error_buffer);
            
            // Try to perform (will likely fail)
            curl_easy_perform(easy2);
            
            // Reset the handle
            curl_easy_reset(easy2);
            
            curl_easy_cleanup(easy2);
        }
    }
    
    // Clean up global curl state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_000

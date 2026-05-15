#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_001 {

// Simple write callback that discards data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the received data
    return size * nmemb;
}

// Progress callback
static int progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    // Always continue
    return 0;
}

// Header callback
static size_t header_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the headers
    return size * nmemb;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for basic operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Test 1: URL API functions
    {
        CURLU *url_handle = curl_url();
        if (url_handle) {
            std::string url_str = fdp.ConsumeRandomLengthString(256);
            if (!url_str.empty()) {
                curl_url_set(url_handle, CURLUPART_URL, url_str.c_str(), CURLU_GUESS_SCHEME);
                
                // Try to get parts back
                char *scheme = NULL;
                char *host = NULL;
                char *path = NULL;
                char *query = NULL;
                
                curl_url_get(url_handle, CURLUPART_SCHEME, &scheme, 0);
                curl_url_get(url_handle, CURLUPART_HOST, &host, 0);
                curl_url_get(url_handle, CURLUPART_PATH, &path, 0);
                curl_url_get(url_handle, CURLUPART_QUERY, &query, 0);
                
                // Free allocated strings
                if (scheme) curl_free(scheme);
                if (host) curl_free(host);
                if (path) curl_free(path);
                if (query) curl_free(query);
            }
            curl_url_cleanup(url_handle);
        }
    }
    
    // Test 2: CURL easy interface with more advanced options
    {
        CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
        if (global_init_result != CURLE_OK) {
            return 0;
        }
        
        CURL *curl = curl_easy_init();
        if (!curl) {
            curl_global_cleanup();
            return 0;
        }
        
        // Consume various parameters
        std::string url = fdp.ConsumeRandomLengthString(256);
        int http_version = fdp.ConsumeIntegralInRange<int>(0, 4);
        bool use_cookies = fdp.ConsumeBool();
        bool use_progress = fdp.ConsumeBool();
        bool use_debug = fdp.ConsumeBool();
        std::string cookie_data = fdp.ConsumeRandomLengthString(512);
        std::string referer = fdp.ConsumeRandomLengthString(100);
        std::string custom_request = fdp.ConsumeRandomLengthString(50);
        
        // Basic URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        // HTTP version
        switch (http_version) {
            case 0: curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_NONE); break;
            case 1: curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0); break;
            case 2: curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1); break;
            case 3: curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0); break;
            case 4: curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS); break;
        }
        
        // Cookie handling
        if (use_cookies && !cookie_data.empty()) {
            curl_easy_setopt(curl, CURLOPT_COOKIE, cookie_data.c_str());
        }
        
        // Referer
        if (!referer.empty()) {
            curl_easy_setopt(curl, CURLOPT_REFERER, referer.c_str());
        }
        
        // Custom request method
        if (!custom_request.empty()) {
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, custom_request.c_str());
        }
        
        // Progress callback
        if (use_progress) {
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
        } else {
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
        }
        
        // Debug callback
        if (use_debug) {
            curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, NULL); // Would need actual debug function
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        }
        
        // Write callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        
        // Header callback
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
        
        // Timeout settings
        int timeout = fdp.ConsumeIntegralInRange<int>(1, 30);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        
        // Follow redirects
        bool follow_redirects = fdp.ConsumeBool();
        int max_redirects = fdp.ConsumeIntegralInRange<int>(0, 10);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow_redirects ? 1L : 0L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, max_redirects);
        
        // SSL options
        bool verify_ssl = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, verify_ssl ? 1L : 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, verify_ssl ? 2L : 0L);
        
        // Try to perform request
        CURLcode result = curl_easy_perform(curl);
        
        // Get info even if perform failed
        long response_code = 0;
        double total_time = 0.0;
        double speed_download = 0.0;
        char *effective_url = NULL;
        
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        curl_easy_getinfo(curl, CURLINFO_SPEED_DOWNLOAD, &speed_download);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
        
        // effective_url is owned by libcurl handle, will be freed by curl_easy_cleanup
        // Do NOT call curl_free(effective_url);
        
        // Clean up
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    
    // Test 3: Test curl_easy_escape and curl_easy_unescape
    {
        std::string to_escape = fdp.ConsumeRandomLengthString(100);
        if (!to_escape.empty()) {
            char *escaped = curl_easy_escape(NULL, to_escape.c_str(), to_escape.length());
            if (escaped) {
                // Try to unescape it back
                int outlen = 0;
                char *unescaped = curl_easy_unescape(NULL, escaped, strlen(escaped), &outlen);
                if (unescaped) {
                    curl_free(unescaped);
                }
                curl_free(escaped);
            }
        }
    }
    
    // Test 4: Test curl_version and curl_version_info
    {
        char *version = curl_version();
        (void)version; // Use version to avoid unused variable warning
        
        curl_version_info_data *version_info = curl_version_info(CURLVERSION_NOW);
        (void)version_info; // Use version_info to avoid unused variable warning
    }
    
    return 0;
}

} // namespace harness_001

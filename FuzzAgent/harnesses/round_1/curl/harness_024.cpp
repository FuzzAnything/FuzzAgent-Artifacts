#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_024 {


// Custom memory callbacks for curl_global_init_mem
static void *custom_malloc(size_t size) {
    return malloc(size);
}

static void custom_free(void *ptr) {
    free(ptr);
}

static void *custom_realloc(void *ptr, size_t size) {
    return realloc(ptr, size);
}

static char *custom_strdup(const char *str) {
    if (!str) return NULL;
    size_t len = strlen(str) + 1;
    char *dup = (char *)malloc(len);
    if (dup) {
        memcpy(dup, str, len);
    }
    return dup;
}

static void *custom_calloc(size_t nmemb, size_t size) {
    return calloc(nmemb, size);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for configuration and trace string
    if (size < 16) {
        return 0; // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize curl with custom memory callbacks using curl_global_init_mem
    // This is the primary target with 12 undiscovered branches
    CURLcode global_result = curl_global_init_mem(CURL_GLOBAL_ALL,
                                                  custom_malloc,
                                                  custom_free,
                                                  custom_realloc,
                                                  custom_strdup,
                                                  custom_calloc);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Configure tracing with curl_global_trace
    // This is the secondary target with 8 undiscovered branches
    // Consume trace configuration string from fuzzer input
    std::string trace_config = fdp.ConsumeRandomLengthString(256);
    CURLcode trace_result = curl_global_trace(trace_config.c_str());
    // Note: trace_result may fail for invalid configurations, but that's okay for fuzzing
    
    // Step 3: Create curl easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Step 4: Test various easy handle operations using fuzzed data
    
    // Consume data for URL configuration
    if (fdp.remaining_bytes() > 8) {
        std::string url = fdp.ConsumeRandomLengthString(128);
        // Use a dummy scheme to avoid actual network connections
        if (!url.empty()) {
            // Prepend a scheme to make it a valid URL for testing
            std::string full_url = "http://" + url;
            curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
        }
    }
    
    // Consume data for various easy options
    // Test verbose mode
    if (fdp.remaining_bytes() > 1) {
        bool verbose = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    }
    
    // Test follow location
    if (fdp.remaining_bytes() > 1) {
        bool follow_location = fdp.ConsumeBool();
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
    }
    
    // Test timeout
    if (fdp.remaining_bytes() > 4) {
        long timeout = fdp.ConsumeIntegralInRange<long>(0, 3600);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    }
    
    // Test easy escape/unescape functions if there's enough data
    if (fdp.remaining_bytes() > 10) {
        std::string to_escape = fdp.ConsumeRandomLengthString(100);
        if (!to_escape.empty()) {
            char* escaped = curl_easy_escape(curl, to_escape.c_str(), to_escape.length());
            if (escaped) {
                // Unescape it back
                int outlength;
                char* unescaped = curl_easy_unescape(curl, escaped, 0, &outlength);
                if (unescaped) {
                    curl_free(unescaped);
                }
                curl_free(escaped);
            }
        }
    }
    
    // Test easy strerror with various error codes
    if (fdp.remaining_bytes() > 4) {
        uint32_t error_code = fdp.ConsumeIntegral<uint32_t>();
        // Mask to valid CURLcode range (0-99 based on header)
        CURLcode code = static_cast<CURLcode>(error_code % 100);
        const char* error_str = curl_easy_strerror(code);
        // Just call it, don't use the result
        (void)error_str;
    }
    
    // Test easy duphandle and reset
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 1) {
        CURL* dup_curl = curl_easy_duphandle(curl);
        if (dup_curl) {
            curl_easy_reset(dup_curl);
            curl_easy_cleanup(dup_curl);
        }
    }
    
    // Step 5: Clean up easy handle
    curl_easy_cleanup(curl);
    
    // Step 6: Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_024

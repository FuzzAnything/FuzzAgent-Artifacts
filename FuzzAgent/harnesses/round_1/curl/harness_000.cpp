#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_000 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 8) {
        return 0; // Need minimum input
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Create an easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Test 1: curl_easy_escape and curl_easy_unescape
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

    // Test 2: curl_easy_strerror - test various error codes
    if (fdp.remaining_bytes() > 4) {
        uint32_t error_code = fdp.ConsumeIntegral<uint32_t>();
        // Mask to valid CURLcode range
        CURLcode code = static_cast<CURLcode>(error_code % 100);
        const char* error_str = curl_easy_strerror(code);
        // Just call it, don't use the result
        (void)error_str;
    }

    // Test 3: Set some simple options that don't require network
    bool verbose = fdp.ConsumeBool();
    curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose ? 1L : 0L);

    // Test 4: curl_easy_duphandle and curl_easy_reset
    if (fdp.ConsumeBool()) {
        CURL* dup_curl = curl_easy_duphandle(curl);
        if (dup_curl) {
            curl_easy_reset(dup_curl);
            curl_easy_cleanup(dup_curl);
        }
    }

    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}


} // namespace harness_000

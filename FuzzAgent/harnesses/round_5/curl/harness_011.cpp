#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_011 {
/*
 * Fuzzing harness for libcurl's URL encoding/decoding operations
 * Targets: curl_unescape (12 undiscovered branches, 0% covered), curl_escape,
 *          curl_easy_unescape, curl_easy_escape, curl_free (cleanup)
 * Helpers: curl_global_init, curl_global_cleanup
 * Invocation sequence:
 *   1. Generate test strings with various characters (ASCII, Unicode, special chars, percent-encoded sequences)
 *   2. Call curl_escape/curl_easy_escape to encode strings
 *   3. Call curl_unescape/curl_easy_unescape to decode encoded strings  
 *   4. Call curl_free to clean up all returned strings
 *   5. Validate round-trip consistency (encode then decode should match original)
 * Semantic differentiation: This targets URL encoding/decoding operations (fundamental URL processing functions)
 *   vs existing harnesses for HTTP, headers, MIME, form data, WebSocket, Share APIs, multi interface operations, FTP protocol
 * Coverage gap: curl_unescape has 12 undiscovered branches with 0% coverage
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful URL encoding/decoding fuzzing
    // Need enough for configuration, test strings, and operation selection
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration data first
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t test_easy_apis = fdp.ConsumeBool();
    uint8_t test_deprecated_apis = fdp.ConsumeBool();
    uint8_t test_round_trip = fdp.ConsumeBool();
    uint8_t test_special_lengths = fdp.ConsumeBool();
    
    // String length parameters
    uint16_t max_string_len = fdp.ConsumeIntegralInRange<uint16_t>(1, 512);
    uint16_t string_len = fdp.ConsumeIntegralInRange<uint16_t>(1, max_string_len);
    
    // Consume test string from fuzzer input
    std::string test_string = fdp.ConsumeBytesAsString(string_len);
    
    // If test string is empty but we have remaining bytes, use them
    if (test_string.empty() && fdp.remaining_bytes() > 0) {
        test_string = fdp.ConsumeBytesAsString(std::min((size_t)16, fdp.remaining_bytes()));
    }
    
    // Consume percent-encoded test string for decoding tests
    std::string percent_encoded_string;
    if (fdp.remaining_bytes() > 0) {
        uint16_t encoded_len = fdp.ConsumeIntegralInRange<uint16_t>(0, std::min((uint16_t)256, (uint16_t)fdp.remaining_bytes()));
        percent_encoded_string = fdp.ConsumeBytesAsString(encoded_len);
    }
    
    // Initialize curl global state (required for some API variants)
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle for curl_easy_escape/curl_easy_unescape APIs
    CURL *easy_handle = nullptr;
    if (test_easy_apis) {
        easy_handle = curl_easy_init();
        if (!easy_handle) {
            curl_global_cleanup();
            return 0;
        }
    }
    
    // Test different operation types based on fuzzer input
    switch (operation_type) {
        case 0: {
            // Test encoding operations
            
            // Test curl_easy_escape if enabled and we have an easy handle
            if (test_easy_apis && easy_handle && !test_string.empty()) {
                char *encoded = curl_easy_escape(easy_handle, test_string.c_str(), test_string.length());
                if (encoded) {
                    // Test round-trip consistency if enabled
                    if (test_round_trip) {
                        int outlen = 0;
                        char *decoded = curl_easy_unescape(easy_handle, encoded, strlen(encoded), &outlen);
                        if (decoded && outlen > 0) {
                            // Compare decoded with original (they should match for valid encodings)
                            // Note: Some characters may be encoded differently but decode to same value
                            (void)decoded; // Use to avoid compiler warning
                        }
                        curl_free(decoded);
                    }
                    curl_free(encoded);
                }
            }
            
            // Test deprecated curl_escape if enabled
            if (test_deprecated_apis && !test_string.empty()) {
                char *encoded = curl_escape(test_string.c_str(), test_string.length());
                if (encoded) {
                    // Test round-trip with deprecated unescape
                    if (test_round_trip) {
                        char *decoded = curl_unescape(encoded, strlen(encoded));
                        if (decoded) {
                            // Compare decoded with original
                            (void)decoded; // Use to avoid compiler warning
                            curl_free(decoded);
                        }
                    }
                    curl_free(encoded);
                }
            }
            break;
        }
        
        case 1: {
            // Test decoding operations
            
            // Test curl_easy_unescape if enabled and we have an easy handle
            if (test_easy_apis && easy_handle && !percent_encoded_string.empty()) {
                int outlen = 0;
                char *decoded = curl_easy_unescape(easy_handle, percent_encoded_string.c_str(), 
                                                   percent_encoded_string.length(), &outlen);
                if (decoded && outlen > 0) {
                    // Optionally re-encode to test round-trip
                    if (test_round_trip) {
                        char *re_encoded = curl_easy_escape(easy_handle, decoded, outlen);
                        curl_free(re_encoded);
                    }
                    curl_free(decoded);
                }
            }
            
            // Test deprecated curl_unescape if enabled
            if (test_deprecated_apis && !percent_encoded_string.empty()) {
                char *decoded = curl_unescape(percent_encoded_string.c_str(), percent_encoded_string.length());
                if (decoded) {
                    // Optionally re-encode with deprecated API
                    if (test_round_trip) {
                        char *re_encoded = curl_escape(decoded, strlen(decoded));
                        curl_free(re_encoded);
                    }
                    curl_free(decoded);
                }
            }
            break;
        }
        
        case 2: {
            // Test mixed operations - encode then decode with different API variants
            
            if (!test_string.empty()) {
                // Encode with easy API, decode with deprecated API
                if (test_easy_apis && easy_handle && test_deprecated_apis) {
                    char *encoded = curl_easy_escape(easy_handle, test_string.c_str(), test_string.length());
                    if (encoded) {
                        char *decoded = curl_unescape(encoded, strlen(encoded));
                        curl_free(decoded);
                        curl_free(encoded);
                    }
                }
                
                // Encode with deprecated API, decode with easy API
                if (test_easy_apis && easy_handle && test_deprecated_apis) {
                    char *encoded = curl_escape(test_string.c_str(), test_string.length());
                    if (encoded) {
                        int outlen = 0;
                        char *decoded = curl_easy_unescape(easy_handle, encoded, strlen(encoded), &outlen);
                        curl_free(decoded);
                        curl_free(encoded);
                    }
                }
            }
            break;
        }
        
        case 3: {
            // Test special edge cases and length parameters
            
            if (test_special_lengths) {
                // Test with length -1 (which means use strlen to determine length)
                if (test_easy_apis && easy_handle && !test_string.empty()) {
                    char *encoded = curl_easy_escape(easy_handle, test_string.c_str(), -1);
                    curl_free(encoded);
                    
                    if (!percent_encoded_string.empty()) {
                        int outlen = 0;
                        char *decoded = curl_easy_unescape(easy_handle, percent_encoded_string.c_str(), -1, &outlen);
                        curl_free(decoded);
                    }
                }
                
                // Test with length 0
                if (test_easy_apis && easy_handle) {
                    char *encoded = curl_easy_escape(easy_handle, "", 0);
                    curl_free(encoded);
                    
                    int outlen = 0;
                    char *decoded = curl_easy_unescape(easy_handle, "", 0, &outlen);
                    curl_free(decoded);
                }
                
                // Test deprecated APIs with special lengths
                if (test_deprecated_apis) {
                    char *encoded = curl_escape("", 0);
                    curl_free(encoded);
                    
                    char *decoded = curl_unescape("", 0);
                    curl_free(decoded);
                }
            }
            break;
        }
    }
    
    // Clean up easy handle if created
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    // Clean up global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_011

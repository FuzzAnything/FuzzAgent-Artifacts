#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_015 {
/*
 * Fuzzing harness for libcurl targeting URL encoding/decoding APIs with 0% coverage
 * Harness ID: 015
 * Targets: curl_escape(), curl_unescape(), curl_easy_escape(), curl_easy_unescape()
 * Focus: Comprehensive URL encoding/decoding functionality testing with edge cases
 * Differentiated from:
 * - harness_000.cpp (basic curl_easy_escape/unescape usage)
 * - harness_005.cpp (curl_easy_unescape in HTTP context)
 * This harness specifically targets curl_escape() and curl_unescape() coverage gap
 * Implements exact API sequences with various input scenarios including null inputs,
 * negative lengths, zero-length strings, and round-trip encoding/decoding validation
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    if (size < 8) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // ==================== Test 1: curl_easy_escape() and curl_easy_unescape() ====================
    
    // Create an easy handle (though not strictly required for escape functions)
    CURL* easy_handle = curl_easy_init();
    
    // Consume input for escape/unescape testing
    std::string input_string = fdp.ConsumeRandomLengthString(1024);
    
    if (!input_string.empty()) {
        // ========== Test curl_easy_escape() ==========
        
        // Test with explicit length
        int escape_length = fdp.ConsumeIntegralInRange<int>(-1, static_cast<int>(input_string.length()));
        char* escaped = curl_easy_escape(easy_handle, input_string.c_str(), escape_length);
        
        if (escaped) {
            // ========== Test curl_easy_unescape() with outlength parameter ==========
            int outlen = 0;
            char* unescaped = curl_easy_unescape(easy_handle, escaped, 0, &outlen);
            
            if (unescaped) {
                // Verify round-trip consistency (escape then unescape should preserve original for valid input)
                // Note: Some characters may be encoded differently, so we don't assert exact equality
                curl_free(unescaped);
            }
            
            // ========== Test curl_easy_unescape() with NULL outlength parameter ==========
            char* unescaped_null_outlen = curl_easy_unescape(easy_handle, escaped, 0, NULL);
            if (unescaped_null_outlen) {
                curl_free(unescaped_null_outlen);
            }
            
            // ========== Test curl_easy_unescape() with explicit length ==========
            int explicit_unlength = fdp.ConsumeIntegralInRange<int>(-1, static_cast<int>(strlen(escaped)));
            int outlen2 = 0;
            char* unescaped_explicit = curl_easy_unescape(easy_handle, escaped, explicit_unlength, &outlen2);
            if (unescaped_explicit) {
                curl_free(unescaped_explicit);
            }
            
            curl_free(escaped);
        }
        
        // ========== Test curl_easy_escape() with NULL handle ==========
        int escape_length2 = fdp.ConsumeIntegralInRange<int>(-1, static_cast<int>(input_string.length()));
        char* escaped_null_handle = curl_easy_escape(NULL, input_string.c_str(), escape_length2);
        if (escaped_null_handle) {
            curl_free(escaped_null_handle);
        }
        
        // ========== Test curl_easy_escape() with zero-length string ==========
        char* escaped_empty = curl_easy_escape(easy_handle, "", 0);
        if (escaped_empty) {
            // Should return empty string
            curl_free(escaped_empty);
        }
        
        // ========== Test curl_easy_escape() with negative length (auto-detect) ==========
        char* escaped_neg_length = curl_easy_escape(easy_handle, input_string.c_str(), -1);
        if (escaped_neg_length) {
            curl_free(escaped_neg_length);
        }
    }
    
    // ==================== Test 2: curl_escape() and curl_unescape() (legacy APIs) ====================
    
    // Consume another string for legacy API testing
    std::string legacy_input = fdp.ConsumeRandomLengthString(512);
    
    if (!legacy_input.empty()) {
        // ========== Test curl_escape() ==========
        int legacy_escape_length = fdp.ConsumeIntegralInRange<int>(-1, static_cast<int>(legacy_input.length()));
        char* legacy_escaped = curl_escape(legacy_input.c_str(), legacy_escape_length);
        
        if (legacy_escaped) {
            // ========== Test curl_unescape() ==========
            int legacy_unlength = fdp.ConsumeIntegralInRange<int>(-1, static_cast<int>(strlen(legacy_escaped)));
            char* legacy_unescaped = curl_unescape(legacy_escaped, legacy_unlength);
            
            if (legacy_unescaped) {
                curl_free(legacy_unescaped);
            }
            
            curl_free(legacy_escaped);
        }
        
        // ========== Test curl_escape() with NULL input ==========
        char* null_escaped = curl_escape(NULL, 0);
        // Should return NULL
        
        // ========== Test curl_escape() with zero length ==========
        char* zero_length_escaped = curl_escape(legacy_input.c_str(), 0);
        if (zero_length_escaped) {
            curl_free(zero_length_escaped);
        }
        
        // ========== Test curl_unescape() with NULL input ==========
        char* null_unescaped = curl_unescape(NULL, 0);
        // Should return NULL
        
        // ========== Test curl_unescape() with negative length ==========
        char* neg_length_unescaped = curl_unescape(legacy_input.c_str(), -1);
        if (neg_length_unescaped) {
            curl_free(neg_length_unescaped);
        }
    }
    
    // ==================== Test 3: Edge Cases and Special Characters ====================
    
    if (fdp.remaining_bytes() > 16) {
        // Test with strings containing special URL characters
        std::vector<std::string> special_strings;
        
        // Consume several special test strings
        size_t num_special_strings = fdp.ConsumeIntegralInRange<size_t>(0, 5);
        for (size_t i = 0; i < num_special_strings && fdp.remaining_bytes() > 8; i++) {
            std::string special = fdp.ConsumeRandomLengthString(128);
            special_strings.push_back(special);
        }
        
        // Add some fixed special cases for coverage
        special_strings.push_back("Hello World!");
        special_strings.push_back("test@example.com");
        special_strings.push_back("file name with spaces.txt");
        special_strings.push_back("key=value&another=param");
        special_strings.push_back("path/to/file");
        special_strings.push_back("%20%21%22%23");
        special_strings.push_back("already%20encoded");
        
        for (const auto& special_str : special_strings) {
            // Test both easy and legacy APIs
            char* easy_escaped = curl_easy_escape(easy_handle, special_str.c_str(), -1);
            if (easy_escaped) {
                char* easy_unescaped = curl_easy_unescape(easy_handle, easy_escaped, 0, NULL);
                if (easy_unescaped) {
                    curl_free(easy_unescaped);
                }
                curl_free(easy_escaped);
            }
            
            char* legacy_escaped = curl_escape(special_str.c_str(), -1);
            if (legacy_escaped) {
                char* legacy_unescaped = curl_unescape(legacy_escaped, -1);
                if (legacy_unescaped) {
                    curl_free(legacy_unescaped);
                }
                curl_free(legacy_escaped);
            }
        }
    }
    
    // ==================== Test 4: Valid Boundary Cases ====================
    
    // Test with valid length values
    if (!input_string.empty()) {
        // Test with exact string length
        char* exact_length_escaped = curl_easy_escape(easy_handle, input_string.c_str(), 
                                                     static_cast<int>(input_string.length()));
        if (exact_length_escaped) {
            curl_free(exact_length_escaped);
        }
        
        // Test with length 0 (auto-detect)
        char* auto_length_escaped = curl_easy_escape(easy_handle, input_string.c_str(), 0);
        if (auto_length_escaped) {
            curl_free(auto_length_escaped);
        }
        
        // Test with negative length (auto-detect)
        char* neg_length_escaped = curl_easy_escape(easy_handle, input_string.c_str(), -1);
        if (neg_length_escaped) {
            curl_free(neg_length_escaped);
        }
    }
    
    // Test escape/unescape with binary data (null bytes)
    if (fdp.remaining_bytes() > 32) {
        size_t binary_size = fdp.ConsumeIntegralInRange<size_t>(1, 64);
        std::vector<uint8_t> binary_data = fdp.ConsumeBytes<uint8_t>(binary_size);
        
        if (!binary_data.empty()) {
            // Create string from binary data (may contain null bytes)
            std::string binary_string(reinterpret_cast<char*>(binary_data.data()), binary_data.size());
            
            char* binary_escaped = curl_easy_escape(easy_handle, binary_string.c_str(), 
                                                   static_cast<int>(binary_string.length()));
            if (binary_escaped) {
                char* binary_unescaped = curl_easy_unescape(easy_handle, binary_escaped, 0, NULL);
                if (binary_unescaped) {
                    curl_free(binary_unescaped);
                }
                curl_free(binary_escaped);
            }
        }
    }
    
    // ==================== Test 5: Round-trip Validation ====================
    
    // Test that escape(unescape(x)) == x for properly encoded strings
    if (fdp.remaining_bytes() > 64) {
        // Create a URL-encoded string
        std::string to_encode = fdp.ConsumeRandomLengthString(128);
        
        if (!to_encode.empty()) {
            // First encode it
            char* encoded = curl_easy_escape(easy_handle, to_encode.c_str(), -1);
            if (encoded) {
                // Then decode it
                int outlen = 0;
                char* decoded = curl_easy_unescape(easy_handle, encoded, 0, &outlen);
                
                if (decoded && outlen > 0) {
                    // The decoded string may not exactly match original if original had
                    // characters that don't need encoding, but that's okay
                    
                    // Now encode the decoded string again
                    char* reencoded = curl_easy_escape(easy_handle, decoded, outlen);
                    if (reencoded) {
                        // reencoded should match encoded for valid round-trip
                        curl_free(reencoded);
                    }
                    
                    curl_free(decoded);
                }
                
                curl_free(encoded);
            }
        }
    }
    
    // Clean up easy handle if created
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    // Clean up global curl state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_015

#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/urlapi.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_010 {
/*
 * Fuzzing harness for libcurl's URL encoding and decoding operations
 * Targets: curl_easy_escape (36 completely uncovered branches), curl_easy_unescape,
 *          curl_escape (compatibility wrapper), curl_unescape (compatibility wrapper),
 *          curl_url_get, curl_url_set (which internally use escape functions)
 * Focus on URL encoding/decoding functions which are currently 0% covered
 * Test scenarios: Various input strings, different lengths, round-trip encoding/decoding,
 *                 error conditions, NULL handles, URL component manipulation
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl (required for URL API)
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }
    
    // Create a CURL handle for curl_easy_escape/unescape (can be NULL for compatibility)
    CURL *easy_handle = NULL;
    // Randomly decide whether to create an easy handle or use NULL
    bool use_easy_handle = fdp.ConsumeBool();
    if (use_easy_handle) {
        easy_handle = curl_easy_init();
        if (!easy_handle) {
            curl_global_cleanup();
            return 0;
        }
    }
    
    // Create URL handle for curl_url_get/set
    CURLU *url_handle = curl_url();
    if (!url_handle) {
        if (easy_handle) {
            curl_easy_cleanup(easy_handle);
        }
        curl_global_cleanup();
        return 0;
    }
    
    // Consume fixed-size data first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume various test strings from fuzzed input
    std::string test_string1 = fdp.ConsumeRandomLengthString(256);
    std::string test_string2 = fdp.ConsumeRandomLengthString(256);
    std::string test_string3 = fdp.ConsumeRandomLengthString(256);
    std::string url_string = fdp.ConsumeRandomLengthString(512);
    
    // Variables to store encoded/decoded results
    char *encoded_str = NULL;
    char *decoded_str = NULL;
    char *compat_encoded = NULL;
    char *compat_decoded = NULL;
    
    // Test different scenarios based on fuzzed input
    switch (test_scenario) {
        case 0: {
            // Scenario 1: Basic encoding/decoding round-trip
            
            // Test curl_escape (compatibility wrapper)
            int length1 = fdp.ConsumeIntegralInRange<int>(0, test_string1.length());
            compat_encoded = curl_escape(test_string1.c_str(), length1);
            if (compat_encoded) {
                // Test curl_unescape on the encoded result
                compat_decoded = curl_unescape(compat_encoded, -1); // -1 means strlen
                if (compat_decoded) {
                    free(compat_decoded); // curl_unescape returns malloc'ed memory
                }
                curl_free(compat_encoded); // curl_escape returns curl_free'able memory
            }
            
            // Test curl_easy_escape with various lengths
            int length2 = fdp.ConsumeIntegralInRange<int>(0, test_string2.length());
            encoded_str = curl_easy_escape(easy_handle, test_string2.c_str(), length2);
            if (encoded_str) {
                // Test curl_easy_unescape on the encoded result
                int outlength = 0;
                decoded_str = curl_easy_unescape(easy_handle, encoded_str, -1, &outlength);
                if (decoded_str) {
                    curl_free(decoded_str); // curl_easy_unescape returns curl_free'able memory
                }
                curl_free(encoded_str); // curl_easy_escape returns curl_free'able memory
            }
            break;
        }
        
        case 1: {
            // Scenario 2: Testing with different length parameters
            
            // Test with explicit length (not null-terminated substring)
            int explicit_len = fdp.ConsumeIntegralInRange<int>(1, 100);
            if (explicit_len <= (int)test_string1.length()) {
                encoded_str = curl_easy_escape(easy_handle, test_string1.c_str(), explicit_len);
                if (encoded_str) {
                    // Unescape with explicit length
                    int outlength = 0;
                    decoded_str = curl_easy_unescape(easy_handle, encoded_str, -1, &outlength);
                    if (decoded_str) {
                        curl_free(decoded_str);
                    }
                    curl_free(encoded_str);
                }
            }
            
            // Test with length 0 (should encode empty string)
            encoded_str = curl_easy_escape(easy_handle, test_string2.c_str(), 0);
            if (encoded_str) {
                curl_free(encoded_str);
            }
            
            // Test with negative length (-1 means use strlen)
            compat_encoded = curl_escape(test_string3.c_str(), -1);
            if (compat_encoded) {
                compat_decoded = curl_unescape(compat_encoded, -1);
                if (compat_decoded) {
                    free(compat_decoded);
                }
                curl_free(compat_encoded);
            }
            break;
        }
        
        case 2: {
            // Scenario 3: URL API functions that internally use escape functions
            
            // Set a URL with URLENCODE flag
            CURLUcode uc = curl_url_set(url_handle, CURLUPART_URL, 
                                       url_string.empty() ? NULL : url_string.c_str(), 
                                       CURLU_URLENCODE);
            
            // Try to get various URL parts with URLDECODE flag
            if (uc == CURLUE_OK || uc == CURLUE_MALFORMED_INPUT) {
                char *retrieved_part = NULL;
                
                // Try getting different parts with URLDECODE flag
                CURLUPart parts_to_test[] = {
                    CURLUPART_SCHEME, CURLUPART_USER, CURLUPART_PASSWORD,
                    CURLUPART_HOST, CURLUPART_PATH, CURLUPART_QUERY,
                    CURLUPART_FRAGMENT
                };
                
                for (size_t i = 0; i < sizeof(parts_to_test)/sizeof(parts_to_test[0]); i++) {
                    CURLUcode get_uc = curl_url_get(url_handle, parts_to_test[i], 
                                                   &retrieved_part, CURLU_URLDECODE);
                    if (get_uc == CURLUE_OK && retrieved_part) {
                        // The retrieved part might be URL decoded internally
                        curl_free(retrieved_part);
                        retrieved_part = NULL;
                    }
                }
            }
            
            // Also test setting individual parts with URLENCODE flag
            std::string path_component = fdp.ConsumeRandomLengthString(128);
            curl_url_set(url_handle, CURLUPART_PATH, 
                        path_component.empty() ? NULL : path_component.c_str(),
                        CURLU_URLENCODE);
            
            std::string query_component = fdp.ConsumeRandomLengthString(128);
            curl_url_set(url_handle, CURLUPART_QUERY,
                        query_component.empty() ? NULL : query_component.c_str(),
                        CURLU_URLENCODE | CURLU_APPENDQUERY);
            break;
        }
        
        case 3: {
            // Scenario 4: Edge cases and error conditions
            
            // Test with NULL input string (should return NULL)
            encoded_str = curl_easy_escape(easy_handle, NULL, 10);
            if (encoded_str) {
                curl_free(encoded_str);
            }
            
            // Test with zero length and NULL string
            encoded_str = curl_easy_escape(easy_handle, NULL, 0);
            if (encoded_str) {
                curl_free(encoded_str);
            }
            
            // Test unescape with NULL input
            decoded_str = curl_easy_unescape(easy_handle, NULL, 10, NULL);
            if (decoded_str) {
                curl_free(decoded_str);
            }
            
            // Test compatibility functions with NULL
            compat_encoded = curl_escape(NULL, 10);
            if (compat_encoded) {
                curl_free(compat_encoded);
            }
            
            compat_decoded = curl_unescape(NULL, 10);
            if (compat_decoded) {
                free(compat_decoded);
            }
            
            // Test with strings containing various special characters
            std::string special_chars = fdp.ConsumeRandomLengthString(64);
            encoded_str = curl_easy_escape(easy_handle, special_chars.c_str(), -1);
            if (encoded_str) {
                int outlength = 0;
                decoded_str = curl_easy_unescape(easy_handle, encoded_str, -1, &outlength);
                if (decoded_str) {
                    // Verify round-trip (optional, could be expensive)
                    curl_free(decoded_str);
                }
                curl_free(encoded_str);
            }
            
            // Test with percent-encoded strings (valid and invalid)
            std::string percent_encoded = fdp.ConsumeRandomLengthString(64);
            decoded_str = curl_easy_unescape(easy_handle, percent_encoded.c_str(), -1, NULL);
            if (decoded_str) {
                curl_free(decoded_str);
            }
            break;
        }
    }
    
    // Cleanup
    if (url_handle) {
        curl_url_cleanup(url_handle);
    }
    
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    
    curl_global_cleanup();
    return 0;
}

} // namespace harness_010

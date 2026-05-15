#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

namespace harness_015 {
/*
 * Fuzzing harness for libcurl's header API with focus on curl_easy_header
 * Target: curl_easy_header API (identified as critical uncovered API with 0% coverage and 50 undiscovered branches)
 * Focus: HTTP header manipulation lifecycle testing to improve coverage in lib/headers.c
 * Semantic diversity: Unique harness focusing on header retrieval and manipulation unlike existing harnesses
 * Testing strategy: 
 *   1. Create custom HTTP headers using curl_slist_append
 *   2. Set headers with CURLOPT_HTTPHEADER and header callback with CURLOPT_HEADERDATA
 *   3. Perform request with curl_easy_perform to generate headers
 *   4. Test curl_easy_header with various parameters (name, index, origin, request)
 *   5. Test curl_easy_nextheader for iterating through headers
 *   6. Cover different header types: CURLH_HEADER, CURLH_TRAILER, CURLH_CONNECT, CURLH_1XX, CURLH_PSEUDO
 *   7. Test error conditions and edge cases
 */


// Write callback to discard response body
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    (void)contents;
    (void)userp;
    return size * nmemb;
}

// Header callback to collect headers during transfer
static size_t header_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    (void)contents;
    (void)userp;
    // Just acknowledge receipt, don't store for fuzzing simplicity
    return realsize;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need sufficient input for meaningful fuzzing
    // Minimum: test scenario (1 byte), URL string, header count, header strings, etc.
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl
    CURLcode global_res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_res != CURLE_OK) {
        return 0;
    }
    
    // Consume test scenario from fuzzed input
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % 8;
    bool use_custom_headers = fdp.ConsumeBool();
    bool use_header_callback = fdp.ConsumeBool();
    uint8_t num_custom_headers = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    
    // Variables for handles and results
    CURL *easy_handle = nullptr;
    struct curl_slist *headers = nullptr;
    CURLHcode header_res;
    struct curl_header *header = nullptr;
    
    // Create easy handle
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Consume URL from fuzzed input
    std::string test_url = fdp.ConsumeRandomLengthString(256);
    if (!test_url.empty() && test_url.length() > 7) {  // Minimum "http://"
        curl_easy_setopt(easy_handle, CURLOPT_URL, test_url.c_str());
    }
    // If no valid URL from fuzzer, we'll still test API error paths
    // Set basic options
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    
    // Enable redirect following to generate more headers
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
    
    // Set header callback if requested
    if (use_header_callback) {
        curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, header_callback);
        // Optional: set header data pointer
        curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, nullptr);
    }
    
    // Create custom headers if requested
    if (use_custom_headers) {
        for (uint8_t i = 0; i < num_custom_headers && fdp.remaining_bytes() > 10; i++) {
            std::string header_line = fdp.ConsumeRandomLengthString(200);
            if (!header_line.empty()) {
                headers = curl_slist_append(headers, header_line.c_str());
            }
        }
        
        if (headers) {
            curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
        }
    }
    
    // Perform the request (this will generate headers)
    CURLcode perform_res = curl_easy_perform(easy_handle);
    
    // Now test curl_easy_header API regardless of perform result
    // (headers might be available even if perform failed)
    
    // Test 1: Try to get specific headers with various parameters
    if (fdp.remaining_bytes() > 0) {
        // Consume header name to search for
        std::string header_name = fdp.ConsumeRandomLengthString(50);
        
        // Consume index for header lookup
        size_t header_index = fdp.ConsumeIntegralInRange<size_t>(0, 10);
        
        // Consume origin mask (combination of CURLH_* flags)
        unsigned int origin_mask = 0;
        uint8_t origin_bits = fdp.ConsumeIntegral<uint8_t>() % 32;
        
        // Set individual origin bits based on fuzzed input
        if (origin_bits & 0x01) origin_mask |= CURLH_HEADER;
        if (origin_bits & 0x02) origin_mask |= CURLH_TRAILER;
        if (origin_bits & 0x04) origin_mask |= CURLH_CONNECT;
        if (origin_bits & 0x08) origin_mask |= CURLH_1XX;
        if (origin_bits & 0x10) origin_mask |= CURLH_PSEUDO;
        
        // Ensure at least one origin bit is set (API requires type != 0)
        if (origin_mask == 0) {
            origin_mask = CURLH_HEADER;
        }
        
        // Consume request number (-1 for latest, or specific request)
        int request_num = fdp.ConsumeIntegralInRange<int>(-1, 5);
        
        // Call curl_easy_header with fuzzed parameters
        header_res = curl_easy_header(easy_handle, 
                                     header_name.empty() ? nullptr : header_name.c_str(),
                                     header_index,
                                     origin_mask,
                                     request_num,
                                     &header);
        
        // Test various scenarios based on test_scenario
        switch (test_scenario) {
            case 0: {
                // Test looking for common headers - consume header names from fuzzer input
                std::string header_name1 = fdp.ConsumeRandomLengthString(50);
                std::string header_name2 = fdp.ConsumeRandomLengthString(50);
                std::string header_name3 = fdp.ConsumeRandomLengthString(50);
                
                if (!header_name1.empty()) {
                    curl_easy_header(easy_handle, header_name1.c_str(), 0, CURLH_HEADER, -1, &header);
                }
                if (!header_name2.empty()) {
                    curl_easy_header(easy_handle, header_name2.c_str(), 0, CURLH_HEADER, -1, &header);
                }
                if (!header_name3.empty()) {
                    curl_easy_header(easy_handle, header_name3.c_str(), 0, CURLH_HEADER, -1, &header);
                }
                break;
            }
            case 1: {
                std::string header_name_idx = fdp.ConsumeRandomLengthString(50);
                size_t max_idx = fdp.ConsumeIntegralInRange<size_t>(0, 10);
                for (size_t idx = 0; idx < max_idx && idx < 10; idx++) {
                    curl_easy_header(easy_handle, header_name_idx.empty() ? nullptr : header_name_idx.c_str(), 
                                    idx, CURLH_HEADER, -1, &header);
                }
                break;
            }
                
            case 2: {
                // Test different header types - consume from fuzzer
                std::string header_name_type = fdp.ConsumeRandomLengthString(50);
                uint8_t type_choice = fdp.ConsumeIntegral<uint8_t>() % 5;
                unsigned int test_origin = CURLH_HEADER;
                switch (type_choice) {
                    case 0: test_origin = CURLH_HEADER; break;
                    case 1: test_origin = CURLH_TRAILER; break;
                    case 2: test_origin = CURLH_CONNECT; break;
                    case 3: test_origin = CURLH_1XX; break;
                    case 4: test_origin = CURLH_PSEUDO; break;
                }
                curl_easy_header(easy_handle, header_name_type.empty() ? nullptr : header_name_type.c_str(),
                                0, test_origin, -1, &header);
                break;
            }
                
            case 3:
                // Test with different index values
                for (size_t i = 0; i < 5; i++) {
                    curl_easy_header(easy_handle, header_name.c_str(), i, origin_mask, -1, &header);
                }
                break;
                
            case 4:
                // Test different origin combinations
                curl_easy_header(easy_handle, header_name.c_str(), 0, CURLH_HEADER, -1, &header);
                curl_easy_header(easy_handle, header_name.c_str(), 0, CURLH_TRAILER, -1, &header);
                curl_easy_header(easy_handle, header_name.c_str(), 0, CURLH_CONNECT, -1, &header);
                break;
                
            case 5: {
                // Test curl_easy_nextheader iteration
                struct curl_header *h = nullptr;
                struct curl_header *prev = nullptr;
                do {
                    h = curl_easy_nextheader(easy_handle, CURLH_HEADER, -1, prev);
                    prev = h;
                } while (h);
                break;
            }
                
            case 6: {
                // Test error conditions with invalid parameters
                curl_easy_header(easy_handle, nullptr, 0, CURLH_HEADER, -1, &header);
                curl_easy_header(easy_handle, header_name.c_str(), 0, 0, -1, &header); // Invalid type
                curl_easy_header(easy_handle, header_name.c_str(), 0, CURLH_HEADER, 999, &header); // Invalid request
                break;
            }
                
            case 7: {
                // Test combination of operations - consume header name from fuzzer
                std::string combo_header = fdp.ConsumeRandomLengthString(50);
                if (!combo_header.empty()) {
                    curl_easy_header(easy_handle, combo_header.c_str(), 0, CURLH_HEADER, -1, &header);
                }
                
                // Then iterate through all headers
                struct curl_header *iter = nullptr;
                struct curl_header *prev_iter = nullptr;
                unsigned int origins[] = {CURLH_HEADER, CURLH_TRAILER, CURLH_CONNECT};
                for (unsigned int orig : origins) {
                    prev_iter = nullptr;
                    do {
                        iter = curl_easy_nextheader(easy_handle, orig, -1, prev_iter);
                        prev_iter = iter;
                    } while (iter);
                }
                break;
            }
        }
    }
    
    // Cleanup
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_015

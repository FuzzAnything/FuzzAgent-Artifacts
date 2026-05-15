#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <functional>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_018 {
/*
 * Fuzzing harness for libcurl targeting HTTP header manipulation APIs with 0% coverage
 * Harness ID: 018
 * Targets: curl_easy_nextheader (0% coverage, 34 undiscovered branches) and related header functions
 * Specific APIs: curl_easy_setopt with CURLOPT_HTTPHEADER, CURLOPT_HEADERFUNCTION, CURLOPT_HEADERDATA
 * Focus: HTTP header manipulation with header callback function and header iteration
 * Differentiated from:
 * - harness_000.cpp (basic curl_url/curl_easy APIs)
 * - harness_005.cpp (curl_easy_header/curl_easy_nextheader but without CURLOPT_HEADERFUNCTION callback)
 * - harness_009.cpp (has CURLOPT_HTTPHEADER but not full header callback sequence)
 * This harness specifically targets the exact invocation sequence recommended for header coverage:
 * 1. curl_easy_init, 2. curl_easy_setopt with CURLOPT_HTTPHEADER, 3. curl_easy_setopt with CURLOPT_HEADERFUNCTION and CURLOPT_HEADERDATA,
 * 4. Simulate/perform HTTP request, 5. curl_easy_nextheader to iterate headers, 6. curl_easy_cleanup.
 */



// Header callback function to capture response headers
static size_t header_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    // Userdata can be used to store headers, but for fuzzing we just want to exercise the callback
    (void)userdata;
    
    // Calculate total size of header data
    size_t total_size = size * nitems;
    
    // For fuzzing, we can parse header line if we want, but just returning the size is fine
    // This exercises the header parsing and callback mechanism
    return total_size;
}

// Simple write callback to discard response body data
static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for header operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // ==================== Step 1: curl_easy_init ====================
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // ==================== Step 2: curl_easy_setopt with CURLOPT_HTTPHEADER ====================
    struct curl_slist* headers = NULL;
    
    // Determine number of custom request headers to create (0-5)
    size_t num_headers = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    
    for (size_t i = 0; i < num_headers && fdp.remaining_bytes() > 16; i++) {
        // Consume header name and value from fuzzer input
        std::string header_name = fdp.ConsumeRandomLengthString(64);
        std::string header_value = fdp.ConsumeRandomLengthString(256);
        
        if (!header_name.empty() && !header_value.empty()) {
            std::string full_header = header_name + ": " + header_value;
            headers = curl_slist_append(headers, full_header.c_str());
        }
    }
    
    // Set custom headers if any were created
    if (headers) {
        curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
    }
    
    // ==================== Step 3: curl_easy_setopt with CURLOPT_HEADERFUNCTION and CURLOPT_HEADERDATA ====================
    
    // Set header callback function
    curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, header_callback);
    
    // Create user data for header callback (simple integer for testing)
    int header_user_data = fdp.ConsumeIntegral<int>();
    curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, &header_user_data);
    
    // Also set write callback to discard response body
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    
    // Consume URL for the request
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (url.empty()) {
        // Use a dummy URL if none provided
        url = "http://localhost/";
    }
    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    
    // Set other options based on fuzzed data
    bool verbose = fdp.ConsumeBool();
    bool follow_location = fdp.ConsumeBool();
    int timeout = fdp.ConsumeIntegralInRange<int>(0, 30);
    
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, (long)timeout);
    
    // ==================== Step 4: Simulate/perform HTTP request ====================
    
    // Perform the request (ignore errors - we want to test header API even if request fails)
    // Note: In real fuzzing, this would need a mock network layer or would fail with network errors
    // but the header callback and header APIs should still be exercised
    CURLcode res = curl_easy_perform(easy_handle);
    
    // ==================== Step 5: curl_easy_nextheader to iterate headers ====================
    
    // Even if request failed, we can still test header iteration APIs
    // Test curl_easy_nextheader with different origin types
    
    unsigned int origins[] = {
        CURLH_HEADER,    // plain server header
        CURLH_TRAILER,   // trailers  
        CURLH_CONNECT,   // CONNECT headers
        CURLH_1XX,       // 1xx headers
        CURLH_PSEUDO     // pseudo headers
    };
    
    // Test with different request numbers (-1 for most recent, 0 for first, etc.)
    int request_numbers[] = {-1, 0, 1};
    
    for (size_t i = 0; i < sizeof(origins) / sizeof(origins[0]); i++) {
        for (size_t j = 0; j < sizeof(request_numbers) / sizeof(request_numbers[0]); j++) {
            struct curl_header* prev = NULL;
            struct curl_header* h = NULL;
            
            // Iterate through all headers of this origin and request
            do {
                h = curl_easy_nextheader(easy_handle, origins[i], request_numbers[j], prev);
                if (h) {
                    // Test accessing header fields to ensure they can be read
                    (void)h->name;
                    (void)h->value;
                    (void)h->amount;
                    (void)h->index;
                    (void)h->origin;
                    prev = h;
                }
            } while (h);
        }
    }
    
    // Also test curl_easy_header for specific header lookup
    if (res == CURLE_OK) {
        // Try to look up common headers that might exist
        const char* common_headers[] = {
            "Content-Type", "Server", "Date", "Cache-Control", 
            "Content-Length", "Connection", "Location", "User-Agent"
        };
        
        for (size_t i = 0; i < sizeof(common_headers) / sizeof(common_headers[0]); i++) {
            struct curl_header* header = NULL;
            CURLHcode hcode = curl_easy_header(easy_handle, 
                                              common_headers[i], 
                                              0,  // First occurrence
                                              CURLH_HEADER,
                                              -1, // Most recent request
                                              &header);
            
            // All results are valid for testing - we want to exercise the code paths
            (void)hcode;
            (void)header;
        }
    }
    
    // ==================== Step 6: curl_easy_cleanup ====================
    
    // Free custom headers if they were created
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_018

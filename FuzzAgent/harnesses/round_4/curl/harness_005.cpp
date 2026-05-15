#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"
#include "curl/header.h"

namespace harness_005 {
/*
 * Fuzzing harness for libcurl targeting HTTP Headers Management API
 * Harness ID: 005
 * Targets: curl_easy_header, curl_easy_nextheader, curl_easy_unescape APIs
 * Focus: HTTP header parsing, retrieval, manipulation with fuzzed header data
 * Differentiated from:
 * - harness_000.cpp (curl_url/curl_easy APIs)
 * - harness_001.cpp (MIME APIs) 
 * - harness_002.cpp (curl_multi APIs)
 * - harness_003.cpp (FTP protocol)
 * - harness_004.cpp (Form APIs)
 * This harness specifically targets HTTP Headers Management coverage gap with 44 branches and 50 undiscovered accumulated branches
 */



// Simple write callback to discard response data
static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for header operations
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Create easy handle for HTTP operations
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // ==================== Custom Header Setup ====================
    
    struct curl_slist* headers = NULL;
    
    // Determine number of custom headers to create (0-10)
    size_t num_headers = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    
    for (size_t i = 0; i < num_headers && fdp.remaining_bytes() > 16; i++) {
        // Consume header name and value
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
    
    // Consume URL for the request
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (url.empty()) {
        url = "http://localhost/";
    }
    
    // Set URL
    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    
    // Set other options based on fuzzed data
    bool verbose = fdp.ConsumeBool();
    bool follow_location = fdp.ConsumeBool();
    int timeout = fdp.ConsumeIntegralInRange<int>(0, 30);
    
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, (long)timeout);
    
    // Set write callback to discard response body
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    
    // ==================== Perform Request ====================
    
    // Perform the request (ignore errors - we want to test header API even if request fails)
    CURLcode res = curl_easy_perform(easy_handle);
    
    // ==================== Header Retrieval API Testing ====================
    
    if (res == CURLE_OK) {
        // Test curl_easy_header for specific headers
        struct curl_header* header = NULL;
        
        // Try to get common headers that might exist
        const char* header_names[] = {
            "Content-Type", "Server", "Date", "Cache-Control", 
            "Content-Length", "Connection", "Location"
        };
        size_t num_header_names = sizeof(header_names) / sizeof(header_names[0]);
        
        for (size_t i = 0; i < num_header_names; i++) {
            CURLHcode hcode = curl_easy_header(easy_handle, 
                                              header_names[i], 
                                              0,  // First occurrence
                                              CURLH_HEADER,
                                              -1, // Most recent request
                                              &header);
            
            if (hcode == CURLHE_OK && header) {
                // Header found - do nothing, just testing retrieval
                (void)header;
            }
        }
        
        // Test getting headers with different origins
        unsigned int origins[] = {CURLH_HEADER, CURLH_TRAILER, CURLH_CONNECT, CURLH_1XX, CURLH_PSEUDO};
        
        for (size_t i = 0; i < sizeof(origins) / sizeof(origins[0]); i++) {
            struct curl_header* h = NULL;
            struct curl_header* prev = NULL;
            
            // Iterate through all headers of this origin
            do {
                h = curl_easy_nextheader(easy_handle, origins[i], -1, prev);
                if (h) {
                    // Test accessing header fields
                    (void)h->name;
                    (void)h->value;
                    (void)h->amount;
                    (void)h->index;
                    (void)h->origin;
                    prev = h;
                }
            } while (h);
        }
        
        // Test getting header by index (try first few indices)
        for (size_t index = 0; index < 5; index++) {
            CURLHcode hcode = curl_easy_header(easy_handle, 
                                              "Content-Type", 
                                              index,
                                              CURLH_HEADER,
                                              -1,
                                              &header);
            if (hcode == CURLHE_BADINDEX || hcode == CURLHE_MISSING) {
                // Expected for non-existent indices
                break;
            }
        }
    }
    
    // ==================== URL Unescaping API Testing ====================
    
    // Test curl_easy_unescape with fuzzed URL-encoded strings
    size_t num_unescape_tests = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    
    for (size_t i = 0; i < num_unescape_tests && fdp.remaining_bytes() > 8; i++) {
        // Consume URL-encoded string to unescape
        std::string encoded_str = fdp.ConsumeRandomLengthString(128);
        
        if (!encoded_str.empty()) {
            int outlen = 0;
            char* decoded = curl_easy_unescape(easy_handle, 
                                              encoded_str.c_str(), 
                                              encoded_str.length(),
                                              &outlen);
            
            if (decoded) {
                // Test the decoded output (if any)
                if (outlen > 0) {
                    // Verify we can access the decoded data
                    (void)decoded[0];
                }
                curl_free(decoded);
            }
        }
    }
    
    // Test curl_easy_unescape with NULL outlength parameter
    if (fdp.remaining_bytes() > 8) {
        std::string encoded_str2 = fdp.ConsumeRandomLengthString(64);
        if (!encoded_str2.empty()) {
            char* decoded2 = curl_easy_unescape(easy_handle,
                                               encoded_str2.c_str(),
                                               encoded_str2.length(),
                                               NULL);
            if (decoded2) {
                curl_free(decoded2);
            }
        }
    }
    
    // Test curl_easy_unescape with zero length (auto-detect)
    if (fdp.remaining_bytes() > 8) {
        std::string encoded_str3 = fdp.ConsumeRandomLengthString(64);
        if (!encoded_str3.empty()) {
            int outlen3 = 0;
            char* decoded3 = curl_easy_unescape(easy_handle,
                                               encoded_str3.c_str(),
                                               0,  // Auto-detect length
                                               &outlen3);
            if (decoded3) {
                curl_free(decoded3);
            }
        }
    }
    
    // ==================== Cleanup ====================
    
    // Free custom headers if they were created
    if (headers) {
        curl_slist_free_all(headers);
    }
    
    curl_easy_cleanup(easy_handle);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_005

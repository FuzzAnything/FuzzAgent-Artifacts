#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_009 {

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

// Debug callback
static int debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    // Do nothing with debug info
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for comprehensive operations
    // We need data for URL, various options, and operation selection
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize curl library - must be called once
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // ============================================
    // TEST: curl_easy_duphandle - PRIMARY TARGET
    // ============================================
    {
        // Create original easy handle
        CURL *original_handle = curl_easy_init();
        if (!original_handle) {
            curl_global_cleanup();
            return 0;
        }
        
        // Consume various configuration parameters from fuzzed data
        // This creates diverse states for the original handle
        
        // 1. URL
        std::string url = fdp.ConsumeRandomLengthString(256);
        if (!url.empty()) {
            curl_easy_setopt(original_handle, CURLOPT_URL, url.c_str());
        }
        
        // 2. HTTP version
        uint8_t http_version = fdp.ConsumeIntegral<uint8_t>() % 5;
        switch (http_version) {
            case 0: curl_easy_setopt(original_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_NONE); break;
            case 1: curl_easy_setopt(original_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0); break;
            case 2: curl_easy_setopt(original_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1); break;
            case 3: curl_easy_setopt(original_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0); break;
            case 4: curl_easy_setopt(original_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS); break;
        }
        
        // 3. Timeout options
        bool use_timeout = fdp.ConsumeBool();
        if (use_timeout) {
            long timeout = fdp.ConsumeIntegralInRange<long>(1, 60);
            curl_easy_setopt(original_handle, CURLOPT_TIMEOUT, timeout);
            
            long connect_timeout = fdp.ConsumeIntegralInRange<long>(1, 30);
            curl_easy_setopt(original_handle, CURLOPT_CONNECTTIMEOUT, connect_timeout);
        }
        
        // 4. Follow location
        bool follow_location = fdp.ConsumeBool();
        curl_easy_setopt(original_handle, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
        
        // 5. Verbose/debug options
        bool verbose = fdp.ConsumeBool();
        curl_easy_setopt(original_handle, CURLOPT_VERBOSE, verbose ? 1L : 0L);
        
        bool debug = fdp.ConsumeBool();
        if (debug) {
            curl_easy_setopt(original_handle, CURLOPT_DEBUGFUNCTION, debug_callback);
            curl_easy_setopt(original_handle, CURLOPT_DEBUGDATA, NULL);
        }
        
        // 6. SSL options
        bool verify_peer = fdp.ConsumeBool();
        bool verify_host = fdp.ConsumeBool();
        curl_easy_setopt(original_handle, CURLOPT_SSL_VERIFYPEER, verify_peer ? 1L : 0L);
        curl_easy_setopt(original_handle, CURLOPT_SSL_VERIFYHOST, verify_host ? 2L : 0L);
        
        // 7. Write callback
        curl_easy_setopt(original_handle, CURLOPT_WRITEFUNCTION, write_callback);
        
        // 8. Progress callback
        bool use_progress = fdp.ConsumeBool();
        if (use_progress) {
            curl_easy_setopt(original_handle, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(original_handle, CURLOPT_PROGRESSFUNCTION, progress_callback);
            curl_easy_setopt(original_handle, CURLOPT_PROGRESSDATA, NULL);
        } else {
            curl_easy_setopt(original_handle, CURLOPT_NOPROGRESS, 1L);
        }
        
        // 9. Header callback
        bool use_header_callback = fdp.ConsumeBool();
        if (use_header_callback) {
            curl_easy_setopt(original_handle, CURLOPT_HEADERFUNCTION, header_callback);
            curl_easy_setopt(original_handle, CURLOPT_HEADERDATA, NULL);
        }
        
        // 10. User agent
        bool set_user_agent = fdp.ConsumeBool();
        if (set_user_agent) {
            std::string user_agent = fdp.ConsumeRandomLengthString(100);
            curl_easy_setopt(original_handle, CURLOPT_USERAGENT, user_agent.c_str());
        }
        
        // 11. Referer
        bool set_referer = fdp.ConsumeBool();
        if (set_referer) {
            std::string referer = fdp.ConsumeRandomLengthString(100);
            curl_easy_setopt(original_handle, CURLOPT_REFERER, referer.c_str());
        }
        
        // 12. Custom request method
        bool use_custom_request = fdp.ConsumeBool();
        if (use_custom_request) {
            std::string custom_request = fdp.ConsumeRandomLengthString(20);
            curl_easy_setopt(original_handle, CURLOPT_CUSTOMREQUEST, custom_request.c_str());
        }
        
        // 13. POST data
        bool use_post = fdp.ConsumeBool();
        if (use_post) {
            std::string post_data = fdp.ConsumeRandomLengthString(512);
            curl_easy_setopt(original_handle, CURLOPT_POSTFIELDS, post_data.c_str());
            curl_easy_setopt(original_handle, CURLOPT_POSTFIELDSIZE, post_data.size());
        }
        
        // 14. Cookie data
        bool use_cookies = fdp.ConsumeBool();
        if (use_cookies) {
            std::string cookie_data = fdp.ConsumeRandomLengthString(256);
            curl_easy_setopt(original_handle, CURLOPT_COOKIE, cookie_data.c_str());
        }
        
        // 15. Range header
        bool use_range = fdp.ConsumeBool();
        if (use_range) {
            std::string range = fdp.ConsumeRandomLengthString(50);
            curl_easy_setopt(original_handle, CURLOPT_RANGE, range.c_str());
        }
        
        // ============================================
        // DUPLICATE THE HANDLE - PRIMARY TARGET FUNCTION
        // ============================================
        CURL *duplicated_handle = curl_easy_duphandle(original_handle);
        
        // Test error path: duphandle with NULL handle (should return NULL)
        CURL *null_duplicate = curl_easy_duphandle(NULL);
        
        // ============================================
        // TEST ORIGINAL HANDLE OPERATIONS
        // ============================================
        if (original_handle) {
            // Try to perform with original handle (will likely fail without network, but that's OK)
            CURLcode perform_result = curl_easy_perform(original_handle);
            
            // Get some info from original handle
            long http_code = 0;
            curl_easy_getinfo(original_handle, CURLINFO_RESPONSE_CODE, &http_code);
            
            double total_time = 0.0;
            curl_easy_getinfo(original_handle, CURLINFO_TOTAL_TIME, &total_time);
            
            char *effective_url = NULL;
            curl_easy_getinfo(original_handle, CURLINFO_EFFECTIVE_URL, &effective_url);
            
            double download_size = 0.0;
            curl_easy_getinfo(original_handle, CURLINFO_SIZE_DOWNLOAD, &download_size);
        }
        
        // ============================================
        // TEST DUPLICATED HANDLE OPERATIONS
        // ============================================
        if (duplicated_handle) {
            // Modify some options on duplicated handle to test independence
            bool modify_duplicate = fdp.ConsumeBool();
            if (modify_duplicate) {
                // Change timeout on duplicate
                long new_timeout = fdp.ConsumeIntegralInRange<long>(1, 120);
                curl_easy_setopt(duplicated_handle, CURLOPT_TIMEOUT, new_timeout);
                
                // Change user agent on duplicate
                std::string new_user_agent = fdp.ConsumeRandomLengthString(100);
                curl_easy_setopt(duplicated_handle, CURLOPT_USERAGENT, new_user_agent.c_str());
            }
            
            // Try to perform with duplicated handle
            CURLcode dup_perform_result = curl_easy_perform(duplicated_handle);
            
            // Get info from duplicated handle
            long dup_http_code = 0;
            curl_easy_getinfo(duplicated_handle, CURLINFO_RESPONSE_CODE, &dup_http_code);
            
            double dup_total_time = 0.0;
            curl_easy_getinfo(duplicated_handle, CURLINFO_TOTAL_TIME, &dup_total_time);
            
            // Test curl_easy_reset on duplicated handle
            bool reset_duplicate = fdp.ConsumeBool();
            if (reset_duplicate && duplicated_handle) {
                curl_easy_reset(duplicated_handle);
                
                // After reset, set a simple URL and try perform
                std::string reset_url = fdp.ConsumeRandomLengthString(128);
                curl_easy_setopt(duplicated_handle, CURLOPT_URL, reset_url.c_str());
                curl_easy_setopt(duplicated_handle, CURLOPT_WRITEFUNCTION, write_callback);
                curl_easy_perform(duplicated_handle);
            }
            
            // Clean up duplicated handle
            curl_easy_cleanup(duplicated_handle);
        }
        
        // Clean up original handle
        curl_easy_cleanup(original_handle);
        
        // Test with invalid handle after cleanup (should crash or fail gracefully)
        // This is a stress test for the fuzzer
    }
    
    // ============================================
    // TEST: Multiple sequential duplications
    // ============================================
    {
        bool test_multiple = fdp.ConsumeBool();
        if (test_multiple && fdp.remaining_bytes() > 32) {
            CURL *base_handle = curl_easy_init();
            if (base_handle) {
                // Set a simple URL
                std::string base_url = fdp.ConsumeRandomLengthString(128);
                curl_easy_setopt(base_handle, CURLOPT_URL, base_url.c_str());
                curl_easy_setopt(base_handle, CURLOPT_WRITEFUNCTION, write_callback);
                
                // Create chain of duplicates
                CURL *dup1 = curl_easy_duphandle(base_handle);
                CURL *dup2 = dup1 ? curl_easy_duphandle(dup1) : NULL;
                CURL *dup3 = dup2 ? curl_easy_duphandle(dup2) : NULL;
                
                // Perform with each duplicate
                if (dup1) {
                    curl_easy_perform(dup1);
                    curl_easy_cleanup(dup1);
                }
                if (dup2) {
                    curl_easy_perform(dup2);
                    curl_easy_cleanup(dup2);
                }
                if (dup3) {
                    curl_easy_perform(dup3);
                    curl_easy_cleanup(dup3);
                }
                
                curl_easy_cleanup(base_handle);
            }
        }
    }
    
    // ============================================
    // TEST: Duplication with minimal configuration
    // ============================================
    {
        bool test_minimal = fdp.ConsumeBool();
        if (test_minimal) {
            CURL *minimal_handle = curl_easy_init();
            if (minimal_handle) {
                // Set only URL and write callback
                std::string minimal_url = fdp.ConsumeRandomLengthString(64);
                curl_easy_setopt(minimal_handle, CURLOPT_URL, minimal_url.c_str());
                curl_easy_setopt(minimal_handle, CURLOPT_WRITEFUNCTION, write_callback);
                
                // Duplicate minimal handle
                CURL *minimal_dup = curl_easy_duphandle(minimal_handle);
                if (minimal_dup) {
                    curl_easy_perform(minimal_dup);
                    curl_easy_cleanup(minimal_dup);
                }
                
                curl_easy_cleanup(minimal_handle);
            }
        }
    }
    
    // ============================================
    // TEST: Error path - duplication after perform
    // ============================================
    {
        bool test_error_path = fdp.ConsumeBool();
        if (test_error_path && fdp.remaining_bytes() > 16) {
            CURL *error_handle = curl_easy_init();
            if (error_handle) {
                std::string error_url = fdp.ConsumeRandomLengthString(128);
                curl_easy_setopt(error_handle, CURLOPT_URL, error_url.c_str());
                curl_easy_setopt(error_handle, CURLOPT_WRITEFUNCTION, write_callback);
                
                // Perform (will fail without network)
                curl_easy_perform(error_handle);
                
                // Now duplicate after perform
                CURL *error_dup = curl_easy_duphandle(error_handle);
                if (error_dup) {
                    // The duplicate should inherit the state
                    curl_easy_perform(error_dup);
                    curl_easy_cleanup(error_dup);
                }
                
                curl_easy_cleanup(error_handle);
            }
        }
    }
    
    // Clean up global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_009

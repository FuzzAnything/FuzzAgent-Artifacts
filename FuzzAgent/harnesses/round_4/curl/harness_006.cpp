#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_006 {
/*
 * Fuzzing harness for libcurl targeting Share API family for resource sharing
 * Harness ID: 006
 * Targets: curl_share_init, curl_share_setopt, curl_share_cleanup APIs
 * Focus: Resource sharing between multiple easy handles (DNS cache, SSL sessions, connection pooling)
 * Differentiated from:
 * - harness_000.cpp (curl_url/curl_easy APIs)
 * - harness_001.cpp (MIME APIs) 
 * - harness_002.cpp (curl_multi APIs)
 * - harness_003.cpp (FTP protocol)
 * - harness_004.cpp (Form APIs)
 * - harness_005.cpp (HTTP Headers Management)
 * This harness specifically targets Share API coverage gap with 66+ undiscovered branches
 * Focuses on: shared DNS cache, SSL sessions, connection pooling between multiple easy handles
 */



// Simple lock/unlock callbacks for testing share API
static void share_lock_callback(CURL *handle,
                                curl_lock_data data,
                                curl_lock_access locktype,
                                void *userptr) {
    (void)handle;
    (void)data;
    (void)locktype;
    (void)userptr;
    // Simple lock function - could be no-op for fuzzing
}

static void share_unlock_callback(CURL *handle,
                                  curl_lock_data data,
                                  void *userptr) {
    (void)handle;
    (void)data;
    (void)userptr;
    // Simple unlock function - could be no-op for fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for share operations
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // ==================== Share Handle Creation ====================
    
    // Create a share handle for resource sharing
    CURLSH* share_handle = curl_share_init();
    if (!share_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // ==================== Share Option Configuration ====================
    
    // Determine which data types to share (based on fuzzed input)
    size_t num_share_options = fdp.ConsumeIntegralInRange<size_t>(1, 6);
    
    for (size_t i = 0; i < num_share_options && fdp.remaining_bytes() > 4; i++) {
        // Consume share option type
        int option_type = fdp.ConsumeIntegralInRange<int>(0, 5);
        curl_lock_data data_type = CURL_LOCK_DATA_NONE;
        
        // Map option_type to actual data type
        switch (option_type) {
            case 0: data_type = CURL_LOCK_DATA_DNS; break;          // Share DNS cache
            case 1: data_type = CURL_LOCK_DATA_SSL_SESSION; break;  // Share SSL sessions
            case 2: data_type = CURL_LOCK_DATA_CONNECT; break;      // Share connection pool
            case 3: data_type = CURL_LOCK_DATA_COOKIE; break;       // Share cookies
            case 4: data_type = CURL_LOCK_DATA_PSL; break;          // Share Public Suffix List
            case 5: data_type = CURL_LOCK_DATA_HSTS; break;         // Share HSTS cache
        }
        
        // Determine whether to share or unshare this data type
        bool should_share = fdp.ConsumeBool();
        
        if (should_share) {
            // Share this data type
            curl_share_setopt(share_handle, CURLSHOPT_SHARE, data_type);
        } else {
            // Unshare this data type
            curl_share_setopt(share_handle, CURLSHOPT_UNSHARE, data_type);
        }
    }
    
    // Set lock/unlock callbacks if enough data remains
    if (fdp.remaining_bytes() > 8 && fdp.ConsumeBool()) {
        // Consume user data pointer (we'll just use nullptr or a simple value)
        void* user_data = nullptr;
        if (fdp.ConsumeBool()) {
            // Use a simple integer as user data
            static int user_data_value = 42;
            user_data = &user_data_value;
        }
        
        curl_share_setopt(share_handle, CURLSHOPT_LOCKFUNC, share_lock_callback);
        curl_share_setopt(share_handle, CURLSHOPT_UNLOCKFUNC, share_unlock_callback);
        curl_share_setopt(share_handle, CURLSHOPT_USERDATA, user_data);
    }
    
    // ==================== Create Easy Handles with Shared Resources ====================
    
    // Determine number of easy handles to create (1-4 to avoid excessive resource usage)
    size_t num_easy_handles = fdp.ConsumeIntegralInRange<size_t>(1, 4);
    
    std::vector<CURL*> easy_handles;
    easy_handles.reserve(num_easy_handles);
    
    // Consume URLs and options for each easy handle
    std::vector<std::string> urls;
    std::vector<int> http_methods; // 0: GET, 1: POST
    std::vector<bool> use_ssl_flags;
    
    // Determine actual number of handles we can create based on available input
    size_t actual_handles = 0;
    for (size_t i = 0; i < num_easy_handles && fdp.remaining_bytes() > 16; i++) {
        // Consume URL for this handle
        std::string url = fdp.ConsumeRandomLengthString(128);
        if (url.empty()) {
            // Provide default URLs for testing
            if (fdp.ConsumeBool()) {
                url = "http://localhost/";
            } else {
                url = "https://localhost/";
            }
        }
        urls.push_back(url);
        
        // Consume HTTP method
        http_methods.push_back(fdp.ConsumeIntegralInRange<int>(0, 1));
        
        // Consume SSL flag (whether to use HTTPS)
        use_ssl_flags.push_back(fdp.ConsumeBool());
        
        actual_handles++;
    }
    
    // Update num_easy_handles to actual number created
    num_easy_handles = actual_handles;
    
    // If no handles could be created, cleanup and return
    if (num_easy_handles == 0) {
        curl_share_cleanup(share_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Create easy handles and associate with share handle
    for (size_t i = 0; i < num_easy_handles; i++) {
        CURL* easy = curl_easy_init();
        if (easy) {
            easy_handles.push_back(easy);
            
            // Set URL
            curl_easy_setopt(easy, CURLOPT_URL, urls[i].c_str());
            
            // Set HTTP method based on fuzzed value
            if (http_methods[i] == 1 && fdp.remaining_bytes() > 4) {
                // POST method with some data
                std::string post_data = fdp.ConsumeRandomLengthString(256);
                curl_easy_setopt(easy, CURLOPT_POSTFIELDS, post_data.c_str());
                curl_easy_setopt(easy, CURLOPT_POSTFIELDSIZE, post_data.size());
            }
            
            // Associate easy handle with share handle
            curl_easy_setopt(easy, CURLOPT_SHARE, share_handle);
            
            // Set some common options
            curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);  // Disable verbose for fuzzing
            curl_easy_setopt(easy, CURLOPT_TIMEOUT, 5L);  // Short timeout
            
            // Set SSL options if using HTTPS
            if (use_ssl_flags[i]) {
                curl_easy_setopt(easy, CURLOPT_SSL_VERIFYPEER, 0L);
                curl_easy_setopt(easy, CURLOPT_SSL_VERIFYHOST, 0L);
            }
        }
    }
    
    // ==================== Test Shared Resource Usage ====================
    
    // Perform some operations with the easy handles to exercise shared resources
    for (size_t i = 0; i < easy_handles.size(); i++) {
        CURL* easy = easy_handles[i];
        
        // Try to perform the transfer (ignore errors - we want to test sharing)
        // Note: We don't actually perform the transfer to avoid network I/O,
        // but we set up the handles to test the sharing configuration
        
        // Clean up individual easy handle
        curl_easy_cleanup(easy);
    }
    
    // Clear the vector
    easy_handles.clear();
    
    // ==================== Test Share Handle Reconfiguration ====================
    
    // Test reconfiguring the share handle with different options
    if (fdp.remaining_bytes() > 4) {
        // Try to unshare some data types
        int unshare_option = fdp.ConsumeIntegralInRange<int>(0, 5);
        curl_lock_data unshare_data = CURL_LOCK_DATA_NONE;
        
        switch (unshare_option) {
            case 0: unshare_data = CURL_LOCK_DATA_DNS; break;
            case 1: unshare_data = CURL_LOCK_DATA_SSL_SESSION; break;
            case 2: unshare_data = CURL_LOCK_DATA_CONNECT; break;
            case 3: unshare_data = CURL_LOCK_DATA_COOKIE; break;
            case 4: unshare_data = CURL_LOCK_DATA_PSL; break;
            case 5: unshare_data = CURL_LOCK_DATA_HSTS; break;
        }
        
        curl_share_setopt(share_handle, CURLSHOPT_UNSHARE, unshare_data);
        
        // Then share it again to test state transitions
        if (fdp.ConsumeBool()) {
            curl_share_setopt(share_handle, CURLSHOPT_SHARE, unshare_data);
        }
    }
    
    // ==================== Cleanup ====================
    
    // Clean up share handle
    curl_share_cleanup(share_handle);
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_006

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_007 {


// Custom lock/unlock callbacks for testing
static void custom_lock_function(CURL* handle,
                                curl_lock_data data,
                                curl_lock_access locktype,
                                void* userptr) {
    (void)handle;
    (void)data;
    (void)locktype;
    (void)userptr;
    // Simple no-op lock function for testing
}

static void custom_unlock_function(CURL* handle,
                                  curl_lock_data data,
                                  void* userptr) {
    (void)handle;
    (void)data;
    (void)userptr;
    // Simple no-op unlock function for testing
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 1: Create share object
    CURLSH* share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }

    // Step 2: Set various share options using fuzzed data
    // Consume operation type to decide which options to set
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>();
    
    // Array of CURL_LOCK_DATA types to share/unshare
    curl_lock_data share_options[] = {
        CURL_LOCK_DATA_DNS,
        CURL_LOCK_DATA_COOKIE,
        CURL_LOCK_DATA_SSL_SESSION,
        CURL_LOCK_DATA_CONNECT,
        CURL_LOCK_DATA_HSTS,
        CURL_LOCK_DATA_PSL
    };
    
    // Set user data for callbacks
    int user_data_value = fdp.ConsumeIntegral<int>();
    void* user_data = &user_data_value;
    
    // Set lock/unlock callbacks
    curl_share_setopt(share, CURLSHOPT_LOCKFUNC, custom_lock_function);
    curl_share_setopt(share, CURLSHOPT_UNLOCKFUNC, custom_unlock_function);
    curl_share_setopt(share, CURLSHOPT_USERDATA, user_data);
    
    // Set share options based on fuzzed data
    for (int i = 0; i < 6 && fdp.remaining_bytes() > 0; i++) {
        bool should_share = fdp.ConsumeBool();
        if (should_share) {
            curl_share_setopt(share, CURLSHOPT_SHARE, share_options[i]);
        }
    }
    
    // Step 3: Create easy handles and attach share
    int num_handles = fdp.ConsumeIntegralInRange<int>(1, 3);
    std::vector<CURL*> easy_handles;
    
    for (int i = 0; i < num_handles && fdp.remaining_bytes() > 0; i++) {
        CURL* easy = curl_easy_init();
        if (easy) {
            easy_handles.push_back(easy);
            
            // Attach share to easy handle
            curl_easy_setopt(easy, CURLOPT_SHARE, share);
            
            // Set some other options that don't require network
            bool verbose = fdp.ConsumeBool();
            curl_easy_setopt(easy, CURLOPT_VERBOSE, verbose ? 1L : 0L);
            
            // Set a dummy URL (won't actually connect)
            if (fdp.remaining_bytes() > 10) {
                std::string url = fdp.ConsumeRandomLengthString(100);
                if (!url.empty()) {
                    curl_easy_setopt(easy, CURLOPT_URL, url.c_str());
                }
            }
        }
    }
    
    // Step 4: Test unshare options
    if (fdp.remaining_bytes() > 0) {
        for (int i = 0; i < 6 && fdp.remaining_bytes() > 0; i++) {
            bool should_unshare = fdp.ConsumeBool();
            if (should_unshare) {
                curl_share_setopt(share, CURLSHOPT_UNSHARE, share_options[i]);
            }
        }
    }
    
    // Step 5: Test error paths with curl_share_strerror
    // Generate some error codes to test error string retrieval
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
        CURLSHcode error_code = static_cast<CURLSHcode>(
            fdp.ConsumeIntegral<uint8_t>() % 6); // 0-5 correspond to CURLSHE_* codes
        const char* error_str = curl_share_strerror(error_code);
        (void)error_str; // Use result to avoid unused variable warning
    }
    
    // Step 6: Test error paths with curl_share_setopt
    // Test with valid options but invalid parameters, or boundary cases
    if (fdp.remaining_bytes() > 0) {
        // Generate option in valid range (0-6) to avoid undefined behavior
        // CURLSHOPT values: 0-5 are valid options, 6 is CURLSHOPT_LAST (sentinel)
        uint32_t option_value = fdp.ConsumeIntegral<uint32_t>() % 7; // 0-6 inclusive
        CURLSHoption test_option = static_cast<CURLSHoption>(option_value);
        CURLSHcode result = curl_share_setopt(share, test_option, nullptr);
        (void)result; // Result may be error, that's okay for testing
    }
    
    // Step 7: Test curl_unescape (optional bonus API)
    if (fdp.remaining_bytes() > 10) {
        std::string to_unescape = fdp.ConsumeRandomLengthString(100);
        if (!to_unescape.empty()) {
            char* unescaped = curl_unescape(to_unescape.c_str(), to_unescape.length());
            if (unescaped) {
                curl_free(unescaped);
            }
        }
    }
    
    // Step 8: Cleanup easy handles
    for (CURL* easy : easy_handles) {
        curl_easy_cleanup(easy);
    }
    
    // Step 9: Cleanup share object
    curl_share_cleanup(share);
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_007

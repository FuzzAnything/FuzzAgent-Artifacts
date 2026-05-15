#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_019 {


// Custom lock/unlock callback functions for testing different patterns
static void custom_lock_function_1(CURL* handle,
                                   curl_lock_data data,
                                   curl_lock_access locktype,
                                   void* userptr) {
    (void)handle;
    (void)data;
    (void)locktype;
    (void)userptr;
    // Simple no-op lock function
}

static void custom_unlock_function_1(CURL* handle,
                                     curl_lock_data data,
                                     void* userptr) {
    (void)handle;
    (void)data;
    (void)userptr;
    // Simple no-op unlock function
}

static void custom_lock_function_2(CURL* handle,
                                   curl_lock_data data,
                                   curl_lock_access locktype,
                                   void* userptr) {
    (void)handle;
    (void)data;
    (void)locktype;
    int* counter = static_cast<int*>(userptr);
    if (counter) {
        (*counter)++;
    }
}

static void custom_unlock_function_2(CURL* handle,
                                     curl_lock_data data,
                                     void* userptr) {
    (void)handle;
    (void)data;
    int* counter = static_cast<int*>(userptr);
    if (counter) {
        (*counter)--;
    }
}

// Write callback for simple HTTP operation testing (won't actually write)
static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;  // Pretend we wrote everything
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        return 0; // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: curl_global_init
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        const char* error_str = curl_easy_strerror(global_result);
        (void)error_str;
        return 0;
    }

    // Step 2: curl_share_init
    CURLSH* share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }

    // Step 3: curl_share_setopt multiple times with different options
    // Consume data for configuration decisions
    uint8_t config_flags = fdp.ConsumeIntegral<uint8_t>();
    int lock_counter = 0;
    void* user_data = &lock_counter;
    
    // Set user data for callbacks
    CURLSHcode setopt_result = curl_share_setopt(share, CURLSHOPT_USERDATA, user_data);
    if (setopt_result != CURLSHE_OK) {
        const char* error_str = curl_share_strerror(setopt_result);
        (void)error_str;
    }

    // Choose which lock/unlock functions to use based on fuzzed input
    if (config_flags & 0x01) {
        curl_share_setopt(share, CURLSHOPT_LOCKFUNC, custom_lock_function_1);
        curl_share_setopt(share, CURLSHOPT_UNLOCKFUNC, custom_unlock_function_1);
    } else {
        curl_share_setopt(share, CURLSHOPT_LOCKFUNC, custom_lock_function_2);
        curl_share_setopt(share, CURLSHOPT_UNLOCKFUNC, custom_unlock_function_2);
    }

    // Array of all CURL_LOCK_DATA types for share/unshare operations
    curl_lock_data lock_types[] = {
        CURL_LOCK_DATA_COOKIE,
        CURL_LOCK_DATA_DNS,
        CURL_LOCK_DATA_SSL_SESSION,
        CURL_LOCK_DATA_CONNECT,
        CURL_LOCK_DATA_PSL,
        CURL_LOCK_DATA_HSTS
    };
    const size_t num_lock_types = sizeof(lock_types) / sizeof(lock_types[0]);

    // Set share options based on fuzzed data
    for (size_t i = 0; i < num_lock_types && fdp.remaining_bytes() > 0; i++) {
        bool should_share = fdp.ConsumeBool();
        if (should_share) {
            CURLSHcode share_result = curl_share_setopt(share, CURLSHOPT_SHARE, lock_types[i]);
            if (share_result != CURLSHE_OK) {
                const char* error_str = curl_share_strerror(share_result);
                (void)error_str;
            }
        }
    }

    // Test some unshare operations
    for (size_t i = 0; i < num_lock_types && fdp.remaining_bytes() > 0; i++) {
        bool should_unshare = fdp.ConsumeBool();
        if (should_unshare) {
            CURLSHcode unshare_result = curl_share_setopt(share, CURLSHOPT_UNSHARE, lock_types[i]);
            if (unshare_result != CURLSHE_OK) {
                const char* error_str = curl_share_strerror(unshare_result);
                (void)error_str;
            }
        }
    }

    // Step 4: curl_easy_init
    CURL* easy = curl_easy_init();
    if (!easy) {
        curl_share_cleanup(share);
        curl_global_cleanup();
        return 0;
    }

    // Step 5: Associate easy handle with share object using curl_easy_setopt with CURLOPT_SHARE
    curl_easy_setopt(easy, CURLOPT_SHARE, share);

    // Step 6: Perform simple HTTP operation (without actual network)
    // Set a dummy URL that won't actually connect
    std::string url = "http://localhost/dummy";
    curl_easy_setopt(easy, CURLOPT_URL, url.c_str());
    
    // Set write callback to avoid actual I/O
    curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, write_callback);
    
    // Set very short timeout to prevent hanging
    curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, 10L);
    curl_easy_setopt(easy, CURLOPT_CONNECTTIMEOUT_MS, 10L);
    
    // Disable signal handling
    curl_easy_setopt(easy, CURLOPT_NOSIGNAL, 1L);
    
    // Try to perform the operation (will fail quickly due to localhost/timeout)
    CURLcode perform_result = curl_easy_perform(easy);
    if (perform_result != CURLE_OK) {
        const char* error_str = curl_easy_strerror(perform_result);
        (void)error_str;
    }

    // Test error handling with curl_share_strerror for various error codes
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
        uint8_t error_code_val = fdp.ConsumeIntegral<uint8_t>() % 7; // 0-6 for CURLSHcode range
        CURLSHcode test_error_code = static_cast<CURLSHcode>(error_code_val);
        const char* error_str = curl_share_strerror(test_error_code);
        (void)error_str;
    }

    // Test boundary cases for curl_share_setopt
    if (fdp.remaining_bytes() > 0) {
        // Test with valid option values (0-5 range for CURLSHoption enum)
        uint32_t option_value = fdp.ConsumeIntegral<uint32_t>() % 6; // 0-5
        CURLSHoption test_option = static_cast<CURLSHoption>(option_value);
        CURLSHcode invalid_result = curl_share_setopt(share, test_option, nullptr);
        if (invalid_result != CURLSHE_OK) {
            const char* error_str = curl_share_strerror(invalid_result);
            (void)error_str;
        }
    }

    // Step 7: curl_easy_cleanup
    curl_easy_cleanup(easy);

    // Step 8: curl_share_cleanup (main target with 111 undiscovered branches)
    curl_share_cleanup(share);

    // Step 9: curl_global_cleanup
    curl_global_cleanup();

    // Step 10: Additional error handling test (already done throughout)
    return 0;
}

} // namespace harness_019

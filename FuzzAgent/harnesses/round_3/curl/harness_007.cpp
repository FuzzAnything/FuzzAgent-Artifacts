#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_007 {
/*
 * Fuzzing harness for libcurl's share API operations
 * Targets: curl_share_cleanup (158 undiscovered branches, highest complexity), 
 *          curl_share_setopt, curl_share_init
 * Also exercises: curl_easy_init, curl_easy_setopt with CURLOPT_SHARE, curl_easy_cleanup
 * Focus on share API lifecycle with 0% coverage
 * Following invocation sequence from coverage guidance:
 * curl_share_init() -> curl_share_setopt() with various CURLSHOPT_SHARE options 
 * -> curl_easy_init() -> curl_easy_setopt() with CURLOPT_SHARE option 
 * -> perform operations -> curl_easy_cleanup() -> curl_share_cleanup()
 */


// Write callback for handling response data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Just discard the data for fuzzing purposes
    (void)contents;
    (void)userp;
    return realsize;
}

// Lock callback for share API (required for proper operation)
static void lock_callback(CURL *handle, curl_lock_data data,
                         curl_lock_access access, void *userptr) {
    (void)handle;
    (void)data;
    (void)access;
    (void)userptr;
    // Minimal lock implementation for fuzzing
}

// Unlock callback for share API (required for proper operation)
static void unlock_callback(CURL *handle, curl_lock_data data, void *userptr) {
    (void)handle;
    (void)data;
    (void)userptr;
    // Minimal unlock implementation for fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing - enough for basic parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }
    
    // Step 1: Create share handle - main target API
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }
    
    // Step 2: Set lock/unlock callbacks for the share handle
    curl_share_setopt(share, CURLSHOPT_LOCKFUNC, lock_callback);
    curl_share_setopt(share, CURLSHOPT_UNLOCKFUNC, unlock_callback);
    
    // Step 3: Set user data for callbacks (optional)
    int user_data = fdp.ConsumeIntegral<int>();
    curl_share_setopt(share, CURLSHOPT_USERDATA, &user_data);
    
    // Step 4: Configure which data types to share using CURLSHOPT_SHARE
    // Consume from fuzzed input which data types to enable
    uint8_t share_options = fdp.ConsumeIntegral<uint8_t>();
    
    // Array of possible lock data types to share (from coverage guidance)
    curl_lock_data lock_types[] = {
        CURL_LOCK_DATA_DNS,
        CURL_LOCK_DATA_COOKIE,
        CURL_LOCK_DATA_SSL_SESSION,
        CURL_LOCK_DATA_CONNECT,
        CURL_LOCK_DATA_PSL
    };
    
    const size_t num_lock_types = sizeof(lock_types) / sizeof(lock_types[0]);
    
    // Enable sharing for specific data types based on fuzzed input
    for (size_t i = 0; i < num_lock_types; i++) {
        if (share_options & (1 << (i % 8))) {
            CURLSHcode share_res = curl_share_setopt(share, CURLSHOPT_SHARE, lock_types[i]);
            (void)share_res; // Ignore result for fuzzing
        }
    }
    
    // Step 5: Create easy handles that will use the share
    // Determine number of easy handles from fuzzed input (1-3 handles)
    int num_easy_handles = fdp.ConsumeIntegralInRange<int>(1, 3);
    std::vector<CURL*> easy_handles;
    
    for (int i = 0; i < num_easy_handles && fdp.remaining_bytes() > 10; i++) {
        CURL *easy = curl_easy_init();
        if (!easy) {
            continue;
        }
        
        // Set URL from fuzzed input
        std::string url = fdp.ConsumeRandomLengthString(256);
        if (!url.empty()) {
            curl_easy_setopt(easy, CURLOPT_URL, url.c_str());
        }
        
        // Set write callback
        curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, write_callback);
        
        // Configure the easy handle to use the share - critical step!
        curl_easy_setopt(easy, CURLOPT_SHARE, share);
        
        easy_handles.push_back(easy);
    }
    
    // Step 6: Perform some operations with the easy handles
    // Since we're fuzzing without network, we can't actually perform transfers,
    // but we can still test API calls and error paths
    
    // Try to set some additional options on easy handles based on remaining input
    for (size_t i = 0; i < easy_handles.size() && fdp.remaining_bytes() > 5; i++) {
        // Consume some boolean options from fuzzed input
        if (fdp.ConsumeBool()) {
            curl_easy_setopt(easy_handles[i], CURLOPT_VERBOSE, 0L); // Disable verbose
        }
        if (fdp.ConsumeBool()) {
            curl_easy_setopt(easy_handles[i], CURLOPT_NOPROGRESS, 1L); // Disable progress
        }
    }
    
    // Step 7: Test unsharing some data types (optional)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        // Unshare a random data type
        uint8_t unshare_index = fdp.ConsumeIntegral<uint8_t>() % num_lock_types;
        curl_share_setopt(share, CURLSHOPT_UNSHARE, lock_types[unshare_index]);
    }
    
    // Step 8: Clean up easy handles in reverse order
    for (auto it = easy_handles.rbegin(); it != easy_handles.rend(); ++it) {
        curl_easy_cleanup(*it);
    }
    
    // Step 9: Main target - curl_share_cleanup (158 undiscovered branches)
    // This is the primary coverage target
    curl_share_cleanup(share);
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_007

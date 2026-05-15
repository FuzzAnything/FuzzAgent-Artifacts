#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_005 {
/*
 * Fuzzing harness for libcurl's Share API operations
 * Targets: curl_share_init (0% coverage, 4 branches, 173 undiscovered), 
 *          curl_share_setopt (0% coverage, high complexity),
 *          curl_share_cleanup
 * Helpers: curl_easy_init, curl_easy_setopt, curl_easy_perform, curl_easy_cleanup
 * Share options to test: CURLSHOPT_SHARE with CURL_LOCK_DATA_DNS, CURL_LOCK_DATA_COOKIE, 
 *          CURL_LOCK_DATA_SSL_SESSION, CURL_LOCK_DATA_CONNECT, CURL_LOCK_DATA_HSTS, 
 *          CURL_LOCK_DATA_PSL, and CURLSHOPT_UNSHARE
 * Semantic differentiation from existing harnesses:
 *   - harness_000: Basic HTTP operations
 *   - harness_001: Header manipulation  
 *   - harness_002: MIME (multipart/form-data) operations
 *   - harness_003: Deprecated form data API
 *   - harness_004: WebSocket operations
 *   - harness_005: Share API for resource sharing between multiple easy handles
 * Focus: Tests resource sharing (DNS cache, cookies, SSL sessions, connections) between 
 *        multiple easy handles using the Share API
 */


// Simple write callback that discards data
static size_t discard_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;  // Always report success
}

// Header callback that discards headers
static size_t discard_header_cb(char *buffer, size_t size, size_t nitems, void *userdata) {
    (void)buffer;
    (void)userdata;
    return size * nitems;  // Always report success
}

// Lock callback for shared data (minimal implementation)
static void lock_callback(CURL *handle, curl_lock_data data, curl_lock_access access, void *userptr) {
    (void)handle;
    (void)data;
    (void)access;
    (void)userptr;
    // Minimal lock implementation for fuzzing
}

// Unlock callback for shared data (minimal implementation)
static void unlock_callback(CURL *handle, curl_lock_data data, void *userptr) {
    (void)handle;
    (void)data;
    (void)userptr;
    // Minimal unlock implementation for fuzzing
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful Share API fuzzing
    // Need enough for configuration, URLs, and some data for handles
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration data first
    uint8_t num_handles = fdp.ConsumeIntegralInRange<uint8_t>(2, 3);
    uint8_t share_options_mask = fdp.ConsumeIntegral<uint8_t>();
    uint8_t unshare_options_mask = fdp.ConsumeIntegral<uint8_t>();
    uint8_t url_length = fdp.ConsumeIntegralInRange<uint8_t>(1, 128);
    bool set_lock_callbacks = fdp.ConsumeBool();
    bool use_user_data = fdp.ConsumeBool();
    
    // Array of shareable data types
    curl_lock_data shareable_data_types[] = {
        CURL_LOCK_DATA_DNS,
        CURL_LOCK_DATA_COOKIE,
        CURL_LOCK_DATA_SSL_SESSION,
        CURL_LOCK_DATA_CONNECT,
        CURL_LOCK_DATA_HSTS,
        CURL_LOCK_DATA_PSL
    };
    const size_t num_shareable_types = sizeof(shareable_data_types) / sizeof(shareable_data_types[0]);
    
    // Consume URL string
    std::string url = fdp.ConsumeBytesAsString(url_length);
    
    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 1. Create share handle
    CURLSH *share = curl_share_init();
    if (!share) {
        curl_global_cleanup();
        return 0;
    }
    
    // 2. Set lock/unlock callbacks if requested
    if (set_lock_callbacks) {
        curl_share_setopt(share, CURLSHOPT_LOCKFUNC, lock_callback);
        curl_share_setopt(share, CURLSHOPT_UNLOCKFUNC, unlock_callback);
        
        if (use_user_data) {
            // Create dummy user data pointer
            int user_data = 42;
            curl_share_setopt(share, CURLSHOPT_USERDATA, &user_data);
        }
    }
    
    // 3. Set share options based on mask
    for (size_t i = 0; i < num_shareable_types; i++) {
        if (share_options_mask & (1 << (i % 8))) {
            curl_share_setopt(share, CURLSHOPT_SHARE, shareable_data_types[i]);
        }
    }
    
    // 4. Set unshare options based on mask (test both share and unshare)
    for (size_t i = 0; i < num_shareable_types; i++) {
        if (unshare_options_mask & (1 << (i % 8))) {
            curl_share_setopt(share, CURLSHOPT_UNSHARE, shareable_data_types[i]);
        }
    }
    
    // Create easy handles
    CURL *handles[3] = {nullptr, nullptr, nullptr};
    CURLcode results[3] = {CURLE_OK, CURLE_OK, CURLE_OK};
    
    // 5. Create and configure multiple easy handles
    for (uint8_t i = 0; i < num_handles; i++) {
        handles[i] = curl_easy_init();
        if (!handles[i]) {
            continue;
        }
        
        // Set URL (use the same or similar URL for all handles)
        std::string handle_url = url;
        if (handle_url.empty()) {
            handle_url = "http://example.com/test" + std::to_string(i);
        }
        curl_easy_setopt(handles[i], CURLOPT_URL, handle_url.c_str());
        
        // Set write callback to discard response
        curl_easy_setopt(handles[i], CURLOPT_WRITEFUNCTION, discard_write_cb);
        curl_easy_setopt(handles[i], CURLOPT_WRITEDATA, nullptr);
        
        // Set header callback to discard headers
        curl_easy_setopt(handles[i], CURLOPT_HEADERFUNCTION, discard_header_cb);
        curl_easy_setopt(handles[i], CURLOPT_HEADERDATA, nullptr);
        
        // Set timeout to prevent hanging
        curl_easy_setopt(handles[i], CURLOPT_TIMEOUT, 5L);
        
        // Use the share handle
        curl_easy_setopt(handles[i], CURLOPT_SHARE, share);
        
        // Set some additional options based on fuzzer input
        uint8_t handle_options = fdp.ConsumeIntegral<uint8_t>();
        if (handle_options & 0x01) {
            curl_easy_setopt(handles[i], CURLOPT_FOLLOWLOCATION, 1L);
        }
        if (handle_options & 0x02) {
            curl_easy_setopt(handles[i], CURLOPT_SSL_VERIFYPEER, 0L);
        }
        if (handle_options & 0x04) {
            curl_easy_setopt(handles[i], CURLOPT_SSL_VERIFYHOST, 0L);
        }
    }
    
    // 6. Perform operations with each handle
    for (uint8_t i = 0; i < num_handles; i++) {
        if (handles[i]) {
            // Perform the request (ignore result, we're fuzzing API usage)
            results[i] = curl_easy_perform(handles[i]);
        }
    }
    
    // 7. Clean up easy handles
    for (uint8_t i = 0; i < num_handles; i++) {
        if (handles[i]) {
            curl_easy_cleanup(handles[i]);
        }
    }
    
    // 8. Clean up share handle
    curl_share_cleanup(share);
    
    // 9. Clean up global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_005

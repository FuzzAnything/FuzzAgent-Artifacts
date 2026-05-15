#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_006 {
/*
 * Fuzzing harness for libcurl's multi interface operations
 * Targets: curl_multi_get_offt (completely uncovered, 26 branches)
 * Required helpers: curl_multi_init (setup), curl_multi_cleanup (cleanup), 
 *                   curl_easy_init (create easy handle for multi operations), 
 *                   curl_easy_cleanup
 * Potential helpers: curl_multi_add_handle, curl_multi_remove_handle
 * Multi info enums to test: CURLMINFO_XFERS_CURRENT, CURLMINFO_XFERS_RUNNING, 
 *                            CURLMINFO_XFERS_PENDING, CURLMINFO_XFERS_DONE, 
 *                            CURLMINFO_XFERS_ADDED
 * Invocation sequence:
 * 1. curl_multi_init to create multi handle
 * 2. curl_easy_init to create easy handle
 * 3. curl_multi_add_handle to add easy handle to multi (if needed for context)
 * 4. curl_multi_get_offt to retrieve offset information (primary target)
 * 5. curl_multi_remove_handle to remove easy handle
 * 6. curl_easy_cleanup to clean up easy handle
 * 7. curl_multi_cleanup to clean up multi handle
 * Semantic differentiation from existing harnesses:
 *   - harness_000: Basic HTTP operations with easy API
 *   - harness_001: Header manipulation
 *   - harness_002: MIME (multipart/form-data) operations
 *   - harness_003: Deprecated form data API
 *   - harness_004: WebSocket operations
 *   - harness_005: Share API for resource sharing between multiple easy handles
 *   - harness_006: Multi interface for handling multiple transfers (core libcurl component)
 * Focus: Tests curl_multi_get_offt API which retrieves transfer statistics and 
 *        handles multiple concurrent transfers via multi interface
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

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful multi interface fuzzing
    // Need enough for configuration, URLs, and some data for easy handles
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration data first
    uint8_t num_easy_handles = fdp.ConsumeIntegralInRange<uint8_t>(1, 3);
    uint8_t info_type_selector = fdp.ConsumeIntegral<uint8_t>();
    bool add_handles = fdp.ConsumeBool();
    bool perform_quick = fdp.ConsumeBool();
    uint8_t url_length = fdp.ConsumeIntegralInRange<uint8_t>(1, 128);
    
    // Array of CURLMinfo_offt enums to test with curl_multi_get_offt
    CURLMinfo_offt info_types[] = {
        CURLMINFO_XFERS_CURRENT,
        CURLMINFO_XFERS_RUNNING,
        CURLMINFO_XFERS_PENDING,
        CURLMINFO_XFERS_DONE,
        CURLMINFO_XFERS_ADDED
    };
    const size_t num_info_types = sizeof(info_types) / sizeof(info_types[0]);
    
    // Consume URL string
    std::string url = fdp.ConsumeBytesAsString(url_length);
    
    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 1. Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Vector to store easy handles
    std::vector<CURL*> easy_handles;
    
    // 2. Create and configure easy handles
    for (uint8_t i = 0; i < num_easy_handles; i++) {
        CURL *easy_handle = curl_easy_init();
        if (!easy_handle) {
            continue;  // Skip if failed to create
        }
        
        // Configure easy handle with basic options
        curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, discard_write_cb);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, nullptr);
        curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, discard_header_cb);
        curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, nullptr);
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 2L);  // Short timeout for fuzzing
        
        // Add some optional configurations based on fuzzer input
        if (fdp.ConsumeBool()) {
            curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
        }
        if (fdp.ConsumeBool()) {
            curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0L);
        }
        
        easy_handles.push_back(easy_handle);
        
        // 3. Add easy handle to multi handle if requested
        if (add_handles) {
            curl_multi_add_handle(multi_handle, easy_handle);
        }
    }
    
    // If we added handles and want to do a quick perform
    if (add_handles && perform_quick && !easy_handles.empty()) {
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
    }
    
    // 4. PRIMARY TARGET: Test curl_multi_get_offt with various info types
    curl_off_t offset_value = 0;
    CURLMcode get_offt_result;
    
    // Test multiple info types based on selector
    for (size_t i = 0; i < num_info_types; i++) {
        if (info_type_selector & (1 << (i % 8))) {
            get_offt_result = curl_multi_get_offt(multi_handle, info_types[i], &offset_value);
            // We don't check the result as we're fuzzing the API call itself
        }
    }
    
    // Also test with a random info value from remaining input
    if (fdp.remaining_bytes() > 0) {
        uint8_t random_info = fdp.ConsumeIntegral<uint8_t>();
        CURLMinfo_offt random_info_enum = static_cast<CURLMinfo_offt>(random_info % (CURLMINFO_LASTENTRY + 1));
        curl_multi_get_offt(multi_handle, random_info_enum, &offset_value);
    }
    
    // 5. Remove handles if they were added
    if (add_handles) {
        for (CURL *easy_handle : easy_handles) {
            curl_multi_remove_handle(multi_handle, easy_handle);
        }
    }
    
    // 6. Clean up easy handles
    for (CURL *easy_handle : easy_handles) {
        curl_easy_cleanup(easy_handle);
    }
    
    // 7. Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Clean up global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_006

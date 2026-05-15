#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_014 {
/*
 * Fuzzing harness for libcurl targeting multi-handle operations with curl_multi_get_offt
 * Harness ID: 014
 * Targets: curl_multi_get_offt API specifically (32 undiscovered branches)
 * Focus: Multi-handle lifecycle sequence targeting all CURLMinfo_offt enum values
 * Differentiated from:
 * - harness_002.cpp (basic curl_multi APIs without curl_multi_get_offt)
 * This harness specifically targets curl_multi_get_offt coverage gap
 * Implements exact invocation sequence: curl_multi_init() -> curl_multi_add_handle() -> curl_multi_perform() -> curl_multi_get_offt() -> curl_multi_remove_handle() -> curl_multi_cleanup()
 * Tests all CURLMinfo_offt values: CURLMINFO_XFERS_CURRENT, CURLMINFO_XFERS_RUNNING, 
 * CURLMINFO_XFERS_PENDING, CURLMINFO_XFERS_DONE, CURLMINFO_XFERS_ADDED
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for multi handle operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // ==================== Multi Handle Creation ====================
    
    // Step 1: Call curl_multi_init() to create a multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // ==================== Easy Handles Creation and Configuration ====================
    
    // Determine number of easy handles to create (1-5 to avoid excessive resource usage)
    size_t num_easy_handles = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    
    std::vector<CURL*> easy_handles;
    easy_handles.reserve(num_easy_handles);
    
    // Create easy handles
    for (size_t i = 0; i < num_easy_handles && fdp.remaining_bytes() > 16; i++) {
        CURL* easy = curl_easy_init();
        if (easy) {
            easy_handles.push_back(easy);
            
            // Consume URL for this easy handle
            std::string url = fdp.ConsumeRandomLengthString(256);
            if (!url.empty()) {
                curl_easy_setopt(easy, CURLOPT_URL, url.c_str());
            } else {
                // Use a dummy URL from fuzzer input instead of hardcoded value
                std::string dummy_url = "dummy://" + fdp.ConsumeRandomLengthString(32);
                curl_easy_setopt(easy, CURLOPT_URL, dummy_url.c_str());
            }
            
            // Consume and set various options based on fuzzed data
            bool verbose = fdp.ConsumeBool();
            bool follow_location = fdp.ConsumeBool();
            int timeout = fdp.ConsumeIntegralInRange<int>(0, 30);
            
            curl_easy_setopt(easy, CURLOPT_VERBOSE, verbose ? 1L : 0L);
            curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
            curl_easy_setopt(easy, CURLOPT_TIMEOUT, (long)timeout);
            curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L); // Disable progress meter
            curl_easy_setopt(easy, CURLOPT_NOBODY, fdp.ConsumeBool() ? 1L : 0L); // HEAD request
        }
    }
    
    // If no easy handles were created, cleanup and return
    if (easy_handles.empty()) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // ==================== Multi Handle Operations Sequence ====================
    
    CURLMcode mcode;
    
    // Step 2: Add easy handles to multi handle
    for (CURL* easy : easy_handles) {
        mcode = curl_multi_add_handle(multi_handle, easy);
        // We don't check return code to explore error paths
    }
    
    // ==================== curl_multi_get_offt Testing - PHASE 1 (after add) ====================
    
    // Test curl_multi_get_offt with all CURLMinfo_offt values after adding handles
    curl_off_t off_value;
    
    // Test CURLMINFO_XFERS_CURRENT with error checking
    mcode = curl_multi_get_offt(multi_handle, CURLMINFO_XFERS_CURRENT, &off_value);
    // Check return code - but don't fail, just continue
    
    // Test CURLMINFO_XFERS_RUNNING
    mcode = curl_multi_get_offt(multi_handle, CURLMINFO_XFERS_RUNNING, &off_value);
    
    // Test CURLMINFO_XFERS_PENDING
    mcode = curl_multi_get_offt(multi_handle, CURLMINFO_XFERS_PENDING, &off_value);
    
    // Test CURLMINFO_XFERS_DONE
    mcode = curl_multi_get_offt(multi_handle, CURLMINFO_XFERS_DONE, &off_value);
    
    // Test CURLMINFO_XFERS_ADDED
    mcode = curl_multi_get_offt(multi_handle, CURLMINFO_XFERS_ADDED, &off_value);
    int still_running = 0;
    mcode = curl_multi_perform(multi_handle, &still_running);
    
    // ==================== curl_multi_get_offt Testing - PHASE 2 (after perform) ====================
    
    // Test curl_multi_get_offt again after perform to see if values changed
    curl_off_t off_value2;
    
    // Test all info types again (only valid enum values 1-5)
    for (int info_val = 1; info_val <= 5; info_val++) {
        CURLMinfo_offt info = static_cast<CURLMinfo_offt>(info_val);
        mcode = curl_multi_get_offt(multi_handle, info, &off_value2);
        
        // Test with NULL pointer (should return CURLM_BAD_FUNCTION_ARGUMENT)
        if (fdp.ConsumeBool()) {
            mcode = curl_multi_get_offt(multi_handle, info, NULL);
        }
    }
    
    // ==================== Remove Some Handles and Test Again ====================
    
    // Remove some handles (but not all) to test state transitions
    size_t handles_to_remove = fdp.ConsumeIntegralInRange<size_t>(0, easy_handles.size());
    for (size_t i = 0; i < handles_to_remove && i < easy_handles.size(); i++) {
        mcode = curl_multi_remove_handle(multi_handle, easy_handles[i]);
        
        // Test curl_multi_get_offt after removal
        if (fdp.ConsumeBool()) {
            CURLMinfo_offt random_info = static_cast<CURLMinfo_offt>(
                fdp.ConsumeIntegralInRange<int>(1, 5));
            mcode = curl_multi_get_offt(multi_handle, random_info, &off_value);
        }
    }
    
    // ==================== curl_multi_get_offt Testing - PHASE 3 (mixed state) ====================
    
    // Test edge cases and mixed states
    for (int i = 0; i < 3 && fdp.remaining_bytes() > 0; i++) {
        // Randomly choose an info type to test (only valid values 1-5)
        int info_choice = fdp.ConsumeIntegralInRange<int>(1, 5);
        CURLMinfo_offt info = static_cast<CURLMinfo_offt>(info_choice);
        
        // Test with valid pointer
        mcode = curl_multi_get_offt(multi_handle, info, &off_value);
        
        // Test with NULL multi handle (should return CURLM_BAD_HANDLE)
        if (fdp.ConsumeBool()) {
            mcode = curl_multi_get_offt(NULL, info, &off_value);
        }
    }
    
    // ==================== Cleanup Sequence ====================
    
    // Step 4: Remove remaining handles
    for (CURL* easy : easy_handles) {
        // Check if handle is still in multi (not previously removed)
        int dummy;
        curl_multi_perform(multi_handle, &dummy);
        mcode = curl_multi_remove_handle(multi_handle, easy);
    }
    
    // Test curl_multi_get_offt after all handles removed
    if (fdp.ConsumeBool()) {
        mcode = curl_multi_get_offt(multi_handle, CURLMINFO_XFERS_CURRENT, &off_value);
        mcode = curl_multi_get_offt(multi_handle, CURLMINFO_XFERS_ADDED, &off_value);
    }
    
    // Step 5: Call curl_multi_cleanup
    mcode = curl_multi_cleanup(multi_handle);
    
    // Clean up easy handles
    for (CURL* easy : easy_handles) {
        curl_easy_cleanup(easy);
    }
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_014

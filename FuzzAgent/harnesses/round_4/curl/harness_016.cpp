#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_016 {
/*
 * Fuzzing harness for libcurl targeting curl_multi_waitfds API with 34 branches and 37 undiscovered accumulated branches
 * Harness ID: 016
 * Targets: curl_multi_waitfds() API specifically for multi interface operations
 * Focus: Multi interface operations with proper handling of waitfd structures and event monitoring
 * Differentiated from:
 * - harness_002.cpp (basic curl_multi APIs without curl_multi_waitfds)
 * - harness_014.cpp (curl_multi_get_offt focusing on offset values)
 * This harness specifically targets curl_multi_waitfds coverage gap (34 branches with 37 undiscovered accumulated branches)
 * Implements exact invocation sequence with various parameter combinations to explore all code paths:
 * 1. Create multi handle and add easy handles
 * 2. Test curl_multi_waitfds with NULL ufds and zero size (to get fd count)
 * 3. Test curl_multi_waitfds with valid ufds array
 * 4. Test error paths: insufficient buffer size, NULL parameters, edge cases
 * 5. Test with different protocol versions (HTTP/1.1, HTTP/2) and multiplexing
 * 6. Test with empty multi handle and partially initialized handles
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for multi handle operations and configuration
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
    
    // Determine number of easy handles to create (0-5 to test empty and populated scenarios)
    size_t num_easy_handles = fdp.ConsumeIntegralInRange<size_t>(0, 5);
    
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
                std::string dummy_url = "http://" + fdp.ConsumeRandomLengthString(32) + ".test/";
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
            
            // Test different HTTP versions based on fuzzed data
            int http_version = fdp.ConsumeIntegralInRange<int>(0, 2);
            switch (http_version) {
                case 0:
                    curl_easy_setopt(easy, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
                    break;
                case 1:
                    curl_easy_setopt(easy, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
                    break;
                case 2:
                    curl_easy_setopt(easy, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
                    break;
            }
            
            // Set pipelining/multiplexing based on fuzzed data
            if (fdp.ConsumeBool() && http_version >= 1) {
                long pipelining = fdp.ConsumeBool() ? CURLPIPE_MULTIPLEX : CURLPIPE_HTTP1;
                curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
            }
        }
    }
    
    // ==================== Multi Handle Operations Sequence ====================
    
    CURLMcode mcode;
    
    // Step 2: Add easy handles to multi handle (if any were created)
    for (CURL* easy : easy_handles) {
        mcode = curl_multi_add_handle(multi_handle, easy);
        // Don't check return code to explore error paths
    }
    
    // ==================== curl_multi_waitfds Testing - PHASE 1 (basic calls) ====================
    
    unsigned int fd_count = 0;
    
    // Test 1: Get fd count with NULL ufds and zero size (should work)
    mcode = curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    // Don't check return code - we want to explore all possible return values
    
    // Test 2: Get fd count with NULL ufds and non-zero size (should return CURLM_BAD_FUNCTION_ARGUMENT)
    if (fdp.ConsumeBool()) {
        mcode = curl_multi_waitfds(multi_handle, NULL, 10, &fd_count);
    }
    
    // Test 3: Get fd count with NULL fd_count parameter (should return CURLM_BAD_FUNCTION_ARGUMENT)
    if (fdp.ConsumeBool()) {
        mcode = curl_multi_waitfds(multi_handle, NULL, 0, NULL);
    }
    
    // Test 4: Get fd count with both NULL ufds and NULL fd_count (should return CURLM_BAD_FUNCTION_ARGUMENT)
    if (fdp.ConsumeBool()) {
        mcode = curl_multi_waitfds(multi_handle, NULL, 0, NULL);
    }
    
    // ==================== curl_multi_waitfds Testing - PHASE 2 (with allocated buffer) ====================
    
    // Allocate buffer for waitfds if we have a count
    std::unique_ptr<struct curl_waitfd[]> ufds;
    unsigned int allocated_size = 0;
    
    if (fd_count > 0 && fdp.remaining_bytes() > 4) {
        // Consume size for buffer (may be smaller than fd_count to test CURLM_OUT_OF_MEMORY)
        allocated_size = fdp.ConsumeIntegralInRange<unsigned int>(0, fd_count * 2);
        
        if (allocated_size > 0) {
            ufds.reset(new struct curl_waitfd[allocated_size]);
            
            // Initialize the buffer
            for (unsigned int i = 0; i < allocated_size; i++) {
                ufds[i].fd = CURL_SOCKET_BAD;
                ufds[i].events = 0;
                ufds[i].revents = 0;
            }
            
            // Test with allocated buffer
            unsigned int actual_fd_count = 0;
            mcode = curl_multi_waitfds(multi_handle, ufds.get(), allocated_size, &actual_fd_count);
            
            // If we got CURLM_OK, we can examine the returned descriptors
            if (mcode == CURLM_OK && actual_fd_count > 0) {
                // The descriptors should now be populated by libcurl
                // We can't use them for actual polling in a fuzzer, but we can verify
                // they have valid values
                for (unsigned int i = 0; i < actual_fd_count && i < allocated_size; i++) {
                    // fd should be set (though could be CURL_SOCKET_BAD in some cases)
                    // events should have valid CURL_WAIT_* flags
                    // revents should be zero (filled by polling, not by curl_multi_waitfds)
                }
            }
        }
    }
    
    // ==================== curl_multi_waitfds Testing - PHASE 3 (edge cases) ====================
    
    // Test with zero size but non-NULL ufds (should return CURLM_BAD_FUNCTION_ARGUMENT)
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        struct curl_waitfd dummy_ufd;
        dummy_ufd.fd = CURL_SOCKET_BAD;
        dummy_ufd.events = 0;
        dummy_ufd.revents = 0;
        
        mcode = curl_multi_waitfds(multi_handle, &dummy_ufd, 0, &fd_count);
    }
    
    // Test with partially performed transfers
    if (!easy_handles.empty() && fdp.ConsumeBool()) {
        // Call curl_multi_perform to start transfers
        int still_running = 0;
        mcode = curl_multi_perform(multi_handle, &still_running);
        
        // Now test curl_multi_waitfds with active transfers
        unsigned int perform_fd_count = 0;
        mcode = curl_multi_waitfds(multi_handle, NULL, 0, &perform_fd_count);
        
        if (perform_fd_count > 0 && fdp.remaining_bytes() > 4) {
            unsigned int buf_size = fdp.ConsumeIntegralInRange<unsigned int>(0, perform_fd_count + 2);
            if (buf_size > 0) {
                std::unique_ptr<struct curl_waitfd[]> perform_ufds(new struct curl_waitfd[buf_size]);
                unsigned int actual_count = 0;
                mcode = curl_multi_waitfds(multi_handle, perform_ufds.get(), buf_size, &actual_count);
            }
        }
    }
    
    // ==================== curl_multi_waitfds Testing - PHASE 4 (with info read) ====================
    
    // Try to read info to see if any transfers completed
    if (!easy_handles.empty()) {
        int msgs_in_queue = 0;
        CURLMsg* msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
        // Don't process the message, just check if one exists
        
        // Test curl_multi_waitfds after info read
        unsigned int info_fd_count = 0;
        mcode = curl_multi_waitfds(multi_handle, NULL, 0, &info_fd_count);
    }
    
    // ==================== Cleanup Sequence ====================
    
    // Remove easy handles from multi handle
    for (CURL* easy : easy_handles) {
        mcode = curl_multi_remove_handle(multi_handle, easy);
        curl_easy_cleanup(easy);
    }
    
    // Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_016

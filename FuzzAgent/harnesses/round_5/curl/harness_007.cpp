#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_007 {
/*
 * Fuzzing harness for libcurl's multi interface waitfd operations
 * Primary target: curl_multi_waitfds (28 undiscovered branches, 0% coverage)
 * Required helpers: curl_multi_init, curl_multi_add_handle, curl_multi_perform, 
 *                   curl_multi_remove_handle, curl_multi_cleanup, curl_easy_init, 
 *                   curl_easy_setopt, curl_easy_cleanup
 * Invocation sequence:
 * 1. curl_easy_init to create easy handle
 * 2. curl_easy_setopt for configuration
 * 3. curl_multi_init to create multi handle  
 * 4. curl_multi_add_handle to add easy handle to multi
 * 5. curl_multi_perform to start transfers
 * 6. curl_multi_waitfds to get file descriptors for polling (primary target)
 * 7. curl_multi_remove_handle to remove easy handle
 * 8. curl_easy_cleanup to clean up easy handle
 * 9. curl_multi_cleanup to clean up multi handle
 * Semantic differentiation from existing harnesses:
 *   - harness_000: Basic HTTP operations with easy API
 *   - harness_001: Header manipulation
 *   - harness_002: MIME (multipart/form-data) operations  
 *   - harness_003: Deprecated form data API
 *   - harness_004: WebSocket operations
 *   - harness_005: Share API for resource sharing between multiple easy handles
 *   - harness_006: Multi interface get_offt operations (transfer statistics)
 *   - harness_007: Multi interface waitfd operations (asynchronous file descriptor waiting)
 * Focus: Tests curl_multi_waitfds API which retrieves file descriptors for polling,
 *        enabling asynchronous I/O operations with the multi interface
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
    // Need enough for configuration, URLs, and waitfd parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration data first
    uint8_t num_easy_handles = fdp.ConsumeIntegralInRange<uint8_t>(1, 3);
    uint8_t waitfd_array_size = fdp.ConsumeIntegralInRange<uint8_t>(0, 10);
    bool perform_transfer = fdp.ConsumeBool();
    uint8_t url_length = fdp.ConsumeIntegralInRange<uint8_t>(1, 128);
    
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
        
        // Configure the easy handle
        curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, discard_write_cb);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, nullptr);
        curl_easy_setopt(easy_handle, CURLOPT_HEADERFUNCTION, discard_header_cb);
        curl_easy_setopt(easy_handle, CURLOPT_HEADERDATA, nullptr);
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 5L);
        curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 5L);
        
        // Add some random options based on remaining input
        if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
            curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
        }
        if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
            curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0L);
        }
        
        easy_handles.push_back(easy_handle);
    }
    
    // 3. Add easy handles to multi handle
    for (CURL* easy_handle : easy_handles) {
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    // 4. Perform transfers if requested
    if (perform_transfer && !easy_handles.empty()) {
        int running_handles = 0;
        curl_multi_perform(multi_handle, &running_handles);
        
        // Allow some time for transfers to start
        // In real usage, this would be in a loop with curl_multi_wait/poll
    }
    
    // 5. PRIMARY TARGET: Test curl_multi_waitfds API
    // This is the main focus - testing waitfd operations
    
    // Allocate waitfd array based on consumed size
    std::vector<struct curl_waitfd> waitfds;
    
    if (waitfd_array_size > 0 && fdp.remaining_bytes() >= waitfd_array_size * sizeof(uint32_t)) {
        waitfds.resize(waitfd_array_size);
        
        // Initialize waitfd structures with fuzzed data
        for (size_t i = 0; i < waitfd_array_size; i++) {
            // Consume socket fd value (simulated - in real usage these would be actual file descriptors)
            // Using int for simulation since we can't create real sockets in fuzzing context
            uint32_t simulated_fd = fdp.ConsumeIntegral<uint32_t>();
            waitfds[i].fd = (curl_socket_t)simulated_fd;
            
            // Consume events flags
            uint16_t events = fdp.ConsumeIntegral<uint16_t>();
            // Mask to valid event flags
            events &= (CURL_WAIT_POLLIN | CURL_WAIT_POLLPRI | CURL_WAIT_POLLOUT);
            waitfds[i].events = (short)events;
            
            // revents will be filled by curl_multi_waitfds
            waitfds[i].revents = 0;
        }
        
        // Call curl_multi_waitfds with the allocated array
        unsigned int fd_count = 0;
        CURLMcode waitfd_result = curl_multi_waitfds(
            multi_handle, 
            waitfds.data(), 
            waitfd_array_size, 
            &fd_count
        );
        
        // The result is ignored for fuzzing purposes - we're testing the API call itself
        (void)waitfd_result;
        (void)fd_count;
    } else {
        // Test with NULL pointer to get count of fds
        unsigned int fd_count = 0;
        CURLMcode waitfd_result = curl_multi_waitfds(
            multi_handle, 
            nullptr, 
            0, 
            &fd_count
        );
        (void)waitfd_result;
        (void)fd_count;
    }
    
    // 6. Alternative: Also test curl_multi_wait API which is related
    if (fdp.remaining_bytes() > sizeof(int)) {
        int timeout_ms = fdp.ConsumeIntegralInRange<int>(-1, 1000);
        int numfds = 0;
        CURLMcode wait_result = curl_multi_wait(multi_handle, nullptr, 0, timeout_ms, &numfds);
        (void)wait_result;
        (void)numfds;
    }
    
    // 7. Clean up: Remove easy handles from multi
    for (CURL* easy_handle : easy_handles) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    // 8. Clean up easy handles
    for (CURL* easy_handle : easy_handles) {
        curl_easy_cleanup(easy_handle);
    }
    
    // 9. Clean up multi handle
    curl_multi_cleanup(multi_handle);
    
    // Clean up global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_007

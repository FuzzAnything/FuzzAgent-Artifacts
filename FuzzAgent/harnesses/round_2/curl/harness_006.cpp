#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_006 {

// Notification callback for curl_multi_notify_enable
static void notify_callback(CURLM *multi,
                            unsigned int notification,
                            CURL *easy,
                            void *user_data) {
    // Simple callback that does nothing, just to exercise the callback path
    (void)multi;
    (void)notification;
    (void)easy;
    (void)user_data;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for basic operations
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize curl library - must be called once
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // Step 1: Create multi handle with curl_multi_init
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Step 2: Test curl_multi_get_handles in empty state
    // This should return an array with first entry as NULL pointer
    CURL **handles_empty = curl_multi_get_handles(multi_handle);
    if (handles_empty != NULL) {
        // According to documentation, if there is no easy handle added,
        // this function returns an array with the first entry as a NULL pointer
        // We should check this
        if (handles_empty[0] != NULL) {
            // Unexpected, but continue
        }
        // Free the allocated array
        curl_free(handles_empty);
    }
    
    // Create an easy handle to test with non-empty state
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Set a simple option on easy handle
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        
        // Add easy handle to multi handle
        CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
        
        // Step 2b: Test curl_multi_get_handles with an easy handle added
        if (add_result == CURLM_OK) {
            CURL **handles_with_easy = curl_multi_get_handles(multi_handle);
            if (handles_with_easy != NULL) {
                // Iterate through handles until NULL terminator
                for (CURL **h = handles_with_easy; *h != NULL; h++) {
                    // Just reference to avoid unused variable warning
                    (void)h;
                }
                curl_free(handles_with_easy);
            }
        }
    }
    
    // Step 3: Test curl_multi_strerror with various error codes
    // Consume multiple error codes from fuzzed data
    for (int i = 0; i < 5 && fdp.remaining_bytes() > 0; i++) {
        // Consume an error code - use modulo to get valid enum range
        CURLMcode error_code = static_cast<CURLMcode>(
            fdp.ConsumeIntegralInRange<int>(0, CURLM_LAST - 1));
        
        const char *error_str = curl_multi_strerror(error_code);
        (void)error_str; // Reference to avoid unused variable warning
        
        // Also test with some specific known error codes
        if (fdp.ConsumeBool()) {
            const char *ok_str = curl_multi_strerror(CURLM_OK);
            const char *bad_handle_str = curl_multi_strerror(CURLM_BAD_HANDLE);
            const char *out_of_memory_str = curl_multi_strerror(CURLM_OUT_OF_MEMORY);
            const char *internal_error_str = curl_multi_strerror(CURLM_INTERNAL_ERROR);
            (void)ok_str;
            (void)bad_handle_str;
            (void)out_of_memory_str;
            (void)internal_error_str;
        }
    }
    
    // Step 4: Test curl_multi_waitfds
    // First get the count of file descriptors needed
    unsigned int fd_count = 0;
    CURLMcode waitfds_count_result = curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    
    if (waitfds_count_result == CURLM_OK && fd_count > 0 && fdp.remaining_bytes() > 0) {
        // Allocate array for file descriptors
        // Use a reasonable max size to avoid excessive memory usage
        unsigned int alloc_size = fd_count;
        if (alloc_size > 100) {
            alloc_size = 100;
        }
        
        std::vector<struct curl_waitfd> waitfds(alloc_size);
        
        // Get the actual file descriptors
        unsigned int actual_fd_count = fd_count;
        CURLMcode waitfds_result = curl_multi_waitfds(multi_handle, 
                                                     waitfds.data(), 
                                                     alloc_size, 
                                                     &actual_fd_count);
        
        // Test with the waitfds if we got them
        if (waitfds_result == CURLM_OK && actual_fd_count > 0) {
            for (unsigned int i = 0; i < actual_fd_count && i < waitfds.size(); i++) {
                curl_socket_t fd = waitfds[i].fd;
                short events = waitfds[i].events;
                short revents = waitfds[i].revents;
                (void)fd;
                (void)events;
                (void)revents;
            }
        }
    }
    
    // Step 5: Test curl_multi_wakeup
    CURLMcode wakeup_result = curl_multi_wakeup(multi_handle);
    (void)wakeup_result;
    
    // Step 6: Test curl_multi_notify_enable and curl_multi_notify_disable
    // Enable notifications
    CURLMcode notify_enable_result = curl_multi_notify_enable(multi_handle, 
                                                             CURLMNOTIFY_INFO_READ);
    (void)notify_enable_result;
    
    // Also enable another notification type
    CURLMcode notify_enable_done = curl_multi_notify_enable(multi_handle,
                                                           CURLMNOTIFY_EASY_DONE);
    (void)notify_enable_done;
    
    // Set notification callback
    curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, notify_callback);
    
    // Disable notifications
    CURLMcode notify_disable_info = curl_multi_notify_disable(multi_handle,
                                                             CURLMNOTIFY_INFO_READ);
    CURLMcode notify_disable_done = curl_multi_notify_disable(multi_handle,
                                                             CURLMNOTIFY_EASY_DONE);
    (void)notify_disable_info;
    (void)notify_disable_done;
    
    // Clean up easy handle if it was created
    if (easy_handle) {
        // Remove from multi handle first
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    // Step 7: Clean up multi handle with curl_multi_cleanup
    CURLMcode cleanup_result = curl_multi_cleanup(multi_handle);
    (void)cleanup_result;
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_006

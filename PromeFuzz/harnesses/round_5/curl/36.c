// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "curl/curl.h"
#include "curl/multi.h"

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // Set some basic options on the easy handle
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 100L);
    
    // Add easy handle to multi handle
    CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
    
    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    
    // Call curl_multi_perform
    int running_handles = 0;
    CURLMcode perform_result = curl_multi_perform(multi_handle, &running_handles);
    
    // Use curl_multi_strerror on the result
    const char *error_str = curl_multi_strerror(perform_result);
    
    // Call curl_multi_socket_action with various parameters based on input
    if (Size >= 2) {
        curl_socket_t dummy_socket = (curl_socket_t)(Data[1] % 256);
        int ev_bitmask = (int)(Data[1] % 8);  // Use lower 3 bits for event mask
        
        if (selector & 0x01) {
            CURLMcode socket_action_result = curl_multi_socket_action(
                multi_handle, dummy_socket, ev_bitmask, &running_handles);
            error_str = curl_multi_strerror(socket_action_result);
        }
        
        // Call deprecated curl_multi_socket if selector bit 2 is set
        if ((selector & 0x02) && Size >= 3) {
            CURLMcode socket_result = curl_multi_socket(
                multi_handle, dummy_socket, &running_handles);
            error_str = curl_multi_strerror(socket_result);
        }
    }
    
    // Call deprecated curl_multi_socket_all if selector bit 3 is set
    if (selector & 0x04) {
        CURLMcode socket_all_result = curl_multi_socket_all(
            multi_handle, &running_handles);
        error_str = curl_multi_strerror(socket_all_result);
    }
    
    // Call curl_multi_remove_handle
    CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handle);
    error_str = curl_multi_strerror(remove_result);
    
    // Re-add handle to test removal again with different parameters
    if (selector & 0x10) {
        add_result = curl_multi_add_handle(multi_handle, easy_handle);
        
        // Test removal with potentially invalid parameters
        if (selector & 0x20) {
            // Test with NULL multi_handle
            curl_multi_remove_handle(NULL, easy_handle);
            
            // Test with NULL easy_handle
            curl_multi_remove_handle(multi_handle, NULL);
            
            // Test with both NULL
            curl_multi_remove_handle(NULL, NULL);
        }
        
        // Remove again for cleanup
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    // Test curl_multi_strerror with various error codes
    if (Size >= 4) {
        for (int i = 0; i < 4 && i < Size; i++) {
            CURLMcode test_code = (CURLMcode)(Data[i] % 20);  // Use modulo to get valid-ish codes
            error_str = curl_multi_strerror(test_code);
            
            // Also test with clearly invalid codes
            error_str = curl_multi_strerror((CURLMcode)999);
        }
    }
    
    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    return 0;
}
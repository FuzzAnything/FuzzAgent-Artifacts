// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

static void dummy_callback(void) {
    // Empty callback for fuzzing context
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create a dummy easy handle to have something to work with
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Set some basic options
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_CONNECT_ONLY, 1L);
        
        // Add to multi handle
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    // Use fuzzer data to influence parameters
    int running_handles = 0;
    long timeout_ms = 0;
    int ret_val = 0;
    int max_fd = 0;
    
    // Extract some values from fuzzer input
    int func_selector = 0;
    int socket_val = 0;
    int extra_fds_count = 0;
    
    if (Size >= 4) {
        func_selector = Data[0] % 6;
        socket_val = (Data[1] << 8) | Data[2];
        extra_fds_count = Data[3] % 5;  // Limit to 5 extra fds max
    }
    
    // Try different functions based on fuzzer input
    switch (func_selector) {
        case 0:
            // Test curl_multi_assign
            curl_multi_assign(multi_handle, (curl_socket_t)socket_val, &dummy_callback);
            break;
            
        case 1:
            // Test curl_multi_timeout
            curl_multi_timeout(multi_handle, &timeout_ms);
            break;
            
        case 2:
            // Test curl_multi_perform
            curl_multi_perform(multi_handle, &running_handles);
            break;
            
        case 3:
            // Test curl_multi_poll
            {
                struct curl_waitfd extra_fds[5];
                for (int i = 0; i < extra_fds_count && i < 5; i++) {
                    extra_fds[i].fd = (curl_socket_t)(socket_val + i);
                    extra_fds[i].events = (Data[i % Size] & 0x03) + 1;  // 1-3
                    extra_fds[i].revents = 0;
                }
                int poll_timeout = (Size > 4) ? (Data[4] * 10) : 100;
                curl_multi_poll(multi_handle, extra_fds, extra_fds_count, poll_timeout, &ret_val);
            }
            break;
            
        case 4:
            // Test curl_multi_fdset
            {
                fd_set read_fds, write_fds, exc_fds;
                FD_ZERO(&read_fds);
                FD_ZERO(&write_fds);
                FD_ZERO(&exc_fds);
                curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);
            }
            break;
            
        case 5:
            // Test curl_multi_wait
            {
                struct curl_waitfd extra_fds[5];
                for (int i = 0; i < extra_fds_count && i < 5; i++) {
                    extra_fds[i].fd = (curl_socket_t)(socket_val + i);
                    extra_fds[i].events = (Data[i % Size] & 0x03) + 1;
                    extra_fds[i].revents = 0;
                }
                int wait_timeout = (Size > 4) ? (Data[4] * 10) : 100;
                curl_multi_wait(multi_handle, extra_fds, extra_fds_count, wait_timeout, &ret_val);
            }
            break;
    }
    
    // Try calling all functions in sequence to explore more states
    if (Size > 0) {
        // Always call perform to drive I/O
        curl_multi_perform(multi_handle, &running_handles);
        
        // Try timeout
        curl_multi_timeout(multi_handle, &timeout_ms);
        
        // Try fdset
        fd_set read_fds, write_fds, exc_fds;
        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        FD_ZERO(&exc_fds);
        curl_multi_fdset(multi_handle, &read_fds, &write_fds, &exc_fds, &max_fd);
        
        // Try assign with different socket values
        for (int i = 0; i < 3 && i < Size; i++) {
            curl_multi_assign(multi_handle, (curl_socket_t)(Data[i] + 1000), NULL);
        }
    }
    
    // Cleanup
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();
    
    return 0;
}
// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    // Use fuzz data to influence parameters
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // Create dummy socket descriptor from fuzz data
    curl_socket_t dummy_sock = -1;
    if (Size > data_offset) {
        dummy_sock = (curl_socket_t)(Data[data_offset] + 1000); // Avoid standard descriptors
        data_offset++;
    }
    
    // Create dummy pointer for assign
    void *dummy_ptr = (void*)(uintptr_t)selector;
    
    // Test curl_multi_timeout
    long timeout_ms = 0;
    CURLMcode res = curl_multi_timeout(multi_handle, &timeout_ms);
    (void)res; // Result checked by libFuzzer via crashes
    
    // Test curl_multi_assign if we have a socket
    if (dummy_sock != -1) {
        res = curl_multi_assign(multi_handle, dummy_sock, dummy_ptr);
        (void)res;
    }
    
    // Test curl_multi_wait
    struct curl_waitfd extra_fds[2];
    int ret_val = 0;
    
    // Initialize extra_fds with dummy data
    for (int i = 0; i < 2 && data_offset + 2 < Size; i++) {
        extra_fds[i].fd = (curl_socket_t)(Data[data_offset] + 2000);
        extra_fds[i].events = (short)(Data[data_offset + 1] & 0x03); // Mask to valid event bits
        extra_fds[i].revents = 0;
        data_offset += 2;
    }
    
    unsigned int extra_nfds = (selector & 0x03); // 0-3 extra fds
    int timeout_param = (selector & 0x0F) * 10; // 0-150 ms
    
    res = curl_multi_wait(multi_handle, 
                         (extra_nfds > 0) ? extra_fds : NULL,
                         extra_nfds,
                         timeout_param,
                         &ret_val);
    (void)res;
    
    // Test curl_multi_socket_action
    int running_handles = 0;
    int ev_bitmask = selector & 0x07; // Mask to valid event bits
    
    res = curl_multi_socket_action(multi_handle,
                                  dummy_sock,
                                  ev_bitmask,
                                  &running_handles);
    (void)res;
    
    // Test curl_multi_poll
    ret_val = 0;
    timeout_param = ((selector >> 4) & 0x0F) * 10; // Different timeout
    
    res = curl_multi_poll(multi_handle,
                         (extra_nfds > 0) ? extra_fds : NULL,
                         extra_nfds,
                         timeout_param,
                         &ret_val);
    (void)res;
    
    // Test curl_multi_perform
    running_handles = 0;
    res = curl_multi_perform(multi_handle, &running_handles);
    (void)res;
    
    // Cleanup
    curl_multi_cleanup(multi_handle);
    
    return 0;
}
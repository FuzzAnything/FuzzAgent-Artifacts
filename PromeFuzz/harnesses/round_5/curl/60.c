// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/multi.h"

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    // Initialize easy handle with dummy URL
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_multi_add_handle(multi_handle, easy_handle);
    
    // Extract parameters from fuzzer input
    curl_socket_t sock = 0;
    int ev_bitmask = 0;
    int running_handles = 0;
    int ret_val = 0;
    void *sockp = NULL;
    
    if (Size >= sizeof(curl_socket_t)) {
        memcpy(&sock, Data, sizeof(curl_socket_t));
    }
    if (Size >= sizeof(curl_socket_t) + sizeof(int)) {
        memcpy(&ev_bitmask, Data + sizeof(curl_socket_t), sizeof(int));
    }
    
    // Create dummy application data for socket association
    static int dummy_data = 42;
    sockp = &dummy_data;
    
    // Test curl_multi_socket_action
    curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
    
    // Test curl_multi_assign
    curl_multi_assign(multi_handle, sock, sockp);
    
    // Test curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);
    
    // Test curl_multi_wait
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = sock;
    extra_fds[0].events = ev_bitmask & 0xFFFF;
    extra_fds[0].revents = 0;
    
    int timeout_ms = 0;
    if (Size >= sizeof(curl_socket_t) + sizeof(int) + sizeof(int)) {
        memcpy(&timeout_ms, Data + sizeof(curl_socket_t) + sizeof(int), sizeof(int));
    }
    
    curl_multi_wait(multi_handle, extra_fds, 1, timeout_ms, &ret_val);
    
    // Test deprecated curl_multi_socket (for compatibility)
    curl_multi_socket(multi_handle, sock, &running_handles);
    
    // Test deprecated curl_multi_socket_all (for compatibility)
    curl_multi_socket_all(multi_handle, &running_handles);
    
    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    return 0;
}
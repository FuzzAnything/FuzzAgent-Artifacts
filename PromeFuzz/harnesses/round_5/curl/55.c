// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CURL_DISABLE_TYPECHECK
#include "multi.h"

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Use fuzzer data to determine which function to test
    if (Size > 0) {
        uint8_t func_selector = Data[0] % 6;
        Data++;
        Size--;

        switch (func_selector) {
            case 0: {
                // Test curl_multi_socket_action
                if (Size >= sizeof(curl_socket_t) + sizeof(int)) {
                    curl_socket_t s;
                    int ev_bitmask;
                    int running_handles;
                    
                    memcpy(&s, Data, sizeof(curl_socket_t));
                    memcpy(&ev_bitmask, Data + sizeof(curl_socket_t), sizeof(int));
                    curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
                }
                break;
            }
            case 1: {
                // Test curl_multi_assign
                if (Size >= sizeof(curl_socket_t)) {
                    curl_socket_t sockfd;
                    void *sockp = NULL;
                    
                    memcpy(&sockfd, Data, sizeof(curl_socket_t));
                    // Use a dummy pointer - the fuzzer data won't contain valid pointers
                    sockp = (void*)(uintptr_t)Data;
                    curl_multi_assign(multi_handle, sockfd, sockp);
                }
                break;
            }
            case 2: {
                // Test curl_multi_perform
                int running_handles;
                curl_multi_perform(multi_handle, &running_handles);
                break;
            }
            case 3: {
                // Test curl_multi_wait
                if (Size >= sizeof(int)) {
                    int timeout_ms;
                    int ret;
                    struct curl_waitfd extra_fds[1];
                    
                    memcpy(&timeout_ms, Data, sizeof(int));
                    // Create a simple waitfd structure
                    extra_fds[0].fd = 0;
                    extra_fds[0].events = 0;
                    extra_fds[0].revents = 0;
                    
                    curl_multi_wait(multi_handle, extra_fds, 1, timeout_ms, &ret);
                }
                break;
            }
            case 4: {
                // Test curl_multi_socket (deprecated)
                if (Size >= sizeof(curl_socket_t)) {
                    curl_socket_t s;
                    int running_handles;
                    
                    memcpy(&s, Data, sizeof(curl_socket_t));
                    curl_multi_socket(multi_handle, s, &running_handles);
                }
                break;
            }
            case 5: {
                // Test curl_multi_socket_all (deprecated)
                int running_handles;
                curl_multi_socket_all(multi_handle, &running_handles);
                break;
            }
        }
    }

    // Cleanup
    curl_multi_cleanup(multi_handle);
    
    return 0;
}
// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "curl/curl.h"
#include "curl/multi.h"

#ifdef __cplusplus
}
#endif

static int dummy_running_handles = 0;
static int dummy_ret = 0;

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Create dummy easy handle and add to multi handle
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    // Use fuzz data to determine which function to call
    if (Size > 0) {
        uint8_t selector = Data[0] % 7;
        Data++;
        Size--;

        // Extract parameters from fuzz data with bounds checking
        curl_socket_t sock = -1;
        int ev_bitmask = 0;
        int timeout_ms = 0;
        void *sockp = NULL;
        
        if (Size >= sizeof(curl_socket_t)) {
            memcpy(&sock, Data, sizeof(curl_socket_t));
            Data += sizeof(curl_socket_t);
            Size -= sizeof(curl_socket_t);
        }
        
        if (Size >= sizeof(int)) {
            memcpy(&ev_bitmask, Data, sizeof(int));
            Data += sizeof(int);
            Size -= sizeof(int);
        }
        
        if (Size >= sizeof(int)) {
            memcpy(&timeout_ms, Data, sizeof(int));
            Data += sizeof(int);
            Size -= sizeof(int);
        }
        
        // Create dummy socket pointer
        sockp = &dummy_running_handles;

        switch (selector) {
            case 0:
                curl_multi_socket_action(multi_handle, sock, ev_bitmask, &dummy_running_handles);
                break;
            case 1:
                curl_multi_assign(multi_handle, sock, sockp);
                break;
            case 2:
                curl_multi_perform(multi_handle, &dummy_running_handles);
                break;
            case 3: {
                struct curl_waitfd extra_fds[1];
                if (Size >= sizeof(struct curl_waitfd)) {
                    memcpy(extra_fds, Data, sizeof(struct curl_waitfd));
                } else {
                    extra_fds[0].fd = sock;
                    extra_fds[0].events = ev_bitmask & 0xFFFF;
                    extra_fds[0].revents = 0;
                }
                unsigned int extra_nfds = 1;
                curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &dummy_ret);
                break;
            }
            case 4:
                curl_multi_socket(multi_handle, sock, &dummy_running_handles);
                break;
            case 5:
                curl_multi_socket_all(multi_handle, &dummy_running_handles);
                break;
            case 6:
                // Call multiple functions in sequence
                curl_multi_perform(multi_handle, &dummy_running_handles);
                curl_multi_socket_action(multi_handle, sock, ev_bitmask, &dummy_running_handles);
                curl_multi_wait(multi_handle, NULL, 0, timeout_ms, &dummy_ret);
                break;
        }
    }

    // Clean up
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);
    
    return 0;
}
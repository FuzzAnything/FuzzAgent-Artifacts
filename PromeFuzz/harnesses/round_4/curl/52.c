// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
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
static void* dummy_sockp = NULL;

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Initialize global curl if needed
    static int curl_initialized = 0;
    if (!curl_initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        curl_initialized = 1;
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Create a dummy easy handle and add it to multi
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        // Set a dummy URL to make the handle valid
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    // Use fuzzer input to determine which functions to call
    // and derive parameters from the input
    if (Size > 0) {
        uint8_t selector = Data[0];
        size_t offset = 1;
        
        // Derive parameters from fuzzer input
        curl_socket_t dummy_socket = -1;
        int dummy_timeout = 0;
        int dummy_ev_bitmask = 0;
        
        if (Size > offset) {
            dummy_socket = (curl_socket_t)Data[offset];
            offset++;
        }
        if (Size > offset) {
            dummy_timeout = (int)Data[offset];
            offset++;
        }
        if (Size > offset) {
            dummy_ev_bitmask = (int)Data[offset];
            offset++;
        }

        // Create dummy waitfd structures if needed
        struct curl_waitfd extra_fds[2];
        if (Size > offset && (Size - offset) >= sizeof(struct curl_waitfd) * 2) {
            memcpy(extra_fds, Data + offset, sizeof(struct curl_waitfd) * 2);
        } else {
            // Default dummy values
            extra_fds[0].fd = -1;
            extra_fds[0].events = 0;
            extra_fds[0].revents = 0;
            extra_fds[1].fd = -1;
            extra_fds[1].events = 0;
            extra_fds[1].revents = 0;
        }

        // Call different functions based on selector
        switch (selector % 6) {
            case 0:
                // curl_multi_socket_all (deprecated)
                curl_multi_socket_all(multi_handle, &dummy_running_handles);
                break;
            case 1:
                // curl_multi_assign
                curl_multi_assign(multi_handle, dummy_socket, dummy_sockp);
                break;
            case 2:
                // curl_multi_wait
                {
                    int ret;
                    unsigned int extra_nfds = (Size > 0) ? (Data[0] % 3) : 0;
                    curl_multi_wait(multi_handle, extra_fds, extra_nfds, dummy_timeout, &ret);
                }
                break;
            case 3:
                // curl_multi_socket (deprecated)
                curl_multi_socket(multi_handle, dummy_socket, &dummy_running_handles);
                break;
            case 4:
                // curl_multi_socket_action
                curl_multi_socket_action(multi_handle, dummy_socket, dummy_ev_bitmask, &dummy_running_handles);
                break;
            case 5:
                // curl_multi_perform
                curl_multi_perform(multi_handle, &dummy_running_handles);
                break;
        }

        // Try calling multiple functions in sequence to explore more states
        if (Size > 1) {
            // Call perform to update state
            curl_multi_perform(multi_handle, &dummy_running_handles);
            
            // Call socket action with different parameters
            if (Size > 2) {
                curl_multi_socket_action(multi_handle, dummy_socket, 
                                        (int)Data[1] | (int)Data[2] << 8, 
                                        &dummy_running_handles);
            }
            
            // Call wait with different parameters
            if (Size > 3) {
                int ret;
                curl_multi_wait(multi_handle, extra_fds, (unsigned int)Data[3] % 3, 
                              (int)Data[3], &ret);
            }
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
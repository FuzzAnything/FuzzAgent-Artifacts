// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Define CURL_DISABLE_TYPECHECK to avoid typecheck macro issues */
#define CURL_DISABLE_TYPECHECK

#ifdef __cplusplus
extern "C" {
#endif

#include "/promefuzz/database/curl/latest/code/include/curl/multi.h"

#ifdef __cplusplus
}
#endif

static int dummy_running_handles = 0;
static int dummy_ret = 0;

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Use the fuzz data to determine which functions to call and with what parameters
    if (Size > 0) {
        uint8_t action = Data[0] % 6;
        size_t data_offset = 1;

        // Extract parameters from fuzz data if available
        curl_socket_t dummy_socket = -1;
        int dummy_ev_bitmask = 0;
        void *dummy_sockp = NULL;
        int dummy_timeout_ms = 0;
        unsigned int dummy_extra_nfds = 0;
        
        if (Size > data_offset + sizeof(curl_socket_t)) {
            memcpy(&dummy_socket, Data + data_offset, sizeof(curl_socket_t));
            data_offset += sizeof(curl_socket_t);
        }
        
        if (Size > data_offset + sizeof(int)) {
            memcpy(&dummy_ev_bitmask, Data + data_offset, sizeof(int));
            data_offset += sizeof(int);
        }
        
        if (Size > data_offset + sizeof(void*)) {
            // Use a valid pointer for sockp
            static char dummy_buffer[1024];
            dummy_sockp = dummy_buffer;
            data_offset += sizeof(void*);
        }
        
        if (Size > data_offset + sizeof(int)) {
            memcpy(&dummy_timeout_ms, Data + data_offset, sizeof(int));
            data_offset += sizeof(int);
        }
        
        if (Size > data_offset + sizeof(unsigned int)) {
            memcpy(&dummy_extra_nfds, Data + data_offset, sizeof(unsigned int));
            data_offset += sizeof(unsigned int);
        }

        // Limit extra_nfds to prevent excessive allocation
        if (dummy_extra_nfds > 10) {
            dummy_extra_nfds = 10;
        }

        // Allocate waitfd array if needed
        struct curl_waitfd *extra_fds = NULL;
        if (dummy_extra_nfds > 0) {
            extra_fds = (struct curl_waitfd*)malloc(dummy_extra_nfds * sizeof(struct curl_waitfd));
            if (extra_fds) {
                for (unsigned int i = 0; i < dummy_extra_nfds; i++) {
                    extra_fds[i].fd = dummy_socket + i;
                    extra_fds[i].events = (short)(Data[(data_offset + i) % Size] & 0x03);
                    extra_fds[i].revents = 0;
                }
            }
        }

        // Call the selected API function
        switch (action) {
            case 0:
                curl_multi_socket_action(multi_handle, dummy_socket, dummy_ev_bitmask, &dummy_running_handles);
                break;
            case 1:
                // Note: curl_multi_socket_all is deprecated but still available
                // We'll use it conditionally
                #ifdef CURL_MULTI_SOCKET_ALL_AVAILABLE
                curl_multi_socket_all(multi_handle, &dummy_running_handles);
                #endif
                break;
            case 2:
                curl_multi_assign(multi_handle, dummy_socket, dummy_sockp);
                break;
            case 3:
                curl_multi_wait(multi_handle, extra_fds, dummy_extra_nfds, dummy_timeout_ms, &dummy_ret);
                break;
            case 4:
                // Note: curl_multi_socket is deprecated but still available
                // We'll use it conditionally
                #ifdef CURL_MULTI_SOCKET_AVAILABLE
                curl_multi_socket(multi_handle, dummy_socket, &dummy_running_handles);
                #endif
                break;
            case 5:
                curl_multi_perform(multi_handle, &dummy_running_handles);
                break;
        }

        // Free allocated memory
        if (extra_fds) {
            free(extra_fds);
        }

        // Try calling other functions to explore more states
        if (Size > 1) {
            uint8_t secondary_action = Data[1] % 3;
            switch (secondary_action) {
                case 0:
                    curl_multi_perform(multi_handle, &dummy_running_handles);
                    break;
                case 1:
                    curl_multi_wait(multi_handle, NULL, 0, dummy_timeout_ms, &dummy_ret);
                    break;
                case 2:
                    curl_multi_socket_action(multi_handle, dummy_socket, 0, &dummy_running_handles);
                    break;
            }
        }
    }

    // Cleanup
    curl_multi_cleanup(multi_handle);
    
    return 0;
}
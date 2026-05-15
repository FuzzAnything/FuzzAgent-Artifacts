// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CURL_DISABLE_TYPECHECK
#include "multi.h"

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    int running_handles = 0;
    curl_socket_t dummy_socket = 0;
    void *dummy_ptr = NULL;
    
    uint8_t selector = Data[0] % 6;
    size_t offset = 1;
    
    switch (selector) {
        case 0: {
            CURLMoption option = (CURLMoption)(Data[offset % Size] % 256);
            long param = (long)(Data[(offset + 1) % Size]);
            curl_multi_setopt(multi_handle, option, param);
            break;
        }
        case 1: {
            if (Size >= offset + sizeof(curl_socket_t)) {
                memcpy(&dummy_socket, Data + offset, sizeof(curl_socket_t));
            }
            curl_multi_socket(multi_handle, dummy_socket, &running_handles);
            break;
        }
        case 2: {
            if (Size >= offset + sizeof(curl_socket_t)) {
                memcpy(&dummy_socket, Data + offset, sizeof(curl_socket_t));
            }
            curl_multi_assign(multi_handle, dummy_socket, dummy_ptr);
            break;
        }
        case 3: {
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
        }
        case 4: {
            curl_multi_perform(multi_handle, &running_handles);
            break;
        }
        case 5: {
            if (Size >= offset + sizeof(curl_socket_t) + sizeof(int)) {
                memcpy(&dummy_socket, Data + offset, sizeof(curl_socket_t));
                int ev_bitmask = (int)(Data[(offset + sizeof(curl_socket_t)) % Size]);
                curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
            }
            break;
        }
    }
    
    curl_multi_cleanup(multi_handle);
    return 0;
}
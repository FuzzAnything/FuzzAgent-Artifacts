// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

static void dummy_callback(CURLM *multi, void *userp) {
    (void)multi;
    (void)userp;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle1 = curl_easy_init();
    CURL *easy_handle2 = curl_easy_init();
    
    if (easy_handle1) {
        curl_easy_setopt(easy_handle1, CURLOPT_URL, "file:///dev/null");
        curl_multi_add_handle(multi_handle, easy_handle1);
    }
    
    if (easy_handle2) {
        curl_easy_setopt(easy_handle2, CURLOPT_URL, "file:///dev/null");
        curl_multi_add_handle(multi_handle, easy_handle2);
    }

    if (Size > 0) {
        int running_handles = 0;
        int ret_val = 0;
        CURLMcode result;
        
        uint8_t op_selector = Data[0];
        
        curl_socket_t sock = -1;
        if (Size >= sizeof(curl_socket_t) + 1) {
            memcpy(&sock, Data + 1, sizeof(curl_socket_t));
        }
        
        int ev_bitmask = 0;
        if (Size >= sizeof(int) + sizeof(curl_socket_t) + 1) {
            memcpy(&ev_bitmask, Data + 1 + sizeof(curl_socket_t), sizeof(int));
        }
        
        int timeout_ms = 0;
        if (Size >= 2*sizeof(int) + sizeof(curl_socket_t) + 1) {
            memcpy(&timeout_ms, Data + 1 + sizeof(curl_socket_t) + sizeof(int), sizeof(int));
        }
        
        static int dummy_app_data = 42;
        
        struct curl_waitfd extra_fds[2];
        if (Size >= 1 + sizeof(curl_socket_t) + 2*sizeof(int) + 2*sizeof(struct curl_waitfd)) {
            for (int i = 0; i < 2; i++) {
                size_t offset = 1 + sizeof(curl_socket_t) + 2*sizeof(int) + i*sizeof(struct curl_waitfd);
                memcpy(&extra_fds[i], Data + offset, sizeof(struct curl_waitfd));
            }
        } else {
            extra_fds[0].fd = -1;
            extra_fds[0].events = 0;
            extra_fds[0].revents = 0;
            extra_fds[1].fd = -1;
            extra_fds[1].events = 0;
            extra_fds[1].revents = 0;
        }
        
        switch (op_selector % 7) {
            case 0:
                result = curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
                break;
                
            case 1:
                result = curl_multi_assign(multi_handle, sock, &dummy_app_data);
                break;
                
            case 2:
                result = curl_multi_perform(multi_handle, &running_handles);
                break;
                
            case 3:
                result = curl_multi_wait(multi_handle, extra_fds, 2, timeout_ms, &ret_val);
                break;
                
            case 4:
                result = curl_multi_socket(multi_handle, sock, &running_handles);
                break;
                
            case 5:
                result = curl_multi_socket_all(multi_handle, &running_handles);
                break;
                
            case 6:
                curl_multi_assign(multi_handle, sock, &dummy_app_data);
                curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
                curl_multi_perform(multi_handle, &running_handles);
                curl_multi_wait(multi_handle, extra_fds, 2, timeout_ms, &ret_val);
                result = CURLM_OK;
                break;
        }
        
        if (Size > 10) {
            curl_socket_t alt_sock = (curl_socket_t)(Data[1] * 256 + Data[2]);
            curl_multi_socket_action(multi_handle, alt_sock, ev_bitmask | 1, &running_handles);
            
            static int alt_app_data = 99;
            curl_multi_assign(multi_handle, alt_sock, &alt_app_data);
            
            curl_multi_wait(multi_handle, NULL, 0, Data[3] % 1000, &ret_val);
        }
    }

    if (easy_handle1) {
        curl_multi_remove_handle(multi_handle, easy_handle1);
        curl_easy_cleanup(easy_handle1);
    }
    
    if (easy_handle2) {
        curl_multi_remove_handle(multi_handle, easy_handle2);
        curl_easy_cleanup(easy_handle2);
    }
    
    curl_multi_cleanup(multi_handle);
    
    return 0;
}
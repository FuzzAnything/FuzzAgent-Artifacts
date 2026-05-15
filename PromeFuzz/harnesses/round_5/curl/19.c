// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
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

static int notify_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

static int socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

static int timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
    curl_multi_add_handle(multi_handle, easy_handle);
    
    unsigned int selector = Data[0] % 6;
    int running_handles = 0;
    
    switch (selector) {
        case 0: {
            curl_socket_t dummy_socket = 0;
            int ev_bitmask = (Size > 1) ? Data[1] : 0;
            curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
            break;
        }
        case 1: {
            unsigned int notification = (Size > 1) ? Data[1] : 0;
            curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, notify_callback);
            curl_multi_notify_enable(multi_handle, notification);
            break;
        }
        case 2: {
            long opt_value = (Size > 1) ? Data[1] : 0;
            curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, socket_callback);
            curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, timer_callback);
            curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, opt_value);
            break;
        }
        case 3:
            curl_multi_remove_handle(multi_handle, easy_handle);
            if (Size > 1 && (Data[1] % 2)) {
                curl_multi_add_handle(multi_handle, easy_handle);
            }
            break;
        case 4: {
            unsigned int notification = (Size > 1) ? Data[1] : 0;
            curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, notify_callback);
            curl_multi_notify_disable(multi_handle, notification);
            break;
        }
        case 5:
            curl_multi_wakeup(multi_handle);
            break;
    }
    
    if (selector != 3) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    remove("./dummy_file");
    return 0;
}
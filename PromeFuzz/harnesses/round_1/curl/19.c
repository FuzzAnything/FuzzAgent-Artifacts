// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/multi.h"

static int dummy_notify_callback(CURL *handle, curl_socket_t s, int action,
                                 void *userp, void *socketp) {
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what,
                                 void *userp, void *socketp) {
    return 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    uint8_t selector = Data[0] % 6;
    size_t data_offset = 1;

    switch (selector) {
        case 0: {
            if (Size - data_offset < 1) {
                break;
            }
            
            uint8_t option_choice = Data[data_offset] % 7;
            data_offset++;
            
            switch (option_choice) {
                case 0:
                    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, 1L);
                    break;
                case 1:
                    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, 10L);
                    break;
                case 2:
                    curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, 5L);
                    break;
                case 3:
                    curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, 3L);
                    break;
                case 4:
                    curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, 1024L);
                    break;
                case 5:
                    curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, 512L);
                    break;
                case 6:
                    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, 20L);
                    break;
            }
            break;
        }
        
        case 1: {
            if (curl_multi_add_handle(multi_handle, easy_handle) == CURLM_OK) {
                curl_multi_remove_handle(multi_handle, easy_handle);
            }
            break;
        }
        
        case 2: {
            unsigned int notification = 0;
            if (Size - data_offset >= sizeof(unsigned int)) {
                memcpy(&notification, Data + data_offset, sizeof(unsigned int));
            }
            
            curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, dummy_notify_callback);
            curl_multi_notify_enable(multi_handle, notification);
            break;
        }
        
        case 3: {
            curl_multi_wakeup(multi_handle);
            break;
        }
        
        case 4: {
            unsigned int notification = 0;
            if (Size - data_offset >= sizeof(unsigned int)) {
                memcpy(&notification, Data + data_offset, sizeof(unsigned int));
            }
            
            curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, dummy_notify_callback);
            curl_multi_notify_disable(multi_handle, notification);
            break;
        }
        
        case 5: {
            curl_socket_t s = -1;
            int ev_bitmask = 0;
            int running_handles = 0;
            
            if (Size - data_offset >= sizeof(curl_socket_t)) {
                memcpy(&s, Data + data_offset, sizeof(curl_socket_t));
                data_offset += sizeof(curl_socket_t);
            }
            
            if (Size - data_offset >= sizeof(int)) {
                memcpy(&ev_bitmask, Data + data_offset, sizeof(int));
            }
            
            if (curl_multi_add_handle(multi_handle, easy_handle) == CURLM_OK) {
                curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
                curl_multi_remove_handle(multi_handle, easy_handle);
            }
            break;
        }
    }

    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    return 0;
}
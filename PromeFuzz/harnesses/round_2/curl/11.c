// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include "curl/easy.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static int dummy_notify_callback(CURLM *multi_handle, void *userp, unsigned int notification, CURL *easy_handle, CURLcode error_code) {
    return 0;
}

static int dummy_timer_callback(CURLM *multi_handle, long timeout_ms, void *userp) {
    return 0;
}

static int dummy_socket_callback(CURL *easy_handle, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    static int dummy_file_initialized = 0;
    static const char *dummy_filename = "./dummy_file";
    
    if (!dummy_file_initialized) {
        int fd = open(dummy_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd >= 0) {
            close(fd);
        }
        dummy_file_initialized = 1;
    }
    
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
            curl_socket_t s = -1;
            int ev_bitmask = 0;
            int running_handles = 0;
            
            if (data_offset < Size && Size - data_offset >= sizeof(curl_socket_t)) {
                memcpy(&s, Data + data_offset, sizeof(curl_socket_t));
                data_offset += sizeof(curl_socket_t);
            }
            if (data_offset < Size && Size - data_offset >= sizeof(int)) {
                memcpy(&ev_bitmask, Data + data_offset, sizeof(int));
                data_offset += sizeof(int);
            }
            
            curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
            break;
        }
        
        case 1: {
            unsigned int notification = 0;
            if (data_offset < Size && Size - data_offset >= sizeof(unsigned int)) {
                memcpy(&notification, Data + data_offset, sizeof(unsigned int));
                data_offset += sizeof(unsigned int);
            }
            
            curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, dummy_notify_callback);
            curl_multi_notify_enable(multi_handle, notification);
            break;
        }
        
        case 2: {
            CURLMoption option = CURLMOPT_PIPELINING;
            if (data_offset < Size) {
                option = (CURLMoption)(Data[data_offset] % 10);
                data_offset = (data_offset + 1) % Size;
            }
            
            switch (option) {
                case CURLMOPT_PIPELINING:
                case CURLMOPT_MAX_HOST_CONNECTIONS:
                case CURLMOPT_MAXCONNECTS:
                case CURLMOPT_MAX_PIPELINE_LENGTH:
                case CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE:
                case CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE:
                case CURLMOPT_PIPELINING_SITE_BL:
                case CURLMOPT_PIPELINING_SERVER_BL:
                case CURLMOPT_MAX_TOTAL_CONNECTIONS: {
                    long value = 0;
                    if (data_offset < Size && Size - data_offset >= sizeof(long)) {
                        memcpy(&value, Data + data_offset, sizeof(long));
                    }
                    curl_multi_setopt(multi_handle, option, value);
                    break;
                }
                case CURLMOPT_SOCKETFUNCTION:
                    curl_multi_setopt(multi_handle, option, dummy_socket_callback);
                    break;
                case CURLMOPT_TIMERFUNCTION:
                    curl_multi_setopt(multi_handle, option, dummy_timer_callback);
                    break;
                case CURLMOPT_NOTIFYFUNCTION:
                    curl_multi_setopt(multi_handle, option, dummy_notify_callback);
                    break;
                default:
                    break;
            }
            break;
        }
        
        case 3: {
            unsigned int notification = 0;
            if (data_offset < Size && Size - data_offset >= sizeof(unsigned int)) {
                memcpy(&notification, Data + data_offset, sizeof(unsigned int));
                data_offset += sizeof(unsigned int);
            }
            
            curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, dummy_notify_callback);
            curl_multi_notify_disable(multi_handle, notification);
            break;
        }
        
        case 4: {
            curl_multi_add_handle(multi_handle, easy_handle);
            curl_multi_remove_handle(multi_handle, easy_handle);
            break;
        }
        
        case 5: {
            curl_multi_wakeup(multi_handle);
            break;
        }
    }
    
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    return 0;
}
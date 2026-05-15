// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include <unistd.h>
#include <fcntl.h>

static int dummy_notify_callback(CURL *handle, curl_socket_t s, int what, void *userp, void *socketp) {
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

static curl_socket_t dummy_socket_callback(CURL *easy, curlsocktype purpose, struct curl_sockaddr *address, void *userp) {
    return open("/dev/null", O_RDWR);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    int running_handles = 0;
    curl_socket_t sockfd = 0;
    int ev_bitmask = 0;
    unsigned int notification = 0;
    CURLMoption option = 0;
    long param = 0;
    
    if (Size < 16) {
        return 0;
    }
    
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }
    
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
    curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
    curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, dummy_notify_callback);
    
    size_t offset = 0;
    
    for (int i = 0; i < 6 && offset + 4 <= Size; i++) {
        int func_selector = Data[offset] % 6;
        offset++;
        
        switch (func_selector) {
            case 0: {
                if (offset + sizeof(curl_socket_t) + sizeof(int) <= Size) {
                    memcpy(&sockfd, Data + offset, sizeof(curl_socket_t));
                    offset += sizeof(curl_socket_t);
                    memcpy(&ev_bitmask, Data + offset, sizeof(int));
                    ev_bitmask &= 0x3;
                    offset += sizeof(int);
                    curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
                }
                break;
            }
            case 1: {
                if (offset + sizeof(unsigned int) <= Size) {
                    memcpy(&notification, Data + offset, sizeof(unsigned int));
                    notification %= 4;
                    offset += sizeof(unsigned int);
                    curl_multi_notify_enable(multi_handle, notification);
                }
                break;
            }
            case 2: {
                if (offset + sizeof(CURLMoption) + sizeof(long) <= Size) {
                    memcpy(&option, Data + offset, sizeof(CURLMoption));
                    option %= 10;
                    offset += sizeof(CURLMoption);
                    memcpy(&param, Data + offset, sizeof(long));
                    offset += sizeof(long);
                    
                    switch (option) {
                        case CURLMOPT_MAXCONNECTS:
                        case CURLMOPT_MAX_HOST_CONNECTIONS:
                        case CURLMOPT_MAX_PIPELINE_LENGTH:
                        case CURLMOPT_MAX_TOTAL_CONNECTIONS:
                        case CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE:
                        case CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE:
                        case CURLMOPT_PIPELINING_SITE_BL:
                        case CURLMOPT_PIPELINING_SERVER_BL:
                            curl_multi_setopt(multi_handle, option, param);
                            break;
                        default:
                            break;
                    }
                }
                break;
            }
            case 3: {
                if (offset + sizeof(unsigned int) <= Size) {
                    memcpy(&notification, Data + offset, sizeof(unsigned int));
                    notification %= 4;
                    offset += sizeof(unsigned int);
                    curl_multi_notify_disable(multi_handle, notification);
                }
                break;
            }
            case 4: {
                curl_multi_wakeup(multi_handle);
                break;
            }
            case 5: {
                curl_multi_perform(multi_handle, &running_handles);
                break;
            }
        }
        
        if (offset >= Size) {
            offset = 0;
        }
    }
    
    curl_multi_cleanup(multi_handle);
    return 0;
}
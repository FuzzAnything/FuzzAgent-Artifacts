// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what,
                                 void *userp, void *socketp) {
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    return 0;
}

static void dummy_notify_callback(CURLM *multi, CURL *easy,
                                  unsigned int notification,
                                  void *userp) {
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        remove_dummy_file();
        return 0;
    }
    
    unsigned int seed = 0;
    for (size_t i = 0; i < Size && i < 4; i++) {
        seed = (seed << 8) | Data[i];
    }
    srand(seed);
    
    int action = Data[0] % 10;
    
    switch (action) {
        case 0: {
            curl_multi_wakeup(multi_handle);
            break;
        }
        case 1: {
            struct curl_waitfd extra_fds[2];
            int ret;
            int timeout_ms = (int)(Data[1 % Size] * 100);
            
            extra_fds[0].fd = open("./dummy_file", O_RDONLY);
            extra_fds[0].events = POLLIN;
            extra_fds[0].revents = 0;
            
            if (Size > 2) {
                extra_fds[1].fd = open("/dev/null", O_WRONLY);
                extra_fds[1].events = POLLOUT;
                extra_fds[1].revents = 0;
                curl_multi_wait(multi_handle, extra_fds, 2, timeout_ms, &ret);
                if (extra_fds[1].fd >= 0) close(extra_fds[1].fd);
            } else {
                curl_multi_wait(multi_handle, extra_fds, 1, timeout_ms, &ret);
            }
            
            if (extra_fds[0].fd >= 0) close(extra_fds[0].fd);
            break;
        }
        case 2:
        case 3: {
            int running_handles;
            curl_socket_t s = (curl_socket_t)(Data[1 % Size]);
            int ev_bitmask = (int)(Data[2 % Size] & 0x07);
            
            curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
            break;
        }
        case 4:
        case 5: {
            CURLMoption options[] = {
                CURLMOPT_SOCKETFUNCTION,
                CURLMOPT_SOCKETDATA,
                CURLMOPT_TIMERFUNCTION,
                CURLMOPT_TIMERDATA,
                CURLMOPT_MAXCONNECTS,
                CURLMOPT_MAX_TOTAL_CONNECTIONS,
                CURLMOPT_PIPELINING,
                CURLMOPT_NOTIFYFUNCTION,
                CURLMOPT_NOTIFYDATA
            };
            
            int opt_idx = Data[1 % Size] % (sizeof(options)/sizeof(options[0]));
            CURLMoption option = options[opt_idx];
            
            switch (option) {
                case CURLMOPT_SOCKETFUNCTION:
                    curl_multi_setopt(multi_handle, option, dummy_socket_callback);
                    break;
                case CURLMOPT_SOCKETDATA:
                    curl_multi_setopt(multi_handle, option, NULL);
                    break;
                case CURLMOPT_TIMERFUNCTION:
                    curl_multi_setopt(multi_handle, option, dummy_timer_callback);
                    break;
                case CURLMOPT_TIMERDATA:
                    curl_multi_setopt(multi_handle, option, NULL);
                    break;
                case CURLMOPT_MAXCONNECTS:
                    curl_multi_setopt(multi_handle, option, (long)(Data[2 % Size] * 10));
                    break;
                case CURLMOPT_MAX_TOTAL_CONNECTIONS:
                    curl_multi_setopt(multi_handle, option, (long)(Data[2 % Size] * 5));
                    break;
                case CURLMOPT_PIPELINING:
                    curl_multi_setopt(multi_handle, option, (long)(Data[2 % Size] & 0x03));
                    break;
                case CURLMOPT_NOTIFYFUNCTION:
                    curl_multi_setopt(multi_handle, option, dummy_notify_callback);
                    break;
                case CURLMOPT_NOTIFYDATA:
                    curl_multi_setopt(multi_handle, option, NULL);
                    break;
                default:
                    break;
            }
            break;
        }
        case 6: {
            unsigned int notification = (unsigned int)(Data[1 % Size] & 0xFF);
            curl_multi_notify_disable(multi_handle, notification);
            break;
        }
        case 7: {
            unsigned int notification = (unsigned int)(Data[1 % Size] & 0xFF);
            curl_multi_notify_enable(multi_handle, notification);
            break;
        }
        case 8: {
            curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, dummy_notify_callback);
            curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYDATA, NULL);
            
            unsigned int notification = (unsigned int)(Data[1 % Size] & 0xFF);
            curl_multi_notify_enable(multi_handle, notification);
            
            if (Size > 2) {
                notification = (unsigned int)(Data[2 % Size] & 0xFF);
                curl_multi_notify_disable(multi_handle, notification);
            }
            break;
        }
        case 9: {
            int running_handles;
            curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, 0, &running_handles);
            curl_multi_wakeup(multi_handle);
            break;
        }
    }
    
    curl_multi_cleanup(multi_handle);
    remove_dummy_file();
    
    return 0;
}
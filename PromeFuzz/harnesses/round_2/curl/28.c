// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include <unistd.h>
#include <fcntl.h>

static void dummy_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {}
static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) { return 0; }

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    static char dummy_file[] = "./dummy_file";
    int fd = open(dummy_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, Data, Size);
        close(fd);
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        unlink(dummy_file);
        return 0;
    }

    int running_handles = 0;
    CURLMcode res;
    
    if (Size > 0) {
        unsigned char selector = Data[0];
        size_t offset = 1;
        
        switch (selector % 6) {
            case 0: {
                curl_socket_t s = -1;
                int ev_bitmask = 0;
                if (Size - offset >= sizeof(curl_socket_t)) {
                    memcpy(&s, Data + offset, sizeof(curl_socket_t));
                    offset += sizeof(curl_socket_t);
                }
                if (Size - offset >= sizeof(int)) {
                    memcpy(&ev_bitmask, Data + offset, sizeof(int));
                }
                res = curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
                break;
            }
            case 1: {
                res = curl_multi_socket_all(multi_handle, &running_handles);
                break;
            }
            case 2: {
                CURLMoption option = CURLMOPT_SOCKETFUNCTION;
                if (Size - offset >= sizeof(CURLMoption)) {
                    memcpy(&option, Data + offset, sizeof(CURLMoption));
                    offset += sizeof(CURLMoption);
                }
                
                if (option == CURLMOPT_SOCKETFUNCTION || option == CURLMOPT_TIMERFUNCTION) {
                    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_callback);
                    curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
                } else if (option == CURLMOPT_MAXCONNECTS) {
                    long maxconns = 10;
                    if (Size - offset >= sizeof(long)) {
                        memcpy(&maxconns, Data + offset, sizeof(long));
                    }
                    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, maxconns);
                } else if (option == CURLMOPT_MAX_TOTAL_CONNECTIONS) {
                    long max_total = 100;
                    if (Size - offset >= sizeof(long)) {
                        memcpy(&max_total, Data + offset, sizeof(long));
                    }
                    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_total);
                }
                break;
            }
            case 3: {
                curl_socket_t sockfd = -1;
                void *sockp = NULL;
                if (Size - offset >= sizeof(curl_socket_t)) {
                    memcpy(&sockfd, Data + offset, sizeof(curl_socket_t));
                    offset += sizeof(curl_socket_t);
                }
                if (Size - offset > 0) {
                    sockp = (void *)(Data + offset);
                }
                res = curl_multi_assign(multi_handle, sockfd, sockp);
                break;
            }
            case 4: {
                curl_socket_t s = -1;
                if (Size - offset >= sizeof(curl_socket_t)) {
                    memcpy(&s, Data + offset, sizeof(curl_socket_t));
                }
                res = curl_multi_socket(multi_handle, s, &running_handles);
                break;
            }
            case 5: {
                res = curl_multi_perform(multi_handle, &running_handles);
                break;
            }
        }
    }

    curl_multi_cleanup(multi_handle);
    unlink(dummy_file);
    return 0;
}
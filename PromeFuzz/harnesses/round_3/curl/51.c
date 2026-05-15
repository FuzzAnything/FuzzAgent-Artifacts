// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, void *userp, void *socketp) {
    (void)easy;
    (void)s;
    (void)what;
    (void)userp;
    (void)socketp;
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi;
    (void)timeout_ms;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        cleanup_dummy_file();
        return 0;
    }
    
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        cleanup_dummy_file();
        return 0;
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
    
    curl_multi_add_handle(multi_handle, easy_handle);
    
    int running_handles = 0;
    curl_socket_t dummy_socket = -1;
    
    uint8_t selector = Data[0] % 6;
    size_t offset = 1;
    
    switch (selector) {
        case 0: {
            if (Size > offset) {
                dummy_socket = (curl_socket_t)Data[offset];
                offset++;
            }
            curl_multi_socket(multi_handle, dummy_socket, &running_handles);
            break;
        }
        case 1: {
            curl_multi_remove_handle(multi_handle, easy_handle);
            curl_multi_add_handle(multi_handle, easy_handle);
            break;
        }
        case 2: {
            curl_multi_wakeup(multi_handle);
            break;
        }
        case 3: {
            int ev_bitmask = 0;
            if (Size > offset) {
                dummy_socket = (curl_socket_t)Data[offset];
                offset++;
            }
            if (Size > offset) {
                ev_bitmask = (int)Data[offset];
                offset++;
            }
            curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
            break;
        }
        case 4: {
            if (Size > offset) {
                uint8_t opt_selector = Data[offset] % 3;
                offset++;
                
                switch (opt_selector) {
                    case 0:
                        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, dummy_socket_callback);
                        break;
                    case 1:
                        curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, dummy_timer_callback);
                        break;
                    case 2:
                        if (Size > offset) {
                            long max_conns = (long)Data[offset];
                            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_conns);
                        }
                        break;
                }
            }
            break;
        }
        case 5: {
            CURLMcode error_code = CURLM_OK;
            if (Size > offset) {
                error_code = (CURLMcode)(Data[offset] % 20);
            }
            const char *error_str = curl_multi_strerror(error_code);
            (void)error_str;
            break;
        }
    }
    
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    cleanup_dummy_file();
    
    return 0;
}
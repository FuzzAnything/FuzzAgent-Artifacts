// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/multi.h"
#include "/promefuzz/database/curl/latest/code/include/curl/easy.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size, f);
        } else {
            const char *default_data = "dummy";
            fwrite(default_data, 1, strlen(default_data), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;
    
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }
    
    curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 100L);
    
    curl_multi_add_handle(multi_handle, easy_handle);
    
    int running_handles = 0;
    int ret_val = 0;
    
    uint8_t selector = Data[0] % 6;
    const uint8_t *params = Data + 1;
    size_t params_size = Size - 1;
    
    switch (selector) {
        case 0: {
            curl_socket_t sock = (params_size >= sizeof(curl_socket_t)) ? 
                                 *(curl_socket_t *)params : 0;
            int ev_bitmask = (params_size >= sizeof(curl_socket_t) + sizeof(int)) ? 
                            *(int *)(params + sizeof(curl_socket_t)) : 0;
            curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
            break;
        }
        case 1: {
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
        }
        case 2: {
            curl_socket_t sock = (params_size >= sizeof(curl_socket_t)) ? 
                                 *(curl_socket_t *)params : 0;
            void *sockp = (params_size >= sizeof(curl_socket_t) + sizeof(void *)) ? 
                         *(void **)(params + sizeof(curl_socket_t)) : NULL;
            curl_multi_assign(multi_handle, sock, sockp);
            break;
        }
        case 3: {
            unsigned int extra_nfds = 0;
            struct curl_waitfd *extra_fds = NULL;
            
            if (params_size >= sizeof(unsigned int)) {
                extra_nfds = *(unsigned int *)params % 4;
                if (extra_nfds > 0 && params_size >= sizeof(unsigned int) + 
                    extra_nfds * sizeof(struct curl_waitfd)) {
                    extra_fds = (struct curl_waitfd *)(params + sizeof(unsigned int));
                }
            }
            
            int timeout_ms = (params_size >= sizeof(unsigned int) + sizeof(int)) ? 
                            *(int *)(params + sizeof(unsigned int)) : 100;
            curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_val);
            break;
        }
        case 4: {
            curl_socket_t sock = (params_size >= sizeof(curl_socket_t)) ? 
                                 *(curl_socket_t *)params : 0;
            curl_multi_socket(multi_handle, sock, &running_handles);
            break;
        }
        case 5: {
            curl_multi_perform(multi_handle, &running_handles);
            break;
        }
    }
    
    int still_running = 0;
    curl_multi_perform(multi_handle, &still_running);
    
    if (still_running > 0) {
        struct curl_waitfd extra_fds[1];
        extra_fds[0].fd = 0;
        extra_fds[0].events = CURL_WAIT_POLLIN;
        extra_fds[0].revents = 0;
        
        int timeout_ms = (params_size >= sizeof(int)) ? *(int *)params % 1000 : 10;
        int numfds = 0;
        curl_multi_wait(multi_handle, extra_fds, 1, timeout_ms, &numfds);
        
        curl_socket_t sock = 0;
        int ev_bitmask = CURL_CSELECT_IN;
        curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
    }
    
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    
    unlink("./dummy_file");
    
    return 0;
}
// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/curl.h"
#include "curl/multi.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        cleanup_dummy_file();
        return 0;
    }
    
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        char url[256];
        snprintf(url, sizeof(url), "file://%s", "./dummy_file");
        curl_easy_setopt(easy_handle, CURLOPT_URL, url);
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    int running_handles = 0;
    int msgs_in_queue = 0;
    long timeout_ms = 0;
    int ret_val = 0;
    
    for (int i = 0; i < 10 && Size > 0; i++) {
        int action = Data[i % Size] % 6;
        
        switch (action) {
            case 0: {
                curl_multi_perform(multi_handle, &running_handles);
                
                CURLMsg *msg;
                while ((msg = curl_multi_info_read(multi_handle, &msgs_in_queue)) != NULL) {
                    if (msg->msg == CURLMSG_DONE) {
                        CURLcode result = msg->data.result;
                        (void)result;
                    }
                }
                break;
            }
            case 1: {
                curl_socket_t dummy_socket = (curl_socket_t)(Data[i % Size]);
                int ev_bitmask = (int)(Data[(i + 1) % Size]) & 0x07;
                curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
                break;
            }
            case 2: {
                int timeout = (int)(Data[i % Size]) * 10;
                struct curl_waitfd extra_fds[1];
                extra_fds[0].fd = (curl_socket_t)(Data[(i + 1) % Size]);
                extra_fds[0].events = (short)(Data[(i + 2) % Size]);
                extra_fds[0].revents = 0;
                unsigned int extra_nfds = (Size > 3) ? 1 : 0;
                curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout, &ret_val);
                break;
            }
            case 3: {
                int timeout = (int)(Data[i % Size]) * 10;
                struct curl_waitfd extra_fds[1];
                extra_fds[0].fd = (curl_socket_t)(Data[(i + 1) % Size]);
                extra_fds[0].events = (short)(Data[(i + 2) % Size]);
                extra_fds[0].revents = 0;
                unsigned int extra_nfds = (Size > 3) ? 1 : 0;
                curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout, &ret_val);
                break;
            }
            case 4: {
                curl_multi_timeout(multi_handle, &timeout_ms);
                break;
            }
            case 5: {
                curl_multi_perform(multi_handle, &running_handles);
                break;
            }
        }
        
        if (easy_handle && (i % 3 == 0)) {
            curl_multi_remove_handle(multi_handle, easy_handle);
            curl_multi_add_handle(multi_handle, easy_handle);
        }
    }
    
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    cleanup_dummy_file();
    
    return 0;
}
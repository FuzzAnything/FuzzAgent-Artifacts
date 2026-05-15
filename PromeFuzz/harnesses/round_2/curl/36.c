// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include "curl/easy.h"

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi;
    (void)timeout_ms;
    (void)userp;
    return 0;
}

static int dummy_socket_callback(CURL *easy, curl_socket_t s, int what, 
                                 void *userp, void *socketp) {
    (void)easy;
    (void)s;
    (void)what;
    (void)userp;
    (void)socketp;
    return 0;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static char dummy_file[] = "./dummy_file";
    
    if (!initialized) {
        FILE *f = fopen(dummy_file, "wb");
        if (f) {
            const char dummy_data[] = "dummy data for fuzzing";
            fwrite(dummy_data, 1, sizeof(dummy_data), f);
            fclose(f);
        }
        initialized = 1;
    }
    
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }
    
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "file://./dummy_file");
        curl_multi_add_handle(multi_handle, easy_handle);
    }
    
    if (Size > 0) {
        int running_handles = 0;
        curl_socket_t dummy_socket = 0;
        void *dummy_ptr = NULL;
        CURLMoption option = CURLMOPT_TIMERFUNCTION;
        long dummy_long = 0;
        
        if (Size >= sizeof(dummy_socket)) {
            memcpy(&dummy_socket, Data, sizeof(dummy_socket));
        }
        if (Size >= sizeof(dummy_ptr) + sizeof(dummy_socket)) {
            memcpy(&dummy_ptr, Data + sizeof(dummy_socket), sizeof(dummy_ptr));
        }
        
        int ev_bitmask = 0;
        if (Size > 0) {
            ev_bitmask = Data[0] & 0x0F;
        }
        
        int func_selector = 0;
        if (Size > 0) {
            func_selector = Data[0] % 6;
        }
        
        switch (func_selector) {
            case 0:
                curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
                break;
            case 1:
                curl_multi_socket_all(multi_handle, &running_handles);
                break;
            case 2:
                if (Size > 1) {
                    int opt_selector = Data[1] % 10;
                    switch (opt_selector) {
                        case 0:
                            option = CURLMOPT_SOCKETFUNCTION;
                            curl_multi_setopt(multi_handle, option, dummy_socket_callback);
                            break;
                        case 1:
                            option = CURLMOPT_TIMERFUNCTION;
                            curl_multi_setopt(multi_handle, option, dummy_timer_callback);
                            break;
                        case 2:
                            option = CURLMOPT_PIPELINING;
                            dummy_long = (Data[1] >> 1) & 0x03;
                            curl_multi_setopt(multi_handle, option, dummy_long);
                            break;
                        case 3:
                            option = CURLMOPT_MAXCONNECTS;
                            dummy_long = Data[1];
                            curl_multi_setopt(multi_handle, option, dummy_long);
                            break;
                        case 4:
                            option = CURLMOPT_MAX_TOTAL_CONNECTIONS;
                            dummy_long = Data[1];
                            curl_multi_setopt(multi_handle, option, dummy_long);
                            break;
                        case 5:
                            option = CURLMOPT_MAX_HOST_CONNECTIONS;
                            dummy_long = Data[1];
                            curl_multi_setopt(multi_handle, option, dummy_long);
                            break;
                        case 6:
                            option = CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE;
                            dummy_long = Data[1];
                            curl_multi_setopt(multi_handle, option, dummy_long);
                            break;
                        case 7:
                            option = CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE;
                            dummy_long = Data[1];
                            curl_multi_setopt(multi_handle, option, dummy_long);
                            break;
                        case 8:
                            option = CURLMOPT_PIPELINING_SITE_BL;
                            dummy_ptr = NULL;
                            curl_multi_setopt(multi_handle, option, dummy_ptr);
                            break;
                        case 9:
                            option = CURLMOPT_PIPELINING_SERVER_BL;
                            dummy_ptr = NULL;
                            curl_multi_setopt(multi_handle, option, dummy_ptr);
                            break;
                    }
                }
                break;
            case 3:
                curl_multi_assign(multi_handle, dummy_socket, dummy_ptr);
                break;
            case 4:
                curl_multi_socket(multi_handle, dummy_socket, &running_handles);
                break;
            case 5:
                curl_multi_perform(multi_handle, &running_handles);
                break;
        }
    }
    
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    
    return 0;
}
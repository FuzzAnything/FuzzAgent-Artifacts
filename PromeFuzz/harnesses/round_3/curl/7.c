// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_info_read at multi.c:2943:10 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include "curl/easy.h"
#undef CURL_DISABLE_TYPECHECK
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static CURLM *create_multi_handle_with_easy(void) {
    CURLM *multi = curl_multi_init();
    if (!multi) return NULL;
    
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
        curl_multi_add_handle(multi, easy);
    }
    return multi;
}

static void cleanup_multi_handle(CURLM *multi) {
    if (!multi) return;
    
    struct CURLMsg *msg;
    int msgs_left;
    CURL *easy = NULL;
    
    while ((msg = curl_multi_info_read(multi, &msgs_left))) {
        if (msg->msg == CURLMSG_DONE) {
            easy = msg->easy_handle;
            if (easy) {
                curl_multi_remove_handle(multi, easy);
                curl_easy_cleanup(easy);
            }
        }
    }
    
    curl_multi_cleanup(multi);
}

int LLVMFuzzerTestOneInput_7(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    write_dummy_file(data, size);
    
    CURLM *multi = create_multi_handle_with_easy();
    if (!multi) return 0;
    
    int running_handles = 0;
    curl_socket_t dummy_socket = -1;
    void *dummy_ptr = (void *)(uintptr_t)data[0];
    
    uint8_t selector = data[0] % 6;
    size_t param_idx = 1;
    
    switch (selector) {
        case 0:
            curl_multi_perform(multi, &running_handles);
            break;
            
        case 1:
            curl_multi_socket_all(multi, &running_handles);
            break;
            
        case 2:
            if (param_idx < size) {
                dummy_socket = (curl_socket_t)data[param_idx];
                curl_multi_socket(multi, dummy_socket, &running_handles);
            }
            break;
            
        case 3:
            if (param_idx + 1 < size) {
                dummy_socket = (curl_socket_t)data[param_idx];
                int ev_bitmask = (int)data[param_idx + 1];
                curl_multi_socket_action(multi, dummy_socket, ev_bitmask, &running_handles);
            }
            break;
            
        case 4:
            if (param_idx < size) {
                CURLMoption option = (CURLMoption)(data[param_idx] % 10);
                switch (option) {
                    case CURLMOPT_PIPELINING:
                    case CURLMOPT_MAXCONNECTS:
                    case CURLMOPT_MAX_HOST_CONNECTIONS:
                    case CURLMOPT_MAX_PIPELINE_LENGTH:
                    case CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE:
                    case CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE:
                    case CURLMOPT_MAX_TOTAL_CONNECTIONS:
                        curl_multi_setopt(multi, option, (long)data[param_idx]);
                        break;
                    default:
                        break;
                }
            }
            break;
            
        case 5:
            if (param_idx < size) {
                dummy_socket = (curl_socket_t)data[param_idx];
                curl_multi_assign(multi, dummy_socket, dummy_ptr);
            }
            break;
    }
    
    curl_multi_perform(multi, &running_handles);
    
    if (selector != 4 && size > 2) {
        CURLMoption option = CURLMOPT_PIPELINING;
        curl_multi_setopt(multi, option, (long)data[size - 1]);
    }
    
    cleanup_multi_handle(multi);
    unlink("./dummy_file");
    
    return 0;
}
// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include "curl/easy.h"

static int dummy_running_handles = 0;

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    static int curl_initialized = 0;
    if (!curl_initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        curl_initialized = 1;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    if (Size > 0) {
        uint8_t func_selector = Data[0] % 6;
        size_t data_offset = 1;

        switch (func_selector) {
            case 0: {
                if (Size - data_offset >= sizeof(CURLMoption)) {
                    CURLMoption option;
                    memcpy(&option, Data + data_offset, sizeof(CURLMoption));
                    data_offset += sizeof(CURLMoption);

                    if (Size - data_offset >= sizeof(long)) {
                        long param;
                        memcpy(&param, Data + data_offset, sizeof(long));
                        curl_multi_setopt(multi_handle, option, param);
                    } else if (Size - data_offset >= sizeof(void*)) {
                        void* param = NULL;
                        memcpy(&param, Data + data_offset, sizeof(void*));
                        curl_multi_setopt(multi_handle, option, param);
                    } else {
                        curl_multi_setopt(multi_handle, option, NULL);
                    }
                }
                break;
            }
            case 1: {
                if (Size - data_offset >= sizeof(curl_socket_t)) {
                    curl_socket_t socket;
                    memcpy(&socket, Data + data_offset, sizeof(curl_socket_t));
                    curl_multi_socket(multi_handle, socket, &dummy_running_handles);
                }
                break;
            }
            case 2: {
                if (Size - data_offset >= sizeof(curl_socket_t)) {
                    curl_socket_t socket;
                    memcpy(&socket, Data + data_offset, sizeof(curl_socket_t));
                    void* sockp = (void*)(Data + data_offset);
                    curl_multi_assign(multi_handle, socket, sockp);
                }
                break;
            }
            case 3: {
                curl_multi_socket_all(multi_handle, &dummy_running_handles);
                break;
            }
            case 4: {
                curl_multi_perform(multi_handle, &dummy_running_handles);
                break;
            }
            case 5: {
                if (Size - data_offset >= sizeof(curl_socket_t) + sizeof(int)) {
                    curl_socket_t socket;
                    memcpy(&socket, Data + data_offset, sizeof(curl_socket_t));
                    data_offset += sizeof(curl_socket_t);
                    
                    int ev_bitmask;
                    memcpy(&ev_bitmask, Data + data_offset, sizeof(int));
                    curl_multi_socket_action(multi_handle, socket, ev_bitmask, &dummy_running_handles);
                }
                break;
            }
        }
    }

    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    
    curl_multi_cleanup(multi_handle);
    
    return 0;
}
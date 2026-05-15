// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
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

static int running_handles = 0;
static curl_socket_t dummy_socket = 0;
static void *dummy_sockp = NULL;

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    curl_multi_add_handle(multi_handle, easy_handle);

    uint8_t selector = Data[0] % 6;
    size_t offset = 1;

    switch (selector) {
        case 0: {
            if (Size - offset >= sizeof(CURLMoption) + sizeof(long)) {
                CURLMoption option;
                long param;
                memcpy(&option, Data + offset, sizeof(CURLMoption));
                offset += sizeof(CURLMoption);
                memcpy(&param, Data + offset, sizeof(long));
                curl_multi_setopt(multi_handle, option, param);
            }
            break;
        }
        case 1: {
            if (Size - offset >= sizeof(curl_socket_t)) {
                memcpy(&dummy_socket, Data + offset, sizeof(curl_socket_t));
                curl_multi_socket(multi_handle, dummy_socket, &running_handles);
            }
            break;
        }
        case 2: {
            if (Size - offset >= sizeof(curl_socket_t)) {
                memcpy(&dummy_socket, Data + offset, sizeof(curl_socket_t));
                dummy_sockp = (void *)(Data + offset);
                curl_multi_assign(multi_handle, dummy_socket, dummy_sockp);
            }
            break;
        }
        case 3: {
            curl_multi_socket_all(multi_handle, &running_handles);
            break;
        }
        case 4: {
            curl_multi_perform(multi_handle, &running_handles);
            break;
        }
        case 5: {
            if (Size - offset >= sizeof(curl_socket_t) + sizeof(int)) {
                int ev_bitmask;
                memcpy(&dummy_socket, Data + offset, sizeof(curl_socket_t));
                offset += sizeof(curl_socket_t);
                memcpy(&ev_bitmask, Data + offset, sizeof(int));
                curl_multi_socket_action(multi_handle, dummy_socket, ev_bitmask, &running_handles);
            }
            break;
        }
    }

    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}
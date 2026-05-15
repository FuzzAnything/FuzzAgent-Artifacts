// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_setopt at multi.c:3186:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "curl/multi.h"
#include "curl/easy.h"

static void *dummy_sockp = NULL;

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    int running_handles = 0;
    CURLMcode res;

    uint8_t func_selector = 0;
    if (Size > 0) {
        func_selector = Data[0] % 6;
    }

    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (Size > 1) {
            write(fd, Data + 1, Size - 1);
        }
        close(fd);
    }

    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_URL, "file://./dummy_file");
        curl_multi_add_handle(multi_handle, easy);
    }

    switch (func_selector) {
        case 0:
            res = curl_multi_perform(multi_handle, &running_handles);
            break;
        case 1:
            res = curl_multi_socket_all(multi_handle, &running_handles);
            break;
        case 2:
            if (Size > 1) {
                curl_socket_t sock = (curl_socket_t)(Data[1]);
                res = curl_multi_socket(multi_handle, sock, &running_handles);
            } else {
                res = curl_multi_socket(multi_handle, 0, &running_handles);
            }
            break;
        case 3:
            if (Size > 2) {
                curl_socket_t sock = (curl_socket_t)(Data[1]);
                int ev_bitmask = Data[2] % 8;
                res = curl_multi_socket_action(multi_handle, sock, ev_bitmask,
                                               &running_handles);
            } else {
                res = curl_multi_socket_action(multi_handle, 0, 0,
                                               &running_handles);
            }
            break;
        case 4:
            if (Size > 1) {
                CURLMoption option = (CURLMoption)(Data[1] % 256);
                long param = (long)(Size > 2 ? Data[2] : 0);
                res = curl_multi_setopt(multi_handle, option, param);
            } else {
                res = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, 0L);
            }
            break;
        case 5:
            if (Size > 1) {
                curl_socket_t sock = (curl_socket_t)(Data[1]);
                res = curl_multi_assign(multi_handle, sock, &dummy_sockp);
            } else {
                res = curl_multi_assign(multi_handle, 0, &dummy_sockp);
            }
            break;
        default:
            res = CURLM_OK;
            break;
    }

    if (easy) {
        curl_multi_remove_handle(multi_handle, easy);
        curl_easy_cleanup(easy);
    }
    curl_multi_cleanup(multi_handle);

    unlink("./dummy_file");

    return 0;
}
// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

static CURL *create_dummy_easy_handle(void) {
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_URL, "http://localhost");
        curl_easy_setopt(easy, CURLOPT_NOBODY, 1L);
    }
    return easy;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    CURL *easy = create_dummy_easy_handle();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    unsigned int scenario = 0;
    if (Size > 0) {
        scenario = Data[0] % 6;
    }

    switch (scenario) {
        case 0: {
            unsigned int fd_count = 0;
            curl_multi_waitfds(multi, NULL, 0, &fd_count);
            if (fd_count > 0) {
                struct curl_waitfd *ufds = malloc(fd_count * sizeof(struct curl_waitfd));
                if (ufds) {
                    curl_multi_waitfds(multi, ufds, fd_count, &fd_count);
                    free(ufds);
                }
            }
            break;
        }
        case 1: {
            curl_multi_add_handle(multi, easy);
            curl_multi_remove_handle(multi, easy);
            break;
        }
        case 2: {
            CURL **handles = curl_multi_get_handles(multi);
            if (handles) {
                free(handles);
            }
            break;
        }
        case 3: {
            curl_multi_add_handle(multi, easy);
            CURL **handles = curl_multi_get_handles(multi);
            if (handles) {
                free(handles);
            }
            curl_multi_remove_handle(multi, easy);
            break;
        }
        case 4: {
            struct curl_waitfd extra_fds[2];
            extra_fds[0].fd = -1;
            extra_fds[0].events = CURL_WAIT_POLLIN;
            extra_fds[1].fd = -1;
            extra_fds[1].events = CURL_WAIT_POLLOUT;
            int ret = 0;
            int timeout_ms = (Size > 1) ? (Data[1] % 1000) : 100;
            curl_multi_wait(multi, extra_fds, 2, timeout_ms, &ret);
            break;
        }
        case 5: {
            curl_multi_add_handle(multi, easy);
            unsigned int fd_count = 0;
            curl_multi_waitfds(multi, NULL, 0, &fd_count);
            if (fd_count > 0) {
                struct curl_waitfd *ufds = malloc(fd_count * sizeof(struct curl_waitfd));
                if (ufds) {
                    curl_multi_waitfds(multi, ufds, fd_count, &fd_count);
                    int ret = 0;
                    int timeout_ms = (Size > 1) ? (Data[1] % 100) : 10;
                    curl_multi_wait(multi, ufds, fd_count, timeout_ms, &ret);
                    free(ufds);
                }
            }
            curl_multi_remove_handle(multi, easy);
            break;
        }
    }

    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);
    return 0;
}
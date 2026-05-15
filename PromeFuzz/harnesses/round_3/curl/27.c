// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "curl/curl.h"
#include "curl/multi.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (size > 0) {
            fwrite(data, 1, size < 1024 ? size : 1024, f);
        } else {
            fwrite("test", 1, 4, f);
        }
        fclose(f);
    }
}

static CURL *create_easy_handle(const uint8_t *data, size_t size) {
    CURL *easy = curl_easy_init();
    if (easy) {
        write_dummy_file(data, size);
        curl_easy_setopt(easy, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
    }
    return easy;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    CURLM *multi = curl_multi_init();
    if (!multi) return 0;
    
    unsigned int selector = data[0] % 6;
    size_t offset = 1;
    
    switch (selector) {
        case 0: { /* curl_multi_waitfds */
            unsigned int fd_count = 0;
            CURLMcode res = curl_multi_waitfds(multi, NULL, 0, &fd_count);
            if (res == CURLM_OK && fd_count > 0) {
                struct curl_waitfd *ufds = malloc(fd_count * sizeof(struct curl_waitfd));
                if (ufds) {
                    for (unsigned int i = 0; i < fd_count; i++) {
                        ufds[i].fd = -1;
                        ufds[i].events = (offset + i) < size ? data[offset + i] % 256 : 1; /* 1 for read event */
                        ufds[i].revents = 0;
                    }
                    curl_multi_waitfds(multi, ufds, fd_count, &fd_count);
                    free(ufds);
                }
            }
            break;
        }
        case 1: { /* curl_multi_wait */
            unsigned int extra_nfds = (offset < size) ? data[offset] % 8 : 0;
            offset++;
            struct curl_waitfd *extra_fds = NULL;
            if (extra_nfds > 0) {
                extra_fds = malloc(extra_nfds * sizeof(struct curl_waitfd));
                if (extra_fds) {
                    for (unsigned int i = 0; i < extra_nfds; i++) {
                        extra_fds[i].fd = -1;
                        extra_fds[i].events = (offset + i) < size ? data[offset + i] % 256 : 1; /* 1 for read event */
                        extra_fds[i].revents = 0;
                    }
                }
            }
            int timeout_ms = (offset < size) ? (data[offset] % 1000) : 10;
            int ret = 0;
            curl_multi_wait(multi, extra_fds, extra_nfds, timeout_ms, &ret);
            free(extra_fds);
            break;
        }
        case 2: { /* curl_multi_add_handle */
            CURL *easy = create_easy_handle(data, size);
            if (easy) {
                curl_multi_add_handle(multi, easy);
                curl_multi_remove_handle(multi, easy);
                curl_easy_cleanup(easy);
            }
            break;
        }
        case 3: { /* curl_multi_get_handles */
            CURL **handles = curl_multi_get_handles(multi);
            if (handles) {
                free(handles);
            }
            break;
        }
        case 4: { /* curl_multi_init - already done, test removal */
            CURL *easy = create_easy_handle(data, size);
            if (easy) {
                curl_multi_add_handle(multi, easy);
                curl_multi_remove_handle(multi, easy);
                curl_easy_cleanup(easy);
            }
            break;
        }
        case 5: { /* curl_multi_remove_handle */
            CURL *easy = create_easy_handle(data, size);
            if (easy) {
                curl_multi_add_handle(multi, easy);
                curl_multi_remove_handle(multi, easy);
                curl_easy_cleanup(easy);
            }
            break;
        }
    }
    
    curl_multi_cleanup(multi);
    unlink("./dummy_file");
    return 0;
}
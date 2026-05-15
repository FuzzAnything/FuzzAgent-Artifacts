// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_easy_setopt at setopt.c:2958:10 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_get_handles at multi.c:3724:8 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include "curl/easy.h"

static CURL *create_dummy_easy_handle(void) {
    CURL *easy = curl_easy_init();
    if (easy) {
        curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy, CURLOPT_NOBODY, 1L);
    }
    return easy;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    unsigned int fd_count = 0;
    struct curl_waitfd *ufds = NULL;
    int ret_val = 0;

    // Test curl_multi_waitfds with NULL first to get count
    CURLMcode res = curl_multi_waitfds(multi, NULL, 0, &fd_count);
    if (res == CURLM_OK && fd_count > 0) {
        ufds = (struct curl_waitfd *)malloc(fd_count * sizeof(struct curl_waitfd));
        if (ufds) {
            curl_multi_waitfds(multi, ufds, fd_count, &fd_count);
            free(ufds);
        }
    }

    // Create and add easy handles based on fuzzer input
    size_t num_handles = Size > 0 ? (Data[0] % 5) + 1 : 1;
    CURL **easy_handles = (CURL **)malloc(num_handles * sizeof(CURL *));
    
    if (easy_handles) {
        for (size_t i = 0; i < num_handles; i++) {
            easy_handles[i] = create_dummy_easy_handle();
            if (easy_handles[i]) {
                curl_multi_add_handle(multi, easy_handles[i]);
            }
        }

        // Test curl_multi_get_handles
        CURL **handle_array = curl_multi_get_handles(multi);
        if (handle_array) {
            free(handle_array);
        }

        // Test curl_multi_wait with short timeout
        int timeout = Size > 1 ? (Data[1] % 100) : 0;
        curl_multi_wait(multi, NULL, 0, timeout, &ret_val);

        // Remove and cleanup easy handles
        for (size_t i = 0; i < num_handles; i++) {
            if (easy_handles[i]) {
                curl_multi_remove_handle(multi, easy_handles[i]);
                curl_easy_cleanup(easy_handles[i]);
            }
        }
        free(easy_handles);
    }

    // Test curl_multi_waitfds again after removing handles
    fd_count = 0;
    curl_multi_waitfds(multi, NULL, 0, &fd_count);

    curl_multi_cleanup(multi);
    return 0;
}
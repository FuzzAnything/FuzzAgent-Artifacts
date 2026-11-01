// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = NULL;
    if (Size >= 2) {
        easy_handle = curl_easy_init();
        if (easy_handle) {
            curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        }
    }

    int action = Data[0] % 6;
    int fd = -1;
    void *sockp = (void *)Data;

    fd_set read_fd_set, write_fd_set, exc_fd_set;
    int max_fd = 0;

    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);

    switch (action) {
        case 0: {
            // curl_multi_assign
            if (Size >= 2) {
                fd = Data[1];
            }
            curl_multi_assign(multi_handle, fd, sockp);
            break;
        }
        case 1: {
            // curl_multi_strerror
            int error_code = 0;
            if (Size >= 2) {
                error_code = Data[1];
            }
            const char *str = curl_multi_strerror(error_code);
            (void)str;
            break;
        }
        case 2: {
            // curl_multi_remove_handle
            if (easy_handle) {
                curl_multi_remove_handle(multi_handle, easy_handle);
            }
            break;
        }
        case 3: {
            // curl_multi_fdset
            curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
            break;
        }
        case 4: {
            // curl_multi_wakeup
            curl_multi_wakeup(multi_handle);
            break;
        }
        case 5: {
            // Try adding handle then removing it
            if (easy_handle) {
                curl_multi_add_handle(multi_handle, easy_handle);
                curl_multi_remove_handle(multi_handle, easy_handle);
            }
            break;
        }
    }

    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }

    curl_multi_cleanup(multi_handle);

    return 0;
}
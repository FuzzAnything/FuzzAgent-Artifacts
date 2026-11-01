// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURLMcode res;
    int max_fd;
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;
    const char *str;
    long long_val;
    CURLcode easy_res;

    // Initialize multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Initialize easy handle
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Set a dummy URL to ensure the handle is somewhat valid for multi operations
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");

    // Add easy handle to multi handle
    res = curl_multi_add_handle(multi_handle, easy_handle);
    if (res != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Use input data to drive fuzzing logic
    size_t idx = 0;
    uint8_t op = (Size > idx) ? Data[idx++] : 0;

    switch (op % 6) {
        case 0: {
            // Fuzz curl_multi_wakeup
            res = curl_multi_wakeup(multi_handle);
            str = curl_multi_strerror(res);
            (void)str; // Avoid unused warning
            break;
        }
        case 1: {
            // Fuzz curl_multi_assign
            if (Size > idx) {
                sockfd = (curl_socket_t)(Data[idx++] & 0xFF);
            }
            if (Size > idx) {
                sockp = (void *)(uintptr_t)(Data[idx++] & 0xFF);
            }
            res = curl_multi_assign(multi_handle, sockfd, sockp);
            str = curl_multi_strerror(res);
            (void)str;
            break;
        }
        case 2: {
            // Fuzz curl_multi_remove_handle
            res = curl_multi_remove_handle(multi_handle, easy_handle);
            str = curl_multi_strerror(res);
            (void)str;
            break;
        }
        case 3: {
            // Fuzz curl_multi_fdset
            FD_ZERO(&read_fd_set);
            FD_ZERO(&write_fd_set);
            FD_ZERO(&exc_fd_set);
            max_fd = -1;
            res = curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
            str = curl_multi_strerror(res);
            (void)str;
            break;
        }
        case 4: {
            // Fuzz curl_easy_getinfo with long type (related to Wcurl_easy_getinfo_err_long)
            easy_res = curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &long_val);
            (void)easy_res;
            (void)long_val;
            break;
        }
        case 5: {
            // Fuzz curl_multi_strerror with various codes
            int code = 0;
            if (Size > idx) {
                code = (int)(Data[idx++] & 0xFF);
            }
            // Try a range of error codes
            for (int i = 0; i < 10; i++) {
                str = curl_multi_strerror((CURLMcode)(code + i));
                (void)str;
            }
            break;
        }
    }

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}
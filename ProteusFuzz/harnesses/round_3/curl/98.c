// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;
    CURLMcode res;

    if (Size < 1) {
        return 0;
    }

    // Initialize multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Initialize easy handle for testing
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Set a dummy URL to avoid immediate errors
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, 5L);

    // Add easy handle to multi handle
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Use input data to drive fuzzing logic
    unsigned int op = Data[0] % 7;

    switch (op) {
        case 0:
            // Test curl_multi_perform
            res = curl_multi_perform(multi_handle, &running_handles);
            if (res != CURLM_OK) {
                // Ignore errors, just continue
            }
            break;

        case 1:
            // Test curl_multi_assign
            if (Size > 1) {
                sockfd = Data[1] % 1024;
            }
            if (Size > 2) {
                sockp = (void *)(uintptr_t)(Data[2] % 1000);
            }
            res = curl_multi_assign(multi_handle, sockfd, sockp);
            if (res != CURLM_OK) {
                // Ignore errors
            }
            break;

        case 2:
            // Test curl_multi_strerror
            {
                CURLMcode error_code = (CURLMcode)(Data[0] % 100);
                const char *str = curl_multi_strerror(error_code);
                if (str) {
                    // Just to use the string
                    (void)strlen(str);
                }
            }
            break;

        case 3:
            // Test deprecated curl_multi_socket
            if (Size > 1) {
                sockfd = Data[1] % 1024;
            }
            res = curl_multi_socket(multi_handle, sockfd, &running_handles);
            if (res != CURLM_OK) {
                // Ignore errors
            }
            break;

        case 4:
            // Test curl_multi_socket_action
            if (Size > 1) {
                sockfd = Data[1] % 1024;
            }
            int ev_bitmask = 1;
            if (Size > 2) {
                ev_bitmask = (Data[2] % 4) + 1; // 1, 2, 3, or 4
            }
            res = curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
            if (res != CURLM_OK) {
                // Ignore errors
            }
            break;

        case 5:
            // Test deprecated curl_multi_socket_all
            res = curl_multi_socket_all(multi_handle, &running_handles);
            if (res != CURLM_OK) {
                // Ignore errors
            }
            break;

        case 6:
            // Mixed operations
            res = curl_multi_perform(multi_handle, &running_handles);
            if (Size > 1) {
                sockfd = Data[1] % 1024;
            }
            res = curl_multi_socket_action(multi_handle, sockfd, 1, &running_handles);
            break;
    }

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}
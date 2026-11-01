// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* Disable type checking macros in curl headers to prevent compilation errors
   in fuzzing environments where full context might be missing or conflicting. */
#define CURL_DISABLE_TYPECHECK 1

#include <curl/multi.h>

static void cleanup_multi(CURLM *multi_handle, CURL *easy_handle) {
    if (multi_handle && easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        cleanup_multi(multi_handle, easy_handle);
        return 0;
    }

    int op = Data[0] % 6;
    int ret = 0;
    CURLMcode result;

    switch (op) {
        case 0: {
            /* Test curl_multi_wakeup */
            result = curl_multi_wakeup(multi_handle);
            (void)result;
            break;
        }
        case 1: {
            /* Test curl_multi_wait */
            struct curl_waitfd extra_fds[1];
            extra_fds[0].fd = -1;
            extra_fds[0].events = 0;
            extra_fds[0].revents = 0;
            int timeout_ms = (Data[1] % 1000) + 1;
            result = curl_multi_wait(multi_handle, extra_fds, 1, timeout_ms, &ret);
            (void)result;
            break;
        }
        case 2: {
            /* Test curl_multi_remove_handle */
            result = curl_multi_remove_handle(multi_handle, easy_handle);
            (void)result;
            break;
        }
        case 3: {
            /* Test curl_multi_assign */
            curl_socket_t sockfd = -1;
            void *sockp = (void *)(uintptr_t)(Data[1]);
            result = curl_multi_assign(multi_handle, sockfd, sockp);
            (void)result;
            break;
        }
        case 4: {
            /* Test curl_multi_init (re-init to test) */
            CURLM *new_multi = curl_multi_init();
            if (new_multi) {
                curl_multi_cleanup(new_multi);
            }
            break;
        }
        case 5: {
            /* Test curl_multi_strerror */
            CURLMcode error_code = (CURLMcode)(Data[1] % 20);
            const char *str = curl_multi_strerror(error_code);
            (void)str;
            break;
        }
        default:
            break;
    }

    /* Cleanup */
    if (op != 2) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}
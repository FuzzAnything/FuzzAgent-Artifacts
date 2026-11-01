// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Targets: curl_multi_init, curl_multi_setopt, curl_multi_assign,
 *          curl_multi_strerror, curl_multi_remove_handle, curl_multi_wakeup.
 */

/* Dummy push function for CURLMOPT_PUSHFUNCTION */
static int dummy_push_callback(CURL *parent, CURL *easy, size_t num_headers,
                               struct curl_slist *headers, void *userp) {
    (void)parent;
    (void)easy;
    (void)num_headers;
    (void)headers;
    (void)userp;
    /* Deny all pushes by returning 1, or approve by returning 0 */
    return 1;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Use input data to drive fuzzing decisions */
    uint8_t mode = Data[0] % 5;
    curl_socket_t sock_fd = -1;
    void *sockp = NULL;
    CURL *easy_handle = NULL;

    /* Create a dummy easy handle if needed */
    if (Size > 1 && (Data[1] % 2 == 0)) {
        easy_handle = curl_easy_init();
    }

    /* Prepare socket pointer for curl_multi_assign */
    if (Size > 2) {
        sock_fd = (curl_socket_t)(Data[2] % 1024) - 512; /* Range: -512 to 511 */
    }
    if (Size > 3) {
        sockp = (void *)(intptr_t)(Data[3] % 256);
    }

    switch (mode) {
        case 0:
            /* Test curl_multi_setopt with CURLMOPT_PUSHFUNCTION */
            curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
            curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
            break;

        case 1:
            /* Test curl_multi_assign */
            curl_multi_assign(multi_handle, sock_fd, sockp);
            /* Test with invalid socket */
            curl_multi_assign(multi_handle, -1, sockp);
            break;

        case 2:
            /* Test curl_multi_strerror with various error codes */
            {
                CURLMcode codes[] = {CURLM_OK, CURLM_BAD_HANDLE, CURLM_BAD_EASY_HANDLE,
                                     CURLM_OUT_OF_MEMORY, CURLM_INTERNAL_ERROR};
                for (int i = 0; i < 5; i++) {
                    const char *msg = curl_multi_strerror(codes[i]);
                    if (msg) {
                        (void)strlen(msg); /* Use the string to avoid unused warning */
                    }
                }
                /* Test with an out-of-range code */
                curl_multi_strerror((CURLMcode)999);
            }
            break;

        case 3:
            /* Test curl_multi_remove_handle */
            if (easy_handle) {
                /* Add easy handle to multi handle first */
                curl_multi_add_handle(multi_handle, easy_handle);
                /* Remove it */
                curl_multi_remove_handle(multi_handle, easy_handle);
                /* Try removing again (should be safe) */
                curl_multi_remove_handle(multi_handle, easy_handle);
            } else {
                /* Try removing a NULL handle */
                curl_multi_remove_handle(multi_handle, NULL);
            }
            break;

        case 4:
            /* Test curl_multi_wakeup */
            curl_multi_wakeup(multi_handle);
            break;
    }

    /* Cleanup */
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);

    return 0;
}
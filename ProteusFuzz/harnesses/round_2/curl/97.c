// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* 
 * Fuzz driver for curl multi-handle API functions.
 * Target functions:
 * 1. curl_multi_strerror
 * 2. curl_multi_wakeup
 * 3. curl_multi_assign
 * 4. curl_multi_socket (deprecated)
 * 5. curl_multi_socket_all (deprecated)
 * 6. curl_multi_remove_handle
 */

static void cleanup_handles(CURLM *multi, CURL *easy) {
    if (multi && easy) {
        (void)curl_multi_remove_handle(multi, easy);
    }
    if (multi) {
        curl_multi_cleanup(multi);
    }
    if (easy) {
        curl_easy_cleanup(easy);
    }
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl library */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    CURLM *multi = NULL;
    CURL *easy = NULL;
    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;
    CURLMcode ret;

    /* Use input data to diversify behavior */
    uint8_t seed = Data[0];
    uint8_t mode = (Size > 1) ? Data[1] : 0;
    uint8_t error_code_byte = (Size > 2) ? Data[2] : 0;

    /* Initialize multi handle */
    multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }

    /* Initialize easy handle */
    easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }

    /* Add easy handle to multi handle for some tests */
    if (mode % 3 == 0) {
        curl_multi_add_handle(multi, easy);
    }

    /* 1. Test curl_multi_strerror with various error codes */
    CURLMcode error_code = (CURLMcode)error_code_byte;
    const char *str = curl_multi_strerror(error_code);
    (void)str; /* Suppress unused variable warning */

    /* Test with a range of error codes by iterating through input if large enough */
    if (Size > 10) {
        for (size_t i = 0; i < Size && i < 256; i++) {
            (void)curl_multi_strerror((CURLMcode)Data[i]);
        }
    }

    /* 2. Test curl_multi_wakeup */
    /* Valid handle */
    ret = curl_multi_wakeup(multi);
    (void)ret;

    /* Invalid handle (NULL) - Safe to call, returns CURLM_BAD_HANDLE */
    ret = curl_multi_wakeup(NULL);
    (void)ret;

    /* 3. Test curl_multi_assign */
    /* Use a dummy socket file descriptor */
    sockfd = (curl_socket_t)(seed % 100);
    sockp = (void *)(uintptr_t)(seed * 2);

    ret = curl_multi_assign(multi, sockfd, sockp);
    (void)ret;

    /* Test with NULL multi handle - Safe to call, returns CURLM_BAD_HANDLE */
    ret = curl_multi_assign(NULL, sockfd, sockp);
    (void)ret;

    /* Test with NULL sockp */
    ret = curl_multi_assign(multi, sockfd, NULL);
    (void)ret;

    /* 4. Test curl_multi_socket (deprecated) */
    /* Only if easy handle was added */
    if (mode % 3 == 0) {
        ret = curl_multi_socket(multi, sockfd, &running_handles);
        (void)ret;
    }
    /* 
     * Do NOT call curl_multi_socket with NULL multi_handle.
     * The internal implementation may dereference the pointer before checking for NULL,
     * causing a segmentation fault as seen in the ASan report.
     * The function expects a valid CURLM* or will crash.
     */

    /* 5. Test curl_multi_socket_all (deprecated) */
    if (mode % 3 == 0) {
        ret = curl_multi_socket_all(multi, &running_handles);
        (void)ret;
    }
    /* 
     * Do NOT call curl_multi_socket_all with NULL multi_handle.
     * Similar to curl_multi_socket, it may crash on NULL input.
     */

    /* 6. Test curl_multi_remove_handle */
    /* Remove the easy handle if it was added */
    if (mode % 3 == 0) {
        ret = curl_multi_remove_handle(multi, easy);
        (void)ret;
    }

    /* Test with NULL multi handle - Safe to call, returns CURLM_BAD_HANDLE */
    ret = curl_multi_remove_handle(NULL, easy);
    (void)ret;

    /* Test with NULL easy handle - Safe to call, returns CURLM_BAD_EASY_HANDLE */
    ret = curl_multi_remove_handle(multi, NULL);
    (void)ret;

    /* Cleanup */
    if (mode % 3 == 0) {
        /* If we added it, try to remove again (should be safe) */
        (void)curl_multi_remove_handle(multi, easy);
    }
    curl_multi_cleanup(multi);
    curl_easy_cleanup(easy);

    curl_global_cleanup();

    return 0;
}
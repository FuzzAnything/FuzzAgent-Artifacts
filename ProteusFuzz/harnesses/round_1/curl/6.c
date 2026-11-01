// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/*
 * Static helper to create a dummy easy handle and set some options.
 * This helps explore states where an easy handle is attached to a multi handle.
 */
static CURL *create_dummy_easy_handle(const uint8_t *data, size_t size) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return NULL;
    }

    /* Set a dummy URL if data is large enough */
    if (size >= 4) {
        char url[256];
        size_t len = (size < 255) ? size : 255;
        memcpy(url, data, len);
        url[len] = '\0';
        /* Ensure it looks like a URL to avoid immediate failures if possible,
         * though fuzzing will naturally hit invalid URLs. */
        if (strncmp(url, "http", 4) != 0 && strncmp(url, "ftp", 3) != 0) {
            url[0] = 'h'; url[1] = 't'; url[2] = 't'; url[3] = 'p';
            url[4] = ':'; url[5] = '/'; url[6] = '/'; url[7] = 'e'; url[8] = 'x'; url[9] = 'a'; url[10] = 'm'; url[11] = 'p'; url[12] = 'l'; url[13] = 'e'; url[14] = '.'; url[15] = 'c'; url[16] = 'o'; url[17] = 'm';
            url[18] = '\0';
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
    }

    /* Set a timeout to prevent hanging */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1L);

    /* Disable SSL verification for fuzzing stability */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    return curl;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize multi handle */
    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    /* Initialize easy handle */
    CURL *easy = create_dummy_easy_handle(Data, Size);
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    /* Test 1: Add easy handle to multi handle */
    CURLMcode res = curl_multi_add_handle(multi, easy);
    if (res != CURLM_OK) {
        /* If adding fails, we can still test remove and other functions with invalid states */
    }

    /* Test 2: curl_multi_remove_handle */
    /* Try removing the handle (whether it was added or not) */
    res = curl_multi_remove_handle(multi, easy);
    (void)res; /* Ignore result, just exercising the path */

    /* Test 3: curl_multi_wakeup */
    res = curl_multi_wakeup(multi);
    (void)res;

    /* Test 4: curl_multi_assign */
    /* Use data to derive a socket fd (int) and a pointer */
    int sock_fd = 0;
    if (Size >= 4) {
        sock_fd = *(int *)(Data);
    }
    void *sockp = (void *)(uintptr_t)(Size);
    res = curl_multi_assign(multi, sock_fd, sockp);
    (void)res;

    /* Test 5: curl_multi_strerror */
    /* Iterate through a range of error codes derived from input */
    int error_code = 0;
    if (Size >= 4) {
        error_code = *(int *)(Data);
    }
    /* Also try a few fixed known codes */
    const char *msg1 = curl_multi_strerror((CURLMcode)error_code);
    const char *msg2 = curl_multi_strerror(CURLM_OK);
    const char *msg3 = curl_multi_strerror(CURLM_BAD_HANDLE);
    const char *msg4 = curl_multi_strerror(CURLM_OUT_OF_MEMORY);
    (void)msg1;
    (void)msg2;
    (void)msg3;
    (void)msg4;

    /* Cleanup easy handle */
    curl_easy_cleanup(easy);

    /* Cleanup multi handle */
    curl_multi_cleanup(multi);

    return 0;
}
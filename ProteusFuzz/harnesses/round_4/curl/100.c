// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
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
#include <stdio.h>

/*
 * Fuzz driver for curl_multi_socket_all and curl_multi_socket.
 * Note: curl_multi_socket is deprecated in favor of curl_multi_socket_action.
 * The Wcurl_* macros are compile-time type checkers and do not have runtime logic to fuzz.
 * We focus on the runtime behavior of the multi-handle functions.
 */

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    int running_handles = 0;
    curl_socket_t sock;
    CURLMcode res;

    if (Size < 2) {
        return 0;
    }

    /* Initialize libcurl */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create easy handle */
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Use fuzzing data to set URL if possible */
    if (Size > 10) {
        /* Ensure null termination for URL */
        char *url_buf = (char *)malloc(Size + 1);
        if (url_buf) {
            memcpy(url_buf, Data, Size);
            url_buf[Size] = '\0';
            /* Basic sanitization: remove newlines to prevent malformed URLs */
            for (size_t i = 0; i < Size; i++) {
                if (url_buf[i] == '\n' || url_buf[i] == '\r') {
                    url_buf[i] = ' ';
                }
            }
            curl_easy_setopt(easy_handle, CURLOPT_URL, url_buf);
            curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
            /* Prevent actual network transfer for fuzzing safety */
            curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 1L);
            curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, 1L);
            free(url_buf);
        }
    }

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Extract socket descriptor from fuzzing data */
    if (Size >= 2) {
        sock = (curl_socket_t)((Data[0] << 8) | Data[1]);
    } else {
        sock = CURL_SOCKET_BAD;
    }

    /*
     * Fuzz curl_multi_socket_all
     * Signature: curl_multi_socket_all(CURLM *multi_handle, int *running_handles);
     */
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res; /* Ignore return code for fuzzing, just exercise the path */

    /*
     * Fuzz curl_multi_socket
     * Signature: curl_multi_socket(CURLM *multi_handle, curl_socket_t s, int *running_handles);
     * This is deprecated but still present.
     */
    res = curl_multi_socket(multi_handle, sock, &running_handles);
    (void)res;

    /*
     * Try to invoke with different socket values to explore states
     */
    if (Size >= 4) {
        curl_socket_t sock2 = (curl_socket_t)((Data[2] << 8) | Data[3]);
        res = curl_multi_socket(multi_handle, sock2, &running_handles);
        (void)res;
    }

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}
// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

/*
 * Fuzz driver for curl multi interface functions.
 * Targets: curl_multi_socket, curl_multi_perform, curl_multi_socket_all.
 * Note: The Wcurl_* macros are compile-time checks and cannot be fuzzed at runtime.
 * We include curl/curl.h first to ensure basic types are defined before multi.h
 * to avoid macro expansion issues in typecheck-gcc.h.
 */

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize global curl environment */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Create a dummy easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Set a minimal URL to make the handle valid */
    if (Size > 0) {
        /* Use the fuzz input as a URL, but ensure it's null-terminated if possible */
        char url_buf[256];
        size_t url_len = (Size < 255) ? Size : 255;
        memcpy(url_buf, Data, url_len);
        url_buf[url_len] = '\0';
        curl_easy_setopt(easy_handle, CURLOPT_URL, url_buf);
        curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L); /* HEAD request to avoid body processing */
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
    }

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    int running_handles = 0;
    curl_socket_t s = CURL_SOCKET_BAD;

    /* Extract socket from input if available, otherwise use CURL_SOCKET_BAD */
    if (Size >= sizeof(curl_socket_t)) {
        memcpy(&s, Data, sizeof(curl_socket_t));
        /* Ensure s is a valid socket descriptor range or CURL_SOCKET_BAD */
        if (s < CURL_SOCKET_BAD || s > 65535) {
            s = CURL_SOCKET_BAD;
        }
    }

    /* Fuzz curl_multi_socket (Deprecated but still functional for testing) */
    (void)curl_multi_socket(multi_handle, s, &running_handles);

    /* Fuzz curl_multi_perform */
    (void)curl_multi_perform(multi_handle, &running_handles);

    /* Fuzz curl_multi_socket_all (Deprecated but still functional for testing) */
    (void)curl_multi_socket_all(multi_handle, &running_handles);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}